#define GLEW_STATIC
#include <windows.h>
#include <cstdlib>
#include <opengl.h>
#include "UI.h"
#include "Framework.h"
#include <SOIL.h> // remove later

static const char pickFragmentSource[] = 
    "#version 330                                       \n"
    "uniform uint ID;                                  \n"
    "layout(location = 0) out uint rtVal;              \n"
    "                                                   \n"
    "void main()                                        \n"
    "{                                                  \n"
    "	rtVal = ID;                                     \n"
    "}                                                  \n";

static const GLuint emptyPickValue = 0xFF7FFFFF;

namespace ui
{
    Stage::Stage():	mDoAllocate(true),
        mLastVisited(0), mPhase(PHASE_DEFAULT)
#if defined(DEBUG) || defined(_DEBUG)
        ,dumpPickImage(0)
#endif
    {
        mFocused = this;
    }

    Stage& Stage::queueRelayout()
    {
        if (mPhase != PHASE_ALLOCATE) {mDoAllocate = true;}	return *this;
    }

    Stage& Stage::captureFocus(Actor* focused)
    {
        if (mFocused != focused)
        {
            if (mFocused) mFocused->onFocusOut();
            mFocused = focused;
            if (mFocused) mFocused->onFocusIn();
        }
        return *this;
    }

    Stage& Stage::releaseFocus()
    {
        if (mFocused) mFocused->onFocusOut();
        mFocused = 0; return *this;
    }


    Stage& Stage::setProjection(float* mat4x4)
    {
        mProjection = glm::mat4(
            mat4x4[0],  mat4x4[1],  mat4x4[2],  mat4x4[3], 
            mat4x4[4],  mat4x4[5],  mat4x4[6],  mat4x4[7], 
            mat4x4[8],  mat4x4[9],  mat4x4[10], mat4x4[11], 
            mat4x4[12], mat4x4[13], mat4x4[14], mat4x4[15]
        );
        return *this;
    }
    
    void Stage::createGLResources()
    {
        mPickIDRB = resources::createRenderbuffer(GL_R32UI, (GLsizei)mWidth, (GLsizei)mHeight);
        mPickZRB  = resources::createRenderbuffer(GL_DEPTH24_STENCIL8, (GLsizei)mWidth, (GLsizei)mHeight);

        glGenFramebuffers(1, &mPickFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mPickFBO);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mPickIDRB);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, mPickZRB);

        GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        assert(status == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        mPickProgram = resources::createProgram(GL_FRAGMENT_SHADER, pickFragmentSource);
        mColorLoc = glGetUniformLocation(mPickProgram, "ID");
    }

    void Stage::destroyGLResources()
    {
        glDeleteProgram(mPickProgram);
        glDeleteFramebuffers(1, &mPickFBO);
        glDeleteRenderbuffers(1, &mPickIDRB);
        glDeleteRenderbuffers(1, &mPickZRB);
    }

    void Stage::processKeyDown(const KeyEvent& event)
    {
        //Also here should be and hotkeys handling
#if defined(DEBUG) || defined(_DEBUG)
        if (event.keysym.sym == SDLK_p && event.type==SDL_KEYDOWN &&
            ((event.keysym.mod&KMOD_LALT) || (event.keysym.mod&KMOD_RALT)))
        {
            dumpPickImage = TRUE;
            return;
        }
#endif
        if (mFocused)
            mFocused->onKeyDown(event);

        //No do not sent any event to childs
    }

    void Stage::processKeyUp(const KeyEvent& event)
    {
        if (mFocused)
            mFocused->onKeyUp(event);

        //No do not sent any event to childs
    }

    void Stage::processTouch(const ButtonEvent& event)
    {
        Actor* actor = doPick((u32)event.x, (u32)event.y);

        if (actor)
            actor->onTouch(event);
    }

    void Stage::processUntouch(const ButtonEvent& event)
    {
        Actor* actor = doPick((u32)event.x, (u32)event.y);

        if (actor)
            actor->onUntouch(event);
    }

    void Stage::processMotion(const MotionEvent& event)
    {
        Actor* actor = doPick(event.x, event.y);

        if (mLastVisited != actor)
        {
            if (mLastVisited)
                mLastVisited->onLeave();

            if (actor)
            {
                actor->onEnter();
                //for (int button = 0; button < MouseButtons::Count; ++button)
                //{
                //	int	curState = mPrevButtonState[button];

                //	ButtonEvent evtTouch = {button, mModifiers, x, y};

                //	if (curState == InputState::Pressed)
                //		actor->onTouch(evtTouch);
                //}
            }
        }

        if (actor)
            actor->onMotion(event);

        mLastVisited = actor;
    }

    Actor* Stage::doPick(u32 x, u32 y)
    {
        GLuint id;
        GLenum err;
        GLint  viewport[4];

        glGetIntegerv(GL_VIEWPORT, viewport);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, mPickFBO);
        glReadPixels (x, viewport[3]-y-1, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &id);
        err = glGetError(); assert(err==GL_NO_ERROR);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        if (id == emptyPickValue)
            return this;

        assert(id < mRenderQueue.size());

        return mRenderQueue[id].actor;
    }

    void Stage::enterPhase(Phase nextPhase)
    {
        switch (nextPhase)
        {
        case PHASE_EVENT_HANDLING:
            {
                PROFILER_CPU_BLOCK("outlineActors");
                outlineActors();
            }
            break;

        case PHASE_ALLOCATE:
            if (mDoAllocate)
            {
                mDoAllocate = false;
                onAllocate();

                mRenderQueue.clear();
                updateRenderQueue(this, getTransformMatrix());
            }
            break;

        case PHASE_RENDERING:
            {
                PROFILER_CPU_BLOCK("renderActors");

                renderActors();
                glFlush();
            }
            break;

        case PHASE_DEFAULT:
            break;

        default:
            assert(0);
        }

        mPhase = nextPhase;
    }

    //!!!!!!!!!!!Refactor
    void setupUIViewMatrix(const glm::mat4& proj, float width, float height)
    {
        glm::mat4 inv = glm::inverseGTX(proj);
        glm::vec4 ur = inv*glm::vec4(1, 1, -1, 1);
        ur/=ur.w;

        glLoadIdentity();
        glScalef(ur.x, ur.y, 1);
        glTranslatef(-1, 1, -0.1f/*z_near*/);
        glScalef(2.0f/width, -2.0f/height, 1.0f/width);
    }

    void Stage::outlineActors()
    {
        GLenum err;

        GLuint  clearColor[4] = {emptyPickValue, 0, 0, 0};
        GLfloat depth1 = 1.0f;

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mPickFBO);
        glClearBufferfv(GL_COLOR, 0, (float*)clearColor);
        glClearBufferfv(GL_DEPTH, 0, &depth1);

        glPushAttrib(GL_ALL_ATTRIB_BITS|GL_MULTISAMPLE_BIT);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_MULTISAMPLE);
        glDepthMask(GL_TRUE);
        glStencilMask(0xFF);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        glUseProgram(mPickProgram);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(mProjection);
        glMatrixMode(GL_MODELVIEW);
        setupUIViewMatrix(mProjection, mWidth, mHeight);

        for(size_t i = 0; i < mRenderQueue.size(); ++i)
        {
            glPushMatrix();
            glMultMatrixf(mRenderQueue[i].transform);
            Actor* actor = mRenderQueue[i].actor;
            GLuint color = i;
            glUniform1ui(mColorLoc, color);
            glBegin(GL_QUADS);
            glVertex2f(0,0);
            glVertex2f(actor->mWidth,0);
            glVertex2f(actor->mWidth,actor->mHeight);
            glVertex2f(0,actor->mHeight);
            glEnd();
            glPopMatrix();
        }
        glPopAttrib();

        err = glGetError();
        assert(err==GL_NO_ERROR);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

#if defined(DEBUG) || defined(_DEBUG)
        if (dumpPickImage)
        {
            GLuint w = (GLuint)mWidth;
            GLuint h = (GLuint)mHeight;
            u8* p = new u8[w*h*4];
            glBindFramebuffer(GL_READ_FRAMEBUFFER, mPickFBO);
            glReadPixels (0, 0, w, h, GL_RED_INTEGER, GL_UNSIGNED_INT, p);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
            SOIL_save_image("pick.tga", SOIL_SAVE_TYPE_TGA, w, h, 4, p);		
            delete [] p;
        }
#endif
    }

    void Stage::renderActors()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0);
        glClearStencil(0);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        std::vector<RenderItem>::iterator	it  = mRenderQueue.begin(),
            end = mRenderQueue.end();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::perspectiveGTX(60.0f, mWidth/mHeight, 0.1f, 600.0f)/*mProjection*/);

        glMatrixMode(GL_MODELVIEW);
        setupUIViewMatrix(mProjection, mWidth, mHeight);

        glPushMatrix();
        onPaint();
        glPopMatrix();

        for (; it != end; ++it)
        {
            if ((*it).actor->isVisible())
            {
                glPushMatrix();
                glMultMatrixf((*it).transform);
                (*it).actor->onPaint();
                glPopMatrix();
            }
        }

        glFlush();
    }

    void Stage::updateRenderQueue(Container* container, const glm::mat4& parentTransform)
    {
        ChildsVector::iterator	it  = container->mChilds.begin(),
            end = container->mChilds.end();
        for (; it != end; ++it)
        {
            Actor* child = *it;

            //if (child->isVisible())
            {
                glm::mat4	actorTransform = parentTransform*child->getTransformMatrix();
                RenderItem	item = {actorTransform, child};

                mRenderQueue.push_back(item);

                Container* childAsContainer = dynamic_cast<Container*>(child);
                if (childAsContainer)
                    updateRenderQueue(childAsContainer, actorTransform);
            }
        }
    }

}