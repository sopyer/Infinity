#define _CRT_SECURE_NO_WARNINGS

#include <framework.h>
#include <glm\glmext.h>
#include <utils.h>
#include <SpectatorCamera.h>
#include <SOIL/SOIL.h>

#include "model.h"

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

enum
{
	FACE_POS_X, FACE_NEG_X,
	FACE_POS_Y, FACE_NEG_Y,
	FACE_POS_Z, FACE_NEG_Z,
	NUM_FACES
};

void xshEval3_Test(float x, float y, float z, float* sh)
{
	// Can be optimize by precomputing constant.
	static const float SqrtPi = sqrt(CP_PI);

	sh[0]  = 0.28209479177387814347f;

	sh[1]  = -(sqrt(3.0f/CP_PI)*y)/2.0f;
	sh[2]  =  (sqrt(3.0f/CP_PI)*z)/2.0f;
	sh[3]  = -(sqrt(3.0f/CP_PI)*x)/2.0f;

	sh[4]  =  (sqrt(15.0f/CP_PI)*x*y)/2.0f;
	sh[5]  = -(sqrt(15.0f/CP_PI)*y*z)/2.0f;
	sh[6]  =  (sqrt( 5.0f/CP_PI)*(3.0f*z*z - 1.0f))/4.0f;
	sh[7]  = -(sqrt(15.0f/CP_PI)*x*z)/2.0f;
	sh[8]  =   sqrt(15.0f/CP_PI)*(x*x - y*y)/4.0f;
}

void xshEval3(float x, float y, float z, float* sh)
{
	float t[3];
	float c[2], s[2];
	float z2 = z * z;

	/* Y{l,0} */
	sh[0] = 0.28209480643272400f;
	sh[2] = 0.48860251903533936f * z;
	sh[6] = 0.94617468118667603f * z2 + -0.31539157032966614f;

	c[0] = x;
	s[0] = y;

	/* Y{l,1} */
	t[0] = -0.48860251903533936f;
	sh[3] = t[0] * c[0];
	sh[1] = t[0] * s[0];
	t[1] = -1.09254848957061770f * z;
	sh[7] = t[1] * c[0];
	sh[5] = t[1] * s[0];

	c[1] = x * c[0] - y * s[0];
	s[1] = x * s[0] + y * c[0];

	/* Y{l,2} */
	t[0] = 0.54627424478530884f;
	sh[8] = t[0] * c[1];
	sh[4] = t[0] * s[1];
}

static void cubemapTexelToVector(int face, float nu, float nv, float* nx, float* ny, float* nz)
{
	switch (face)
    {
	    case FACE_POS_X: *nx =  1;  *ny = -nv; *nz = -nu; break;
	    case FACE_NEG_X: *nx = -1;  *ny = -nv; *nz =  nu; break;
	    case FACE_POS_Y: *nx =  nu; *ny =  1;  *nz =  nv; break;
	    case FACE_NEG_Y: *nx =  nu; *ny = -1;  *nz = -nv; break;
	    case FACE_POS_Z: *nx =  nu; *ny = -nv; *nz =  1;  break;
	    case FACE_NEG_Z: *nx = -nu; *ny = -nv; *nz = -1;  break;
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

void xshProjectCubeMap3(int size, uint32_t* faces[NUM_FACES], float shRed[9], float shGreen[9], float shBlue[9])
{
	for (int i = 0; i < 9; ++i)
    {
		shRed  [i] = 0.0f;
		shGreen[i] = 0.0f;
		shBlue [i] = 0.0f;
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

                //TODO: add gamma correction????
                float r, g, b;
                r = ((texel    ) & 0xFF) / 255.0f;
                g = ((texel>>8 ) & 0xFF) / 255.0f;
                b = ((texel>>16) & 0xFF) / 255.0f;

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
                    shRed  [i] += r * shFunc[i] * solidAngle;
		            shGreen[i] += g * shFunc[i] * solidAngle;
		            shBlue [i] += b * shFunc[i] * solidAngle;
                }

                sphereArea += solidAngle;
			}
		}
	}
}

void xshBuildEnvMapMatrix(float sh[9], float chMat[16])
{
    /* Form the quadratic form matrix (see equations 11 and 12 in paper) */
    float c1 = 1.0f;//0.429043f/3.14f; 
    float c2 = 1.0f;//0.511664f/3.14f;
    float c3 = 1.0f;//0.743125f/3.14f;
    float c4 = 1.0f;//0.886227f/3.14f;
    float c5 = 1.0f;//0.247708f/3.14f;

    chMat[0]  = c1*sh[8]; /* c1 L_{22}  */
    chMat[1]  = c1*sh[4]; /* c1 L_{2-2} */
    chMat[2]  = c1*sh[7]; /* c1 L_{21}  */
    chMat[3]  = c2*sh[3]; /* c2 L_{11}  */

    chMat[4]  =  c1*sh[4]; /* c1 L_{2-2} */
    chMat[5]  = -c1*sh[8]; /*-c1 L_{22}  */
    chMat[6]  =  c1*sh[5]; /* c1 L_{2-1} */
    chMat[7]  =  c2*sh[1]; /* c2 L_{1-1} */

    chMat[8]  = c1*sh[7]; /* c1 L_{21}  */
    chMat[9]  = c1*sh[5]; /* c1 L_{2-1} */
    chMat[10] = c3*sh[6]; /* c3 L_{20}  */
    chMat[11] = c2*sh[2]; /* c2 L_{10}  */

    chMat[12] = c2*sh[3]; /* c2 L_{11}  */
    chMat[13] = c2*sh[1]; /* c2 L_{1-1} */
    chMat[14] = c2*sh[2]; /* c2 L_{10}  */
    chMat[15] = c4*sh[0] - c5*sh[6]; /* c4 L_{00} - c5 L_{20} */
}

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

    GLuint              prgSH;
    GLuint              ubo;

public:
    Anima()
    {
        model.LoadModel("boblampclean.md5mesh");
        //model.LoadAnim ("boblampclean.md5anim");

        mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 10000.0f);

        camera.acceleration = glm::vec3(150, 150, 150);
        camera.maxVelocity  = glm::vec3(60, 60, 60);

        prgSkybox = resources::createProgramFromFiles("skybox.vert", "skybox.frag");
        prgSH     = resources::createProgramFromFiles("box.vert", "wireframe.geom", "box.frag");

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

            int w, h, size;
            uint32_t* faceData[NUM_FACES];
            float shRed[9], shGreen[9], shBlue[9];

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

            xshProjectCubeMap3(size, faceData, shRed, shGreen, shBlue);
            xshBuildEnvMapMatrix(shRed,   matSHRed  );
            xshBuildEnvMapMatrix(shGreen, matSHGreen);
            xshBuildEnvMapMatrix(shBlue,  matSHBlue );

            float sh[9];
            xshEval3(0, 0, -1, sh);

            float vr = 0.0f;
            float vg = 0.0f;
            float vb = 0.0f;
            for (int i=0; i<9; ++i)
            {
                vr += sh[i]*shRed[i];
                vg += sh[i]*shGreen[i];
                vb += sh[i]*shBlue[i];
            }

            //__asm int 3;

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
        glSamplerParameteri(smpSkybox, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*GL_LINEAR_MIPMAP_LINEAR*/);

        CHECK_GL_ERROR();
    }

    ~Anima()
    {
        glDeleteSamplers(1, &smpSkybox);
        glDeleteProgram(prgSkybox);
        glDeleteProgram(prgSH);
        glDeleteTextures(1, &texSkyboxCubemap);
        glDeleteBuffers(1, &ubo);
    }

protected:
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
        glTranslatef(0, 9, 0);
        glScalef(4, 4, 4);
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, ubo, 0, 3*sizeof(ml::mat4x4));
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0,   64, matSHRed);
        glBufferSubData(GL_UNIFORM_BUFFER, 64,  64, matSHGreen);
        glBufferSubData(GL_UNIFORM_BUFFER, 128, 64, matSHBlue);
        glUseProgram(prgSH);
        drawBox();
        //model.Render(mProj*vm, matSHRed, matSHGreen, matSHBlue);
        gpuTimer.stop();

        drawSkybox(texSkyboxCubemap);
 
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glDisable(GL_BLEND);

        ui::displayStats(10.0f, 10.0f, 300.0f, 70.0f, cpuTime, gpuTime);

        CHECK_GL_ERROR();
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