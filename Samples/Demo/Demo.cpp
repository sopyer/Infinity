#include <fwk/fwk.h>

#include "tiny_obj_loader.h"
#include <set>

#include <mjson.h>

#define MAX_MESHES     2048
#define MAX_MODELS      512
#define MAX_MATERIALS    64
#define MAX_LIGHTS     1024

#define LIGHT_GRID_TILE_DIM_X 64
#define LIGHT_GRID_TILE_DIM_Y 64

#define LIGHT_GRID_MAX_DIM_X ((1920 + LIGHT_GRID_TILE_DIM_X - 1) / LIGHT_GRID_TILE_DIM_X)
#define LIGHT_GRID_MAX_DIM_Y ((1080 + LIGHT_GRID_TILE_DIM_Y - 1) / LIGHT_GRID_TILE_DIM_Y)
#define LIGHT_GRID_MAX_DIM_Z 256 

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

struct light_t
{
  ml::vec3 pos;
  float    range;
  ml::vec3 color;
  uint32_t pad;
};

#define DEBUG_SHADER

struct gpu_clusters_t
{
    uint32_t uGridTileX;
    uint32_t uGridTileY;
    uint32_t uGridDimX;
    uint32_t uGridDimY;
    float uInvZNear;
    float uLogScale;
#ifdef DEBUG_SHADER
    uint32_t uDebugMaxClusters;
    uint32_t uDebugMaxLightList;
    uint32_t uDebugMaxLights;
    uint32_t pad[3];
#else
    uint32_t pad[2];
#endif

    struct 
    {
        uint32_t offset;
        uint32_t count;
        uint64_t pad;
    } lightLists[1];
};

struct gpu_globals_t
{
    ml::vec4  uMatDiffuse;
    ml::vec3  uMatSpecular;
    uint32_t  pad1;
    ml::vec3  uAmbientGlobal;
    float     uR0;
    float     uMatSpecPow;
};

STATIC_ASSERT(sizeof(light_t)==32);

namespace app
{
    GLuint prgStatic;
    GLuint prgStaticAT;
    GLuint prgStaticDiffuse;

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
    void generateLights(int num);
    static void assignLightsToClustersCpu(v128 modelView[4], v128 proj[4]);

    mem::arena_t appArena;

    size_t numViewLights;
    light_t  lights[MAX_LIGHTS];
    light_t  lightsView[MAX_LIGHTS];

    ml::vec3 scene_min = {0.0f, 0.0f, 0.0f};
    ml::vec3 scene_max = {0.0f, 0.0f, 0.0f};

    GLuint lightDescSSBO = 0;

    GLuint lightOffset,       lightSize;
    GLuint lightListOffset,   lightListSize;
    GLuint clusterListOffset, clusterListSize;

    void init()
    {
        appArena = mem::create_arena(20*1024*1024, 0);

        //convertOBJ();

        loadMaterials();
        loadModels();

        generateLights(MAX_LIGHTS);

        camera.acceleration.x = camera.acceleration.y = camera.acceleration.z = 150;
        camera.maxVelocity.x  = camera.maxVelocity.y  = camera.maxVelocity.z  =  60;

        gfx::var_desc_t gpu_globals_desc[] = 
        {
            {0, GL_FLOAT_VEC4, 1, offsetof(gpu_globals_t, uMatDiffuse)},
            {0, GL_FLOAT_VEC3, 1, offsetof(gpu_globals_t, uMatSpecular)},
            {0, GL_FLOAT_VEC3, 1, offsetof(gpu_globals_t, uAmbientGlobal)},
            {0, GL_FLOAT,      1, offsetof(gpu_globals_t, uR0)},
            {0, GL_FLOAT,      1, offsetof(gpu_globals_t, uMatSpecPow)},
        };
        assert(gfx::matchInterface(prgStatic, "Globals", true, ARRAY_SIZE(gpu_globals_desc), gpu_globals_desc)==gfx::MATCH_SUCCESS);


        gfx::var_desc_t gpu_clusters_desc[] = 
        {
            {0, GL_UNSIGNED_INT, 1, offsetof(gpu_clusters_t, uGridTileX)},
            {0, GL_UNSIGNED_INT, 1, offsetof(gpu_clusters_t, uGridTileY)},
            {0, GL_UNSIGNED_INT, 1, offsetof(gpu_clusters_t, uGridDimX)},
            {0, GL_UNSIGNED_INT, 1, offsetof(gpu_clusters_t, uGridDimY)},
            {0, GL_FLOAT,        1, offsetof(gpu_clusters_t, uInvZNear)},
            {0, GL_FLOAT,        1, offsetof(gpu_clusters_t, uLogScale)},
            {0, GL_UNSIGNED_INT, 1, offsetof(gpu_clusters_t, lightLists[0].offset)},
            {0, GL_UNSIGNED_INT, 1, offsetof(gpu_clusters_t, lightLists[0].count)},
        };
        //assert(gfx::matchInterface(prgStatic, "ClusterData", false, ARRAY_SIZE(gpu_clusters_desc), gpu_clusters_desc)==gfx::MATCH_SUCCESS);

        gfx::var_desc_t gpu_lights_desc[] = 
        {
            {0, GL_FLOAT_VEC3, 1, offsetof(light_t, pos)},
            {0, GL_FLOAT,      1, offsetof(light_t, range)},
            {0, GL_FLOAT_VEC3, 1, offsetof(light_t, color)},
        };
        //assert(gfx::matchInterface(prgStatic, "Lights", false, ARRAY_SIZE(gpu_lights_desc), gpu_lights_desc)==gfx::MATCH_SUCCESS);

        gfx::gpu_timer_init(&gpuTimer);

        glGenBuffers(1, &lightDescSSBO);

        glNamedBufferStorageEXT(lightDescSSBO, sizeof(light_t)*MAX_LIGHTS, lights, 0);
    }

    void fini()
    {
        glDeleteBuffers(1, &lightDescSSBO);

        destroyModels();
        destroyMaterials();
        gfx::gpu_timer_fini(&gpuTimer);
        mem::destroy_arena(appArena);
    }

    uint32_t gridDimX;
    uint32_t gridDimY;
    uint32_t gridDimZ;
    uint32_t numClusters;
    float zLogScale;
    float invZNear;

    float fov   = 30.0f * FLT_DEG_TO_RAD_SCALE;
    float znear = 0.1f;
    float zfar  = 1000.0f;

    void render()
    {
        gpu_timer_start(&gpuTimer);
        cpu_timer_start(&cpuTimer);

        v128 m[4];
        camera.getViewMatrix(m);
        memcpy(&gfx::autoVars.matMV, (float*)m, sizeof(float) * 16);

        glEnable(GL_FRAMEBUFFER_SRGB);

        gfx::drawXZGrid(-500.0f, -500.0f, 500.0f, 500.0f, 40, vi_set(0.0f, 1.0f, 0.0f, 1.0f));

        assignLightsToClustersCpu(m, proj);
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
        ml::make_perspective_mat4(proj, fov, (float)width/(float)height, znear, zfar);

        gfx::autoVars.projParams.x = proj[0].m128_f32[0];
        gfx::autoVars.projParams.y = proj[1].m128_f32[1];
        gfx::autoVars.projParams.z = proj[2].m128_f32[2];
        gfx::autoVars.projParams.w = proj[3].m128_f32[2];
    }

    size_t  numModels;
    size_t  numMeshes;
    size_t  numMaterials;
    size_t  numLights;

    model_t      models       [MAX_MODELS];
    mesh_t       meshes       [MAX_MESHES];
    material_t*  materialRefs [MAX_MESHES];
    material_t   materials    [MAX_MATERIALS];
    const char*  materialNames[MAX_MATERIALS];

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

        ui::debugAddPrograms(1, &prgStatic);

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

    float randomUnitFloat()
    {
      return float(rand()) / float(RAND_MAX);
    }

    float randomRange(float low, float high)
    {
      return low + (high - low) * randomUnitFloat();
    }

    static v128 hueToRGB(float hue)
    {
        const float s = hue * 6.0f;
        float r0 = core::min(core::max(s - 4.0f, 0.0f), 1.0f);
        float g0 = core::min(core::max(s - 0.0f, 0.0f), 1.0f);
        float b0 = core::min(core::max(s - 2.0f, 0.0f), 1.0f);

        float r1 = core::min(core::max(2.0f - s, 0.0f), 1.0f);
        float g1 = core::min(core::max(4.0f - s, 0.0f), 1.0f);
        float b1 = core::min(core::max(6.0f - s, 0.0f), 1.0f);

        // annoying that it wont quite vectorize...
        return vi_set(r0 + r1, g0 * g1, b0 * b1, 0.0f);
    }

    void generateLights(int num)
    {
        assert(num<=MAX_LIGHTS);
        // divide volume equally amongst lights
        const float volume = (scene_max.x-scene_min.x)*(scene_max.y-scene_min.y)*(scene_max.z-scene_min.z);
        const float lightVol = volume / float(num);
        // set radius to be the cube root of volume for a light
        const float lightRad = pow(lightVol, 1.0f / 3.0f);
        // and allow some overlap
        const float maxRad = lightRad;// * 2.0f;
        const float minRad = lightRad;

        numLights = 0;
        for (int i = 0; i < num; ++i)
        {
            float rad = randomRange(minRad, maxRad);
            ml::vec3 col;
            vi_store_v3(&col, vi_mul(hueToRGB(randomUnitFloat()), vi_set_ffff(randomRange(0.4f, 0.7f))));
            //float3 pos = { randomRange(aabb.min.x + rad, aabb.max.x - rad), randomRange(aabb.min.y + rad, aabb.max.y - rad), randomRange(aabb.min.z + rad, aabb.max.z - rad) };
            const float ind =  rad / 8.0f;
            ml::vec3 pos = { randomRange(scene_min.x + ind, scene_max.x - ind), randomRange(scene_min.y + ind, scene_max.y - ind), randomRange(scene_min.z + ind, scene_max.z - ind) };
            light_t l = { pos, rad, col };
            lights[numLights++] = l;
        }
        //numLights = 0;
        //for (size_t i=0; i<=20; ++i)
        //{
        //    lights[numLights].pos = ml::make_vec3(-100.0f+ i*10, 20.0f, 0.0f);;
        //    lights[numLights].range = 50;
        //    ml::vec3 col;
        //    vi_store_v3(&col, vi_mul(hueToRGB(randomUnitFloat()), vi_set_ffff(randomRange(0.4f, 0.7f))));
        //    lights[numLights].color = col;
        //    ++numLights;
        //}
    }

    void renderMesh(mesh_t* mesh, material_t* material)
    {
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
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 2, gfx::dynBuffer, clusterListOffset, clusterListSize);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 3, gfx::dynBuffer, lightListOffset,   lightListSize);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 4, gfx::dynBuffer, lightOffset,       lightSize);

        //!!!!HACK: in order to upload SSBO, otherwise they are not updated
        glUseProgram(prgStatic);
        renderMesh( &meshes[100], materialRefs[100] );

        for (size_t i=0; i<numMeshes; ++i)
        {
            glUseProgram(materialRefs[i]->program);
            GLuint globalsOffset;
            GLuint globalsSize = sizeof(gpu_globals_t);
            gpu_globals_t* globals = (gpu_globals_t*)gfx::dynbufAllocMem(globalsSize, gfx::caps.uboAlignment, &globalsOffset);

            globals->uAmbientGlobal = ml::make_vec3(0.02f, 0.02f, 0.02f);
            globals->uMatDiffuse    = *(ml::vec4*)materialRefs[i]->diffuse;
            globals->uMatSpecular   = *(ml::vec3*)materialRefs[i]->specular;
            globals->uMatSpecPow    = materialRefs[i]->specPow;
            globals->uR0            = materialRefs[i]->fresnel;

            glBindBufferRange(GL_UNIFORM_BUFFER, 1, gfx::dynBuffer, globalsOffset, globalsSize);

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

            if (!numModels)
            {
                scene_min.x = header->minx;
                scene_min.y = header->miny;
                scene_min.z = header->minz;
                scene_max.x = header->maxx;
                scene_max.y = header->maxy;
                scene_max.z = header->maxz;
            }
            else
            {
                scene_min.x = core::min(scene_min.x, header->minx);
                scene_min.y = core::min(scene_min.y, header->miny);
                scene_min.z = core::min(scene_min.z, header->minz);
                scene_max.x = core::max(scene_max.x, header->maxx);
                scene_max.y = core::max(scene_max.y, header->maxy);
                scene_max.z = core::max(scene_max.z, header->maxz);
            }

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
        const float scale = 0.1f;
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

            mesh_header_v0 h = {MESH_FILE_MAGIC, 0, nv, ni, minx*scale, miny*scale, minz*scale, maxx*scale, maxy*scale, maxz*scale, s.submeshes.size()};

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
                float vp3[3] = {p[v*3]*scale, p[v*3+1]*scale, p[v*3+2]*scale};
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

    struct ScreenRect3D
    {
        int minx, miny, minz;
        int maxx, maxy, maxz;
        uint32_t index;
    };

    float calcClusterZ(float viewSpaceZ)
    {
        float gridLocZ = logf(-viewSpaceZ * invZNear) * zLogScale;

        return gridLocZ;
    }

    //--------------------------------------------------------------------------------------
    // Bounds computation utilities, similar to PointLightBounds.cpp
    void updateClipRegionRoot(float nc,          // Tangent plane x/y normal coordinate (view space)
        float lc,          // Light x/y coordinate (view space)
        float lz,          // Light z coordinate (view space)
        float lightRadius,
        float cameraScale, // Project scale for coordinate (_11 or _22 for x/y respectively)
        float &clipMin,
        float &clipMax)
    {
        float nz = (lightRadius - nc * lc) / lz;
        float pz = (lc * lc + lz * lz - lightRadius * lightRadius) / (lz - (nz / nc) * lc);

        if (pz < 0.0f) 
        {
            float c = -nz * cameraScale / nc;
            if (nc < 0.0f) 
            {        
                // Left side boundary
                clipMin = std::max(clipMin, c);
            } 
            else 
            {                          // Right side boundary
                clipMax = std::min(clipMax, c);
            }
        }
    }

    void updateClipRegion(float lc,          // Light x/y coordinate (view space)
        float lz,          // Light z coordinate (view space)
        float lightRadius,
        float cameraScale, // Project scale for coordinate (_11 or _22 for x/y respectively)
        float &clipMin,
        float &clipMax)
    {
        float rSq = lightRadius * lightRadius;
        float lcSqPluslzSq = lc * lc + lz * lz;
        float d = rSq * lc * lc - lcSqPluslzSq * (rSq - lz * lz);

        if (d >= 0.0f) 
        {
            float a = lightRadius * lc;
            float b = sqrt(d);
            float nx0 = (a + b) / lcSqPluslzSq;
            float nx1 = (a - b) / lcSqPluslzSq;

            updateClipRegionRoot(nx0, lc, lz, lightRadius, cameraScale, clipMin, clipMax);
            updateClipRegionRoot(nx1, lc, lz, lightRadius, cameraScale, clipMin, clipMax);
        }
    }

    // Returns bounding box [min.xy, max.xy] in clip [-1, 1] space.
    void computeClipRegion(
        const ml::vec3& lightPosView, float lightRadius, float cameraNear, v128 proj[4],
        float& minx, float& miny, float& maxx, float& maxy
    )
    {
        minx = 1.0f; miny = 1.0f; maxx = -1.0f; maxy = -1.0f;
        // Early out with empty rectangle if the light is too far behind the view frustum
        if (lightPosView.z - lightRadius <= -cameraNear) 
        {
            minx = -1.0f; miny = -1.0f; maxx = 1.0f; maxy = 1.0f;

            updateClipRegion(lightPosView.x, lightPosView.z, lightRadius, proj[0].m128_f32[0], minx, maxx);
            updateClipRegion(lightPosView.y, lightPosView.z, lightRadius, proj[1].m128_f32[1], miny, maxy);
        }
    }

    void findScreenSpaceBounds(
        v128 proj[4], const ml::vec3& pt, float rad,
        int width, int height, float near,
        int& minx, int& miny, int& maxx, int& maxy
    )
    {
        float fminx=1.0f, fminy=1.0f, fmaxx=-1.0f, fmaxy=-1.0f;
        computeClipRegion(pt, rad, near, proj, fminx, fminy, fmaxx, fmaxy);

        fminx = -fminx;
        fminy = -fminy;
        fmaxx = -fmaxx;
        fmaxy = -fmaxy;

        core::swap(fminx, fmaxx);
        core::swap(fminy, fmaxy);

        fminx = 0.5f * fminx + 0.5f;
        fminy = 0.5f * fminy + 0.5f;
        fmaxx = 0.5f * fmaxx + 0.5f;
        fmaxy = 0.5f * fmaxy + 0.5f;

        minx = int(fminx * float(width));
        miny = int(fminy * float(height));
        maxx = int(fmaxx * float(width));
        maxy = int(fmaxy * float(height));

        minx = core::max(0, core::min(width,  minx));
        miny = core::max(0, core::min(height, miny));
        maxx = core::max(0, core::min(width,  maxx));
        maxy = core::max(0, core::min(height, maxy));
    }

    static void buildRects3D(v128 matMV[4], v128 matP[4], uint32_t maxRects, uint32_t& numRects, ScreenRect3D* rects)
    {
        PROFILER_CPU_TIMESLICE("BuildRects");

        numRects = 0;
        for (uint32_t i = 0; i < numViewLights; ++i)
        {
            const light_t &l = lightsView[i];
            int minx=0, miny=0, maxx=0, maxy=0;
            findScreenSpaceBounds(matP, l.pos, l.range, gfx::width, gfx::height, znear, minx, miny, maxx, maxy);

            if (minx < maxx && miny < maxy)
            {
                ScreenRect3D& r3 = rects[numRects++];

                r3.index = i;
                
                r3.minx = minx / LIGHT_GRID_TILE_DIM_X;
                r3.miny = miny / LIGHT_GRID_TILE_DIM_Y;
                r3.minz = uint32_t(core::max(0.0f, calcClusterZ(l.pos.z + l.range)));;

                r3.maxx = (maxx + LIGHT_GRID_TILE_DIM_X - 1) / LIGHT_GRID_TILE_DIM_X;
                r3.maxy = (maxy + LIGHT_GRID_TILE_DIM_Y - 1) / LIGHT_GRID_TILE_DIM_Y;
                r3.maxz = uint32_t(core::max(0.0f, ceilf(calcClusterZ(l.pos.z - l.range)) + 0.5f));
            }
        }
        assert(numRects<=maxRects);
    }

    static void assignLightsToClustersCpu(v128 modelView[4], v128 proj[4])
    {
        PROFILER_CPU_TIMESLICE("LightGridBuild");

        numViewLights = 0;
        for (size_t i=0; i < numLights; ++i)
        {
            light_t& l = lightsView[i];

            l = lights[i];

            v128 vlp = ml::mul_mat4_vec4(modelView, vi_set(l.pos.x, l.pos.y, l.pos.z, 1.0f));
            vi_store_v3(&l.pos, vlp);
            ++numViewLights;
        }

        lightOffset = 0;
        lightSize   = sizeof(light_t)* core::max(4U, numViewLights);

        light_t* data = (light_t*)gfx::dynbufAllocMem(lightSize, gfx::caps.ssboAlignment, &lightOffset);
        memcpy(data, lightsView, numViewLights*sizeof(light_t));

        gridDimX = (gfx::width  + LIGHT_GRID_TILE_DIM_X - 1) / LIGHT_GRID_TILE_DIM_X;
        gridDimY = (gfx::height + LIGHT_GRID_TILE_DIM_Y - 1) / LIGHT_GRID_TILE_DIM_Y;
        gridDimZ = 60;

        numClusters = gridDimX * gridDimY * gridDimZ;

        zLogScale = gridDimZ / logf(zfar / znear);
        invZNear  = 1.0f / znear;

        uint32_t numRects;
        ScreenRect3D* rects = mem::alloc_array<ScreenRect3D>(appArena, MAX_LIGHTS);
        uint32_t* offsets = mem::alloc_array<uint32_t>(appArena, numClusters);
        uint32_t* counts  = mem::alloc_array<uint32_t>(appArena, numClusters);

        buildRects3D(modelView, proj, MAX_LIGHTS, numRects, rects);

        memset(counts, 0,  sizeof(uint32_t)*numClusters);
        memset(offsets, 0, sizeof(uint32_t)*numClusters);

        uint32_t totalus = 0;
        {  
            PROFILER_CPU_TIMESLICE("Pass1");
            for (size_t i = 0; i < numRects; ++i)
            {
                ScreenRect3D r = rects[i];

                uint32_t lx = core::max(0, core::min<int>(gridDimX, r.minx));
                uint32_t ly = core::max(0, core::min<int>(gridDimY, r.miny));
                uint32_t lz = core::max(0, core::min<int>(gridDimZ, r.minz));

                uint32_t ux = core::max(0, core::min<int>(gridDimX, r.maxx));
                uint32_t uy = core::max(0, core::min<int>(gridDimY, r.maxy));
                uint32_t uz = core::max(0, core::min<int>(gridDimZ, r.maxz));

                for (uint32_t z = lz; z < uz; ++z)
                {
                    for (uint32_t y = ly; y < uy; ++y)
                    {
                        for (uint32_t x = lx; x < ux; ++x)
                        {
                            uint32_t idx = (z * gridDimY + y) * gridDimX + x;
                            ++counts[idx];
                            ++totalus;
                        }
                    }
                }
            }
        }

        uint32_t offset = 0;
        {  
            PROFILER_CPU_TIMESLICE("BuildOffsets");
            for (size_t idx = 0; idx < numClusters; ++idx)
            {
                offsets[idx] = offset;

                offset += counts[idx];
            }
        }

        memset(counts, 0,  sizeof(uint32_t)*numClusters);

        {
            PROFILER_CPU_TIMESLICE("Pass2");

            lightListOffset = 0;
            lightListSize   = core::max(4U, totalus) * sizeof(v128);

            uint32_t* data = (uint32_t*)gfx::dynbufAllocMem(lightListSize, gfx::caps.ssboAlignment, &lightListOffset);

            for (size_t i = 0; i < numRects; ++i)
            {
                ScreenRect3D r = rects[i];

                uint32_t lx = core::max(0, core::min<int>(gridDimX, r.minx));
                uint32_t ly = core::max(0, core::min<int>(gridDimY, r.miny));
                uint32_t lz = core::max(0, core::min<int>(gridDimZ, r.minz));

                uint32_t ux = core::max(0, core::min<int>(gridDimX, r.maxx));
                uint32_t uy = core::max(0, core::min<int>(gridDimY, r.maxy));
                uint32_t uz = core::max(0, core::min<int>(gridDimZ, r.maxz));

                for (uint32_t z = lz; z < uz; ++z)
                {
                    for (uint32_t y = ly; y < uy; ++y)
                    {
                        for (uint32_t x = lx; x < ux; ++x)
                        {
                            uint32_t idx = (z * gridDimY + y) * gridDimX + x;

                            uint32_t count = counts[idx];
                            ++counts[idx];
                            uint32_t offset = offsets[idx] + count;

                            data[offset*4] = r.index;
                            assert(r.index<numViewLights);
                        }
                    }
                }
            }
        }
        {
            PROFILER_CPU_TIMESLICE("copyGridFromHost");

            clusterListOffset = 0;
            clusterListSize   = calcBlobSize1<gpu_clusters_t, v128>(numClusters);

            gpu_clusters_t* data = (gpu_clusters_t*)gfx::dynbufAllocMem(clusterListSize, gfx::caps.ssboAlignment, &clusterListOffset);

            data->uGridTileX = LIGHT_GRID_TILE_DIM_X;
            data->uGridTileY = LIGHT_GRID_TILE_DIM_Y;
            data->uGridDimX  = gridDimX;
            data->uGridDimY  = gridDimY;
            data->uInvZNear  = 1.0f / znear;
            data->uLogScale  = zLogScale;
#ifdef DEBUG_SHADER
            data->uDebugMaxClusters  = numClusters;
            data->uDebugMaxLightList = totalus;
            data->uDebugMaxLights    = numViewLights;
#endif

            for (size_t i = 0; i < numClusters; ++i)
            {
                data->lightLists[i].offset = offsets[i];
                data->lightLists[i].count  = counts [i];
                assert(offsets[i]+counts [i]<=totalus);
            }
        }
        mem::free(appArena, offsets);
        mem::free(appArena, counts);
        mem::free(appArena, rects);
    }
}
