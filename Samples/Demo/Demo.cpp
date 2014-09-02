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
    GLuint    firstVertex;
    GLenum    idxFormat;
    GLsizei   idxOffset;
    GLsizei   numIndices;
};

struct material_t
{
    GLuint  program;
    GLuint  textures[3];
    GLuint  matOffset;
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

struct gpu_clustered_lighting_t
{
    ml::vec3  uAmbientGlobal;
    int32_t   uGridTileX;
    int32_t   uGridTileY;
    int32_t   uGridDimX;
    int32_t   uGridDimY;
    float     uInvZNear;
    float     uLogScale;
#ifdef DEBUG_SHADER
    int32_t   uDebugMaxClusters;
    int32_t   uDebugMaxLightList;
    int32_t   uDebugMaxLights;
#endif
};

struct gpu_material_t
{
    ml::vec4  uMatDiffuse;
    ml::vec3  uMatSpecular;
    float     uR0;
    float     uMatSpecPow;
};

STATIC_ASSERT(sizeof(light_t)==32);

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
    void generateLights(int num);
    static void assignLightsToClustersCpu(v128 modelView[4], v128 proj[4]);

    mem::arena_t appArena;

    size_t numViewLights;
    light_t  lights[MAX_LIGHTS];
    light_t  lightsView[MAX_LIGHTS];

    ml::vec3 scene_min = {0.0f, 0.0f, 0.0f};
    ml::vec3 scene_max = {0.0f, 0.0f, 0.0f};

    GLuint lightOffset,       lightSize;
    GLuint lightListOffset,   lightListSize;
    GLuint clusterListOffset, clusterListSize;

    GLuint texClusterData;
    GLuint texLightListData;
    GLuint texLightData;

    GLuint materialUBO;
    GLuint matBufferSize;
    GLuint materialSize;

    GLuint vertexBuffer;
    GLuint indexBuffer;

    const size_t vertexBufferSize = 7 * (1<<20);
    const size_t indexBufferSize  = 5 * (1<<20);

    enum
    {
        PRG_PERM_DIFFUSE    = 1<<0,
        PRG_PERM_SPECULAR   = 1<<1,
        PRG_PERM_NORMAL     = 1<<2,
        PGR_PERM_ALPHA_TEST = 1<<3,

        NUM_PERM = 16
    };

    GLuint staticPrograms[NUM_PERM];

    GLuint getProgram(bool diffuse, bool specular, bool normal, bool alphaTest)
    {
        int idx = 0;
        idx |= diffuse   ? PRG_PERM_DIFFUSE    : 0;
        idx |= specular  ? PRG_PERM_SPECULAR   : 0;
        idx |= normal    ? PRG_PERM_NORMAL     : 0;
        idx |= alphaTest ? PGR_PERM_ALPHA_TEST : 0;

        assert(idx<NUM_PERM);
        return staticPrograms[idx];
    }

    void init()
    {
        appArena = mem::create_arena(20*1024*1024, 0);

        const char* version        = "#version 430\n\n";
        const char* enableDebug    = "#define ENABLE_DEBUG\n";
        const char* enableAT       = "#define ENABLE_ALPHA_TEST\n";
        const char* enableDiffuse  = "#define ENABLE_DIFFUSE\n";
        const char* enableSpecular = "#define ENABLE_SPECULAR\n";
        const char* enableNormal   = "#define ENABLE_NORMAL\n";

        const char* headers[6] = 
        {
            version,
#ifdef DEBUG_SHADER
            enableDebug,
#endif
        };

        for (size_t i=0; i<NUM_PERM; ++i)
        {
#ifdef DEBUG_SHADER
            size_t numHeaders = 2;
#else
            size_t numHeaders = 1;
#endif
            if (i&PRG_PERM_DIFFUSE)
            {
                headers[numHeaders++] = enableDiffuse;
            }
            if (i&PRG_PERM_SPECULAR)
            {
                headers[numHeaders++] = enableSpecular;
            }
            if (i&PRG_PERM_NORMAL)
            {
                headers[numHeaders++] = enableNormal;
            }
            if (i&PGR_PERM_ALPHA_TEST)
            {
                headers[numHeaders++] = enableAT;
            }
            staticPrograms[i] = res::createProgramFromFiles("MESH.Static.vert", "MESH.StdLighting.frag", numHeaders, headers);
        }

        ui::debugAddPrograms(NUM_PERM, staticPrograms);

        //convertOBJ();

        glGenTextures(1, &texClusterData);
        glGenTextures(1, &texLightListData);
        glGenTextures(1, &texLightData);

        materialSize  = bit_align_up(sizeof(gpu_material_t), gfx::caps.uboAlignment);
        matBufferSize = materialSize*MAX_MATERIALS;
        glGenBuffers(1, &materialUBO);
        glNamedBufferStorageEXT(materialUBO, matBufferSize, 0, GL_MAP_WRITE_BIT);

        glGenBuffers(1, &vertexBuffer);
        glNamedBufferStorageEXT(vertexBuffer, vertexBufferSize, 0, GL_MAP_WRITE_BIT);

        glGenBuffers(1, &indexBuffer);
        glNamedBufferStorageEXT(indexBuffer, indexBufferSize, 0, GL_MAP_WRITE_BIT);

        loadMaterials();
        loadModels();

        generateLights(MAX_LIGHTS);

        camera.acceleration.x = camera.acceleration.y = camera.acceleration.z = 150;
        camera.maxVelocity.x  = camera.maxVelocity.y  = camera.maxVelocity.z  =  60;

        camera.setPosition(vi_set(80.0f, 100.0f, 0.0f, 1.0f));
        camera.setOrientation(vi_set(0.0f, 0.707f, 0.0f, -0.707f));

        gfx::gpu_timer_init(&gpuTimer);
    }

    void fini()
    {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &indexBuffer);
        glDeleteBuffers(1, &materialUBO);

        glDeleteTextures(1, &texLightData);
        glDeleteTextures(1, &texLightListData);
        glDeleteTextures(1, &texClusterData);

        for (size_t i=0; i<NUM_PERM; ++i)
        {
            glDeleteProgram(staticPrograms[i]);
        }

        destroyModels();
        destroyMaterials();
        gfx::gpu_timer_fini(&gpuTimer);
        mem::destroy_arena(appArena);
    }

    int32_t gridDimX;
    int32_t gridDimY;
    int32_t gridDimZ;
    int32_t numClusters;
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

        assignLightsToClustersCpu(m, proj);
        glEnable(GL_FRAMEBUFFER_SRGB);

        renderModels();

        glDisable(GL_FRAMEBUFFER_SRGB);

        {
            PROFILER_CPU_TIMESLICE("ui::displayStats");
            ui::displayStats(
                10.0f, 10.0f, 300.0f, 70.0f,
                cpu_timer_measured(&cpuTimer) / 1000.0f,
                gfx::gpu_timer_measured(&gpuTimer) / 1000.0f
            );
        }

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

    void loadModel(
        const char* name,
        int& first, int& count,
        uint8_t* vertices, GLuint& vertexOffset,
        uint8_t* indices,  GLuint& indexOffset
    );
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

            uint8_t* vertexPtr = (uint8_t*)glMapNamedBufferRangeEXT(vertexBuffer, 0, vertexBufferSize, GL_MAP_WRITE_BIT);
            uint8_t* indexPtr  = (uint8_t*)glMapNamedBufferRangeEXT(indexBuffer,  0, indexBufferSize,  GL_MAP_WRITE_BIT);
            GLuint vertexOffset = 0;
            GLuint indexOffset  = 0;

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
                loadModel(model, first, count, vertexPtr, vertexOffset, indexPtr, indexOffset);
                mat = mjson_get_element_first(matList);
                for (int i = 0; i<count; ++i)
                {
                    materialRefs[first+i] = findMaterial(mjson_get_string(mat, ""));
                    mat = mjson_get_element_next(matList, mat);
                }

                modelDesc = mjson_get_element_next(root, modelDesc);
            }

            glUnmapNamedBufferEXT(vertexBuffer);
            glUnmapNamedBufferEXT(indexBuffer);

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

            uint8_t* matPtr = (uint8_t*)glMapNamedBufferRangeEXT(materialUBO, 0, matBufferSize, GL_MAP_WRITE_BIT);
            GLuint matOffset = 0;

            material = mjson_get_member_first(root, &dict);
            while (material)
            {
                const char* matName =  mjson_get_string(material, 0);
                const char* dmap = 0;
                const char* nmap = 0;
                const char* smap = 0;
                int mask = FALSE;
                ml::vec4 diffuse  = {1.0f, 1.0f, 1.0f, 1.0f};
                ml::vec3 specular = {0.0f, 0.0f, 0.0f};
                ml::vec3 emissive = {0.0f, 0.0f, 0.0f};
                float    specPow  = 0.0f;
                float    fresnel  = 1.0f;

                gpu_material_t* matGPU = (gpu_material_t*)(matPtr+matOffset);

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
                        readVector(value, 4, &diffuse.x);
                    }
                    else if (strcmp(name, "SpecularColor")==0)
                    {
                        readVector(value, 3, &specular.x);
                    }
                    else if (strcmp(name, "EmissiveColor")==0)
                    {
                        readVector(value, 3, &emissive.x);
                    }
                    else if (strcmp(name, "SpecPow")==0)
                    {
                        specPow = mjson_get_float(value, 0.0f);
                    }
                    else if (strcmp(name, "Fresnel")==0)
                    {
                        fresnel = mjson_get_float(value, 0.0f);
                    }

                    key = mjson_get_member_next(dict, key, &value);
                }

                material_t& mat = materials[numMaterials];

                if (dmap)
                {
                    GLuint texDiffuse = res::createTexture2D(dmap, TRUE);
                    glTextureParameteriEXT(texDiffuse, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTextureParameteriEXT(texDiffuse, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    mat.textures[0] = texDiffuse;
                }
                else
                {
                    mat.textures[0] = 0;
                }

                if (nmap)
                {
                    GLuint texNormal = res::createTexture2D(nmap);
                    glTextureParameteriEXT(texNormal, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTextureParameteriEXT(texNormal, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    mat.textures[2] = texNormal;
                }
                else
                    mat.textures[2] = 0;

                if (smap)
                {
                    GLuint texSpecular = res::createTexture2D(smap);
                    glTextureParameteriEXT(texSpecular, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTextureParameteriEXT(texSpecular, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    mat.textures[1] = texSpecular;
                }
                else
                    mat.textures[1] = 0;

                mat.program = getProgram(dmap!=0, smap!=0, nmap!=0, mask!=0);
                mat.matOffset = matOffset;

                matGPU->uMatDiffuse    = diffuse;
                matGPU->uMatSpecular   = specular;
                matGPU->uMatSpecPow    = specPow;
                matGPU->uR0            = fresnel;

                ++numMaterials;
                material = mjson_get_member_next(root, material, &dict);
                matOffset+=materialSize;
            }

            glUnmapNamedBufferEXT(materialUBO);

            assert(matOffset<=matBufferSize);

            mem_free(&inText);
            mem_free(&bjson);
        }
    }

    void destroyMaterials()
    {
        for (size_t i=0; i < numMaterials; ++i)
        {
            if (materials[i].textures[0])
            {
                glDeleteTextures(1, &materials[i].textures[0]);
            }
            if (materials[i].textures[1])
            {
                glDeleteTextures(1, &materials[i].textures[1]);
            }
            if (materials[i].textures[2])
            {
                glDeleteTextures(1, &materials[i].textures[2]);
            }
            mem::free(appArena, (void*)materialNames[i]);
        }
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

        return vi_set(r0 + r1, g0 * g1, b0 * b1, 0.0f);
    }

    void generateLights(int num)
    {
        assert(num<=MAX_LIGHTS);
        const float volume   = (scene_max.x-scene_min.x)*(scene_max.y-scene_min.y)*(scene_max.z-scene_min.z);
        const float lightVol = volume / float(num);
        const float lightRad = pow(lightVol, 1.0f / 3.0f);
        const float maxRad   = lightRad;
        const float minRad   = 0.7f*lightRad;

        numLights = 0;
        for (int i = 0; i < num; ++i)
        {
            float rad = randomRange(minRad, maxRad);
            ml::vec3 col;
            vi_store_v3(&col, vi_mul(hueToRGB(randomUnitFloat()), vi_set_ffff(randomRange(0.4f, 0.7f))));
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

        // Draw mesh from index buffer

    }

    void renderModels()
    {
        PROFILER_CPU_TIMESLICE("renderModels");

        glEnable(GL_CULL_FACE);

        gfx::setStdTransforms();
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, gfx::dynBuffer, clusterListOffset, clusterListSize);

        GLuint cluserRenderTextures[] = {texClusterData, texLightListData, texLightData};
        glBindTextures(3, ARRAY_SIZE(cluserRenderTextures), cluserRenderTextures);

        glBindVertexArray(vf::static_geom_t::vao);

        glBindVertexBuffer(0, vertexBuffer, 0, sizeof(vf::static_geom_t));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

        GLuint      currentPrg = 0;
        material_t* currentMat = 0;

        for (size_t i=0; i<numMeshes; ++i)
        {
            PROFILER_CPU_TIMESLICE("drawMesh");
            material_t* mat = materialRefs[i];
            if (currentMat!=mat)
            {
                PROFILER_CPU_TIMESLICE("materialChange");
                GLuint prg = mat->program;
                if (prg!=currentPrg)
                {
                    PROFILER_CPU_TIMESLICE("glUseProgram");
                    glUseProgram(prg);
                    currentPrg = prg;
                }

                glBindBufferRange(GL_UNIFORM_BUFFER, 1, materialUBO, mat->matOffset, sizeof(gpu_material_t));
                glBindTextures(0, ARRAY_SIZE(mat->textures), mat->textures);

                currentMat = mat;
            }

            mesh_t& m = meshes[i];

            {
                PROFILER_CPU_TIMESLICE("glDrawElementsBaseVertex");
                glDrawElementsBaseVertex(GL_TRIANGLES, m.numIndices, m.idxFormat, BUFFER_OFFSET(m.idxOffset), m.firstVertex);
            }
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDisable(GL_CULL_FACE);
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

    void loadModel(
        const char* name,
        int& first, int& count,
        uint8_t* vertices, GLuint& vertexOffset,
        uint8_t* indices,  GLuint& indexOffset
    )
    {
        memory_t  data = {0, 0, 0};


        if (mem_file(&data, name))
        {
            mesh_header_v0*    header    = mem_raw_data<mesh_header_v0>(&data);
            vf::static_geom_t* fvertices = mem_raw_array<vf::static_geom_t>(&data, header->numVertices);
            uint32_t*          findices  = mem_raw_array<uint32_t>(&data, header->numIndices);

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

            GLuint baseVOffset = vertexOffset;

            assert(indexOffset % 4 ==0);

            GLuint rem = vertexOffset % sizeof(vf::static_geom_t) * header->numVertices;
            baseVOffset += (rem==0) ? 0 : (sizeof(vf::static_geom_t) - rem);

            GLuint verticesSize = sizeof(vf::static_geom_t) * header->numVertices;
            GLuint indicesSize  = sizeof(uint32_t) * header->numIndices;

            memcpy(vertices+baseVOffset, fvertices, verticesSize);
            memcpy(indices+indexOffset,  findices,  indicesSize);

            models[numModels].idxFormat    = GL_UNSIGNED_INT;
            models[numModels].numSubmeshes = header->numSubmeshes;

            first = numMeshes;
            count = header->numSubmeshes;

            for (size_t i = 0; i < header->numSubmeshes; ++i)
            {
                meshes[numMeshes].vbo = vertexBuffer;
                meshes[numMeshes].ibo = indexBuffer;
                meshes[numMeshes].firstVertex = baseVOffset / sizeof(vf::static_geom_t);
                meshes[numMeshes].idxFormat   = GL_UNSIGNED_INT;
                meshes[numMeshes].idxOffset   = indexOffset+header->offsetSize[i*2  ] * sizeof(uint32_t);
                meshes[numMeshes].numIndices  = header->offsetSize[i*2+1];

                ++numMeshes;
            }

            vertexOffset  = baseVOffset+verticesSize;
            indexOffset  += indicesSize;

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

#define BACKWARD_STORE_LIGHT_INDICES

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

        light_t* data = (light_t*)gfx::dynbufAllocMem(lightSize, gfx::caps.tboAlignment, &lightOffset);
        memcpy(data, lightsView, numViewLights*sizeof(light_t));
        glTextureBufferRangeEXT(texLightData, GL_TEXTURE_BUFFER, GL_RGBA32F, gfx::dynBuffer, lightOffset, lightSize);

        assert(lightSize % (sizeof(float)*8) == 0);

        gridDimX = (gfx::width  + LIGHT_GRID_TILE_DIM_X - 1) / LIGHT_GRID_TILE_DIM_X;
        gridDimY = (gfx::height + LIGHT_GRID_TILE_DIM_Y - 1) / LIGHT_GRID_TILE_DIM_Y;
        gridDimZ = 64;

        numClusters = gridDimX * gridDimY * gridDimZ;

        zLogScale = gridDimZ / logf(zfar / znear);
        invZNear  = 1.0f / znear;

        uint32_t numRects;
        ScreenRect3D* rects = mem::alloc_array<ScreenRect3D>(appArena, MAX_LIGHTS);
        int32_t* offsets = mem::alloc_array<int32_t>(appArena, numClusters);
        int32_t* counts  = mem::alloc_array<int32_t>(appArena, numClusters);

        buildRects3D(modelView, proj, MAX_LIGHTS, numRects, rects);

        memset(counts, 0,  sizeof(int32_t)*numClusters);
        memset(offsets, 0, sizeof(int32_t)*numClusters);

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
            for (int32_t idx = 0; idx < numClusters; ++idx)
            {
#ifdef BACKWARD_STORE_LIGHT_INDICES
                offset += counts[idx];
                offsets[idx] = offset;
#else
                offsets[idx] = offset;
                offset += counts[idx];
#endif
            }
        }

#ifndef BACKWARD_STORE_LIGHT_INDICES
        memset(counts, 0,  sizeof(uint32_t)*numClusters);
#endif

        {
            PROFILER_CPU_TIMESLICE("Pass2");

            lightListOffset = 0;
            lightListSize   = core::max(4U, totalus) * sizeof(uint32_t);

            int32_t* data = (int32_t*)gfx::dynbufAllocMem(lightListSize, gfx::caps.tboAlignment, &lightListOffset);

            for (size_t i = 0; i < numRects; ++i)
            {
                ScreenRect3D r = rects[i];

                uint32_t lx = core::max(0, core::min<int>(gridDimX, r.minx));
                uint32_t ly = core::max(0, core::min<int>(gridDimY, r.miny));
                uint32_t lz = core::max(0, core::min<int>(gridDimZ, r.minz));

                uint32_t ux = core::max(0, core::min<int>(gridDimX, r.maxx));
                uint32_t uy = core::max(0, core::min<int>(gridDimY, r.maxy));
                uint32_t uz = core::max(0, core::min<int>(gridDimZ, r.maxz));

                assert(r.index<numViewLights);

                for (uint32_t z = lz; z < uz; ++z)
                {
                    for (uint32_t y = ly; y < uy; ++y)
                    {
                        for (uint32_t x = lx; x < ux; ++x)
                        {
                            uint32_t idx = (z * gridDimY + y) * gridDimX + x;

#ifdef BACKWARD_STORE_LIGHT_INDICES
                            int32_t offset = --offsets[idx];
#else
                            int32_t count = counts[idx];
                            ++counts[idx];
                            int32_t offset = offsets[idx] + count;
#endif

                            data[offset] = r.index;
                        }
                    }
                }
            }
            glTextureBufferRangeEXT(texLightListData, GL_TEXTURE_BUFFER, GL_R32I, gfx::dynBuffer, lightListOffset, lightListSize);
        }
        {
            PROFILER_CPU_TIMESLICE("copyGridFromHost");

            clusterListOffset = 0;
            clusterListSize   = sizeof(gpu_clustered_lighting_t);

            gpu_clustered_lighting_t* data = (gpu_clustered_lighting_t*)gfx::dynbufAllocMem(clusterListSize, gfx::caps.uboAlignment, &clusterListOffset);

            data->uAmbientGlobal = ml::make_vec3(0.02f, 0.02f, 0.02f);
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

            uint32_t clusterDataSize   = 2*sizeof(uint32_t)*numClusters;
            uint32_t clusterDataOffset = 0;
            int32_t* clusterData = (int32_t*)gfx::dynbufAllocMem(clusterDataSize, gfx::caps.tboAlignment, &clusterDataOffset);
            for (int32_t i = 0; i < numClusters; ++i)
            {
                clusterData[2*i]   = offsets[i];
                clusterData[2*i+1] = counts [i];
                assert(offsets[i]+counts [i]<=totalus);
            }
            glTextureBufferRangeEXT(texClusterData, GL_TEXTURE_BUFFER, GL_RG32I, gfx::dynBuffer, clusterDataOffset, clusterDataSize);
        }
        mem::free(appArena, offsets);
        mem::free(appArena, counts);
        mem::free(appArena, rects);
    }
}
