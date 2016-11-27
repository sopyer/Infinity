#include <gfx/gfx.h>
#include "gfx_res.h"
#include "Paint.h"

namespace vg
{
    struct uPaintLinGradient
    {
        ml::vec4   uStops [8];
        ml::vec4   uScales[8];
        ml::vec2   uStartPoint;
        ml::vec2   uDirection;
        float  uInvStopCount;
    };

    Paint createSolidPaint(float* color4f)
    {
        Paint  newPaint = mem::alloc<PaintOpaque>(gfx::memArena);

        newPaint->program       = gfx_res::prgPaintSolid;
        newPaint->allocUniforms = etlsf_alloc(gfx_res::vgGArena, sizeof(v128));
        newPaint->offset        = etlsf_block_offset(gfx_res::vgGArena, newPaint->allocUniforms);
        newPaint->size          = sizeof(v128);
        newPaint->texture       = 0;

        assert(newPaint->offset%gfx::caps.uboAlignment == 0);

        v128* udata = (v128*)glMapNamedBufferRange(gfx_res::buffer, newPaint->offset, newPaint->size, GL_MAP_WRITE_BIT);
        memcpy(udata, color4f, sizeof(v128));
        glUnmapNamedBuffer(gfx_res::buffer);

        return newPaint;
    }

    Paint createSolidPaint(unsigned int color)
    {
        v128 colorf = vi_cvt_u8x4_to_v128(color);

        return createSolidPaint(colorf.m128_f32);
    }

    Paint createLinearGradientPaint(float x0, float y0, float x1, float y1, size_t stopCount, float stops[], unsigned int colorRamp[])
    {
        Paint  newPaint = mem::alloc<PaintOpaque>(gfx::memArena);

        glCreateTextures(GL_TEXTURE_1D, 1, &newPaint->texture);
        glTextureStorage1D(newPaint->texture, 1, GL_RGBA8, stopCount);
        glTextureSubImage1D(newPaint->texture, 0, 0, stopCount, GL_RGBA, GL_UNSIGNED_BYTE, colorRamp);
        glTextureParameteri(newPaint->texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(newPaint->texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        newPaint->program = gfx_res::prgPaintLinGradient;

        newPaint->allocUniforms = etlsf_alloc(gfx_res::vgGArena, sizeof(uPaintLinGradient));
        newPaint->offset        = etlsf_block_offset(gfx_res::vgGArena, newPaint->allocUniforms);
        newPaint->size          = sizeof(uPaintLinGradient);

        assert(newPaint->offset%gfx::caps.uboAlignment == 0);
 
        uPaintLinGradient* udata = (uPaintLinGradient*)glMapNamedBufferRange(gfx_res::buffer, newPaint->offset, newPaint->size, GL_MAP_WRITE_BIT);
        
        float stopsData[8*4], scalesData[8*4];

        for (int i=0; i<8*4; ++i)
        {
            stopsData[i]  = 1.0f;
            scalesData[i] = 0.0f;
        }

        memcpy(stopsData, stops, stopCount*4);

        for (size_t i=0; i<stopCount-1; ++i)
        {
            float delta = stops[i+1]-stops[i];
            assert(delta>=0);
            scalesData[i] = (delta>0)?(1.0f/delta):FLT_MAX;
        }

        memcpy(udata->uStops,  stopsData,  sizeof(v128)*4);
        memcpy(udata->uScales, scalesData, sizeof(v128)*4);
        float dx=x1-x0, dy=y1-y0;
        float scale = dx*dx+dy*dy;

        udata->uStartPoint.x = x0;
        udata->uStartPoint.y = y0;
        udata->uDirection.x = scale?dx/scale:FLT_MAX;
        udata->uDirection.y = scale?dy/scale:FLT_MAX;
        udata->uInvStopCount = 1.0f/stopCount;

        glUnmapNamedBuffer(gfx_res::buffer);

        return newPaint;
    }

    void applyPaintAsGLProgram(Paint paint)
    {
        glUseProgram(paint->program);
        gfx::setMVP();
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, gfx_res::buffer, paint->offset, paint->size);
        if (paint->texture)
        {
            glBindTextures(0, 1, &paint->texture);
        }
    }

    void destroyPaint(Paint paint)
    {
        if (paint->texture) glDeleteTextures(1, &paint->texture);
        etlsf_free(gfx_res::vgGArena, paint->allocUniforms);

        mem::free(gfx::memArena, paint);
    }
}
