#include "UI.h"

#include "DefaultFontData.h"
#include "SpectatorCamera.h"
#include "CameraDirector.h"
#include "ResourceHelpers.h"
#include "Framework.h"

namespace ui
{
    vg::Font defaultFont;

    int width;
    int height;

    int   mouseX;
    int   mouseY;
    int   deltaX;
    int   deltaY;
    uint8_t keyStatePrev[SDLK_LAST];
    uint8_t keyStateCur[SDLK_LAST];
    uint8_t mouseStatePrev;
    uint8_t mouseStateCur;

    GLuint  fboPick;
    GLuint  rbPickID;
    GLuint  rbPickZ;
    GLuint  prgWriteID;
    GLint   locID;

    static const size_t MAX_SELECTORS = 16;
    static const size_t MAX_AREAS     = 256;
    static const size_t SELECTOR_ID       = 0xAA000000;
    static const size_t TYPE_ID_MASK      = 0xFF000000;
    static const size_t CONTROL_ID_MASK   = 0x007FF000;
    static const size_t CONTROL_ID_OFFSET = 12;
    static const size_t OPTION_ID_MASK    = 0x00000FFF;

    size_t selectorCount;
    int*   selectorValues[MAX_SELECTORS];
    size_t areaCount;
    Area   areas[MAX_AREAS];
    GLuint ids[MAX_AREAS];

    static const char pickFragmentSource[] = 
        "#version 330                                       \n"
        "uniform uint ID;                                   \n"
        "layout(location = 0) out uint rtVal;               \n"
        "                                                   \n"
        "void main()                                        \n"
        "{                                                  \n"
        "	rtVal = ID;                                     \n"
        "}                                                  \n";

    static const GLuint emptyPickValue = 0xFF7FFFFF;

    void init(GLsizei w, GLsizei h)
    {
        selectorCount = 0;
        areaCount     = 0;

        width  = w;
        height = h;

        defaultFont = vg::createFont(anonymousProBTTF, sizeof(anonymousProBTTF), 16);

        memcpy(keyStateCur,  SDL_GetKeyState(NULL), SDLK_LAST);
        memcpy(keyStatePrev, keyStateCur,           SDLK_LAST);

        mouseStatePrev = SDL_GetMouseState(&mouseX, &mouseY);
        mouseStateCur  = SDL_GetRelativeMouseState(&deltaX, &deltaY);
        deltaX = 0;
        deltaY = 0;

        //init pick resources
        rbPickID = resources::createRenderbuffer(GL_R32UI, width, height);
        rbPickZ = resources::createRenderbuffer(GL_DEPTH24_STENCIL8, width, height);

        glGenFramebuffers(1, &fboPick);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboPick);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbPickID);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, rbPickZ );

        GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        assert(status == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        prgWriteID = resources::createProgram(GL_FRAGMENT_SHADER, pickFragmentSource);
        locID      = glGetUniformLocation(prgWriteID, "ID");
    }

    void update()
    {
        memcpy(keyStatePrev, keyStateCur,           SDLK_LAST);
        memcpy(keyStateCur,  SDL_GetKeyState(NULL), SDLK_LAST);

        mouseStatePrev = mouseStateCur;
        mouseStateCur  = SDL_GetMouseState(&mouseX, &mouseY);
        SDL_GetRelativeMouseState(&deltaX, &deltaY);

        GLuint id = pickID(mouseX, mouseY);
        if ((id&TYPE_ID_MASK)==SELECTOR_ID && mouseWasReleased(SDL_BUTTON(SDL_BUTTON_LEFT)))
        {
            size_t selector = (id&CONTROL_ID_MASK)>>CONTROL_ID_OFFSET;
            size_t option   = id&OPTION_ID_MASK;
            assert(selector<selectorCount);
            int* value = selectorValues[selector];
            if (value) *value = option;
        }
    }

    void cleanup()
    {
        vg::destroyFont(defaultFont);

        memset(keyStateCur,  0, SDLK_LAST);
        memset(keyStatePrev, 0, SDLK_LAST);

        mouseStatePrev = 0;
        mouseStateCur  = 0;

        deltaX = 0;
        deltaY = 0;
        
        //fini pick resources
        glDeleteProgram(prgWriteID);
        glDeleteFramebuffers(1, &fboPick);
        glDeleteRenderbuffers(1, &rbPickID);
        glDeleteRenderbuffers(1, &rbPickZ);
    }

    void beginPickOutline()
    {
        CHECK_GL_ERROR();

        GLuint  clearColor[4] = {0, 0, 0, 0};
        GLfloat depth1 = 1.0f;

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboPick);
        glClearBufferuiv(GL_COLOR, 0, clearColor);
        glClearBufferfv (GL_DEPTH, 0, &depth1);

        glPushAttrib(GL_ALL_ATTRIB_BITS|GL_MULTISAMPLE_BIT);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_MULTISAMPLE);
        glDepthMask(GL_TRUE);
        glStencilMask(0xFF);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        glUseProgram(prgWriteID);

        CHECK_GL_ERROR();
    }

    void addPickOutlineRect(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLuint id)
    {
        glUniform1ui(locID, id);
        glBegin(GL_QUADS);
        glVertex2f(x,     y    );
        glVertex2f(x + w, y    );
        glVertex2f(x,     y + h);
        glVertex2f(x + w, y + h);
        glEnd();
    }

    void endPickOutline()
    {
        CHECK_GL_ERROR();

        //TODO: probably should be moved to another location
        for (size_t i = 0; i < areaCount; ++i)
        {
            glUniform1ui(locID, ids[i]);
            glBegin(GL_QUADS);
            glVertex2f(areas[i].x0, areas[i].y0);
            glVertex2f(areas[i].x1, areas[i].y0);
            glVertex2f(areas[i].x1, areas[i].y1);
            glVertex2f(areas[i].x0, areas[i].y1);
            glEnd();
        }

        glPopAttrib();
        
        CHECK_GL_ERROR();

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    GLuint pickID(GLuint x, GLuint y)
    {
        GLuint id;

        glBindFramebuffer(GL_READ_FRAMEBUFFER, fboPick);
        glReadPixels (x, height-y-1, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &id);

        CHECK_GL_ERROR();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        return id;
    }

    void readPickBuffer(GLsizei sz, void* data)
    {
        assert(sz>=width*height*4);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fboPick);
        glReadPixels (0, 0, width, height, GL_RED_INTEGER, GL_UNSIGNED_INT, data);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }

    void addSelector(int* value, size_t count, Area* regions)
    {
        assert(selectorCount<MAX_SELECTORS);
        assert(areaCount+count<=MAX_AREAS);
        assert(value);

        memcpy(areas+areaCount, regions, sizeof(Area)*count);

        for (size_t i = 0; i < count; ++i, ++areaCount)
        {
            ids[areaCount] = SELECTOR_ID |
                             ((selectorCount&CONTROL_ID_MASK)<<CONTROL_ID_OFFSET) |
                             (i&OPTION_ID_MASK);
        }

        selectorValues[selectorCount++] = value;

    }

    bool keyIsPressed  (int key)
    {
        assert(key<SDLK_LAST);
        return keyStateCur[key] != 0;
    }

    bool keyWasPressed (int key)
    {
        assert(key<SDLK_LAST);
        return !keyStatePrev[key] && keyStateCur[key];
    }

    bool keyIsReleased (int key)
    {
        assert(key<SDLK_LAST);
        return !keyStateCur[key];
    }

    bool keyWasReleased(int key)
    {
        assert(key<SDLK_LAST);
        return keyStatePrev[key] && !keyStateCur[key];
    }

    bool mouseIsPressed  (int button)
    {
        return (mouseStateCur&SDL_BUTTON(button)) != 0;
    }

    bool mouseWasPressed (int button)
    {
        return !(mouseStatePrev&SDL_BUTTON(button)) && (mouseStateCur&SDL_BUTTON(button));
    }

    bool mouseIsReleased (int button)
    {
        return !(mouseStateCur&SDL_BUTTON(button));
    }

    bool mouseWasReleased(int button)
    {
        return (mouseStatePrev&SDL_BUTTON(button)) && !(mouseStateCur&SDL_BUTTON(button));
    }

    void mouseRelOffset(int* dx, int* dy)
    {
        if (dx) *dx = deltaX;
        if (dy) *dy = deltaY;
    }

    void mouseAbsOffset(int* x, int* y)
    {
        if (x) *x = mouseX;
        if (y) *y = mouseY;
    }

    void processCameraInput(SpectatorCamera* camera, float dt)
    {
        glm::vec3   direction(0.0f, 0.0f, 0.0f);
        float       heading = 0.0f;
        float       pitch   = 0.0f;

        uint8_t *keystate = SDL_GetKeyState(NULL);

        direction.z += ui::keyStateCur[SDLK_w]?1.0f:0.0f;
        direction.z -= ui::keyStateCur[SDLK_s]?1.0f:0.0f;
        direction.x -= ui::keyStateCur[SDLK_a]?1.0f:0.0f;
        direction.x += ui::keyStateCur[SDLK_d]?1.0f:0.0f;

        pitch   += ui::keyStateCur[SDLK_KP8]?1.50f:0.0f;
        pitch   -= ui::keyStateCur[SDLK_KP5]?1.50f:0.0f;
        heading += ui::keyStateCur[SDLK_KP4]?1.50f:0.0f;
        heading -= ui::keyStateCur[SDLK_KP6]?1.50f:0.0f;

        camera->rotateSmoothly(heading, pitch);
        camera->rotateSmoothly((float)-ui::deltaX, (float)-ui::deltaY);
        camera->updatePosition(direction, dt);
    }

    void displayStats(float x, float y, float w, float h, float cpuTime, float gpuTime)
    {
        char* gpuName;
        char  str[256];

        vg::drawRoundedRect(x, y, x+w, y+h, 5, 5, 0xC0000000, 0xFF00FF32);

        glColor3f(1.0f, 1.0f, 1.0f);

        gpuName = (char*)glGetString(GL_RENDERER);
        vg::drawString(ui::defaultFont, x+15.0f, y+18.0f, gpuName, strlen(gpuName));

        _snprintf(str, 256, "CPU time - %f ms", cpuTime);
        vg::drawString(ui::defaultFont, x+15.0f, y+37.0f, str, strlen(str));

        _snprintf(str, 256, "GPU time - %f ms", gpuTime);
        vg::drawString(ui::defaultFont, x+15.0f, y+55.0f, str, strlen(str));
    }

    void processCameraDirectorInput(CameraDirector* camDirector, SpectatorCamera* camera)
    {
        assert(camDirector && camera);

        if (ui::keyWasReleased(SDLK_0))
        {
            camDirector->savedCamRotation.push_back(camera->getOrientation());
            camDirector->savedCamLocation.push_back(camera->getPosition());
        }

        if (ui::keyWasReleased(SDLK_EQUALS) &&
            ui::keyIsPressed(SDLK_LALT)     &&
            camDirector->savedCamRotation.size())
        {
            int max = camDirector->savedCamRotation.size() - 1;
            int idx = camDirector->index;

            idx = idx + 1;
            idx = idx > max ? 0 : idx;

            camDirector->index = idx;

            camera->setOrientation(camDirector->savedCamRotation[camDirector->index]);
            camera->setPosition   (camDirector->savedCamLocation[camDirector->index]);
        }

        if (ui::keyWasReleased(SDLK_MINUS) &&
            ui::keyIsPressed(SDLK_LALT)    &&
            camDirector->savedCamRotation.size())
        {
            int max = camDirector->savedCamRotation.size() - 1;
            int idx = camDirector->index;

            idx = idx - 1;
            idx = idx < 0 ? max : idx;

            camDirector->index = idx;

            camera->setOrientation(camDirector->savedCamRotation[camDirector->index]);
            camera->setPosition   (camDirector->savedCamLocation[camDirector->index]);
        }
    }
}
