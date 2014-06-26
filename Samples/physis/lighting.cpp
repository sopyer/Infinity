#include <opengl.h>
#include <stdint.h>
#include <math.h>
#include <ml.h>
#include <SOIL/SOIL.h>
#include <utils.h>
#include <framework.h>
#include <SpectatorCamera.h>
#include <glm/glm.h>

#include "lighting.h"

namespace lighting
{

#define CP_PI 3.14159265358979323846f

    GLfloat cubeVertices[8*3] =
    {
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
    };

    GLushort cubeIndices[6*2*3] = 
    {
        0, 1, 3, 0, 3, 4, 0, 4, 1,
        2, 3, 1, 2, 1, 6, 2, 6, 3,
        5, 6, 1, 5, 1, 4, 5, 4, 6,
        7, 4, 3, 7, 3, 6, 7, 6, 4,
    };

    enum
    {
        FACE_POS_X, FACE_NEG_X,
        FACE_POS_Y, FACE_NEG_Y,
        FACE_POS_Z, FACE_NEG_Z,
        NUM_FACES
    };

    void xshEval3(float x, float y, float z, float* sh)
    {
        float t[3];
        float c[2], s[2];
        float y2 = y * y;

        /* Y{l,0} */
        sh[0] = 0.28209480643272400f;
        sh[2] = 0.48860251903533936f * y;
        sh[6] = 0.94617468118667603f * y2 + -0.31539157032966614f;

        c[0] = x;
        s[0] = z;

        /* Y{l,1} */
        t[0] =  0.48860251903533936f;
        sh[3] = t[0] * c[0];
        sh[1] = t[0] * s[0];
        t[1] =  1.09254848957061770f * y;
        sh[7] = t[1] * c[0];
        sh[5] = t[1] * s[0];

        c[1] = x * c[0] - z * s[0];
        s[1] = x * s[0] + z * c[0];

        /* Y{l,2} */
        t[0] = 0.54627424478530884f;
        sh[8] = t[0] * c[1];
        sh[4] = t[0] * s[1];
    }

    static void cubemapTexelToVector(int face, float nu, float nv, float* nx, float* ny, float* nz)
    {
        switch (face)
        {
        case FACE_POS_X: *nx =  1;  *ny = -nv; *nz =  nu; break;
        case FACE_NEG_X: *nx = -1;  *ny = -nv; *nz = -nu; break;
        case FACE_POS_Y: *nx =  nu; *ny =  1;  *nz = -nv; break;
        case FACE_NEG_Y: *nx =  nu; *ny = -1;  *nz =  nv; break;
        case FACE_POS_Z: *nx =  nu; *ny = -nv; *nz = -1;  break;
        case FACE_NEG_Z: *nx = -nu; *ny = -nv; *nz =  1;  break;
        }

        // Normalize vector
        float len = sqrtf((*nx) * (*nx) + (*ny) * (*ny) + (*nz) * (*nz));

        *nx /= len;	*ny /= len;	*nz /= len;
    }

    static float areaIntegral(float x, float y)
    {
        return atan2(x * y, sqrt(x * x + y * y + 1));
    }

    static float cubemapTexelSolidAngle(float nu, float nv, float texelSize)
    {
        // u and v are the -1..1 texture coordinate on the current face.
        // Get projected area for this texel
        float x0 = nu - texelSize;
        float y0 = nv - texelSize;
        float x1 = nu + texelSize;
        float y1 = nv + texelSize;
        float solidAngle = areaIntegral(x0, y0) - areaIntegral(x0, y1) - areaIntegral(x1, y0) + areaIntegral(x1, y1);

        return solidAngle;
    }

    void xshProjectCubeMap3(int size, uint32_t* faces[NUM_FACES], v128 sh[9])
    {
        for (int i = 0; i < 9; ++i)
        {
            sh[i] = vi_set_0000();
        }

        float sphereArea = 0.0f;
        float texelSize  = 1.0f / (float)size;

        for (int face = 0; face < NUM_FACES; ++face)
        {
            const uint32_t* data = faces[face];

            for (int y = 0; y < size; ++y)
            {
                for (int x = 0; x < size; ++x)
                {
                    uint32_t texel = data[y*size+x];

                    float nu = (2.0f * texelSize * ((float)x + 0.5f)) - 1.0f;
                    float nv = (2.0f * texelSize * ((float)y + 0.5f)) - 1.0f;

                    float solidAngle;

                    float shFunc[9];
                    float nx, ny, nz;

                    solidAngle = cubemapTexelSolidAngle(nu, nv, texelSize);
                    cubemapTexelToVector(face, nu, nv, &nx, &ny, &nz);
                    xshEval3(nx, ny, nz, shFunc);

                    for (int i = 0; i < 9; ++i)
                    {
                        v128 scale = vi_set_ffff(shFunc[i] * solidAngle);
                        v128 color = vi_cvt_ubyte4_to_vec4(texel); //TODO: add gamma correction????

                        sh[i] = vi_add(sh[i], vi_mul(scale, color));
                    }

                    sphereArea += solidAngle;
                }
            }
        }
    }

    void xshGenEnvMap(v128 env[9], v128 cubeProj[9])
    {
        float Al[9] = {1.0f, 0.66666666666666667f, 0.66666666666666667f, 0.66666666666666667f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f};

        env[0] = cubeProj[0];
        for (size_t i = 1; i < 9; ++i)
        {
            env[i] = vi_mul(cubeProj[i], vi_set_ffff(Al[i]));
        }
    }

    void xshBuildPoly(v128 poly[10], v128 sh[9])
    {
        float c1 = 0.54627424478530884f;
        float c2 = 0.48860251903533936f;
        float c3 = 0.94617468118667603f;
        float c4 = 0.28209480643272400f;
        float c5 = 0.31539157032966614f;

        poly[0] = vi_mul(vi_set_ffff(  c1), sh[8]);
        poly[1] = vi_mul(vi_set_ffff(  c3), sh[6]);
        poly[2] = vi_mul(vi_set_ffff( -c1), sh[8]);

        poly[3] = vi_mul(vi_set_ffff(2*c1), sh[7]);
        poly[4] = vi_mul(vi_set_ffff(2*c1), sh[5]);
        poly[5] = vi_mul(vi_set_ffff(2*c1), sh[4]);

        poly[6] = vi_mul(vi_set_ffff(  c2), sh[3]);
        poly[7] = vi_mul(vi_set_ffff(  c2), sh[2]);
        poly[8] = vi_mul(vi_set_ffff(  c2), sh[1]);

        poly[9] = vi_sub(vi_mul(vi_set_ffff(c4), sh[0]), vi_mul(vi_set_ffff(c5), sh[6]));
    }

    GLuint prgSkybox;
    GLuint texSkyboxCubemap;
    GLuint smpSkybox;

    GLuint prgSH;
    GLuint ubo;
    v128   shPoly[10];

    SpectatorCamera     camera;
    glm::mat4           mProj;

    void init()
    {
        mProj = glm::perspectiveGTX(30.0f, 1280.0f/720.0f, 0.1f, 10000.0f);

        camera.acceleration = glm::vec3(150, 150, 150);
        camera.maxVelocity  = glm::vec3(60, 60, 60);

        prgSkybox = resources::createProgramFromFiles("skybox.vert", "skybox.frag");
        prgSH     = resources::createProgramFromFiles("box.vert", "wireframe.geom", "box.frag");

        memory_t dataPosX = {0, 0, 0};
        memory_t dataPosY = {0, 0, 0};
        memory_t dataPosZ = {0, 0, 0};
        memory_t dataNegX = {0, 0, 0};
        memory_t dataNegY = {0, 0, 0};
        memory_t dataNegZ = {0, 0, 0};

        mopen(&dataPosX, "posx.jpg");
        mopen(&dataPosY, "posy.jpg");
        mopen(&dataPosZ, "posz.jpg");
        mopen(&dataNegX, "negx.jpg");
        mopen(&dataNegY, "negy.jpg");
        mopen(&dataNegZ, "negz.jpg");

        if (dataPosX.buffer && dataPosY.buffer && dataPosZ.buffer &&
            dataNegX.buffer && dataNegY.buffer && dataNegZ.buffer)
        {
            texSkyboxCubemap = SOIL_load_OGL_cubemap_from_memory(
                (unsigned char*)dataPosX.buffer, dataPosX.size,
                (unsigned char*)dataNegX.buffer, dataNegX.size,
                (unsigned char*)dataPosY.buffer, dataPosY.size,
                (unsigned char*)dataNegY.buffer, dataNegY.size,
                (unsigned char*)dataPosZ.buffer, dataPosZ.size,
                (unsigned char*)dataNegZ.buffer, dataNegZ.size,
                0, 0, 0);

            int w, h, size;
            uint32_t* faceData[NUM_FACES];
            v128  vSH[9], vEnv[9];

            faceData[FACE_POS_X] = (uint32_t*)SOIL_load_image_from_memory(dataPosX.buffer, dataPosX.size, &size, &h, 0, 4);
            assert(size == h);
            faceData[FACE_NEG_X] = (uint32_t*)SOIL_load_image_from_memory(dataNegX.buffer, dataNegX.size, &w, &h, 0, 4);
            assert(w == h && w==size);
            faceData[FACE_POS_Y] = (uint32_t*)SOIL_load_image_from_memory(dataPosY.buffer, dataPosY.size, &w, &h, 0, 4);
            assert(w == h && w==size);
            faceData[FACE_NEG_Y] = (uint32_t*)SOIL_load_image_from_memory(dataNegY.buffer, dataNegY.size, &w, &h, 0, 4);
            assert(w == h && w==size);
            faceData[FACE_POS_Z] = (uint32_t*)SOIL_load_image_from_memory(dataPosZ.buffer, dataPosZ.size, &w, &h, 0, 4);
            assert(w == h && w==size);
            faceData[FACE_NEG_Z] = (uint32_t*)SOIL_load_image_from_memory(dataNegZ.buffer, dataNegZ.size, &w, &h, 0, 4);
            assert(w == h && w==size);

            xshProjectCubeMap3(size, faceData, vSH);
            xshGenEnvMap(vEnv, vSH);
            xshBuildPoly(shPoly, vEnv);

            for (int i = 0; i < NUM_FACES; ++i)
            {
                SOIL_free_image_data((uint8_t*)faceData[i]);
            }
        }

        mfree(&dataPosX);
        mfree(&dataPosY);
        mfree(&dataPosZ);
        mfree(&dataNegX);
        mfree(&dataNegY);
        mfree(&dataNegZ);

        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, 256, 0, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenSamplers(1, &smpSkybox);
        glSamplerParameteri(smpSkybox, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(smpSkybox, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(smpSkybox, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(smpSkybox, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(smpSkybox, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    void fini()
    {
        glDeleteSamplers(1, &smpSkybox);
        glDeleteProgram(prgSkybox);
        glDeleteProgram(prgSH);
        glDeleteTextures(1, &texSkyboxCubemap);
        glDeleteBuffers(1, &ubo);
    }

    void drawBox();
    void drawSkybox(GLuint cubemap);

    void draw()
    {
        glDisable(GL_CULL_FACE);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadMatrixf(mProj);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glm::mat4 vm;
        camera.getViewMatrix(vm);
        glLoadMatrixf(vm);

        glUseProgram(0);
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0);
        glVertex3f(-3, -1, -10);
        glColor3f(0, 1, 0);
        glVertex3f( 3, -1, -10);
        glColor3f(0, 0, 1);
        glVertex3f( 0, 4, -10);
        glEnd();

        //glEnable   (GL_CULL_FACE);

        glTranslatef(0, 9, 0);
        glScalef(4, 4, 4);
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, ubo, 0, 160);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, 160, shPoly);
        glUseProgram(prgSH);
        drawBox();

        drawSkybox(texSkyboxCubemap);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glDisable(GL_BLEND);
        CHECK_GL_ERROR();
    }

    void update(float dt)
    {
        //if (isMouseCaptured())
            ui::processCameraInput(&camera, dt);
    }

    void drawBox()
    {
        CHECK_GL_ERROR();
        glEnableVertexAttribArray(0);
        CHECK_GL_ERROR();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, cubeVertices);
        CHECK_GL_ERROR();

        glDrawElements(GL_TRIANGLES, 6*2*3, GL_UNSIGNED_SHORT, cubeIndices);
        CHECK_GL_ERROR();

        glDisableVertexAttribArray(0);
        CHECK_GL_ERROR();
    }

    void drawSkybox(GLuint cubemap)
    {
        glCullFace(GL_FRONT);

        glUseProgram(prgSkybox);

        glBindSampler(0, smpSkybox);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

        drawBox();

        glUseProgram(0);
        glBindSampler(0, 0);

        glCullFace(GL_BACK);

    }
}
