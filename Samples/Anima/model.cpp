#include "model.h"

#include <ResourceHelpers.h>
#include <math.h>
#include <utils.h>
#include <algorithm>

#include "md5.h"

struct SkinnedVertex
{
    float   px, py, pz;
    float   nx, ny, nz;
    float   u, v;
    float	w[4];
    uint8_t b[4];
};

struct Mesh
{
	GLuint			vbo;
	GLuint          ibo;
	GLuint          texture;
	GLsizei			numIndices;
};

void RenderSkeleton(int numJoints, int* hierarchy, ml::dual_quat* joints);

void createMesh (Mesh* mesh,
                 size_t numJoints,   ml::dual_quat* joints, 
                 size_t numVertices, md5_vertex_t*  md5Vertices,
                 size_t numWeights,  md5_weight_t*  md5Weights,
                 size_t numIndices,  uint16_t*      indices);

void destroyMesh(Mesh* mesh);

MD5Model::MD5Model()
: mNumJoints(0)
, mNumMeshes(0)
, mHasAnimation(false)
, mNumFrames(0)
, mFrameRate(0)
, mAnimTime(0)
, mMeshes(0)
, mBoneHierarchy(0)
, mBindPose(0)
, mInvBindPose(0)
, mFramePoses(0)
, mBoneTransform(0)
, mPose(0)
{
    program = resources::createProgramFromFiles("Skinning4.vert", "wireframe.geom", "SHLighting.frag");

    resetUniforms();
    assert(glGetError()==GL_NO_ERROR);
}

MD5Model::~MD5Model()
{
    glDeleteProgram(program);

    Mesh* mesh = mMeshes;
    while (mNumMeshes--)
    {
        destroyMesh(mesh);
        ++mesh;
    }

    mNumJoints = 0;

    if (mMeshes)        free(mMeshes);
    if (mBoneHierarchy) free(mBoneHierarchy);
    if (mBindPose)      free(mBindPose);
    if (mInvBindPose)   free(mInvBindPose);
    if (mFramePoses)    free(mFramePoses);
    if (mBoneTransform) free(mBoneTransform);
    if (mPose)          free(mPose);

    mMeshes        = 0;
    mBoneHierarchy = 0;
    mBindPose      = 0;
    mInvBindPose   = 0;
    mFramePoses    = 0;
    mBoneTransform = 0;
    mPose          = 0;
}

bool MD5Model::LoadModel(const char* name)
{
    memory_t inText;
    memory_t outBinary;

    if (marea(&outBinary, 4*1024*1024) &&
        mopen(&inText, name)           &&
        md5meshConvertToBinary(&inText, &outBinary))
    {
        mfree( &inText );

        md5_model_t* model = (md5_model_t*)outBinary.buffer;

        mNumMeshes = model->numMeshes;
        mNumJoints = model->numJoints;

        mMeshes        = (Mesh*)         malloc(mNumMeshes*sizeof(Mesh));
        mBoneHierarchy = (int*)          malloc(mNumJoints*sizeof(int));
        mBindPose      = (ml::dual_quat*)malloc(mNumJoints*sizeof(ml::dual_quat));
        mInvBindPose   = (ml::dual_quat*)malloc(mNumJoints*sizeof(ml::dual_quat));
        mBoneTransform = (ml::dual_quat*)malloc(mNumJoints*sizeof(ml::dual_quat));
        mPose          = (ml::dual_quat*)malloc(mNumJoints*sizeof(ml::dual_quat));

        md5BuildJoints(model->numJoints,
            model->joints,
            mBoneHierarchy,
            mBindPose,
            mInvBindPose);

        md5_mesh_t* mesh = model->meshes;
        for (int i=0; i<mNumMeshes; ++i)
        {
            createMesh(&mMeshes[i],
                        mNumJoints,        mBindPose,
                        mesh->numVertices, mesh->vertices,
                        mesh->numWeights,  mesh->weights,
                        mesh->numIndices,  mesh->indices);

            ++mesh;
        }

        mfree(&outBinary);

        return true;
    }

    mfree(&inText);
    mfree(&outBinary);

    return false;
}

bool MD5Model::LoadAnim( const char *name )
{
    memory_t inText;
    memory_t outBinary;

    if (mopen(&inText, name)            &&
        marea(&outBinary, 4*1024*1024)  &&
        md5animConvertToBinary(&inText, &outBinary))
    {
        mfree( &inText );

        md5_anim_t* anim = (md5_anim_t*)outBinary.buffer;

        if (mNumJoints != anim->numJoints) goto cleanup;

        mNumFrames = anim->numFrames;
        mFrameRate = anim->frameRate;

        mFramePoses = (ml::dual_quat*)malloc(mNumFrames*mNumJoints*sizeof(ml::dual_quat));

        md5BuildAnimations(anim->numJoints,
                           anim->numFrames,
                           mBoneHierarchy,
                           anim->frameData,
                           mFramePoses);

        mHasAnimation = true;

        mfree(&outBinary);

        return true;
    }

cleanup:

    mHasAnimation = false;

    mfree(&inText);
    mfree(&outBinary);

    return false;
}

void MD5Model::Update( float fDeltaTime )
{
    if ( mHasAnimation && mNumFrames > 1)
    {
        int   frame0, frame1;
        float lerpK;

        mAnimTime += fDeltaTime * (float)mFrameRate;
        mAnimTime  = fmodf(mAnimTime, (float)mNumFrames);

        frame0 = (int)floorf( mAnimTime );
        frame0 = frame0 % mNumFrames;
        frame1 = frame0 + 1;
        frame1 = frame1 % mNumFrames;

        lerpK  = mAnimTime - floorf(mAnimTime);

        // I assume there is no zero quaternions in orientation data
        // so I do not check whether real quaternion norm is zero
        v128 qr0, qd0, qr1, qd1, r, n;

        ml::dual_quat* frame0Data = mFramePoses + frame0*mNumJoints; 
        ml::dual_quat* frame1Data = mFramePoses + frame1*mNumJoints; 

        for ( int i = 0; i < mNumJoints; ++i )
        {
            qr0 = ml::load_quat(&frame0Data[i].real);
            qd0 = ml::load_quat(&frame0Data[i].dual);
            qr1 = ml::load_quat(&frame1Data[i].real);
            qd1 = ml::load_quat(&frame1Data[i].dual);

            if (vi_cmpx_lt(vi_dot4(qr0, qr1), vi_load_zero()))
            {
                qr1 = vi_neg(qr1);
                qd1 = vi_neg(qd1);
            }

            r  = vi_lerp(qr0, qr1, lerpK);
            n  = ml::length_quat(r);
            r  = vi_div(r, n);
            ml::store_quat(&mPose[i].real, r);

            r  = vi_lerp(qd0, qd1, lerpK);
            r  = vi_div(r, n);
            ml::store_quat(&mPose[i].dual, r);

            ml::mul_dual_quat(&mBoneTransform[i], &mPose[i], &mInvBindPose[i]);
        }
    }
    else
    {
        // No animation. Just use identity quaternions for each bone.
        std::fill(mBoneTransform, mBoneTransform+mNumJoints, ml::identity_dual_quat);
    }
}

void MD5Model::Render(float* MVP)
{
    glUseProgram(program);
    glUniformMatrix4fv(uMVP, 1, false, MVP);
    for (int i=0; i<mNumMeshes; ++i )
    {
        RenderMesh( mMeshes[i] );
    }

    glUseProgram(0);
    RenderSkeleton(mNumJoints, mBoneHierarchy, mPose);
}

void MD5Model::RenderMesh( Mesh& mesh )
{
    glUniformMatrix2x4fv(uBoneDualQuat, mNumJoints, false, &mBoneTransform[0].real.x);

    // Position data
    glBindBuffer( GL_ARRAY_BUFFER, mesh.vbo );

    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), BUFFER_OFFSET(offsetof(SkinnedVertex, px)));
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), BUFFER_OFFSET(offsetof(SkinnedVertex, nx)));
    glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), BUFFER_OFFSET(offsetof(SkinnedVertex, u)));
    glVertexAttribIPointer(3, 4, GL_UNSIGNED_BYTE,   sizeof(SkinnedVertex), BUFFER_OFFSET(offsetof(SkinnedVertex, b)));
    glVertexAttribPointer (4, 4, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex), BUFFER_OFFSET(offsetof(SkinnedVertex, w)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glActiveTexture( GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, mesh.texture );

    // Draw mesh from index buffer
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glDrawElements( GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0) );

    glActiveTexture( GL_TEXTURE0 );
    glDisable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, 0 );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    {
        GLenum err = glGetError();
        assert(err==GL_NO_ERROR);
    }
}

void MD5Model::resetUniforms()
{
    uTexture       = glGetUniformLocation(program, "uTexture");
    uBoneMatrix    = glGetUniformLocation(program, "uBoneMatrix");
    uBoneDualQuat  = glGetUniformLocation(program, "uBoneDualQuat");
    uMVP           = glGetUniformLocation(program, "uMVP");
}

void createMesh( Mesh*  mesh,
                 size_t numJoints,   ml::dual_quat* joints, 
                 size_t numVertices, md5_vertex_t*  md5Vertices,
                 size_t numWeights,  md5_weight_t*  weights,
                 size_t numIndices,  uint16_t*      indices )
{
    SkinnedVertex* vertices;
    size_t         vertexDataSize;

    vertexDataSize = sizeof(SkinnedVertex)*numVertices;
    vertices = (SkinnedVertex*)malloc(vertexDataSize);
    memset(vertices, 0, vertexDataSize);

    for ( unsigned int i = 0; i < numVertices; ++i )
    {
        SkinnedVertex& vert        = vertices[i];
        int            weightCount = md5Vertices[i].count;
        int            startWeight = md5Vertices[i].start;

        assert( weightCount <= 4 );

        v128 pos;

        pos = vi_load_zero();

        // Sum the position of the weights
        for ( int j = 0; j < weightCount; ++j )
        {
            md5_weight_t&  weight = weights[startWeight + j];
            ml::dual_quat& joint  = joints[weight.joint];

            v128 r, d, v, t;

            r = ml::load_quat(&joint.real);
            d = ml::load_quat(&joint.dual);
            v = ml::load_vec3(&weight.location);

            v = ml::rotate_quat_vec3(r, v);
            t = ml::translation_dual_quat(r, d);
            t = vi_add(v, t);

            pos = vi_mad(t, vi_set_xxxx(weight.bias), pos);

            assert(weight.joint<256);

            vert.b[j] = (uint8_t)weight.joint;
            vert.w[j] = weight.bias;
        }

        ml::store_vec3((ml::vec3*)&vert.px, pos);
    }

    // Loop through all triangles and calculate the normal of each triangle
    for ( unsigned int i = 0; i < numIndices/3; ++i )
    {
        v128 v0, v1, v2;
        v128 n0, n1, n2;
        v128 n;

        v0 = ml::load_vec3((ml::vec3*)&vertices[ indices[i*3+0] ].px);
        v1 = ml::load_vec3((ml::vec3*)&vertices[ indices[i*3+1] ].px);
        v2 = ml::load_vec3((ml::vec3*)&vertices[ indices[i*3+2] ].px);

        n = vi_cross3(vi_sub(v2, v0), vi_sub(v1, v0));

        n0 = ml::load_vec3((ml::vec3*)&vertices[ indices[i*3+0] ].nx);
        n1 = ml::load_vec3((ml::vec3*)&vertices[ indices[i*3+1] ].nx);
        n2 = ml::load_vec3((ml::vec3*)&vertices[ indices[i*3+2] ].nx);

        n0 = vi_add(n0, n);
        n1 = vi_add(n1, n);
        n2 = vi_add(n2, n);

        ml::store_vec3((ml::vec3*)&vertices[ indices[i*3+0] ].nx, n0);
        ml::store_vec3((ml::vec3*)&vertices[ indices[i*3+1] ].nx, n1);
        ml::store_vec3((ml::vec3*)&vertices[ indices[i*3+2] ].nx, n2);
    }

    // Now normalize all the normals
    for ( unsigned int i = 0; i < numVertices; ++i )
    {
        v128           n;
        SkinnedVertex& vert = vertices[i];

        n = ml::load_vec3((ml::vec3*)&vert.nx);
        n = ml::normalize(n);
        ml::store_vec3((ml::vec3*)&vert.nx, n);
    }

    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ibo);

    glBindBuffer( GL_ARRAY_BUFFER, mesh->vbo );
    glBufferData( GL_ARRAY_BUFFER, vertexDataSize, vertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh->ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * numIndices, indices, GL_STATIC_DRAW ); 

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    mesh->numIndices = numIndices;

    {
        GLuint err = glGetError();
        assert(err==GL_NO_ERROR);
    }

    free(vertices);
}

void destroyMesh(Mesh* mesh)
{
    if (mesh->vbo)
    {
        glDeleteBuffers(1, &mesh->vbo);
        mesh->vbo = 0;
    }
    if (mesh->ibo)
    {
        glDeleteBuffers(1, &mesh->ibo);
        mesh->ibo = 0;
    }
    if (mesh->texture)
    {
        glDeleteTextures(1, &mesh->texture);
        mesh->texture = 0;
    }
}

void RenderSkeleton(int numJoints, int* hierarchy, ml::dual_quat* joints)
{
    glPointSize( 5.0f );
    glColor3f( 1.0f, 0.0f, 0.0f );

    glPushAttrib( GL_ENABLE_BIT );

    glDisable(GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    // Draw the joint positions
    glBegin( GL_POINTS );
    {
        for ( int i = 0; i < numJoints; ++i )
        {
            ml::vec3 pos;

            ml::get_translation_from_dual_quat(&pos, &joints[i]);
            glVertex3fv( &pos.x );
        }
    }
    glEnd();

    // Draw the bones
    glColor3f( 0.0f, 1.0f, 0.0f );
    glBegin( GL_LINES );
    {
        for ( int i = 0; i < numJoints; ++i )
        {
            ml::vec3 pos0, pos1;

            ml::get_translation_from_dual_quat(&pos0, &joints[i]);
            const int    parent = hierarchy[i];
            if ( parent != -1 )
            {
                ml::get_translation_from_dual_quat(&pos1, &joints[parent]);
                glVertex3fv( &pos0.x );
                glVertex3fv( &pos1.x );
            }
        }
    }
    glEnd();

    glPopAttrib();
}
