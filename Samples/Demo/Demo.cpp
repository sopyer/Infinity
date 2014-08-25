#include <fwk/fwk.h>

#include "tiny_obj_loader.h"
#include <set>

#include <mjson.h>

#define MAX_MESHES     2048
#define MAX_MODELS      512
#define MAX_MATERIALS    64

struct mesh_header_v0
{
    uint32_t magic;
    uint32_t version;
    uint32_t numVertices;
    uint32_t numIndices;

    float minx, miny, minz;
    float maxx, maxy, maxz;

    uint32_t numSubmeshes;
    uint32_t offsetSize[16*2];
};

#define MESH_FILE_MAGIC 'HSEM'

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
    GLuint  program;
    GLuint  diffuseMap;
    GLuint  specularMap;
    GLuint  normalMap;
    float   diffuse [4];
    float   specular[4];
    float   emissive[4];
    float   specPow;
    float   fresnel;
};

struct model_t
{
    GLuint       vbo;
    GLuint       ibo;
    float        minx, miny, minz;
    float        maxx, maxy, maxz;
    GLenum       idxFormat;
    uint32_t     numSubmeshes;
    uint32_t     offsetSize[16*2];
    material_t*  materials[16];
};

namespace app
{
    SpectatorCamera     camera;
    v128                proj[4];

    cpu_timer_t        cpuTimer;
    gfx::gpu_timer_t   gpuTimer;

    void loadMaterials();
    void loadModels();
    void destroyMaterials();
    void destroyModels();
    void renderModels();
    void convertOBJ();

    mem::arena_t appArena;

    void init()
    {
        appArena = mem::create_arena(1024*1024, 0);

        //convertOBJ();

        loadMaterials();
        loadModels();

        camera.acceleration.x = camera.acceleration.y = camera.acceleration.z = 150;
        camera.maxVelocity.x  = camera.maxVelocity.y  = camera.maxVelocity.z  =  60;

        gfx::gpu_timer_init(&gpuTimer);
    }

    void fini()
    {
        destroyModels();
        destroyMaterials();
        gfx::gpu_timer_fini(&gpuTimer);
        mem::destroy_arena(appArena);
    }

    void render()
    {
        gpu_timer_start(&gpuTimer);
        cpu_timer_start(&cpuTimer);

        v128 m[4];
        camera.getViewMatrix(m);
        memcpy(&gfx::autoVars.matMV, (float*)m, sizeof(float) * 16);

        glEnable(GL_FRAMEBUFFER_SRGB);

        gfx::drawXZGrid(-500.0f, -500.0f, 500.0f, 500.0f, 40, vi_set(0.0f, 1.0f, 0.0f, 1.0f));

        renderModels();

        glDisable(GL_FRAMEBUFFER_SRGB);

        ui::displayStats(
            10.0f, 10.0f, 300.0f, 70.0f,
            cpu_timer_measured(&cpuTimer) / 1000.0f,
            gfx::gpu_timer_measured(&gpuTimer) / 1000.0f
        );

        cpu_timer_stop(&cpuTimer);
        gpu_timer_stop(&gpuTimer);
    }

    void update(float dt)
    {
        ui::processCameraInput(&camera, dt);
    }

    void recompilePrograms() {}

    void resize(int width, int height)
    {
        ml::make_perspective_mat4(proj, 30.0f * FLT_DEG_TO_RAD_SCALE, (float)width/(float)height, 0.1f, 10000.0f);

        gfx::autoVars.projParams.x = proj[0].m128_f32[0];
        gfx::autoVars.projParams.y = proj[1].m128_f32[1];
        gfx::autoVars.projParams.z = proj[2].m128_f32[2];
        gfx::autoVars.projParams.w = proj[3].m128_f32[2];
    }

    size_t  numModels;
    size_t  numMeshes;
    size_t  numMaterials;

    model_t      models       [MAX_MODELS];
    mesh_t       meshes       [MAX_MESHES];
    material_t*  materialRefs [MAX_MESHES];
    material_t   materials    [MAX_MATERIALS];
    const char*  materialNames[MAX_MATERIALS];

    GLuint prgStatic;
    GLuint prgStaticAT;
    GLuint prgStaticDiffuse;

    void loadModel(const char* name, int& first, int& count);
    material_t* findMaterial(const char* name);

    void loadModels()
    {
        numModels = 0;
        numMeshes = 0;

        memory_t        inText = {0, 0, 0};
        memory_t        bjson  = {0, 0, 0};
        mjson_element_t root = 0;

        if (mem_file(&inText, "models/sponza/sponza.models"))
        {
            mem_area(&bjson, 100*1024);
            int result = mjson_parse((const char*)inText.buffer, inText.size, bjson.buffer, bjson.size, &root);

            assert(result && mjson_get_type(root) == MJSON_ID_ARRAY32);

            mjson_element_t modelDesc, key, value;
            mjson_element_t matList, mat;

            modelDesc = mjson_get_element_first(root);
            while (modelDesc)
            {
                const char* model = 0;
                const char* material = 0;

                assert(mjson_get_type(modelDesc) == MJSON_ID_DICT32);

                key = mjson_get_member_first(modelDesc, &value);
                while (key)
                {
                    const char* name = mjson_get_string(key, 0);

                    if (strcmp(name, "model")==0)
                    {
                        model = mjson_get_string(value, 0);
                    }
                    else if (strcmp(name, "materials")==0)
                    {
                        matList = value;
                    }

                    key = mjson_get_member_next(modelDesc, key, &value);
                }

                int first, count;
                loadModel(model, first, count);
                mat = mjson_get_element_first(matList);
                for (int i = 0; i<count; ++i)
                {
                    materialRefs[first+i] = findMaterial(mjson_get_string(mat, ""));
                    mat = mjson_get_element_next(matList, mat);
                }

                modelDesc = mjson_get_element_next(root, modelDesc);
            }

            mem_free(&inText);
            mem_free(&bjson);
        }
    }

    void readVector(mjson_element_t array, size_t num, float* vec)
    {
        mjson_element_t el = mjson_get_element_first(array);
        for (size_t i=0; i<num && el; ++i)
        {
            vec[i] = mjson_get_float(el, 0.0f);
            el = mjson_get_element_next(array, el);
        }
    }

    void loadMaterials()
    {
        prgStatic        = res::createProgramFromFiles("MESH.Static.vert", "MESH.Texture.frag");
        prgStaticAT      = res::createProgramFromFiles("MESH.Static.vert", "MESH.AT.Texture.frag");
        prgStaticDiffuse = res::createProgramFromFiles("MESH.Static.vert", "MESH.Diffuse.frag");

        numMaterials = 0;

        memory_t        inText = {0, 0, 0};
        memory_t        bjson  = {0, 0, 0};
        mjson_element_t root = 0;

        if (mem_file(&inText, "models/sponza/sponza.mtllib"))
        {
            mem_area(&bjson, 100*1024);
            int result = mjson_parse((const char*)inText.buffer, inText.size, bjson.buffer, bjson.size, &root);

            assert(result && mjson_get_type(root) == MJSON_ID_DICT32);

            mjson_element_t material, dict, key, value;

            material = mjson_get_member_first(root, &dict);
            while (material)
            {
                const char* matName =  mjson_get_string(material, 0);
                const char* dmap = 0;
                const char* nmap = 0;
                const char* smap = 0;
                int mask = FALSE;

                size_t len = strlen(matName);
                materialNames[numMaterials] = (const char*)mem::alloc(appArena, len+1);
                strcpy((char*)materialNames[numMaterials], matName);

                assert(mjson_get_type(dict) == MJSON_ID_DICT32);

                key = mjson_get_member_first(dict, &value);
                while (key)
                {
                    const char* name = mjson_get_string(key, 0);

                    if (strcmp(name, "DiffuseMap")==0)
                    {
                        dmap = mjson_get_string(value, 0);
                    }
                    else if (strcmp(name, "NormalMap")==0)
                    {
                        nmap = mjson_get_string(value, 0);
                    }
                    else if (strcmp(name, "SpecularMap")==0)
                    {
                        smap = mjson_get_string(value, 0);
                    }
                    else if (strcmp(name, "AlphaTest")==0)
                    {
                        mask = mjson_get_bool(value, FALSE);
                    }
                    else if (strcmp(name, "DiffuseColor")==0)
                    {
                        readVector(value, 4, materials[numMaterials].diffuse);
                    }
                    else if (strcmp(name, "SpecularColor")==0)
                    {
                        readVector(value, 3, materials[numMaterials].specular);
                    }
                    else if (strcmp(name, "EmissiveColor")==0)
                    {
                        readVector(value, 3, materials[numMaterials].emissive);
                    }
                    else if (strcmp(name, "SpecPow")==0)
                    {
                        materials[numMaterials].specPow = mjson_get_float(value, 0.0f);
                    }
                    else if (strcmp(name, "Fresnel")==0)
                    {
                        materials[numMaterials].fresnel = mjson_get_float(value, 0.0f);
                    }

                    key = mjson_get_member_next(dict, key, &value);
                }

                materials[numMaterials].program = mask ? prgStaticAT : prgStatic;
                if (dmap)
                {
                    materials[numMaterials].diffuseMap = res::createTexture2D(dmap, TRUE);
                    glTextureParameteriEXT(materials[numMaterials].diffuseMap, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTextureParameteriEXT(materials[numMaterials].diffuseMap, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                }
                else
                {
                    materials[numMaterials].diffuseMap = 0;
                    materials[numMaterials].program = prgStaticDiffuse;
                }

                if (nmap)
                {
                    materials[numMaterials].normalMap = res::createTexture2D(nmap);
                    glTextureParameteriEXT(materials[numMaterials].normalMap, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTextureParameteriEXT(materials[numMaterials].normalMap, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                }
                else
                    materials[numMaterials].normalMap = 0;

                if (smap)
                {
                    materials[numMaterials].specularMap = res::createTexture2D(smap);
                    glTextureParameteriEXT(materials[numMaterials].specularMap, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTextureParameteriEXT(materials[numMaterials].specularMap, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                }
                else
                    materials[numMaterials].specularMap = 0;

                ++numMaterials;
                material = mjson_get_member_next(root, material, &dict);
            }

            mem_free(&inText);
            mem_free(&bjson);
        }
    }

    void destroyMaterials()
    {
        for (size_t i=0; i < numMaterials; ++i)
        {
            if (materials[i].diffuseMap)
            {
                glDeleteTextures(1, &materials[i].diffuseMap);
            }
            if (materials[i].specularMap)
            {
                glDeleteTextures(1, &materials[i].specularMap);
            }
            if (materials[i].normalMap)
            {
                glDeleteTextures(1, &materials[i].normalMap);
            }
            mem::free(appArena, (void*)materialNames[i]);
        }
        glDeleteProgram(prgStatic);
        glDeleteProgram(prgStaticAT);
        glDeleteProgram(prgStaticDiffuse);
        memset(materials,     0, sizeof(material_t)*MAX_MATERIALS);
        memset(materialNames, 0, sizeof(const char*)*MAX_MATERIALS);
    }

    void destroyModels()
    {
        for (size_t i=0; i < numModels; ++i)
        {
            if (models[i].vbo)
            {
                glDeleteBuffers(1, &models[i].vbo);
            }

            if (models[i].ibo)
            {
                glDeleteBuffers(1, &models[i].ibo);
            }
        }
        memset(meshes, 0, sizeof(mesh_t)*MAX_MESHES);
        memset(models, 0, sizeof(model_t)*MAX_MODELS);
    }

    void renderMesh(mesh_t* mesh, material_t* material)
    {
        glUseProgram(material->program);

        // Position data
        glBindVertexArray(vf::static_geom_t::vao);
        glBindVertexBuffer(0, mesh->vbo, 0, sizeof(vf::static_geom_t));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);

        GLuint textureSet[3] = {material->diffuseMap, material->specularMap, material->normalMap};
        glBindTextures(0, ARRAY_SIZE(textureSet), textureSet);

        // Draw mesh from index buffer
        glDrawElements(GL_TRIANGLES, mesh->numIndices, mesh->idxFormat, BUFFER_OFFSET(mesh->idxOffset));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void renderModels()
    {
        gfx::setStdTransforms();

        for (size_t i=0; i<numMeshes; ++i)
        {
            renderMesh( &meshes[i], materialRefs[i] );
        }
    }

    material_t* findMaterial(const char* name)
    {
        for (size_t i=0; i<numMaterials; ++i)
        {
            if (strcmp(name, materialNames[i])==0)
                return &materials[i];
        }
        assert(0);
        return 0;
    }

    void loadModel(const char* name, int& first, int& count)
    {
        memory_t  data = {0, 0, 0};


        if (mem_file(&data, name))
        {
            mesh_header_v0*    header   = mem_raw_data<mesh_header_v0>(&data);
            vf::static_geom_t* vertices = mem_raw_array<vf::static_geom_t>(&data, header->numVertices);
            uint32_t*          indices  = mem_raw_array<uint32_t>(&data, header->numIndices);

            glGenBuffers(1, &models[numModels].vbo);
            glGenBuffers(1, &models[numModels].ibo);

            glNamedBufferStorageEXT( models[numModels].vbo, sizeof(vf::static_geom_t) * header->numVertices, vertices, 0 );
            glNamedBufferStorageEXT( models[numModels].ibo, sizeof(uint32_t) * header->numIndices, indices, 0 );

            models[numModels].idxFormat  = GL_UNSIGNED_INT;
            models[numModels].numSubmeshes =header->numSubmeshes;

            first = numMeshes;
            count = header->numSubmeshes;

            for (size_t i = 0; i < header->numSubmeshes; ++i)
            {
                meshes[numMeshes].vbo = models[numModels].vbo;
                meshes[numMeshes].ibo = models[numModels].ibo;
                meshes[numMeshes].idxFormat  = GL_UNSIGNED_INT;
                meshes[numMeshes].idxOffset  = header->offsetSize[i*2  ] * sizeof(uint32_t);
                meshes[numMeshes].numIndices = header->offsetSize[i*2+1];

                ++numMeshes;
            }

            ++numModels;
            mem_free(&data);
        }
    }

    void convertOBJ()
    {
        const char* filename = "D:\\projects\\graphics\\Infinity\\AppData\\models\\sponza\\sponza.obj";
        const char* mtl_basepath = "D:\\projects\\graphics\\Infinity\\AppData\\models\\sponza\\";
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string res = tinyobj::LoadObj(shapes, materials, filename, mtl_basepath);

        for (size_t i = 0; i < shapes.size(); ++i)
        {
            tinyobj::shape_t& s=shapes[i];

            FILE* f = fopen(("D:\\projects\\graphics\\Infinity\\AppData\\models\\sponza\\"+s.name+".mesh").c_str(), "wb");
            assert(f);

            assert(s.mesh.positions.size() > 0);
            assert(s.mesh.normals.size()   > 0);
            assert(s.mesh.texcoords.size() > 0);

            size_t nv = s.mesh.positions.size()/3;
            size_t ni = s.mesh.indices.size();

            float* p  = &s.mesh.positions[0];
            float* n  = &s.mesh.normals[0];
            float* uv = &s.mesh.texcoords[0];

            float minx, miny, minz;
            float maxx, maxy, maxz;

            minx = maxx = s.mesh.positions[0];
            miny = maxy = s.mesh.positions[1];
            minz = maxz = s.mesh.positions[2];

            for (size_t v = 0; v < nv; ++v)
            {
                minx = core::min(minx, p[v*3+0]);
                miny = core::min(miny, p[v*3+1]);
                minz = core::min(minz, p[v*3+2]);

                maxx = core::max(maxx, p[v*3+0]);
                maxy = core::max(maxy, p[v*3+1]);
                maxz = core::max(maxz, p[v*3+2]);
            }

            mesh_header_v0 h = {MESH_FILE_MAGIC, 0, nv, ni, minx, miny, minz, maxx, maxy, maxz, s.submeshes.size()};

            for (size_t i = 0; i < h.numSubmeshes; ++i)
            {
                h.offsetSize[i*2  ] = s.submeshes[i].first;
                h.offsetSize[i*2+1] = s.submeshes[i].second;
                assert(s.submeshes[i].second%3==0);
                assert(s.submeshes[i].first+s.submeshes[i].second<=ni);
            }

            assert(s.mesh.positions.size()==s.mesh.normals.size());
            assert(s.mesh.positions.size()/3==s.mesh.texcoords.size()/2);

            fwrite(&h, sizeof(h), 1, f);
            for (size_t v = 0; v < nv; ++v)
            {
                float vp3[3] = {p[v*3]/10.0f, p[v*3+1]/10.0f, p[v*3+2]/10.0f};
                float uv2[2] = {uv[v*2], 1.0f-uv[v*2+1]};
                fwrite(vp3, sizeof(float), 3, f);
                fwrite(n  + v*3, sizeof(float), 3, f);
                fwrite(uv2, sizeof(float), 2, f);
            }

            for (size_t idx = 0; idx < ni; ++idx)
            {
                assert(s.mesh.indices[idx]<nv);
            }

            assert(ni%3==0);

            fwrite(&s.mesh.indices[0], sizeof(uint32_t), ni, f);
            fclose(f);
        }

        FILE* f = fopen("D:\\projects\\graphics\\Infinity\\AppData\\models\\sponza\\sponza.mtllib", "w");
        fprintf(f, "{\n");

        for (size_t i = 0; i < materials.size(); ++i)
        {
            tinyobj::material_t& m=materials[i];

            fprintf(f, "    %s : {\n", m.name.c_str());
            if (!m.diffuse_texname.empty())
            {
                fprintf(f, "        DiffuseMap    = \"%s\"\n", m.diffuse_texname.c_str());
            }
            if (!m.specular_texname.empty())
            {
                fprintf(f, "        SpecularMap   = \"%s\"\n", m.specular_texname.c_str());
            }
            if (m.unknown_parameter.find("map_bump") != m.unknown_parameter.end())
            {
                fprintf(f, "        NormalMap     = \"%s\"\n", m.unknown_parameter["map_bump"].c_str());
            }
            if (m.unknown_parameter.find("map_d") != m.unknown_parameter.end())
            {
                fprintf(f, "        AlphaTest     = true\n");
            }
            fprintf(f, "        DiffuseColor  = [%f %f %f %f]\n", m.diffuse[0], m.diffuse[1], m.diffuse[2], m.dissolve);
            fprintf(f, "        SpecularColor = [%f %f %f]\n", m.specular[0], m.specular[1], m.specular[2]);
            fprintf(f, "        EmissiveColor = [%f %f %f]\n", m.emission[0], m.emission[1], m.emission[2]);
            fprintf(f, "        SpecPow       = %f\n", m.shininess);
            float ior = core::max(m.ior, 0.0001f);
            float fresnel = (1-m.ior)/(1+m.ior);
            fresnel*=fresnel;
            fprintf(f, "        Fresnel       = %f\n", fresnel);
            fprintf(f, "    }\n");
        }
        fprintf(f, "}\n");
        fclose(f);

        f = fopen("D:\\projects\\graphics\\Infinity\\AppData\\models\\sponza\\sponza.models", "w");
        fprintf(f, "[\n");
        for (size_t i = 0; i < shapes.size(); ++i)
        {
            tinyobj::shape_t& s=shapes[i];
            fprintf(f, "    {\n");
            fprintf(f, "        model    = \"models/sponza/%s.mesh\"\n", s.name.c_str());
            fprintf(f, "        materials = [\n");
            for (size_t j=0; j<s.submeshes.size(); ++j)
            {
                const char* name = "";
                if (s.materials[j]!=-1)
                {
                    name = materials[s.materials[j]].name.c_str();
                }
                fprintf(f, "            \"%s\"\n", name);
            }
            fprintf(f, "        ]\n");
            fprintf(f, "    }\n");
        }
        fprintf(f, "]\n");
        fclose(f);
    }
}
