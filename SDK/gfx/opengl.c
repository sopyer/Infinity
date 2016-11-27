/* WARNING: This file was automatically generated */
/* Do not edit. */

#include "opengl.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

static void   glextLoadFunctions(void);
static void   glextLoadModule(void);
static void   glextUnloadModule(void);
static void*  glextGetProc(const char *proc);
static void   glextAddExtension(const char* extension);

int importOpenGL(void)
{
    GLint minor, major;
    GLint num_extensions;
    int i;

    glextLoadModule();
    glextLoadFunctions();
    glextUnloadModule();

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    /* --- Check for minimal version and profile --- */
    if (10 * major + minor < 45) {
        fprintf(stderr, "Error: OpenGL version 4.5 not supported.\n");
        fprintf(stderr, "       Your version is %d.%d.\n", major, minor);
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }

    
    /* --- Check for extensions --- */

    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
    
    for (i = 0; i < num_extensions; i++) {
        glextAddExtension((const char*)glGetStringi(GL_EXTENSIONS, i));
    }


    return GL_TRUE;
}

/* ----------------------- Extension flag definitions ---------------------- */

/* ---------------------- Function pointer definitions --------------------- */
GLFP glfp = {
/* GL_VERSION_1_2 */
/* 1 functions */
    "glDrawRangeElements",
/* GL_VERSION_1_3 */
/* 1 functions */
    "glSampleCoverage",
/* GL_VERSION_1_4 */
/* 9 functions */
    "glBlendColor",
    "glBlendEquation",
    "glBlendFuncSeparate",
    "glMultiDrawArrays",
    "glMultiDrawElements",
    "glPointParameterf",
    "glPointParameterfv",
    "glPointParameteri",
    "glPointParameteriv",
/* GL_VERSION_1_5 */
/* 12 functions */
    "glBeginQuery",
    "glBindBuffer",
    "glBufferData",
    "glDeleteBuffers",
    "glDeleteQueries",
    "glEndQuery",
    "glGenQueries",
    "glGetQueryObjectiv",
    "glGetQueryObjectuiv",
    "glGetQueryiv",
    "glIsBuffer",
    "glIsQuery",
/* GL_VERSION_2_0 */
/* 37 functions */
    "glAttachShader",
    "glBindAttribLocation",
    "glBlendEquationSeparate",
    "glCompileShader",
    "glCreateProgram",
    "glCreateShader",
    "glDeleteProgram",
    "glDeleteShader",
    "glDetachShader",
    "glDisableVertexAttribArray",
    "glDrawBuffers",
    "glEnableVertexAttribArray",
    "glGetActiveAttrib",
    "glGetActiveUniform",
    "glGetAttachedShaders",
    "glGetAttribLocation",
    "glGetProgramInfoLog",
    "glGetProgramiv",
    "glGetShaderInfoLog",
    "glGetShaderSource",
    "glGetShaderiv",
    "glGetUniformLocation",
    "glGetUniformfv",
    "glGetUniformiv",
    "glGetVertexAttribPointerv",
    "glGetVertexAttribdv",
    "glGetVertexAttribfv",
    "glGetVertexAttribiv",
    "glIsProgram",
    "glIsShader",
    "glLinkProgram",
    "glShaderSource",
    "glStencilFuncSeparate",
    "glStencilMaskSeparate",
    "glStencilOpSeparate",
    "glUseProgram",
    "glValidateProgram",
/* GL_VERSION_3_0 */
/* 55 functions */
    "glBeginConditionalRender",
    "glBeginTransformFeedback",
    "glBindBufferBase",
    "glBindBufferRange",
    "glBindFragDataLocation",
    "glBindFramebuffer",
    "glBindRenderbuffer",
    "glBindVertexArray",
    "glBlitFramebuffer",
    "glCheckFramebufferStatus",
    "glClampColor",
    "glClearBufferfi",
    "glClearBufferfv",
    "glClearBufferiv",
    "glClearBufferuiv",
    "glColorMaski",
    "glDeleteFramebuffers",
    "glDeleteRenderbuffers",
    "glDeleteVertexArrays",
    "glDisablei",
    "glEnablei",
    "glEndConditionalRender",
    "glEndTransformFeedback",
    "glFlushMappedBufferRange",
    "glFramebufferRenderbuffer",
    "glFramebufferTexture1D",
    "glFramebufferTexture2D",
    "glFramebufferTexture3D",
    "glFramebufferTextureLayer",
    "glGenFramebuffers",
    "glGenRenderbuffers",
    "glGenVertexArrays",
    "glGenerateMipmap",
    "glGetBooleani_v",
    "glGetFragDataLocation",
    "glGetFramebufferAttachmentParameteriv",
    "glGetIntegeri_v",
    "glGetRenderbufferParameteriv",
    "glGetStringi",
    "glGetTexParameterIiv",
    "glGetTexParameterIuiv",
    "glGetTransformFeedbackVarying",
    "glGetUniformuiv",
    "glGetVertexAttribIiv",
    "glGetVertexAttribIuiv",
    "glIsEnabledi",
    "glIsFramebuffer",
    "glIsRenderbuffer",
    "glIsVertexArray",
    "glMapBufferRange",
    "glRenderbufferStorage",
    "glRenderbufferStorageMultisample",
    "glTexParameterIiv",
    "glTexParameterIuiv",
    "glTransformFeedbackVaryings",
/* GL_VERSION_3_1 */
/* 11 functions */
    "glCopyBufferSubData",
    "glDrawArraysInstanced",
    "glDrawElementsInstanced",
    "glGetActiveUniformBlockName",
    "glGetActiveUniformBlockiv",
    "glGetActiveUniformName",
    "glGetActiveUniformsiv",
    "glGetUniformBlockIndex",
    "glGetUniformIndices",
    "glPrimitiveRestartIndex",
    "glUniformBlockBinding",
/* GL_VERSION_3_2 */
/* 17 functions */
    "glClientWaitSync",
    "glDeleteSync",
    "glDrawElementsBaseVertex",
    "glDrawElementsInstancedBaseVertex",
    "glDrawRangeElementsBaseVertex",
    "glFenceSync",
    "glFramebufferTexture",
    "glGetBufferParameteri64v",
    "glGetInteger64i_v",
    "glGetInteger64v",
    "glGetMultisamplefv",
    "glGetSynciv",
    "glIsSync",
    "glMultiDrawElementsBaseVertex",
    "glProvokingVertex",
    "glSampleMaski",
    "glWaitSync",
/* GL_VERSION_3_3 */
/* 18 functions */
    "glBindFragDataLocationIndexed",
    "glBindSampler",
    "glDeleteSamplers",
    "glGetFragDataIndex",
    "glGetQueryObjecti64v",
    "glGetQueryObjectui64v",
    "glGetSamplerParameterIiv",
    "glGetSamplerParameterIuiv",
    "glGetSamplerParameterfv",
    "glGetSamplerParameteriv",
    "glIsSampler",
    "glQueryCounter",
    "glSamplerParameterIiv",
    "glSamplerParameterIuiv",
    "glSamplerParameterf",
    "glSamplerParameterfv",
    "glSamplerParameteri",
    "glSamplerParameteriv",
/* GL_VERSION_4_0 */
/* 29 functions */
    "glBeginQueryIndexed",
    "glBindTransformFeedback",
    "glBlendEquationSeparatei",
    "glBlendEquationi",
    "glBlendFuncSeparatei",
    "glBlendFunci",
    "glDeleteTransformFeedbacks",
    "glDrawArraysIndirect",
    "glDrawElementsIndirect",
    "glDrawTransformFeedback",
    "glDrawTransformFeedbackStream",
    "glEndQueryIndexed",
    "glGenTransformFeedbacks",
    "glGetActiveSubroutineName",
    "glGetActiveSubroutineUniformName",
    "glGetActiveSubroutineUniformiv",
    "glGetProgramStageiv",
    "glGetQueryIndexediv",
    "glGetSubroutineIndex",
    "glGetSubroutineUniformLocation",
    "glGetUniformSubroutineuiv",
    "glGetUniformdv",
    "glIsTransformFeedback",
    "glMinSampleShading",
    "glPatchParameterfv",
    "glPatchParameteri",
    "glPauseTransformFeedback",
    "glResumeTransformFeedback",
    "glUniformSubroutinesuiv",
/* GL_VERSION_4_1 */
/* 79 functions */
    "glActiveShaderProgram",
    "glBindProgramPipeline",
    "glClearDepthf",
    "glCreateShaderProgramv",
    "glDeleteProgramPipelines",
    "glDepthRangeArrayv",
    "glDepthRangeIndexed",
    "glDepthRangef",
    "glGenProgramPipelines",
    "glGetDoublei_v",
    "glGetFloati_v",
    "glGetProgramBinary",
    "glGetProgramPipelineInfoLog",
    "glGetProgramPipelineiv",
    "glGetShaderPrecisionFormat",
    "glGetVertexAttribLdv",
    "glIsProgramPipeline",
    "glProgramBinary",
    "glProgramParameteri",
    "glProgramUniform1d",
    "glProgramUniform1dv",
    "glProgramUniform1f",
    "glProgramUniform1fv",
    "glProgramUniform1i",
    "glProgramUniform1iv",
    "glProgramUniform1ui",
    "glProgramUniform1uiv",
    "glProgramUniform2d",
    "glProgramUniform2dv",
    "glProgramUniform2f",
    "glProgramUniform2fv",
    "glProgramUniform2i",
    "glProgramUniform2iv",
    "glProgramUniform2ui",
    "glProgramUniform2uiv",
    "glProgramUniform3d",
    "glProgramUniform3dv",
    "glProgramUniform3f",
    "glProgramUniform3fv",
    "glProgramUniform3i",
    "glProgramUniform3iv",
    "glProgramUniform3ui",
    "glProgramUniform3uiv",
    "glProgramUniform4d",
    "glProgramUniform4dv",
    "glProgramUniform4f",
    "glProgramUniform4fv",
    "glProgramUniform4i",
    "glProgramUniform4iv",
    "glProgramUniform4ui",
    "glProgramUniform4uiv",
    "glProgramUniformMatrix2dv",
    "glProgramUniformMatrix2fv",
    "glProgramUniformMatrix2x3dv",
    "glProgramUniformMatrix2x3fv",
    "glProgramUniformMatrix2x4dv",
    "glProgramUniformMatrix2x4fv",
    "glProgramUniformMatrix3dv",
    "glProgramUniformMatrix3fv",
    "glProgramUniformMatrix3x2dv",
    "glProgramUniformMatrix3x2fv",
    "glProgramUniformMatrix3x4dv",
    "glProgramUniformMatrix3x4fv",
    "glProgramUniformMatrix4dv",
    "glProgramUniformMatrix4fv",
    "glProgramUniformMatrix4x2dv",
    "glProgramUniformMatrix4x2fv",
    "glProgramUniformMatrix4x3dv",
    "glProgramUniformMatrix4x3fv",
    "glReleaseShaderCompiler",
    "glScissorArrayv",
    "glScissorIndexed",
    "glScissorIndexedv",
    "glShaderBinary",
    "glUseProgramStages",
    "glValidateProgramPipeline",
    "glViewportArrayv",
    "glViewportIndexedf",
    "glViewportIndexedfv",
/* GL_VERSION_4_2 */
/* 9 functions */
    "glBindImageTexture",
    "glDrawArraysInstancedBaseInstance",
    "glDrawElementsInstancedBaseInstance",
    "glDrawElementsInstancedBaseVertexBaseInstance",
    "glDrawTransformFeedbackInstanced",
    "glDrawTransformFeedbackStreamInstanced",
    "glGetActiveAtomicCounterBufferiv",
    "glGetInternalformativ",
    "glMemoryBarrier",
/* GL_VERSION_4_3 */
/* 42 functions */
    "glBindVertexBuffer",
    "glClearBufferData",
    "glClearBufferSubData",
    "glCopyImageSubData",
    "glDebugMessageCallback",
    "glDebugMessageControl",
    "glDebugMessageInsert",
    "glDispatchCompute",
    "glDispatchComputeIndirect",
    "glFramebufferParameteri",
    "glGetDebugMessageLog",
    "glGetFramebufferParameteriv",
    "glGetInternalformati64v",
    "glGetObjectLabel",
    "glGetObjectPtrLabel",
    "glGetPointerv",
    "glGetProgramInterfaceiv",
    "glGetProgramResourceIndex",
    "glGetProgramResourceLocation",
    "glGetProgramResourceLocationIndex",
    "glGetProgramResourceName",
    "glGetProgramResourceiv",
    "glInvalidateBufferData",
    "glInvalidateBufferSubData",
    "glInvalidateFramebuffer",
    "glInvalidateSubFramebuffer",
    "glInvalidateTexImage",
    "glInvalidateTexSubImage",
    "glMultiDrawArraysIndirect",
    "glMultiDrawElementsIndirect",
    "glObjectLabel",
    "glObjectPtrLabel",
    "glPopDebugGroup",
    "glPushDebugGroup",
    "glShaderStorageBlockBinding",
    "glTexBufferRange",
    "glTextureView",
    "glVertexAttribBinding",
    "glVertexAttribFormat",
    "glVertexAttribIFormat",
    "glVertexAttribLFormat",
    "glVertexBindingDivisor",
/* GL_VERSION_4_4 */
/* 9 functions */
    "glBindBuffersBase",
    "glBindBuffersRange",
    "glBindImageTextures",
    "glBindSamplers",
    "glBindTextures",
    "glBindVertexBuffers",
    "glBufferStorage",
    "glClearTexImage",
    "glClearTexSubImage",
/* GL_VERSION_4_5 */
/* 110 functions */
    "glBindTextureUnit",
    "glBlitNamedFramebuffer",
    "glCheckNamedFramebufferStatus",
    "glClearNamedBufferData",
    "glClearNamedBufferSubData",
    "glClearNamedFramebufferfi",
    "glClearNamedFramebufferfv",
    "glClearNamedFramebufferiv",
    "glClearNamedFramebufferuiv",
    "glClipControl",
    "glCompressedTextureSubImage1D",
    "glCompressedTextureSubImage2D",
    "glCompressedTextureSubImage3D",
    "glCopyNamedBufferSubData",
    "glCopyTextureSubImage1D",
    "glCopyTextureSubImage2D",
    "glCopyTextureSubImage3D",
    "glCreateBuffers",
    "glCreateFramebuffers",
    "glCreateProgramPipelines",
    "glCreateQueries",
    "glCreateRenderbuffers",
    "glCreateSamplers",
    "glCreateTextures",
    "glCreateTransformFeedbacks",
    "glCreateVertexArrays",
    "glDisableVertexArrayAttrib",
    "glEnableVertexArrayAttrib",
    "glFlushMappedNamedBufferRange",
    "glGenerateTextureMipmap",
    "glGetCompressedTextureImage",
    "glGetCompressedTextureSubImage",
    "glGetGraphicsResetStatus",
    "glGetNamedBufferParameteri64v",
    "glGetNamedBufferParameteriv",
    "glGetNamedBufferPointerv",
    "glGetNamedBufferSubData",
    "glGetNamedFramebufferAttachmentParameteriv",
    "glGetNamedFramebufferParameteriv",
    "glGetNamedRenderbufferParameteriv",
    "glGetQueryBufferObjecti64v",
    "glGetQueryBufferObjectiv",
    "glGetQueryBufferObjectui64v",
    "glGetQueryBufferObjectuiv",
    "glGetTextureImage",
    "glGetTextureLevelParameterfv",
    "glGetTextureLevelParameteriv",
    "glGetTextureParameterIiv",
    "glGetTextureParameterIuiv",
    "glGetTextureParameterfv",
    "glGetTextureParameteriv",
    "glGetTextureSubImage",
    "glGetTransformFeedbacki64_v",
    "glGetTransformFeedbacki_v",
    "glGetTransformFeedbackiv",
    "glGetVertexArrayIndexed64iv",
    "glGetVertexArrayIndexediv",
    "glGetVertexArrayiv",
    "glGetnCompressedTexImage",
    "glGetnTexImage",
    "glGetnUniformdv",
    "glGetnUniformfv",
    "glGetnUniformiv",
    "glGetnUniformuiv",
    "glInvalidateNamedFramebufferData",
    "glInvalidateNamedFramebufferSubData",
    "glMapNamedBuffer",
    "glMapNamedBufferRange",
    "glMemoryBarrierByRegion",
    "glNamedBufferData",
    "glNamedBufferStorage",
    "glNamedBufferSubData",
    "glNamedFramebufferDrawBuffer",
    "glNamedFramebufferDrawBuffers",
    "glNamedFramebufferParameteri",
    "glNamedFramebufferReadBuffer",
    "glNamedFramebufferRenderbuffer",
    "glNamedFramebufferTexture",
    "glNamedFramebufferTextureLayer",
    "glNamedRenderbufferStorage",
    "glNamedRenderbufferStorageMultisample",
    "glReadnPixels",
    "glTextureBarrier",
    "glTextureBuffer",
    "glTextureBufferRange",
    "glTextureParameterIiv",
    "glTextureParameterIuiv",
    "glTextureParameterf",
    "glTextureParameterfv",
    "glTextureParameteri",
    "glTextureParameteriv",
    "glTextureStorage1D",
    "glTextureStorage2D",
    "glTextureStorage2DMultisample",
    "glTextureStorage3D",
    "glTextureStorage3DMultisample",
    "glTextureSubImage1D",
    "glTextureSubImage2D",
    "glTextureSubImage3D",
    "glTransformFeedbackBufferBase",
    "glTransformFeedbackBufferRange",
    "glUnmapNamedBuffer",
    "glVertexArrayAttribBinding",
    "glVertexArrayAttribFormat",
    "glVertexArrayAttribIFormat",
    "glVertexArrayAttribLFormat",
    "glVertexArrayBindingDivisor",
    "glVertexArrayElementBuffer",
    "glVertexArrayVertexBuffer",
    "glVertexArrayVertexBuffers",
};

void glextLoadFunctions(void)
{
    /* --- Function pointer loading --- */
    int i;
    for(i = 0 ; i < 439 ; i++){
        glfp.fp[i] = glextGetProc((const char*)glfp.fp[i]);
    }
}

static void glextAddExtension(const char* extension)
{
}

/* ------------------ code from Slavomir Kaslev's gl3w ----------------- */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

static HMODULE libgl;

static void glextLoadModule(void)
{
    libgl = LoadLibraryA("opengl32.dll");
}

static void glextUnloadModule(void)
{
    FreeLibrary(libgl);
}

static void* glextGetProc(const char *proc)
{
    void* res = wglGetProcAddress(proc);
    if (!res)
        res = GetProcAddress(libgl, proc);
    return res;
}
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#include <Carbon/Carbon.h>

CFBundleRef bundle;
CFURLRef bundleURL;

static void glextLoadModule(void)
{
    bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                CFSTR("/System/Library/Frameworks/OpenGL.framework"),
                kCFURLPOSIXPathStyle, true);

    bundle = CFBundleCreate(kCFAllocatorDefault, bundleURL);
    assert(bundle != NULL);
}

static void glextUnloadModule(void)
{
    CFRelease(bundle);
    CFRelease(bundleURL);
}

static void* glextGetProc(const char *proc)
{
    void* res;

    CFStringRef procname = CFStringCreateWithCString(kCFAllocatorDefault, proc,
                kCFStringEncodingASCII);
    res = CFBundleGetFunctionPointerForName(bundle, procname);
    CFRelease(procname);
    return res;
}
#elif defined(ANDROID)
#include <EGL/egl.h>

static void glextLoadModule(void)
{
    // nothing to do
}

static void glextUnloadModule(void)
{
    // nothing to do
}

static void* glextGetProc(const char *proc)
{
    return eglGetProcAddress((const char *) proc);
}
#else
#include <dlfcn.h>
#include <GL/glx.h>

static void *libgl;

static void glextLoadModule(void)
{
    libgl = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
}

static void glextUnloadModule(void)
{
    dlclose(libgl);
}

static void* glextGetProc(const char *proc)
{
    void* res = glXGetProcAddress((const GLubyte *) proc);
    if (!res)
        res = dlsym(libgl, proc);
    return res;
}
#endif

#ifdef __cplusplus
}
#endif
