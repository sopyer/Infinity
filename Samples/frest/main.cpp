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

class Frest: public ui::Stage
{
private:
    SpectatorCamera     camera;

    glm::mat4	mProj;

    rcMeshLoaderObj* m_geom;

public:
    Frest()
    {
        mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 10000.0f);

        camera.acceleration = glm::vec3(15, 15, 15);
        camera.maxVelocity  = glm::vec3(6, 6, 6);

        m_geom = new rcMeshLoaderObj();
        m_geom->load("dungeon.obj");

    }

    ~Frest()
    {
        delete m_geom;
    }

protected:
    void onShaderRecompile()
    {
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

        v128 vm[4];
        camera.getViewMatrix(vm);
        glLoadMatrixf((float*)vm);

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

        DebugDrawGL dd;
        duDebugDrawTriMeshSlope(&dd, m_geom->getVerts(), m_geom->getVertCount(),
            m_geom->getTris(), m_geom->getNormals(), m_geom->getTriCount(),
            45.0f, 0.33333f);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glDisable(GL_BLEND);

        ui::displayStats(10.0f, 10.0f, 300.0f, 70.0f, 0.0f, 0.0f);

        err = glGetError();
        assert(err==GL_NO_ERROR);
    }

    void onUpdate(float dt)
    {
        ui::processCameraInput(&camera, dt);
    }
};

int main(int argc, char** argv)
{
    fwk::init(argv[0]);

    {
        Frest app;
        app.run();
    }

    fwk::fini();

    return 0;
}