#include "UI.h"

#include "DefaultFontData.h"
#include "SpectatorCamera.h"
#include "CameraDirector.h"
#include "ResourceHelpers.h"
#include "Framework.h"

namespace ui
{
    struct Area
    {
        GLfloat x0, y0, x1, y1;
    };

    enum ControlID
    {
        ID_CHECKBOX = 0x01000000,
        ID_MASK     = 0xFF000000
    };

    vg::Font defaultFont;

    int width;
    int height;
    int fullscreen;

    int   mouseX;
    int   mouseY;
    int   deltaX;
    int   deltaY;
    uint8_t keyStatePrev[SDLK_LAST];
    uint8_t keyStateCur[SDLK_LAST];
    uint8_t mouseStatePrev;
    uint8_t mouseStateCur;

    uint32_t mouseOverAreaID;

    GLuint  fboPick;
    GLuint  rbPickID;
    GLuint  rbPickZ;
    GLuint  prgWriteID;
    GLint   locID;

    static const size_t MAX_AREAS     = 256;

    size_t areaCount;
    Area   areas[MAX_AREAS];
    GLuint ids[MAX_AREAS];

    struct CheckBox
    {
        float x0, y0, x1, y1;
        uint32_t id:24;
        int      checked:1;
        int      hover:1;
    };

    static const size_t MAX_CHECKBOX_COUNT = 16;

    size_t   checkBoxCount;
    CheckBox checkBoxes[MAX_CHECKBOX_COUNT];

    static const char pickFragmentSource[] = 
        "#version 330                                       \n"
        "uniform uint ID;                                   \n"
        "layout(location = 0) out uint rtVal;               \n"
        "                                                   \n"
        "void main()                                        \n"
        "{                                                  \n"
        "	rtVal = ID;                                     \n"
        "}                                                  \n";

    void checkBoxUpdateAll();
    void checkBoxRenderAll();

    void createSurfaces()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboPick);
        rbPickID = resources::createRenderbuffer(GL_R32UI, width, height);
        rbPickZ = resources::createRenderbuffer(GL_DEPTH24_STENCIL8, width, height);

        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbPickID);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, rbPickZ );

        GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        assert(status == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    void destroySurfaces()
    {
        glDeleteRenderbuffers(1, &rbPickID);
        glDeleteRenderbuffers(1, &rbPickZ);
    }

    void init(GLsizei w, GLsizei h)
    {
        //!!!! Refactor
        fullscreen = false;
        width      = w;
        height     = h;

        SDL_EnableUNICODE(TRUE);
        SDL_EnableKeyRepeat(40, 40);

        areaCount     = 0;
        checkBoxCount = 0;

        defaultFont = vg::createFont(anonymousProBTTF, sizeof(anonymousProBTTF), 16);

        memcpy(keyStateCur,  SDL_GetKeyState(NULL), SDLK_LAST);
        memcpy(keyStatePrev, keyStateCur,           SDLK_LAST);

        mouseStatePrev = SDL_GetMouseState(&mouseX, &mouseY);
        mouseStateCur  = SDL_GetRelativeMouseState(&deltaX, &deltaY);
        deltaX = 0;
        deltaY = 0;

        mouseOverAreaID = 0;

        //init pick resources
        glGenFramebuffers(1, &fboPick);

        createSurfaces();

        prgWriteID = resources::createProgram(GL_FRAGMENT_SHADER, pickFragmentSource);
        locID      = glGetUniformLocation(prgWriteID, "ID");
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

        destroySurfaces();
    }

    void update()
    {
        memcpy(keyStatePrev, keyStateCur,           SDLK_LAST);
        memcpy(keyStateCur,  SDL_GetKeyState(NULL), SDLK_LAST);

        mouseStatePrev = mouseStateCur;
        mouseStateCur  = SDL_GetMouseState(&mouseX, &mouseY);
        SDL_GetRelativeMouseState(&deltaX, &deltaY);

        mouseOverAreaID = pickID(mouseX, mouseY);

        checkBoxUpdateAll();
    }

    void render()
    {
        checkBoxRenderAll();
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

    uint32_t mouseOverID()
    {
        return mouseOverAreaID;
    }

    void mouseAddEventArea(float x0, float y0, float x1, float y1, uint32_t id)
    {
        assert(areaCount<MAX_AREAS);

        areas[areaCount].x0 = x0;
        areas[areaCount].y0 = y0;
        areas[areaCount].x1 = x1;
        areas[areaCount].y1 = y1;

        ids[areaCount] = id;

        areaCount++;
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

    CheckBoxID checkBoxAdd(float x0, float y0, float x1, float y1, int checked)
    {
        assert(checkBoxCount<MAX_CHECKBOX_COUNT);

        CheckBoxID id = checkBoxCount;

        checkBoxes[id].x0 = x0;
        checkBoxes[id].y0 = y0;
        checkBoxes[id].x1 = x1;
        checkBoxes[id].y1 = y1;

        checkBoxes[id].id      = id;
        checkBoxes[id].checked = checked;

        checkBoxCount++;

        mouseAddEventArea(x0, y0, x1, y1, ID_CHECKBOX|id);

        return id;
    }

    int checkBoxIsChecked(CheckBoxID id)
    {
        return checkBoxes[id].checked;
    }

    void checkBoxUpdateAll()
    {
        for (size_t i=0; i<checkBoxCount; ++i) //avoid cycle implement pre/post update
        {
            checkBoxes[i].hover = false;
        }

        uint32_t id = mouseOverAreaID;
        if ((id&ID_MASK) == ID_CHECKBOX)
        {
            CheckBoxID cid = id&0x00FFFFFF;
            assert(cid<checkBoxCount);

            checkBoxes[cid].hover = true;
            if (mouseWasReleased(SDL_BUTTON_LEFT))
                checkBoxes[cid].checked = !checkBoxes[cid].checked;
        }
    }

    void checkBoxRenderAll()
    {
        for (size_t i=0; i<checkBoxCount; ++i)
        {
		    static const uint32_t colors[] = {
			    0xFF232563,
			    0xFF4547A5,
			    0xFF3CE484,
			    0xFF7FFDA3,
		    };

            bool isChecked = checkBoxes[i].checked != 0;
            bool isHover   = checkBoxes[i].hover   != 0;
            
            float x0 = checkBoxes[i].x0;
            float y0 = checkBoxes[i].y0;
            float x1 = checkBoxes[i].x1;
            float y1 = checkBoxes[i].y1;
            
            vg::drawRoundedRect(x0, y0, x0+16.0f, y0+16.0f, 2.5f, 2.5f, colors[(isChecked<<1)+isHover], 0xFF00FF32);
        }
    }
}
