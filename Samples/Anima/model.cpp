#include "model.h"

#include "md5.h"
#include "mjson.h"

#define MAX_BONES 128

#define UNI_GLOBAL   0
#define UNI_BONES    1
#define UNI_LIGHTING 2

namespace Model
{

    GLuint prgLighting;
    GLuint prgDefault;
    material_t* mWireframe;

    gfx::ubo_desc_t ubufLighting;
    gfx::ubo_desc_t ubufGlobal;

    struct mesh_t
    {
        GLuint    vbo;
        GLuint    ibo;
        GLenum    idxFormat;
        GLsizei   idxOffset;
        GLsizei   numIndices;
    };

    struct material_t
    {
        GLuint          program;
        GLuint          diffuse;
        GLuint          normal;
    };

    void RenderSkeleton(int numJoints, int* hierarchy, ml::dual_quat* joints);

    void destroyMesh(mesh_t* mesh);

    bool loadMaterial   (material_t* mat, const char* name);
    void destroyMaterial(material_t* mat);

    void init()
    {
        prgDefault = res::createProgramFromFiles("MESH.Wireframe.Skinning4.vert", "MESH.Wireframe.geom", "MESH.Wireframe.SHLighting.frag");

        mWireframe = (material_t*)malloc(sizeof(material_t));
        mWireframe->program = prgDefault;
        mWireframe->diffuse = mWireframe->normal = 0;

        prgLighting = res::createProgramFromFiles("MESH.Skinning4.vert", "MESH.SHLighting.frag");

        ubufLighting = gfx::createUBODesc(prgDefault, "uniLighting");
        ubufGlobal   = gfx::createUBODesc(prgDefault, "uniGlobal");

#ifdef _DEBUG
        {
            GLint structSize;
            GLint uniGlobal, uniLighting;

            uniGlobal    = glGetUniformBlockIndex(prgDefault, "uniGlobal");
            uniLighting  = glGetUniformBlockIndex(prgDefault, "uniLighting");

            glGetActiveUniformBlockiv(prgDefault, uniGlobal,   GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
            assert(structSize==20*sizeof(float));
            glGetActiveUniformBlockiv(prgDefault, uniLighting, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
            assert(structSize==10*sizeof(v128));

            uniGlobal   = glGetUniformBlockIndex(prgLighting, "uniGlobal");
            uniLighting = glGetUniformBlockIndex(prgLighting, "uniLighting");

            glGetActiveUniformBlockiv(prgLighting, uniGlobal,   GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
            assert(structSize==20*sizeof(float));
            glGetActiveUniformBlockiv(prgLighting, uniLighting, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
            assert(structSize==10*sizeof(v128));
        }
#endif
    }

    void fini()
    {
        glDeleteProgram(prgLighting);
        glDeleteProgram(prgDefault);

        gfx::destroyUBODesc(ubufLighting);
        gfx::destroyUBODesc(ubufGlobal);

        free(mWireframe);
    }

    static ml::quat rotx = {-0.7071067812f, 0.0f, 0.0f, 0.7071067812f};

    void md5CreateSkeleton(skeleton_t* skel, int numJoints, md5_joint_t* joints)
    {
        skel->numJoints     = numJoints;
        skel->boneHierarchy = (int*)          malloc(skel->numJoints*sizeof(int));
        skel->bindPose      = (ml::dual_quat*)malloc(skel->numJoints*sizeof(ml::dual_quat));
        skel->invBindPose   = (ml::dual_quat*)malloc(skel->numJoints*sizeof(ml::dual_quat));

        int*            hierarchy   = skel->boneHierarchy;
        ml::dual_quat*  bindPose    = skel->bindPose;
        ml::dual_quat*  invBindPose = skel->invBindPose;

        while (numJoints--)
        {
            *hierarchy = joints->parent;

            ml::make_dual_quat   (bindPose, &joints->rotation, &joints->location);

            ml::mul_quat           (&bindPose->real, &rotx, &bindPose->real);
            ml::mul_quat           (&bindPose->dual, &rotx, &bindPose->dual);
            ml::conjugate_dual_quat(invBindPose, bindPose);

            ++joints;
            ++bindPose;
            ++invBindPose;
            ++hierarchy;
        }
    }

    void md5CreateMesh(mesh_t* mesh, md5_mesh_t* md5Mesh, skeleton_t* skel)
    {
        vf::skinned_geom_t* vertices;
        size_t         vertexDataSize;

        vertexDataSize = sizeof(vf::skinned_geom_t)*md5Mesh->numVertices;
        vertices = (vf::skinned_geom_t*)malloc(vertexDataSize);
        memset(vertices, 0, vertexDataSize);

        for ( int i = 0; i < md5Mesh->numVertices; ++i )
        {
            vf::skinned_geom_t& vert        = vertices[i];
            int                 weightCount = md5Mesh->vertices[i].count;
            int                 startWeight = md5Mesh->vertices[i].start;

            assert( weightCount <= 4 );

            v128 pos;

            pos = vi_set_0000();

            // Sum the position of the weights
            for ( int j = 0; j < weightCount; ++j )
            {
                md5_weight_t&  weight = md5Mesh->weights[startWeight + j];
                ml::dual_quat& joint  = skel->bindPose[weight.joint];

                v128 r, d, v, t;

                r = vi_loadu_v4(&joint.real);
                d = vi_loadu_v4(&joint.dual);
                v = vi_load_v3(&weight.location);

                v = ml::rotate_vec3_quat(r, v);
                t = ml::translation_dual_quat(r, d);
                t = vi_add(v, t);

                pos = vi_mad(t, vi_set_ffff(weight.bias), pos);

                assert(weight.joint<256);

                vert.b[j] = (uint8_t)weight.joint;
                vert.w[j] = weight.bias;
            }

            vi_store_v3(&vert.px, pos);
        }

        // Loop through all triangles and calculate the normal of each triangle
        for ( int i = 0; i < md5Mesh->numIndices/3; ++i )
        {
            v128 v0, v1, v2;
            v128 n0, n1, n2;
            v128 n;

            v0 = vi_load_v3(&vertices[ md5Mesh->indices[i*3+0] ].px);
            v1 = vi_load_v3(&vertices[ md5Mesh->indices[i*3+1] ].px);
            v2 = vi_load_v3(&vertices[ md5Mesh->indices[i*3+2] ].px);

            n = vi_cross3(vi_sub(v2, v0), vi_sub(v1, v0));

            n0 = vi_load_v3(&vertices[ md5Mesh->indices[i*3+0] ].nx);
            n1 = vi_load_v3(&vertices[ md5Mesh->indices[i*3+1] ].nx);
            n2 = vi_load_v3(&vertices[ md5Mesh->indices[i*3+2] ].nx);

            n0 = vi_add(n0, n);
            n1 = vi_add(n1, n);
            n2 = vi_add(n2, n);

            vi_store_v3(&vertices[ md5Mesh->indices[i*3+0] ].nx, n0);
            vi_store_v3(&vertices[ md5Mesh->indices[i*3+1] ].nx, n1);
            vi_store_v3(&vertices[ md5Mesh->indices[i*3+2] ].nx, n2);
        }

        // Now normalize all the normals
        for ( int i = 0; i < md5Mesh->numVertices; ++i )
        {
            v128                n;
            vf::skinned_geom_t& vert = vertices[i];

            n = vi_load_v3(&vert.nx);
            n = ml::normalize(n);
            vi_store_v3(&vert.nx, n);
        }

        // Copy texture coordinates
        for ( int i = 0; i < md5Mesh->numVertices; ++i )
        {
            vertices[i].u = md5Mesh->vertices[i].u;
            vertices[i].v = md5Mesh->vertices[i].v;
        }

        glGenBuffers(1, &mesh->vbo);
        glGenBuffers(1, &mesh->ibo);

        glNamedBufferStorageEXT( mesh->vbo, vertexDataSize, vertices, 0 );
        glNamedBufferStorageEXT( mesh->ibo, sizeof(uint16_t) * md5Mesh->numIndices, md5Mesh->indices, 0 ); 

        mesh->numIndices = md5Mesh->numIndices;
        mesh->idxFormat  = GL_UNSIGNED_SHORT;
        mesh->idxOffset  = 0;

        free(vertices);
    }

    void md5CreateAnimation( animation_t* anim, md5_anim_t* md5Anim, skeleton_t* skel )
    {
        anim->numFrames = md5Anim->numFrames;
        anim->frameRate = md5Anim->frameRate;

        anim->framePoses = (ml::dual_quat*)malloc(md5Anim->numFrames * skel->numJoints * sizeof(ml::dual_quat));

        int              numFrames  = md5Anim->numFrames;
        int*             hierarchy  = skel->boneHierarchy;
        md5_anim_data_t* animData   = md5Anim->frameData;
        ml::dual_quat*   framePoses = anim->framePoses;

        while(numFrames--)
        {
            for (int i=0; i<skel->numJoints; ++i)
            {
                ml::make_dual_quat(&framePoses[i], &animData[i].rotation, &animData[i].location);

                int parent = hierarchy[i];
                if( parent > -1 )
                {
                    ml::mul_dual_quat(&framePoses[i], &framePoses[parent], &framePoses[i]);
                }
                else
                {
                    ml::mul_quat(&framePoses[i].real, &rotx, &framePoses[i].real);
                    ml::mul_quat(&framePoses[i].dual, &rotx, &framePoses[i].dual);
                }
            }

            framePoses += skel->numJoints;
            animData   += skel->numJoints;
        }
    }
    
    bool loadModel(const char* name, model_t* model, skeleton_t* skel)
    {
        memory_t inText    = {0, 0, 0};
        memory_t outBinary = {0, 0, 0};

        memset(model, 0, sizeof(model_t   ));
        memset(skel,  0, sizeof(skeleton_t));

        if (mem_area(&outBinary, 4*1024*1024) &&
            mem_file(&inText, name)           &&
            md5meshConvertToBinary(&inText, &outBinary))
        {
            mem_free(&inText);

            md5_model_t* md5Model = (md5_model_t*)outBinary.buffer;

            model->numMeshes = md5Model->numMeshes;
            model->meshes    = (mesh_t*)    malloc(model->numMeshes*sizeof(mesh_t));
            model->materials = (material_t*)malloc(model->numMeshes*sizeof(material_t));

            md5CreateSkeleton(skel, md5Model->numJoints, md5Model->joints);

            md5_mesh_t* md5Mesh = md5Model->meshes;
            for (int i=0; i<model->numMeshes; ++i)
            {
                md5CreateMesh(&model->meshes[i], md5Mesh, skel);

                model->materials[i] = *mWireframe;
                loadMaterial(&model->materials[i], md5Mesh->shader);
                if (model->materials[i].diffuse && model->materials[i].normal) model->materials[i].program = prgLighting;

                ++md5Mesh;
            }

            mem_free(&outBinary);

            return true;
        }

        mem_free(&inText);
        mem_free(&outBinary);

        return false;
    }

    bool loadAnimation(const char *name, animation_t* anim, skeleton_t* skel)
    {
        memory_t inText    = {0, 0, 0};
        memory_t outBinary = {0, 0, 0};

        memset(anim, 0, sizeof(animation_t));

        if (mem_file(&inText, name)            &&
            mem_area(&outBinary, 4*1024*1024)  &&
            md5animConvertToBinary(&inText, &outBinary))
        {
            mem_free( &inText );

            md5_anim_t* md5Anim = (md5_anim_t*)outBinary.buffer;

            if (skel->numJoints != md5Anim->numJoints) goto cleanup;


            md5CreateAnimation(anim, md5Anim, skel);

            mem_free(&outBinary);

            return true;
        }

cleanup:
        mem_free(&inText);
        mem_free(&outBinary);

        return false;
    }

    void update(float fDeltaTime, animation_t* anim, skeleton_t* skel, pose_t* pose)
    {
        if ( anim->numFrames > 1)
        {
            int   frame0, frame1;
            float lerpK;

            pose->animTime += fDeltaTime * (float)anim->frameRate;
            pose->animTime  = ml::mod(pose->animTime, (float)anim->numFrames);

            frame0 = (int)ml::floor( pose->animTime );
            frame0 = frame0 % anim->numFrames;
            frame1 = frame0 + 1;
            frame1 = frame1 % anim->numFrames;

            lerpK  = pose->animTime - ml::floor(pose->animTime);

            // I assume there is no zero quaternions in orientation data
            // so I do not check whether real quaternion norm is zero
            v128 qr0, qd0, qr1, qd1, r, n;

            ml::dual_quat* frame0Data = anim->framePoses + frame0*skel->numJoints; 
            ml::dual_quat* frame1Data = anim->framePoses + frame1*skel->numJoints; 

            for ( int i = 0; i < skel->numJoints; ++i )
            {
                qr0 = vi_loadu_v4(&frame0Data[i].real);
                qd0 = vi_loadu_v4(&frame0Data[i].dual);
                qr1 = vi_loadu_v4(&frame1Data[i].real);
                qd1 = vi_loadu_v4(&frame1Data[i].dual);

                if (vi_cmpx_lt(vi_dot4(qr0, qr1), vi_set_0000()))
                {
                    qr1 = vi_neg(qr1);
                    qd1 = vi_neg(qd1);
                }

                r  = vi_lerp(qr0, qr1, lerpK);
                n  = ml::length_quat(r);
                r  = vi_div(r, n);
                vi_storeu_v4(&pose->pose[i].real, r);

                r  = vi_lerp(qd0, qd1, lerpK);
                r  = vi_div(r, n);
                vi_storeu_v4(&pose->pose[i].dual, r);

                ml::mul_dual_quat(&pose->boneTransforms[i], &pose->pose[i], &skel->invBindPose[i]);
            }
        }
        else
        {
            // No animation. Just use identity quaternions for each bone.
            ml::dual_quat* bone = pose->boneTransforms;
            for (int i = 0; i < skel->numJoints; ++i, ++bone)
            {
                ml::set_identity_dual_quat(bone);
            }
        }
    }

    void renderMesh(mesh_t* mesh, material_t* material)
    {
        glUseProgram(material->program);

        // Position data
        glBindVertexArray(vf::skinned_geom_t::vao);
        glBindVertexBuffer(0, mesh->vbo, 0, sizeof(vf::skinned_geom_t));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);

        GLuint textureSet[2] = {material->diffuse, material->normal};
        glBindTextures(0, 2, textureSet);

        // Draw mesh from index buffer
        glDrawElements(GL_TRIANGLES, mesh->numIndices, mesh->idxFormat, BUFFER_OFFSET(mesh->idxOffset));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void renderSkeleton(int numJoints, int* hierarchy, ml::dual_quat* joints)
    {
        GLuint  offset;
        GLuint  size;
        v128*   vertices;

        glDisable( GL_DEPTH_TEST );

        size     = sizeof(gfx::line_t) * (numJoints - 1);
        vertices = (v128*)gfx::dynbufAllocMem(size, gfx::caps.ssboAlignment, &offset);
        for ( int i = 0; i < numJoints; ++i )
        {
            const int parent = hierarchy[i];
            if ( parent != -1 )
            {
                *vertices++ = ml::translation_dual_quat(vi_loadu_v4(&joints[parent].real), vi_loadu_v4(&joints[parent].dual));
                *vertices++ = ml::translation_dual_quat(vi_loadu_v4(&joints[i].real), vi_loadu_v4(&joints[i].dual));
            }
        }
        gfx::drawLines(vi_set(0.0f, 1.0f, 0.0f, 1.0f), numJoints - 1, gfx::dynBuffer, offset, size);

        size     = sizeof(gfx::point_t) * numJoints;
        vertices = (v128*)gfx::dynbufAllocMem(size, gfx::caps.ssboAlignment, &offset);
        for ( int i = 0; i < numJoints; ++i )
        {
            *vertices++ = ml::translation_dual_quat(vi_loadu_v4(&joints[i].real), vi_loadu_v4(&joints[i].dual));
        }
        gfx::drawPoints(5.0f, vi_set(1.0f, 0.0f, 0.0f, 1.0f), numJoints, gfx::dynBuffer, offset, size);

        glEnable(GL_DEPTH_TEST);
    }

    void render(model_t* model, skeleton_t* skel, pose_t* pose)
    {
        GLuint offsetGlobal, offsetBones, offsetLighting;
        
        GLsizeiptr sizeGlobal   = 20*sizeof(float);
        GLsizeiptr sizeLighting = 10*sizeof(v128);
        GLsizeiptr sizeBones    = skel->numJoints*sizeof(ml::dual_quat);

        void* memGlobal   = gfx::dynbufAllocMem(sizeGlobal,   gfx::caps.uboAlignment,  &offsetGlobal);
        void* memLighting = gfx::dynbufAllocMem(sizeLighting, gfx::caps.uboAlignment,  &offsetLighting);
        void* memBones    = gfx::dynbufAllocMem(sizeBones,    gfx::caps.ssboAlignment, &offsetBones);

        memcpy(memBones, &pose->boneTransforms[0].real.x, sizeBones);
        gfx::updateUBO(ubufGlobal,   memGlobal,   sizeGlobal);
        gfx::updateUBO(ubufLighting, memLighting, sizeLighting);

        glBindBufferRange(GL_UNIFORM_BUFFER,        UNI_GLOBAL,   gfx::dynBuffer, offsetGlobal,   sizeGlobal);
        glBindBufferRange(GL_UNIFORM_BUFFER,        UNI_LIGHTING, gfx::dynBuffer, offsetLighting, sizeLighting);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, UNI_BONES,    gfx::dynBuffer, offsetBones,    sizeBones);

        for (int i=0; i<model->numMeshes; ++i)
        {
            renderMesh( &model->meshes[i], &model->materials[i] );
        }

        renderSkeleton(skel->numJoints, skel->boneHierarchy, pose->pose);
    }

    void createPose(pose_t* pose, skeleton_t* skel)
    {
        memset(pose, 0, sizeof(pose_t));

        pose->boneTransforms = (ml::dual_quat*) malloc(skel->numJoints * sizeof(ml::dual_quat));
        pose->pose           = (ml::dual_quat*) malloc(skel->numJoints * sizeof(ml::dual_quat));
    }

    bool loadMaterial(material_t* mat, const char* name)
    {
        memory_t        inText = {0, 0, 0};
        memory_t        bjson  = {0, 0, 0};
        mjson_element_t root = 0;
        char            path[1024];

        strcpy_s(path, name);
        strcat_s(path, ".material");

        if (mem_file(&inText, path))
        {
            mem_area(&bjson, 10*1024);
            int result = mjson_parse((const char*)inText.buffer, inText.size, bjson.buffer, bjson.size, &root);
            
            assert(result && mjson_get_type(root) == MJSON_ID_DICT32);

            mjson_element_t key, value;
            
            key = mjson_get_member_first(root, &value);
            while (key)
            {
                const char* name = mjson_get_string(key, "");

                if (strcmp(name, "diffuse")==0)
                {
                    mat->diffuse = res::createTexture2D(mjson_get_string(value, ""));
                }
                else if (strcmp(name, "normal")==0)
                {
                    mat->normal = res::createTexture2D(mjson_get_string(value, ""));
                }

                key = mjson_get_member_next(root, key, &value);
            }

            mem_free(&inText);
            mem_free(&bjson);

            return true;
        }

        return false;
    }

    void destroyMaterial(material_t* mat)
    {
        if (mat->diffuse)
        {
            glDeleteTextures(1, &mat->diffuse);
        }

        if (mat->normal)
        {
            glDeleteTextures(1, &mat->normal);
        }

        memset(mat, 0, sizeof(material_t));
    }

    void destroyMesh(mesh_t* mesh)
    {
        if (mesh->vbo)
        {
            glDeleteBuffers(1, &mesh->vbo);
        }

        if (mesh->ibo)
        {
            glDeleteBuffers(1, &mesh->ibo);
        }

        memset(mesh, 0, sizeof(mesh_t));
    }

    void destroyModel(model_t* model)
    {
        for (int i = 0; i < model->numMeshes; ++i)
        {
            destroyMesh    (&model->meshes   [i]);
            destroyMaterial(&model->materials[i]);
        }

        if (model->meshes   ) free(model->meshes   );
        if (model->materials) free(model->materials);

        memset(model, 0, sizeof(model_t));
    }

    void destroySkeleton(skeleton_t* skel)
    {
        if (skel->boneHierarchy) free(skel->boneHierarchy);
        if (skel->bindPose     ) free(skel->bindPose     );
        if (skel->invBindPose  ) free(skel->invBindPose  );

        memset(skel, 0, sizeof(skeleton_t));
    }

    void destroyAnimation(animation_t* anim)
    {
        if (anim->framePoses) free(anim->framePoses);

        memset(anim, 0, sizeof(animation_t));
    }

    void destroyPose(pose_t* pose)
    {
        if (pose->boneTransforms) free(pose->boneTransforms);
        if (pose->pose          ) free(pose->pose          );

        memset(pose, 0, sizeof(pose_t));
    }
}