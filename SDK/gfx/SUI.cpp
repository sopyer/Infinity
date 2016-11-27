#include <gfx/gfx.h>
#include "gfx_res.h"

namespace vg
{
    enum
    {
        UNI_SIMPLE_UI_FILL_COLOR   = 0,
        UNI_SIMPLE_UI_BORDER_COLOR = 1,
        UNI_SIMPLE_UI_ZONES        = 2,
    };

    void drawRect(float x0, float y0, float x1, float y1, VGuint fillColor, VGuint borderColor)
    {
        PROFILER_CPU_TIMESLICE("vg::drawRect");

        v128 fillColorF   = vi_cvt_u8x4_to_v128(fillColor),
             borderColorF = vi_cvt_u8x4_to_v128(borderColor);

        glUseProgram(gfx_res::prgUI);
        glProgramUniform4fv(gfx_res::prgUI, UNI_SIMPLE_UI_FILL_COLOR, 1, (float*)&fillColorF);
        glProgramUniform4fv(gfx_res::prgUI, UNI_SIMPLE_UI_BORDER_COLOR, 1, (float*)&borderColorF);
        glProgramUniform2f (gfx_res::prgUI, UNI_SIMPLE_UI_ZONES, 0, 0);
        gfx::setMVP();

        GLuint baseVertex;

        glBindVertexArray(vf::p2_vertex_t::vao);
        glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p2_vertex_t));
        vf::p2_vertex_t* v = gfx::frameAllocVertices<vf::p2_vertex_t>(4, &baseVertex);
        v[0].x = x0; v[0].y = y0;
        v[1].x = x1; v[1].y = y0;
        v[2].x = x0; v[2].y = y1;
        v[3].x = x1; v[3].y = y1;
        glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 4);
    }

    void drawRoundedRect(float x0, float y0, float x1, float y1, float cx, float cy, VGuint fillColor, VGuint borderColor)
    {
        PROFILER_CPU_TIMESLICE("vg::drawRoundedRect");

        v128 fillColorF   = vi_cvt_u8x4_to_v128(fillColor),
             borderColorF = vi_cvt_u8x4_to_v128(borderColor);

        glUseProgram(gfx_res::prgUI);
        glProgramUniform4fv(gfx_res::prgUI, UNI_SIMPLE_UI_FILL_COLOR, 1, (float*)&fillColorF);
        glProgramUniform4fv(gfx_res::prgUI, UNI_SIMPLE_UI_BORDER_COLOR, 1, (float*)&borderColorF);
        glProgramUniform2f (gfx_res::prgUI, UNI_SIMPLE_UI_ZONES, cx - 1, cx - 2);
        gfx::setMVP();

        float xb = cx;
        float yb = cy;

        float px0 = x0;
        float px1 = x0+cx;
        float px2 = x1-cx;
        float px3 = x1;

        float py0 = y0;
        float py1 = y0+cy;
        float py2 = y1-cy;
        float py3 = y1;

        GLuint baseVertex;

        glBindVertexArray(vf::p2uv3_vertex_t::vao);
        glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p2uv3_vertex_t));
        vf::p2uv3_vertex_t* v = gfx::frameAllocVertices<vf::p2uv3_vertex_t>(24, &baseVertex);

        vf::set(v++, px0, py0, xb, yb);
        vf::set(v++, px1, py0,  0, yb);

        vf::set(v++, px0, py1, xb,  0);
        vf::set(v++, px1, py1,  0,  0);

        vf::set(v++, px0, py2, xb, 0);
        vf::set(v++, px1, py2,  0, 0);

        vf::set(v++, px0, py3, xb, yb);
        vf::set(v++, px1, py3,  0, yb);
        glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 8);
        baseVertex+=8;

        vf::set(v++, px2, py0, 0, yb);
        vf::set(v++, px3, py0, xb, yb);

        vf::set(v++, px2, py1, 0, 0);
        vf::set(v++, px3, py1, xb, 0);

        vf::set(v++, px2, py2, 0, 0);
        vf::set(v++, px3, py2, xb, 0);

        vf::set(v++, px2, py3, 0, yb);
        vf::set(v++, px3, py3, xb, yb);
        glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 8);
        baseVertex+=8;

        vf::set(v++, px1, py0, 0, yb);
        vf::set(v++, px2, py0, 0, yb);

        vf::set(v++, px1, py1, 0, 0);
        vf::set(v++, px2, py1, 0, 0);

        vf::set(v++, px1, py2, 0, 0);
        vf::set(v++, px2, py2, 0, 0);

        vf::set(v++, px1, py3, 0, yb);
        vf::set(v++, px2, py3, 0, yb);
        glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 8);
    }

    void drawRoundedRectOutline(float x0, float y0, float x1, float y1, float cx, float cy, VGuint borderColor)
    {
        PROFILER_CPU_TIMESLICE("vg::drawRoundedRectOutline");

        v128 translucentF = vi_cvt_u8x4_to_v128(0x00000000),
             borderColorF = vi_cvt_u8x4_to_v128(borderColor);

        glUseProgram(gfx_res::prgUI);
        glProgramUniform4fv(gfx_res::prgUI, UNI_SIMPLE_UI_FILL_COLOR, 1, (float*)&translucentF);
        glProgramUniform4fv(gfx_res::prgUI, UNI_SIMPLE_UI_BORDER_COLOR, 1, (float*)&borderColorF);
        glProgramUniform2f (gfx_res::prgUI, UNI_SIMPLE_UI_ZONES, cx - 1, cx - 2);
        gfx::setMVP();

        float xb = cx;
        float yb = cy;

        float px0 = x0;
        float px1 = x0 + cx;
        float px2 = x1 - cx;
        float px3 = x1;

        float py0 = y0;
        float py1 = y0 + cy;
        float py2 = y1 - cy;
        float py3 = y1;

        GLuint baseVertex;

        glBindVertexArray(vf::p2uv3_vertex_t::vao);
        glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p2uv3_vertex_t));
        vf::p2uv3_vertex_t* v = gfx::frameAllocVertices<vf::p2uv3_vertex_t>(24, &baseVertex);

        vf::set(v++, px0, py0, xb, yb);
        vf::set(v++, px1, py0,  0, yb);

        vf::set(v++, px0, py1, xb,  0);
        vf::set(v++, px1, py1,  0,  0);

        vf::set(v++, px0, py2, xb, 0);
        vf::set(v++, px1, py2,  0, 0);

        vf::set(v++, px0, py3, xb, yb);
        vf::set(v++, px1, py3,  0, yb);
        glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 8);
        baseVertex+=8;

        vf::set(v++, px2, py0, 0, yb);
        vf::set(v++, px3, py0, xb, yb);

        vf::set(v++, px2, py1, 0, 0);
        vf::set(v++, px3, py1, xb, 0);

        vf::set(v++, px2, py2, 0, 0);
        vf::set(v++, px3, py2, xb, 0);

        vf::set(v++, px2, py3, 0, yb);
        vf::set(v++, px3, py3, xb, yb);
        glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 8);
        baseVertex+=8;

        vf::set(v++, px1, py0, 0, yb);
        vf::set(v++, px2, py0, 0, yb);

        vf::set(v++, px1, py1, 0, 0);
        vf::set(v++, px2, py1, 0, 0);
        glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 4);
        baseVertex+=4;

        vf::set(v++, px1, py2, 0, 0);
        vf::set(v++, px2, py2, 0, 0);

        vf::set(v++, px1, py3, 0, yb);
        vf::set(v++, px2, py3, 0, yb);
        glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 4);
    }

    //   void drawCircle( const Rect& rect, int fillColorId, int borderColorId );

    void drawImage(float x0, float y0, float x1, float y1, GLuint texture)
    {
        PROFILER_CPU_TIMESLICE("vg::drawImage");
        gfx::setStdProgram(gfx::STD_FEATURE_TEXTURE);
        gfx::setMVP();

        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTextureUnit(0, texture);

        GLuint baseVertex;

        glBindVertexArray(vf::p2uv2_vertex_t::vao);
        glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p2uv2_vertex_t));
        vf::p2uv2_vertex_t* v = gfx::frameAllocVertices<vf::p2uv2_vertex_t>(4, &baseVertex);

        vf::set(v++, x0, y0, 0.0f, 0.0f);
        vf::set(v++, x1, y0, 1.0f, 0.0f);
        vf::set(v++, x0, y1, 0.0f, 1.0f);
        vf::set(v++, x1, y1, 1.0f, 1.0f);

        glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 4);
    }
}
