#define _CRT_SECURE_NO_WARNINGS

#include <fwk/fwk.h>
#include "MeshLoaderObj.h"

inline unsigned int duRGBA(int r, int g, int b, int a)
{
    return ((unsigned int)r) | ((unsigned int)g << 8) | ((unsigned int)b << 16) | ((unsigned int)a << 24);
}

inline unsigned int duLerpCol(unsigned int ca, unsigned int cb, unsigned int u)
{
    const unsigned int ra = ca & 0xff;
    const unsigned int ga = (ca >> 8) & 0xff;
    const unsigned int ba = (ca >> 16) & 0xff;
    const unsigned int aa = (ca >> 24) & 0xff;
    const unsigned int rb = cb & 0xff;
    const unsigned int gb = (cb >> 8) & 0xff;
    const unsigned int bb = (cb >> 16) & 0xff;
    const unsigned int ab = (cb >> 24) & 0xff;
    
    unsigned int r = (ra*(255-u) + rb*u)/255;
    unsigned int g = (ga*(255-u) + gb*u)/255;
    unsigned int b = (ba*(255-u) + bb*u)/255;
    unsigned int a = (aa*(255-u) + ab*u)/255;
    return duRGBA(r,g,b,a);
}

void genCheckerTexture(GLuint tex)
{
    // Create checker pattern.
    const unsigned int col0 = duRGBA(215,215,215,255);
    const unsigned int col1 = duRGBA(255,255,255,255);
    static const int TSIZE = 64;
    unsigned int data[TSIZE*TSIZE];

    int level = 0;
    int size = TSIZE;
    while (size > 0)
    {
        for (int y = 0; y < size; ++y)
            for (int x = 0; x < size; ++x)
                data[x+y*size] = (x==0 || y==0) ? col0 : col1;
        glTextureImage2DEXT(tex, GL_TEXTURE_2D, level, GL_RGBA, size,size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        size /= 2;
        level++;
    }

    glTextureParameteriEXT(tex, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTextureParameteriEXT(tex, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void duDebugDrawTriMeshSlope(const float* verts, int /*nverts*/,
    const int* tris, const float* normals, int ntris,
    const float walkableSlopeAngle, const float texScale)
{
    if (!verts) return;
    if (!tris) return;
    if (!normals) return;

    const float walkableThr = ml::cos(walkableSlopeAngle/180.0f*FLT_PI);

    float uva[2];
    float uvb[2];
    float uvc[2];

    const unsigned int unwalkable = duRGBA(192,128,0,255);

    gfx::setStdProgram(gfx::STD_FEATURE_COLOR|gfx::STD_FEATURE_TEXTURE);
    gfx::setMVP();

    glBindVertexArray(vf::p3uv2cu4_vertex_t::vao);
    glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p3uv2cu4_vertex_t));

    GLuint numVertices = ntris*3;
    GLuint baseVertex;
    vf::p3uv2cu4_vertex_t* v = gfx::frameAllocVertices<vf::p3uv2cu4_vertex_t>(numVertices, &baseVertex);

    for (GLuint i = 0; i < numVertices; i += 3)
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
        if (ml::abs(norm[1]) > ml::abs(norm[ax]))
            ax = 1;
        if (ml::abs(norm[2]) > ml::abs(norm[ax]))
            ax = 2;
        ax = (1<<ax)&3; // +1 mod 3
        ay = (1<<ax)&3; // +1 mod 3

        uva[0] = va[ax]*texScale;
        uva[1] = va[ay]*texScale;
        uvb[0] = vb[ax]*texScale;
        uvb[1] = vb[ay]*texScale;
        uvc[0] = vc[ax]*texScale;
        uvc[1] = vc[ay]*texScale;

        vf::set(v++, va[0], va[1], va[2], uva[0], uva[1], color);
        vf::set(v++, vb[0], vb[1], vb[2], uvb[0], uvb[1], color);
        vf::set(v++, vc[0], vc[1], vc[2], uvc[0], uvc[1], color);
    }
    glDrawArrays(GL_TRIANGLES, baseVertex, numVertices);
}

namespace app
{
    SpectatorCamera     camera;

    v128   proj[4];

    rcMeshLoaderObj* geom;

    GLuint debugTex;

    void init()
    {
        camera.acceleration.x = camera.acceleration.y = camera.acceleration.z = 150;
        camera.maxVelocity.x = camera.maxVelocity.y = camera.maxVelocity.z = 60;

        glGenTextures(1, &debugTex);
        genCheckerTexture(debugTex);

        geom = new rcMeshLoaderObj();
        geom->load("dungeon.obj");

    }

    void fini()
    {
        glDeleteTextures(1, &debugTex);
        delete geom;
    }

    void recompilePrograms()
    {
    }

    void resize(int width, int height)
    {
        ml::make_perspective_mat4(proj, 30.0f * FLT_DEG_TO_RAD_SCALE, (float)width/(float)height, 0.1f, 10000.0f);
    }

    void render()
    {
        v128 vm[4];
        camera.getViewMatrix(vm);

        gfx::set3DStates();

        gfx::setProjectionMatrix(proj);
        gfx::setModelViewMatrix(vm);

        glDisable(GL_CULL_FACE);

        gfx::setStdProgram(gfx::STD_FEATURE_COLOR);
        gfx::setMVP();

        glBindVertexArray(vf::p3cu4_vertex_t::vao);
        glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p3cu4_vertex_t));

        GLuint baseVertex;
        vf::p3cu4_vertex_t* v = gfx::frameAllocVertices<vf::p3cu4_vertex_t>(3, &baseVertex);

        vf::set(v++, -3.0f, -1.0f, -10.0f, 0xFF0000FF);
        vf::set(v++,  3.0f, -1.0f, -10.0f, 0xFF00FF00);
        vf::set(v++,  0.0f,  4.0f, -10.0f, 0xFFFF0000);
        glDrawArrays(GL_TRIANGLES, baseVertex, 3);

        glEnable(GL_CULL_FACE);

        glBindMultiTextureEXT(GL_TEXTURE0, GL_TEXTURE_2D, debugTex);
        duDebugDrawTriMeshSlope(geom->getVerts(), geom->getVertCount(),
            geom->getTris(), geom->getNormals(), geom->getTriCount(),
            45.0f, 0.33333f);

        gfx::set2DStates();
        gfx::setUIMatrices();

        ui::displayStats(10.0f, 10.0f, 300.0f, 70.0f, 0.0f, 0.0f);
    }

    void update(float dt)
    {
        ui::processCameraInput(&camera, dt);
    }
}
