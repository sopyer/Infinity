#define _CRT_SECURE_NO_WARNINGS

#include "model.h"

#include <ResourceHelpers.h>
#include <vfs++/vfs++.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

typedef std::vector<uint16_t> IndexBuffer;

struct md5_weight_t
{
    int         joint;
    float       bias;
    ml::vec3    location;
};

struct md5_vertex_t
{
	float   u, v;
	int     start;
	int     count;
};

struct MeshVertex
{
	float   px, py, pz;
	float   nx, ny, nz;
	float   u, v;
	float	w[4];
	uint8_t b[4];
};

typedef std::vector<md5_vertex_t>	MD5VertexList;
typedef std::vector<md5_weight_t>   WeightList;
typedef std::vector<MeshVertex>  	MeshVertexList;

void createMesh ( Mesh& mesh,
                  size_t numJoints,   ml::dual_quat* joints, 
                  size_t numVertices, md5_vertex_t*  md5Vertices,
                  size_t numWeights,  md5_weight_t*  md5Weights,
                  size_t numIndices,  uint16_t*      indices );
void destroyMesh( Mesh& mesh );

void build_w_quat( ml::quat& quat )
{
    float t = 1.0f - ( quat.x * quat.x ) - ( quat.y * quat.y ) - ( quat.z * quat.z );
    if ( t < 0.0f )
    {
        quat.w = 0.0f;
    }
    else
    {
        quat.w = -sqrtf(t);
    }
}

struct memory_t
{
    uint8_t* buffer;
    size_t   size;
};

bool mopen(memory_t* mem, const char* name)
{
    File	src = VFS::openRead(name);

    if (!src) return false;

    //explicit conversion to avoid warning on 32-bit system
    assert(src.size()<SIZE_MAX);

    mem->size   = (size_t)src.size();
    mem->buffer = (uint8_t*)malloc(mem->size+1);

    src.read(mem->buffer, mem->size, 1);

    src.close();

    return true;
}

void mclose(memory_t* mem)
{
    free(mem->buffer);
    mem->buffer = 0;
    mem->size = 0;
}

bool MD5Model::LoadModel(const char* name)
{
    memory_t mem;

    if (!mopen(&mem, name)) return false;
	
	char *line = strtok( ( char * )mem.buffer, "\n" );
	
	int int_val = 0;
	
	unsigned int mesh_index = 0;
	
	while( line )
	{
		if( sscanf( line, "MD5Version %d", &int_val ) == 1 )
		{
			if( int_val != 10 ) goto cleanup;
		}
		
		else if( sscanf( line, "numJoints %d", &mNumJoints ) == 1 )
		{
            mBoneHierarchy.resize(mNumJoints);
            mBindPoseDQ.resize(mNumJoints);
            mInvBindPoseDQ.resize(mNumJoints);
            mBoneTransformDQ.resize(mNumJoints);
		}
		
		else if( sscanf( line, "numMeshes %d", &mNumMeshes ) == 1 )
		{
            mMeshes.resize(mNumMeshes);
		}

		else if( !strncmp( line, "joints {", 8 ) )
		{
			char name[64];
            unsigned int i      =  0;
			int          parent = -1;

			line = strtok( NULL, "\n" );
			
			while( line[ 0 ] != '}' )
			{
                ml::quat      rotation;
                ml::vec3      location;

                if( sscanf( line,
							"%s %d ( %f %f %f ) ( %f %f %f )",
							 name,
							&parent,
							&location.x,
							&location.y,
							&location.z,
							&rotation.x,
							&rotation.y,
							&rotation.z ) == 8 )
				{
                    mBoneHierarchy[i] = parent;

                    build_w_quat( rotation );
				
                    ml::create_dual_quat   (&mBindPoseDQ[i], &rotation, &location);
                    ml::conjugate_dual_quat(&mInvBindPoseDQ[i], &mBindPoseDQ[i]);

                    ++i;
				}
				
				line = strtok( NULL, "\n" );
			}
		}
		
		else if( !strncmp( line, "mesh {", 6 ) )
		{
			md5_vertex_t    md5vertex;
			md5_weight_t    md5weight;
			uint16_t        md5triangle[3];
			char            shader[128];
		    MD5VertexList   vertices;
		    WeightList      weights;
		    IndexBuffer     indices;
			int             numVertices;
			int             numIndices;
			int             numWeights;

			line = strtok( NULL, "\n" );
			
			while( line[ 0 ] != '}' )
			{
				if( sscanf( line, " shader \"%[^\"]", shader ) == 1 ) goto next_mesh_line;

				else if( sscanf( line, " numverts %d", &numVertices ) == 1 )
				{
                    vertices.resize(numVertices);
				}
				
				else if( sscanf( line,
								 " vert %d ( %f %f ) %d %d",
								 &int_val,
								 &md5vertex.u,
								 &md5vertex.v,
								 &md5vertex.start,
								 &md5vertex.count ) == 5 )
				{
					memcpy( &vertices[ int_val ], &md5vertex, sizeof( md5vertex ) );
				}

				else if( sscanf( line, " numtris %d", &numIndices ) == 1 )
				{
                    numIndices *= 3;
                    indices.resize(numIndices);
				}
				
				else if( sscanf( line,
								 " tri %d %hu %hu %hu",
								 &int_val,
								 &md5triangle[ 0 ],
								 &md5triangle[ 1 ],
								 &md5triangle[ 2 ] ) == 4 )
				{
					memcpy( &indices[ int_val * 3 ], md5triangle, sizeof( md5triangle ) );
				}
				
				else if( sscanf( line, " numweights %d", &numWeights ) == 1 )
				{
                    weights.resize(numWeights);
				}
				
				else if( sscanf( line,
								 " weight %d %d %f ( %f %f %f )",
								 &int_val,
								 &md5weight.joint,
								 &md5weight.bias,
								 &md5weight.location.x,
								 &md5weight.location.y,
								 &md5weight.location.z ) == 6 )
								 
				{
					memcpy( &weights[ int_val ], &md5weight, sizeof( md5weight ) );
				}

next_mesh_line:
				line = strtok( NULL, "\n" );
			}
			
            createMesh( mMeshes[mesh_index],
                        mNumJoints,  &mBindPoseDQ[0],
                        numVertices, &vertices[0],
                        numWeights,  &weights[0],
                        numIndices,  &indices[0]);

            ++mesh_index;
		}

		line = strtok( NULL, "\n" );
	}

	mclose( &mem );
	
	return true;
	

cleanup:

	mclose( &mem );

	return false;
}

bool MD5Model::LoadAnim( const char *name )
{
    memory_t mem;

    if (!mopen(&mem, name)) return false;

	char *line = strtok( ( char * )mem.buffer, "\n" );
	
	int int_val = 0;
	
	while( line )
	{
		if( sscanf( line, "MD5Version %d", &int_val ) == 1 )
		{
			if( int_val != 10 ) goto cleanup;
		}
		
		else if( sscanf( line, "numFrames %d", &mNumFrames ) == 1 )
		{
            mFramePoses.resize(mNumFrames);

            unsigned int i = 0;
			
			while( i != mNumFrames )
			{
                mFramePoses[i].resize(mNumJoints);

                ++i;
			}
		}

		else if( sscanf( line, "numJoints %d", &int_val ) == 1 )
		{
			if( mNumJoints != int_val ) goto cleanup;
		}
		
		else if( sscanf( line, "frameRate %d", &int_val ) == 1 )
		{ 
            mFrameRate = int_val;
        }

		else if( sscanf( line, "frame %d", &int_val ) )
		{
			DualQuatList& frameJoints = mFramePoses[ int_val ];

			line = strtok( NULL, "\n" );
			
			unsigned int i = 0;
			
			while( i != mNumJoints )
			{
                ml::quat rotation;
                ml::vec3 location;

				if( sscanf( line,
							" %f %f %f %f %f %f",
							&location.x,
							&location.y,
							&location.z,
							&rotation.x,
							&rotation.y,
							&rotation.z ) == 6 )
				{
					build_w_quat( rotation );

                    ml::create_dual_quat(&frameJoints[i], &rotation, &location);

                    int parent = mBoneHierarchy[i];
				    if( parent > -1 )
				    {

                        ml::mul_dual_quat(&frameJoints[i], &frameJoints[parent], &frameJoints[i]);
                    }
				}
				
				line = strtok( NULL, "\n" );
				
				++i;
			}
		}
			
		line = strtok( NULL, "\n" );	
	}

    mHasAnimation = true;
    mPoseDQ.resize(mNumJoints);

	mclose( &mem );
	
	return true;
	
	
cleanup:

    mHasAnimation = false;
    mFramePoses.clear();

	mclose( &mem );
	
	return false;
}

void createMesh( Mesh& mesh,
                 size_t numJoints,   ml::dual_quat* joints, 
                 size_t numVertices, md5_vertex_t*  md5Vertices,
                 size_t numWeights,  md5_weight_t*  weights,
                 size_t numIndices,  uint16_t*      indices )
{
    MeshVertex* vertices;
    size_t      vertexDataSize;

    vertexDataSize = sizeof(MeshVertex)*numVertices;
    vertices = (MeshVertex*)malloc(vertexDataSize);
    memset(vertices, 0, vertexDataSize);

    for ( unsigned int i = 0; i < numVertices; ++i )
    {
        MeshVertex& vert        = vertices[i];
		int         weightCount = md5Vertices[i].count;
		int         startWeight = md5Vertices[i].start;

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
        v128        n;
        MeshVertex& vert = vertices[i];

        n = ml::load_vec3((ml::vec3*)&vert.nx);
        n = ml::normalize(n);
        ml::store_vec3((ml::vec3*)&vert.nx, n);
    }

    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ibo);

	glBindBuffer( GL_ARRAY_BUFFER, mesh.vbo );
	glBufferData( GL_ARRAY_BUFFER, vertexDataSize, vertices, GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh.ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * numIndices, indices, GL_STATIC_DRAW ); 

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	mesh.numIndices = numIndices;

    {
        GLuint err = glGetError();
        assert(err==GL_NO_ERROR);
    }

    free(vertices);
}

void destroyMesh( Mesh& mesh )
{
    if (mesh.vbo)
    {
        glDeleteBuffers(1, &mesh.vbo);
        mesh.vbo = 0;
    }
    if (mesh.ibo)
    {
        glDeleteBuffers(1, &mesh.ibo);
        mesh.ibo = 0;
    }
    if (mesh.texture)
    {
        glDeleteTextures(1, &mesh.texture);
        mesh.texture = 0;
    }
}

MD5Model::MD5Model()
: mNumJoints(0)
, mNumMeshes(0)
, mHasAnimation(false)
, mNumFrames( 0 )
, mFrameRate( 0 )
, mAnimTime( 0 )
{
}

MD5Model::~MD5Model()
{
    MeshList::iterator iter = mMeshes.begin();
    while ( iter != mMeshes.end() )
    {
        destroyMesh( *iter );
        ++iter;
    }
    mMeshes.clear();
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

        for ( int i = 0; i < mNumJoints; ++i )
        {
            qr0 = ml::load_quat(&mFramePoses[frame0][i].real);
            qd0 = ml::load_quat(&mFramePoses[frame0][i].dual);
            qr1 = ml::load_quat(&mFramePoses[frame1][i].real);
            qd1 = ml::load_quat(&mFramePoses[frame1][i].dual);

            if (vi_cmpx_lt(vi_dot4(qr0, qr1), vi_load_zero()))
            {
                qr1 = vi_neg(qr1);
                qd1 = vi_neg(qd1);
            }

            r  = vi_lerp(qr0, qr1, lerpK);
            n  = ml::length_quat(r);
            r  = vi_div(r, n);
            ml::store_quat(&mPoseDQ[i].real, r);

            r  = vi_lerp(qd0, qd1, lerpK);
            r  = vi_div(r, n);
            ml::store_quat(&mPoseDQ[i].dual, r);

            ml::mul_dual_quat(&mBoneTransformDQ[i], &mPoseDQ[i], &mInvBindPoseDQ[i]);
        }
    }
    else
    {
        // No animation. Just use identity quaternions for each bone.
	    mBoneTransformDQ.assign(mNumJoints, ml::identity_dual_quat);
    }
}

void MD5Model::Render(float* MVP)
{
	glUseProgram(skin4PointShader.program);
	glUniformMatrix4fv(skin4PointShader.uMVP, 1, false, MVP);
    for ( unsigned int i = 0; i < mMeshes.size(); ++i )
    {
        RenderMesh( mMeshes[i] );
    }

    glUseProgram(0);
    RenderSkeleton(mPoseDQ);
}

void MD5Model::RenderMesh( Mesh& mesh )
{
    glUniformMatrix2x4fv(skin4PointShader.uBoneDualQuat, mNumJoints, false, &mBoneTransformDQ[0].real.x);

    // Position data
	glBindBuffer( GL_ARRAY_BUFFER, mesh.vbo );

	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), BUFFER_OFFSET(offsetof(MeshVertex, px)));
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), BUFFER_OFFSET(offsetof(MeshVertex, nx)));
	glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), BUFFER_OFFSET(offsetof(MeshVertex, u)));
	glVertexAttribIPointer(3, 4, GL_UNSIGNED_BYTE,   sizeof(MeshVertex), BUFFER_OFFSET(offsetof(MeshVertex, b)));
	glVertexAttribPointer (4, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), BUFFER_OFFSET(offsetof(MeshVertex, w)));

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

void MD5Model::RenderSkeleton( DualQuatList& joints )
{
    glPointSize( 5.0f );
    glColor3f( 1.0f, 0.0f, 0.0f );
    
    glPushAttrib( GL_ENABLE_BIT );

    glDisable(GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    // Draw the joint positions
    glBegin( GL_POINTS );
    {
        for ( unsigned int i = 0; i < joints.size(); ++i )
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
        for ( unsigned int i = 0; i < joints.size(); ++i )
        {
            ml::vec3 pos0, pos1;

            ml::get_translation_from_dual_quat(&pos0, &joints[i]);
            const int    parent = mBoneHierarchy[i];
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

void Skinning4PointShader::Create()
{
	assert(glGetError()==GL_NO_ERROR);

    program = resources::createProgramFromFiles("Skinning4.vert", "SHLighting.frag");//CreateProgram("Resources/shaders/Skinning4.vert", "Resources/shaders/SHLighting.frag");

	assert(glGetError()==GL_NO_ERROR);

	uTexture       = glGetUniformLocation(program, "uTexture");
	uBoneMatrix    = glGetUniformLocation(program, "uBoneMatrix");
	uBoneDualQuat  = glGetUniformLocation(program, "uBoneDualQuat");
	uMVP           = glGetUniformLocation(program, "uMVP");
}

void Skinning4PointShader::Destroy()
{
	glDeleteProgram(program);
}
