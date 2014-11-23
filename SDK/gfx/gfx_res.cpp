#include <gfx/gfx.h>
#include "gfx_res.h"

namespace vf
{
    GLuint skinned_geom_t::vao;
    GLuint static_geom_t::vao;
    GLuint p2_vertex_t::vao;
    GLuint p2uv2_vertex_t::vao;
    GLuint p2uv3_vertex_t::vao;
    GLuint p2cu4_vertex_t::vao;
    GLuint p2uv2cu4_vertex_t::vao;
    GLuint p3_vertex_t::vao;
    GLuint p3cu4_vertex_t::vao;
    GLuint p3uv2cu4_vertex_t::vao;
    GLuint empty_geom_t::vao;

    static const gfx::vertex_element_t descSkinnedGeom[5] = 
    {
        { 0, offsetof(vf::skinned_geom_t, px), gfx::ATTR_POSITION,      GL_FLOAT,         3, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::skinned_geom_t, nx), gfx::ATTR_NORMAL,        GL_FLOAT,         3, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::skinned_geom_t, u),  gfx::ATTR_UV0,           GL_FLOAT,         2, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::skinned_geom_t, b),  gfx::ATTR_BLEND_INDICES, GL_UNSIGNED_BYTE, 4, GL_TRUE,  GL_FALSE },
        { 0, offsetof(vf::skinned_geom_t, w),  gfx::ATTR_BLEND_WEIGHTS, GL_FLOAT,         4, GL_FALSE, GL_FALSE },
    };

    static const gfx::vertex_element_t descStaticGeom[3] = 
    {
        { 0, offsetof(vf::skinned_geom_t, px), gfx::ATTR_POSITION, GL_FLOAT, 3, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::skinned_geom_t, nx), gfx::ATTR_NORMAL,   GL_FLOAT, 3, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::skinned_geom_t, u),  gfx::ATTR_UV0,      GL_FLOAT, 2, GL_FALSE, GL_FALSE },
    };

    static const gfx::vertex_element_t fmtdesc_p2[1] = 
    {
        {0, 0, 0, GL_FLOAT, 2, GL_FALSE, GL_FALSE},
    };

    static const gfx::vertex_element_t fmtdesc_p2uv2[2] = 
    {
        { 0, offsetof(vf::p2uv2_vertex_t, x), gfx::ATTR_POSITION, GL_FLOAT, 2, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::p2uv2_vertex_t, u), gfx::ATTR_UV0,      GL_FLOAT, 2, GL_FALSE, GL_FALSE },
    };

    static const gfx::vertex_element_t fmtdesc_p2uv3[2] = 
    {
        { 0, offsetof(vf::p2uv3_vertex_t, x), gfx::ATTR_POSITION, GL_FLOAT, 2, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::p2uv3_vertex_t, u), gfx::ATTR_UV0,      GL_FLOAT, 3, GL_FALSE, GL_FALSE },
    };

    static const gfx::vertex_element_t fmtdesc_p2cu4[2] = 
    {
        { 0, offsetof(vf::p2cu4_vertex_t, x), gfx::ATTR_POSITION, GL_FLOAT,         2, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::p2cu4_vertex_t, c), gfx::ATTR_COLOR,    GL_UNSIGNED_BYTE, 4, GL_FALSE, GL_TRUE },
    };

    static const gfx::vertex_element_t fmtdesc_p2uv2cu4[3] = 
    {
        { 0, offsetof(vf::p2uv2cu4_vertex_t, x), gfx::ATTR_POSITION, GL_FLOAT,         2, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::p2uv2cu4_vertex_t, u), gfx::ATTR_UV0,      GL_FLOAT,         2, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::p2uv2cu4_vertex_t, c), gfx::ATTR_COLOR,    GL_UNSIGNED_BYTE, 4, GL_FALSE, GL_TRUE },
    };

    static const gfx::vertex_element_t fmtdesc_p3[1] = 
    {
        { 0, 0, gfx::ATTR_POSITION, GL_FLOAT, 3, GL_FALSE, GL_FALSE },
    };

    static const gfx::vertex_element_t fmtdesc_p3cu4[2] = 
    {
        { 0, offsetof(vf::p3cu4_vertex_t, x), gfx::ATTR_POSITION, GL_FLOAT,         3, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::p3cu4_vertex_t, c), gfx::ATTR_COLOR,    GL_UNSIGNED_BYTE, 4, GL_FALSE, GL_TRUE },
    };

    static const gfx::vertex_element_t fmtdesc_p3uv2cu4[3] = 
    {
        { 0, offsetof(vf::p3uv2cu4_vertex_t, x), gfx::ATTR_POSITION, GL_FLOAT,         3, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::p3uv2cu4_vertex_t, u), gfx::ATTR_UV0,      GL_FLOAT,         2, GL_FALSE, GL_FALSE },
        { 0, offsetof(vf::p3uv2cu4_vertex_t, c), gfx::ATTR_COLOR,    GL_UNSIGNED_BYTE, 4, GL_FALSE, GL_TRUE },
    };

    void set(p2uv2_vertex_t* vert, float x, float y, float u, float v)
    {
        vert->x = x;
        vert->y = y;
        vert->u = u;
        vert->v = v;
    }

    void set(p2uv3_vertex_t* vert, float x, float y, float u, float v, float w)
    {
        vert->x = x;
        vert->y = y;
        vert->u = u;
        vert->v = v;
        vert->w = w;
    }

    void set(p2cu4_vertex_t* vert, float x, float y, uint32_t c)
    {
        vert->x = x;
        vert->y = y;
        vert->c = c;
    }

    void set(p2uv2cu4_vertex_t* vert, float x, float y, float u, float v, uint32_t c)
    {
        vert->x = x;
        vert->y = y;
        vert->u = u;
        vert->v = v;
        vert->c = c;
    }

    void set(p3cu4_vertex_t* vert, float x, float y, float z, uint32_t c)
    {
        vert->x = x;
        vert->y = y;
        vert->z = z;
        vert->c = c;
    }

    void set(p3uv2cu4_vertex_t* vert, float x, float y, float z, float u, float v, uint32_t c)
    {
        vert->x = x;
        vert->y = y;
        vert->z = z;
        vert->u = u;
        vert->v = v;
        vert->c = c;
    }
}

namespace gfx_res
{
    GLuint stdPrograms[STD_PROGRAM_COUNT];

    GLuint prgUI;
    GLuint prgRasterCubic;
    GLuint prgRasterCubicAA;
    GLuint prgPaintSolid;
    GLuint prgPaintLinGradient;
    GLuint prgLine;
    GLuint prgPoint;

    void init()
    {
        vf::skinned_geom_t::vao    = createVAO(ARRAY_SIZE(vf::descSkinnedGeom),  vf::descSkinnedGeom,  0, NULL);
        vf::static_geom_t::vao     = createVAO(ARRAY_SIZE(vf::descStaticGeom),   vf::descStaticGeom,   0, NULL);
        vf::p2_vertex_t::vao       = createVAO(ARRAY_SIZE(vf::fmtdesc_p2),       vf::fmtdesc_p2,       0, NULL);
        vf::p2uv2_vertex_t::vao    = createVAO(ARRAY_SIZE(vf::fmtdesc_p2uv2),    vf::fmtdesc_p2uv2,    0, NULL);
        vf::p2uv3_vertex_t::vao    = createVAO(ARRAY_SIZE(vf::fmtdesc_p2uv3),    vf::fmtdesc_p2uv3,    0, NULL);
        vf::p2cu4_vertex_t::vao    = createVAO(ARRAY_SIZE(vf::fmtdesc_p2cu4),    vf::fmtdesc_p2cu4,    0, NULL);
        vf::p2uv2cu4_vertex_t::vao = createVAO(ARRAY_SIZE(vf::fmtdesc_p2uv2cu4), vf::fmtdesc_p2uv2cu4, 0, NULL);
        vf::p3_vertex_t::vao       = createVAO(ARRAY_SIZE(vf::fmtdesc_p3),       vf::fmtdesc_p3,       0, NULL);
        vf::p3cu4_vertex_t::vao    = createVAO(ARRAY_SIZE(vf::fmtdesc_p3cu4),    vf::fmtdesc_p3cu4,    0, NULL);
        vf::p3uv2cu4_vertex_t::vao = createVAO(ARRAY_SIZE(vf::fmtdesc_p3uv2cu4), vf::fmtdesc_p3uv2cu4, 0, NULL);
        glGenVertexArrays(1, &vf::empty_geom_t::vao);

        prgUI               = res::createProgramFromFiles("UI.vert",         "UI.frag");
        prgRasterCubic      = res::createProgramFromFiles("VG.Cubic.vert",   "VG.Mask.Cubic.frag");
        prgRasterCubicAA    = res::createProgramFromFiles("VG.Cubic.vert",   "VG.Mask.Cubic.AA.frag");
        prgPaintSolid       = res::createProgramFromFiles("VG.Paint.vert",   "VG.Paint.Solid.frag");
        prgPaintLinGradient = res::createProgramFromFiles("VG.Paint.vert",   "VG.Paint.LinearGradient.frag");

        const char* version       = "#version 430\n\n";
        const char* enableColor   = "#define ENABLE_COLOR\n";
        const char* enableTexture = "#define ENABLE_TEXTURE\n";

        const char* headers[3] = {version};
        size_t numHeaders;

        for (size_t i=0; i<STD_PROGRAM_COUNT; ++i)
        {
            numHeaders = 1;
            if (i&gfx::STD_FEATURE_COLOR)
            {
                headers[numHeaders++] = enableColor;
            }
            if (i&gfx::STD_FEATURE_TEXTURE)
            {
                headers[numHeaders++] = enableTexture;
            }
            stdPrograms[i] = res::createProgramFromFiles("MESH.std.vert", "MESH.std.frag", numHeaders, headers);
        }
        
        headers[1] = enableColor;
        numHeaders = 2;
        prgLine = res::createProgramFromFiles("MESH.Line.vert", "MESH.std.frag", numHeaders, headers);
        prgPoint = res::createProgramFromFiles("MESH.Point.vert", "MESH.std.frag", numHeaders, headers);
    }

    void fini()
    {
        glDeleteProgram(prgUI);
        glDeleteProgram(prgRasterCubic);
        glDeleteProgram(prgRasterCubicAA);
        glDeleteProgram(prgPaintSolid);
        glDeleteProgram(prgPaintLinGradient);
        glDeleteProgram(prgPoint);
        glDeleteProgram(prgLine);

        for (size_t i=0; i<STD_PROGRAM_COUNT; ++i)
        {
            glDeleteProgram(stdPrograms[i]);
        }

        glDeleteVertexArrays(1, &vf::skinned_geom_t::vao);
        glDeleteVertexArrays(1, &vf::static_geom_t::vao);
        glDeleteVertexArrays(1, &vf::p2_vertex_t::vao);
        glDeleteVertexArrays(1, &vf::p2uv2_vertex_t::vao);
        glDeleteVertexArrays(1, &vf::p2uv3_vertex_t::vao);
        glDeleteVertexArrays(1, &vf::p2cu4_vertex_t::vao);
        glDeleteVertexArrays(1, &vf::p2uv2cu4_vertex_t::vao);
        glDeleteVertexArrays(1, &vf::p3_vertex_t::vao);
        glDeleteVertexArrays(1, &vf::p3cu4_vertex_t::vao);
        glDeleteVertexArrays(1, &vf::p3uv2cu4_vertex_t::vao);
        glDeleteVertexArrays(1, &vf::empty_geom_t::vao);
    }
}
