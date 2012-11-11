#define _CRT_SECURE_NO_WARNINGS

#include <framework.h>
#include <glm\glmext.h>
#include <cassert>
#include <ResourceHelpers.h>
#include <SOIL.h>
#include <utils.h>
#include <SpectatorCamera.h>
#include "DebugDraw.h"
#include "MeshLoaderObj.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// OpenGL debug draw implementation.
class DebugDrawGL : public duDebugDraw
{
public:
	virtual void depthMask(bool state);
	virtual void texture(bool state);
	virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f);
	virtual void vertex(const float* pos, unsigned int color);
	virtual void vertex(const float x, const float y, const float z, unsigned int color);
	virtual void vertex(const float* pos, unsigned int color, const float* uv);
	virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v);
	virtual void end();
};

class GLCheckerTexture
{
	unsigned int m_texId;
public:
	GLCheckerTexture() : m_texId(0)
	{
	}
	
	~GLCheckerTexture()
	{
		if (m_texId != 0)
			glDeleteTextures(1, &m_texId);
	}
	void bind()
	{
		if (m_texId == 0)
		{
			// Create checker pattern.
			const unsigned int col0 = duRGBA(215,215,215,255);
			const unsigned int col1 = duRGBA(255,255,255,255);
			static const int TSIZE = 64;
			unsigned int data[TSIZE*TSIZE];
			
			glGenTextures(1, &m_texId);
			glBindTexture(GL_TEXTURE_2D, m_texId);

			int level = 0;
			int size = TSIZE;
			while (size > 0)
			{
				for (int y = 0; y < size; ++y)
					for (int x = 0; x < size; ++x)
						data[x+y*size] = (x==0 || y==0) ? col0 : col1;
				glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, size,size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				size /= 2;
				level++;
			}
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_texId);
		}
	}
};
GLCheckerTexture g_tex;


void DebugDrawGL::depthMask(bool state)
{
	glDepthMask(state ? GL_TRUE : GL_FALSE);
}

void DebugDrawGL::texture(bool state)
{
	if (state)
	{
		glEnable(GL_TEXTURE_2D);
		g_tex.bind();
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void DebugDrawGL::begin(duDebugDrawPrimitives prim, float size)
{
	switch (prim)
	{
		case DU_DRAW_POINTS:
			glPointSize(size);
			glBegin(GL_POINTS);
			break;
		case DU_DRAW_LINES:
			glLineWidth(size);
			glBegin(GL_LINES);
			break;
		case DU_DRAW_TRIS:
			glBegin(GL_TRIANGLES);
			break;
		case DU_DRAW_QUADS:
			glBegin(GL_QUADS);
			break;
	};
}

void DebugDrawGL::vertex(const float* pos, unsigned int color)
{
	glColor4ubv((GLubyte*)&color);
	glVertex3fv(pos);
}

void DebugDrawGL::vertex(const float x, const float y, const float z, unsigned int color)
{
	glColor4ubv((GLubyte*)&color);
	glVertex3f(x,y,z);
}

void DebugDrawGL::vertex(const float* pos, unsigned int color, const float* uv)
{
	glColor4ubv((GLubyte*)&color);
	glTexCoord2fv(uv);
	glVertex3fv(pos);
}

void DebugDrawGL::vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v)
{
	glColor4ubv((GLubyte*)&color);
	glTexCoord2f(u,v);
	glVertex3f(x,y,z);
}

void DebugDrawGL::end()
{
	glEnd();
	glLineWidth(1.0f);
	glPointSize(1.0f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void duDebugDrawTriMeshSlope(duDebugDraw* dd, const float* verts, int /*nverts*/,
							 const int* tris, const float* normals, int ntris,
							 const float walkableSlopeAngle, const float texScale)
{
	if (!dd) return;
	if (!verts) return;
	if (!tris) return;
	if (!normals) return;
	
	const float walkableThr = cosf(walkableSlopeAngle/180.0f*DU_PI);
	
	float uva[2];
	float uvb[2];
	float uvc[2];
	
	dd->texture(true);

	const unsigned int unwalkable = duRGBA(192,128,0,255);
	
	dd->begin(DU_DRAW_TRIS);
	for (int i = 0; i < ntris*3; i += 3)
	{
		const float* norm = &normals[i];
		unsigned int color;
		unsigned char a = (unsigned char)(220*(2+norm[0]+norm[1])/4);
		if (norm[1] < walkableThr)
			color = duLerpCol(duRGBA(a,a,a,255), unwalkable, 64);
		else
			color = duRGBA(a,a,a,255);
		
		const float* va = &verts[tris[i+0]*3];
		const float* vb = &verts[tris[i+1]*3];
		const float* vc = &verts[tris[i+2]*3];
		
		int ax = 0, ay = 0;
		if (fabs(norm[1]) > fabs(norm[ax]))
			ax = 1;
		if (fabs(norm[2]) > fabs(norm[ax]))
			ax = 2;
		ax = (1<<ax)&3; // +1 mod 3
		ay = (1<<ax)&3; // +1 mod 3
		
		uva[0] = va[ax]*texScale;
		uva[1] = va[ay]*texScale;
		uvb[0] = vb[ax]*texScale;
		uvb[1] = vb[ay]*texScale;
		uvc[0] = vc[ax]*texScale;
		uvc[1] = vc[ay]*texScale;
		
		dd->vertex(va, color, uva);
		dd->vertex(vb, color, uvb);
		dd->vertex(vc, color, uvc);
	}
	dd->end();

	dd->texture(false);
}

class Exest: public ui::Stage
{
private:
    SpectatorCamera     camera;

    VFS				mVFS;

    glm::mat4	mProj, VP;

    float vertAngle, horzAngle;

    mt::Task                mUpdateTask;
    ui::ProfileStatsBox     mStatsBox;
    GLuint colorTex;

	rcMeshLoaderObj* m_geom;

public:
    Exest():moveFwd(false), moveBwd(false), moveLeft(false), moveRight(false), loc(0)
    {
        lastTimeMark = timerAbsoluteTime();

        VFS::mount("AppData");
        VFS::mount("../AppData");
        VFS::mount("../../AppData");

        mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 10000.0f);

        camera.acceleration = glm::vec3(15, 15, 15);
        camera.maxVelocity  = glm::vec3(6, 6, 6);
        //camera.viewMatrix = glm::mat4(-0.68835747f,   2.7175300e-008f,   0.72537768f,   0.0f,
        //    0.094680540f,  0.99144882f,       0.089849062f,  0.0f,
        //    -0.71917069f,   0.13052642f,      -0.68246961f,   0.0f,
        //    0.0f,          0.0f,              0.0f,          1.0f);
        //camera.pos = glm::vec3(-451.47745f,  45.857117f, -437.07669f);

        glm::mat4 lookAt = glm::lookAtGTX<float>(glm::vec3(0, 0, 0), glm::vec3(0, 10, 0), glm::vec3(1, 0, 0));
        glm::mat4 proj = glm::perspectiveGTX<float>(33.0f, 1.33333333f, 0.1f, 1200.0f);

        VP = proj*lookAt;

        mt::addTimedTask<Exest, &Exest::handleInput>(this, 20);
        colorTex = resources::createTexture2D("debugTexture.png");

        add(mStatsBox.add(mSelectTimeLabel)
            .add(mDrawTimeLabel)
            .add(mGeomStatLabel)
            .setPos(10, 10)
            .setSize(300, 140)
            );

        mt::addTimedTask<Exest, &Exest::onUpdateStats>(this, 250);

        FILE* cam = fopen("cameras.txt", "r");
        if (cam)
        {
            while(!feof(cam))
            {
                glm::__quatGTX  q;
                glm::vec3       p;
                fscanf(cam, "%f %f %f %f %f %f %f\n", &q.x, &q.y, &q.z, &q.w, &p.x, &p.y, &p.z);
                savedCamOrient.push_back(q);
                savedCamPos.push_back(p);
            }
            fclose(cam);
        }

        m_geom = new rcMeshLoaderObj();
        m_geom->load("k:\\projects\\graphics\\Infinity\\AppData\\dungeon.obj");

    }

    ui::Label	mDrawTimeLabel;
    ui::Label	mSelectTimeLabel;
    ui::Label	mGeomStatLabel;

    ~Exest()
    {
        delete m_geom;
        FILE* cam = fopen("cameras.txt", "w");
        if (cam)
        {
            for (size_t i=0; i<savedCamOrient.size(); ++i)
            {
                glm::__quatGTX  q=savedCamOrient[i];
                glm::vec3       p=savedCamPos[i];
                fprintf(cam, "%f %f %f %f %f %f %f\n", q.x, q.y, q.z, q.w, p.x, p.y, p.z);
            }
            fclose(cam);
        }
        glDeleteTextures(1, &colorTex);
    }

protected:
    void onShaderRecompile()
    {
    }

    std::vector<glm::__quatGTX> savedCamOrient;
    std::vector<glm::vec3>      savedCamPos;
    int                         loc;

    void onKeyUp(const KeyEvent& event)
    {
        if ((event.keysym.sym==SDLK_LALT  && event.keysym.mod==KMOD_LCTRL||
             event.keysym.sym==SDLK_LCTRL && event.keysym.mod==KMOD_LALT))
        {
            releaseMouse();
        }
        if (event.keysym.sym==SDLK_0)
        {
            savedCamOrient.push_back(camera.getOrientation());
            savedCamPos.push_back(camera.getPosition());
        }
        if (event.keysym.sym==SDLK_PERIOD && savedCamOrient.size())
        {
            loc=(loc+1)%savedCamOrient.size();
            camera.setOrientation(savedCamOrient[loc]);
            camera.setPosition(savedCamPos[loc]);
        }
        if (event.keysym.sym==SDLK_COMMA && savedCamOrient.size())
        {
            loc=(loc-1)%savedCamOrient.size();
            camera.setOrientation(savedCamOrient[loc]);
            camera.setPosition(savedCamPos[loc]);
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

        glEnable(GL_CULL_FACE);
        //glCullFace(GL_CCW);

        DebugDrawGL dd;
        duDebugDrawTriMeshSlope(&dd, m_geom->getVerts(), m_geom->getVertCount(),
								m_geom->getTris(), m_geom->getNormals(), m_geom->getTriCount(),
								45.0f, 0.33333f);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glDisable(GL_BLEND);

        err = glGetError();
        assert(err==GL_NO_ERROR);
    }

    void onUpdateStats()
    {
        //mStatsBox.setStats(terrain.getCPUTime(), terrain.getGPUTime());
        //wchar_t str[256];
        //_snwprintf(str, 256, L"CPU select time - %f ms", terrain.getCPUSelectTime());
        //mSelectTimeLabel.setText(str);
        //_snwprintf(str, 256, L"CPU draw time - %f ms", terrain.getCPUDrawTime());
        //mDrawTimeLabel.setText(str);
        //int patches = terrain.patchCount;
        //int vtx = patches*terrain.patchDim*terrain.patchDim;
        //_snwprintf(str, 256, L"Patches: %d, Vtx: %d", patches, vtx);
        //mGeomStatLabel.setText(str);
    }

    bool moveFwd, moveBwd, moveLeft, moveRight;
    __int64 lastTimeMark;

    void onMotion(const MotionEvent& event)
    {
        camera.rotateSmoothly((float)-event.xrel, (float)-event.yrel);
    }

    void handleInput()
    {
        glm::vec3 direction;
        float heading = 0.0f, pitch = 0.0f;
        uint8_t *keystate = SDL_GetKeyState(NULL);
        if (keystate[SDLK_w])
        {
            camera.velocity.z = moveFwd?camera.velocity.z:0.0f;
            moveFwd = true;
            direction.z += 1.0f;
        }
        else
            moveFwd = false;
        if (keystate[SDLK_s])
        {
            camera.velocity.z = moveBwd?camera.velocity.z:0.0f;
            moveBwd = true;
            direction.z -= 1.0f;
        }
        else
            moveBwd = false;
        if (keystate[SDLK_a])
        {
            camera.velocity.x = moveLeft?camera.velocity.x:0.0f;
            moveLeft = true;
            direction.x -= 1.0f;
        }
        else
            moveLeft = false;
        if (keystate[SDLK_d])
        {
            camera.velocity.x = moveRight?camera.velocity.x:0.0f;
            moveRight = true;
            direction.x += 1.0f;
        }
        else
            moveRight = false;
        if (keystate[SDLK_KP8])
            pitch += 1.50f;
        if (keystate[SDLK_KP5])
            pitch += -1.50f;
        if (keystate[SDLK_KP4])
            heading += 1.50f;
        if (keystate[SDLK_KP6])
            heading += -1.50f;

        _int64 time = timerAbsoluteTime();

        camera.rotateSmoothly(heading, pitch);
        camera.updatePosition(direction, (time-lastTimeMark)*0.000001f);

        lastTimeMark = time;
    }
};

int main(int argc, char** argv)
{
    Exest app;
    app.run();

    return 0;
}