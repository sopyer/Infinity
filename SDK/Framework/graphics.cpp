#include <assert.h>
#include "graphics.h"

namespace graphics
{
    static GLsync frameSync[2] = {0, 0};
    static int    frameID = 0;

    void init()
    {
    }

    void fini()
    {
        glDeleteSync(frameSync[0]);
        glDeleteSync(frameSync[1]);
    }

    void beginFrame()
    {
        if (frameSync[frameID])
        {
            GLenum result = glClientWaitSync(frameSync[frameID], GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_INFINITE);
            assert(result==GL_ALREADY_SIGNALED || result==GL_CONDITION_SATISFIED);
            glDeleteSync(frameSync[frameID]);
            frameSync[frameID] = 0;
        }
    }

    void endFrame()
    {
        frameSync[frameID] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        frameID = (frameID + 1) & 0x01;
    }
}