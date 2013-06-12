#define _CRT_SECURE_NO_WARNINGS

#include <framework.h>
#include <glm\glmext.h>
#include <utils.h>
#include <SpectatorCamera.h>
#include <SOIL/SOIL.h>

#include "model.h"

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

float matSHRed[16] = {
     0.09010f, -0.04719f,  0.24026f, -0.14838f,
    -0.04719f, -0.09010f, -0.11155f,  0.19955f,
     0.24026f, -0.11155f, -0.11890f, -0.17397f,
    -0.14838f,  0.19955f, -0.17397f, -0.07239f,
};

float matSHGreen[16] = {
    -0.02145f, -0.02145f,  0.09010f, -0.03070f,
    -0.02145f,  0.02145f, -0.09439f,  0.17908f,
     0.09010f, -0.09439f, -0.06688f, -0.09210f,
    -0.03070f,  0.17908f, -0.09210f, -0.01280f,
};

float matSHBlue[16] = {
    -0.12871f, -0.05149f,  0.06007f,  0.00512f,
    -0.05149f,  0.12871f, -0.20165f,  0.30700f,
     0.06007f, -0.20165f, -0.11147f, -0.13815f,
     0.00512f,  0.30700f, -0.13815f, -0.03463f,
};

class Anima: public ui::Stage
{
private:
    SpectatorCamera     camera;
    glm::mat4           mProj;

    MD5Model            model;

    CPUTimer            cpuTimer;
    GPUTimer            gpuTimer;

    GLuint              prgSkybox;
    GLuint              texSkyboxCubemap;
    GLuint              smpSkybox;

public:
    Anima()
    {
        model.LoadModel("boblampclean.md5mesh");
        model.LoadAnim ("boblampclean.md5anim");

        mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 10000.0f);

        camera.acceleration = glm::vec3(150, 150, 150);
        camera.maxVelocity  = glm::vec3(60, 60, 60);

        prgSkybox = resources::createProgramFromFiles("skybox.vert", "skybox.frag");

        addPrograms(1, &prgSkybox);
        addPrograms(1, &model.prgLighting);
        addPrograms(1, &model.prgDefault);

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
        }

        mfree(&dataPosX);
        mfree(&dataPosY);
        mfree(&dataPosZ);
        mfree(&dataNegX);
        mfree(&dataNegY);
        mfree(&dataNegZ);

        glGenSamplers(1, &smpSkybox);
        glSamplerParameteri(smpSkybox, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(smpSkybox, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(smpSkybox, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(smpSkybox, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(smpSkybox, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*GL_LINEAR_MIPMAP_LINEAR*/);

        CHECK_GL_ERROR();
    }

    ~Anima()
    {
        glDeleteSamplers(1, &smpSkybox);
        glDeleteProgram(prgSkybox);
        glDeleteTextures(1, &texSkyboxCubemap);
    }

protected:
    void drawSkybox(GLuint cubemap)
    {
        glCullFace(GL_FRONT);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, cubeVertices);

        glUseProgram(prgSkybox);
        
        glBindSampler(0, smpSkybox);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

        glDrawElements(GL_TRIANGLES, 6*2*3, GL_UNSIGNED_SHORT, cubeIndices);

        glUseProgram(0);
        glBindSampler(0, 0);

        glDisableVertexAttribArray(0);
        glCullFace(GL_BACK);

    }

    void onKeyUp(const KeyEvent& event)
    {
        if ((event.keysym.sym==SDLK_LALT  && event.keysym.mod==KMOD_LCTRL||
            event.keysym.sym==SDLK_LCTRL && event.keysym.mod==KMOD_LALT))
        {
            releaseMouse();
        }
    }

    void onTouch(const ButtonEvent& event)
    {
        UNUSED(event);
        captureMouse();
    }

    void onPaint()
    {
        GLenum err;

        glClearDepth(1.0);

        glFrontFace(GL_CCW);
        glCullFace (GL_BACK);
        glEnable   (GL_CULL_FACE);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadMatrixf(mProj);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glm::mat4 vm;
        camera.getViewMatrix(vm);
        glLoadMatrixf(vm);

        glDisable(GL_CULL_FACE);

        glUseProgram(0);
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0);
        glVertex3f(-3, -1, -10);
        glColor3f(0, 1, 0);
        glVertex3f( 3, -1, -10);
        glColor3f(0, 0, 1);
        glVertex3f( 0, 4, -10);
        glEnd();

        gpuTimer.start();
        model.Render(mProj*vm, matSHRed, matSHGreen, matSHBlue);
        gpuTimer.stop();

        drawSkybox(texSkyboxCubemap);
 
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glDisable(GL_BLEND);

        ui::displayStats(10.0f, 10.0f, 300.0f, 70.0f, cpuTime, gpuTime);

        err = glGetError();
        assert(err==GL_NO_ERROR);
    }

    float cpuTime, gpuTime;

    void onUpdate(float dt)
    {
        static const float maxTimeStep = 0.03333f;

        cpuTimer.start();

        model.Update(std::min(dt, maxTimeStep));

        cpuTime = (float)cpuTimer.elapsed();
        cpuTimer.stop();

        gpuTime = (float)gpuTimer.getResult();

        if (isMouseCaptured())
            ui::processCameraInput(&camera, dt);
    }
};

int main(int argc, char** argv)
{
    fwk::init(argv[0]);

    {
        Anima app;
        app.run();
    }

    fwk::cleanup();

    return 0;
}