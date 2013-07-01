#include <framework.h>
#include "FTGLTextureFont.h"
#include <assert.h>

class FontTest: public ui::Stage
{
public:
    FontTest()
    {
        font = createFont("C:\\Windows\\Fonts\\times.ttf", 20);

        prgGradient = resources::createProgramFromFiles(NULL, "Gradient.frag");

        GLint uStops, uScales, samColorRamp, uInvStopCount;
        uStops    = glGetUniformLocation(prgGradient, "uStops");
        uScales   = glGetUniformLocation(prgGradient, "uScales");
        samColorRamp = glGetUniformLocation(prgGradient, "samColorRamp");
        uInvStopCount = glGetUniformLocation(prgGradient, "uInvStopCount");

        glUseProgram(prgGradient);

        const int stopCount = 5;
        float stops[stopCount] = {0.0000f, 0.1250f, 0.3750f, 0.7500f, 1.000f};

        float stopsData[8*4], scalesData[8*4];

        for (int i=0; i<8*4; ++i)
        {
            stopsData[i]  = 1.0f;
            scalesData[i] = 0.0f;
        }

        memcpy(stopsData, stops, stopCount*4);

        for (int i=0; i<stopCount-1; ++i)
        {
            float delta = stops[i+1]-stops[i];
            assert(delta>=0);
            scalesData[i] = (delta>0)?(1.0f/delta):FLT_MAX;
        }

        glUniform1i(samColorRamp, 0);
        glUniform4fv(uStops,  1, stopsData);
        glUniform4fv(uScales, 1, scalesData);
        glUniform1f(uInvStopCount, 1.0f/stopCount);

        GLuint texColorRamp;
        glGenTextures(1, &texColorRamp);
        glBindTexture(GL_TEXTURE_1D, texColorRamp);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char c[] = {
            240, 240,  64, 255,
            32, 160,   0, 255,
            224, 224,   0, 255,
            128, 128, 128, 255,
            255, 255, 255, 255,
        };

        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, stopCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, c);

        const int stopCount1 = 4;
        float stops1[stopCount1] = {0.0000f, 0.5f, 0.5f, 1.000f};
        unsigned char c1[] = {
            0, 0,   255, 255,
            224, 0,   0, 255,
            0, 128, 0, 255,
            255, 255, 255, 255,
        };

        paint = vg::createLinearGradientPaint(-0.1f, -0.1f, 0.1f, 0.1f, stopCount1, stops1, (unsigned int*)c1);
        //paint = vg::createSolidPaint(0xFF00FF00);
        path = vg::createUnitQuad();

        CHECK_GL_ERROR();
    }

    ~FontTest()
    {
        vg::destroyPaint(paint);
        vg::destroyPath(path);
        glDeleteProgram(prgGradient);
        destroyFont(font);
    }

protected:
    virtual void onPaint()
    {
        glMatrixMode(GL_MODELVIEW);
        glTranslatef(100, 100, 0);
        glScalef(1, -1, 1);
        glColor3f(1, 1, 1);
        glUseProgram(0);
        drawString(font, 0, 0, "Hello World!!!");

        float xmin, ymin, xmax, ymax;
        getBounds(font, "Hello World!!!", xmin, ymin, xmax, ymax);
        glColor3f(1, 0, 0);
        glBegin(GL_LINE_STRIP);
        glVertex2f(xmin,  ymin);
        glVertex2f(xmin,  ymax);
        glVertex2f(xmax,  ymax);
        glVertex2f(xmax,  ymin);
        glVertex2f(xmin,  ymin);
        glEnd();


        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, -1);

        vg::drawPath(path, paint);

        //glUseProgram(prgGradient);
        //glColor3f(1, 1, 0);
        //glBegin(GL_QUADS);
        //glVertex2f(-10,  10);
        //glVertex2f( 10,  10);
        //glVertex2f( 10, -10);
        //glVertex2f(-10, -10);
        //glEnd();
        
        CHECK_GL_ERROR();
    }

private:
    vg::Paint		paint;
    vg::Path		path;
    Font			font;
    GLuint			prgGradient;
};

extern "C" int main(int argc, char** argv)
{
    fwk::init(argv[0]);

    {
        FontTest app;
        app.run();
    }

    fwk::cleanup();

    return 0;
}