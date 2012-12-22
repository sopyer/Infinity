#define GLEW_STATIC
#include <windows.h>
#include <cstdlib>
#include <opengl.h>
#include <SOIL.h> // remove later
#include <SDL.h>
#include <SDL_syswm.h>
#include <utils.h>
#include <vg/vg.h>
#include <vg/impl/SharedResources.h>
#include <ShaderEditOverlay.h>
#include <ProfilerOverlay.h>
#include <profiler.h>
#include "UI.h"
#include "Framework.h"
#include "Timer.h"

void Platform_Initialise(HWND hWnd);
void Platform_Finalise();

enum
{
    STATE_DEFAULT,
    STATE_SHADER_EDIT,
    STATE_PROFILER
};

namespace ui
{
    Stage::Stage():	mDoAllocate(true),
        mLastVisited(0),
        mPhase(PHASE_DEFAULT),
        mState(STATE_DEFAULT)
#if defined(DEBUG) || defined(_DEBUG)
        ,dumpPickImage(0)
#endif
    {
        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0)											// Init The SDL Library, The VIDEO Subsystem
        {
            logging::message("Unable to open SDL: %s\n", SDL_GetError() );					// If SDL Can't Be Initialized
            exit(1);															// Get Out Of Here. Sorry.
        }

        //!!!! Refactor
        mFullscreen = false;
        mWidth = 1280;
        mHeight = 720;

        uint32_t flags = SDL_HWSURFACE|SDL_OPENGLBLIT;									// We Want A Hardware Surface And Special OpenGLBlit Mode

        if (mFullscreen)		
        {
            flags |= SDL_FULLSCREEN;
        }

        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );								// In order to use SDL_OPENGLBLIT we have to
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );							// set GL attributes first
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
        SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, TRUE );							// colors and doublebuffering
        SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
        SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );
        SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );

        if(!(mScreen = SDL_SetVideoMode((int)mWidth, (int)mHeight, 32, flags)))
        {
            logging::message("Unable to open screen surface: %s\n", SDL_GetError() );		// If Something's Gone Wrong, Report
            exit(1);															// And Exit
        }

        SDL_EnableUNICODE(TRUE);
        SDL_EnableKeyRepeat(40, 40);

        setProjection(glm::perspectiveGTX(60.0f, mWidth/mHeight, 0.1f, 600.0f));

        importOpenGL();

        mt::init();
        vg::init();
        ui::init((GLsizei)mWidth, (GLsizei)mHeight);

        SDL_SysWMinfo info = {{0, 0}, 0, 0};
        SDL_GetWMInfo(&info);
        Platform_Initialise(info.window);

        mShaderEditOverlay = new ShaderEditOverlay;
        mShaderEditOverlay->initialise(mWidth, mHeight);

        mProfilerOverlay = new ProfilerOverlay;
        mProfilerOverlay->initialise(mWidth, mHeight);

        GLuint programs[] = {
            impl::simpleUIProgram, impl::stencilArcAreaProgram, impl::stencilQuadAreaProgram,
            impl::stencilCubicAreaAAProgram, impl::stencilCubicAreaProgram, impl::linGradProgram
        };

        mShaderEditOverlay->addPrograms(ARRAY_SIZE(programs), programs);

        mFocused = this;

        mPrevTime = timerAbsoluteTime();
    }

    Stage::~Stage()
    {
        delete mShaderEditOverlay;
        delete mProfilerOverlay;
        Platform_Finalise();
        ui::cleanup();
        vg::cleanup();
        mt::cleanup();
        SDL_Quit();
    }

    void Stage::addPrograms(size_t count, GLuint* programs)
    {
        mShaderEditOverlay->addPrograms(count, programs);
    }

    Stage& Stage::captureMouse()
    {
        SDL_ShowCursor(FALSE);
        SDL_WM_GrabInput(SDL_GRAB_ON);
        captureFocus(this);
        return *this;
    }

    Stage& Stage::releaseMouse()
    {
        SDL_ShowCursor(TRUE);
        SDL_WM_GrabInput(SDL_GRAB_OFF);
        return *this;
    }

    bool Stage::isMouseCaptured()
    {
        return SDL_WM_GrabInput(SDL_GRAB_QUERY)==SDL_GRAB_ON;
    }

    void Stage::handleInput()
    {
        PROFILER_CPU_BLOCK("handleInput");

        enterPhase(PHASE_EVENT_HANDLING);
        SDL_Event	E;
        uint32_t i = 0;
        while (SDL_PollEvent(&E) && i<10)
        {
            switch(E.type)
            {
            case SDL_QUIT:
                close();
                break;

            case SDL_KEYDOWN:
                switch (mState)
                {
                case STATE_DEFAULT:
                    processKeyDown(E.key);
                    break;
                case STATE_SHADER_EDIT:
                    mShaderEditOverlay->handleKeyDown(E.key);
                    break;
                case STATE_PROFILER:
                    mProfilerOverlay->handleKeyDown(E.key);
                    break;
                }
                break;
            case SDL_KEYUP:		
                if (E.key.keysym.sym==SDLK_ESCAPE)
                {
                    close();
                }
                else if (E.key.keysym.sym==SDLK_F5)
                {
                    mState = mState==STATE_SHADER_EDIT?STATE_DEFAULT:STATE_SHADER_EDIT;
                    if (mState==STATE_SHADER_EDIT) mShaderEditOverlay->reset();
                }
                else if (E.key.keysym.sym==SDLK_F4)
                {
                    mState = mState==STATE_PROFILER?STATE_DEFAULT:STATE_PROFILER;
                }
                else if (mState==STATE_DEFAULT)
                    processKeyUp(E.key);
                break;
            case SDL_MOUSEMOTION:
                if (mState==STATE_DEFAULT)
                {
                    Actor* actor = doPick((uint32_t)E.button.x, (uint32_t)E.button.y);

                    if (!(actor==(Actor*)this && SDL_WM_GrabInput(SDL_GRAB_QUERY)==SDL_GRAB_OFF))
                        processMotion(E.motion);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (mState==STATE_DEFAULT)
                {
                    processTouch(E.button);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (mState==STATE_DEFAULT)
                    processUntouch(E.button);
                break;
            }
            ++i;
        }
    }

    void Stage::frameStep()
    {
        profilerBeginFrame();

        glViewport(0, 0, (GLsizei)mWidth, (GLsizei)mHeight);

        handleInput();
        ui::update();
        
        uint64_t time;

        time = timerAbsoluteTime();
        onUpdate((time-mPrevTime)*0.000001f);
        mPrevTime = time;

        handleRender();

        profilerEndFrame();

        if (mState==STATE_PROFILER)
        {
            mProfilerOverlay->update();
            mProfilerOverlay->renderFullscreen();
        }
        if (mState==STATE_SHADER_EDIT)
            mShaderEditOverlay->renderFullscreen();
        SDL_GL_SwapBuffers();
    }

    void Stage::handleRender()
    {
        if (mShaderEditOverlay->requireReset())
        {
            impl::readyProgramsForUse();
            onShaderRecompile();
        }

        {
            PROFILER_CPU_BLOCK("STAGE PHASE ALLOCATE");
            enterPhase(PHASE_ALLOCATE);
        }
        {
            PROFILER_CPU_BLOCK("STAGE PHASE RENDERING");
            enterPhase(PHASE_RENDERING);
        }
        {
            PROFILER_CPU_BLOCK("STAGE PHASE DEFAULT");
            enterPhase(PHASE_DEFAULT);
        }
    }

    void Stage::close()
    {
        mt::terminateLoop();
    }

    void Stage::run()
    {
        //mt::addTimedTask<SDLStage, &SDLStage::handleInput>(this, 33);
        mt::addFrameTask<Stage, &Stage::frameStep>(this);

        mt::mainLoop();
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
        Actor* actor = doPick((uint32_t)event.x, (uint32_t)event.y);

        if (actor)
            actor->onTouch(event);
    }

    void Stage::processUntouch(const ButtonEvent& event)
    {
        Actor* actor = doPick((uint32_t)event.x, (uint32_t)event.y);

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

    Actor* Stage::doPick(uint32_t x, uint32_t y)
    {
        GLuint id = ui::pickID(x, y);

        if (id == 0)
            return this;

        if (id>0x007FFFFF)
            return 0;

        --id;

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
        beginPickOutline();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(mProjection);
        glMatrixMode(GL_MODELVIEW);
        setupUIViewMatrix(mProjection, mWidth, mHeight);

        for(size_t i = 0; i < mRenderQueue.size(); ++i)
        {
            glPushMatrix();
            glMultMatrixf(mRenderQueue[i].transform);
            Actor* actor = mRenderQueue[i].actor;
            addPickOutlineRect(0, 0, actor->mWidth, actor->mHeight, i+1);
            glPopMatrix();
        }
        
        endPickOutline();

#if defined(DEBUG) || defined(_DEBUG)
        if (dumpPickImage)
        {
            GLuint w = (GLuint)mWidth;
            GLuint h = (GLuint)mHeight;
            uint8_t* p = new uint8_t[w*h*4];
            ui::readPickBuffer(w*h*4, p);
            SOIL_save_image("pick.tga", SOIL_SAVE_TYPE_TGA, w, h, 4, p);		
            delete [] p;
            dumpPickImage = 0;
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