/* WARNING: This file was automatically generated */
/* Do not edit. */

#include "opengl.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ---------------------- Function pointer definitions --------------------- */
GLAPI glapi = {
/* GL_VERSION_1_0 */
/* 37 functions */
    "glBlendFunc",
    "glClear",
    "glClearColor",
    "glClearDepth",
    "glClearStencil",
    "glColorMask",
    "glCullFace",
    "glDepthFunc",
    "glDepthMask",
    "glDepthRange",
    "glDisable",
    "glDrawBuffer",
    "glEnable",
    "glFinish",
    "glFlush",
    "glFrontFace",
    "glGetBooleanv",
    "glGetDoublev",
    "glGetError",
    "glGetFloatv",
    "glGetIntegerv",
    "glGetString",
    "glHint",
    "glIsEnabled",
    "glLineWidth",
    "glLogicOp",
    "glPixelStoref",
    "glPixelStorei",
    "glPointSize",
    "glPolygonMode",
    "glReadBuffer",
    "glReadPixels",
    "glScissor",
    "glStencilFunc",
    "glStencilMask",
    "glStencilOp",
    "glViewport",
/* GL_VERSION_1_1 */
/* 5 functions */
    "glDeleteTextures",
    "glDrawArrays",
    "glDrawElements",
    "glIsTexture",
    "glPolygonOffset",
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
/* GL_VERSION_2_1 */
/* 0 functions */
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
/* 41 functions */
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

/* ------------------ code from Slavomir Kaslev's gl3w ----------------- */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

void importOpenGL(void)
{
    uintptr_t i;
    HMODULE libgl;
    
    libgl = LoadLibraryA("opengl32.dll");
    for(i = 0 ; i < EGLAPI_COUNT; i++){
        PGLAPI_PROC res = wglGetProcAddress(glapi[i].n);
        if (!res) {res = GetProcAddress(libgl, glapi[i].n);}
        glapi[i].f = res;
    }
}
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#include <Carbon/Carbon.h>

void importOpenGL(void)
{
    uintptr_t i;
    CFBundleRef bundle;
    CFURLRef bundleURL;

    bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                CFSTR("/System/Library/Frameworks/OpenGL.framework"),
                kCFURLPOSIXPathStyle, true);
    bundle = CFBundleCreate(kCFAllocatorDefault, bundleURL);
    for(i = 0 ; i < EGLAPI_COUNT; i++){
        PGLAPI_PROC res;

        CFStringRef procname = CFStringCreateWithCString(kCFAllocatorDefault, glapi[i].n,
                    kCFStringEncodingASCII);
        glapi[i].f = CFBundleGetFunctionPointerForName(bundle, procname);
        CFRelease(procname);
    }
    CFRelease(bundle);
    CFRelease(bundleURL);
}
#elif defined(ANDROID)
#include <EGL/egl.h>

void importOpenGL(void)
{
    uintptr_t i;

    for(i = 0 ; i < EGLAPI_COUNT; i++){
        glapi[i].f = eglGetProcAddress(glapi[i].n);
    }
}
#else
#include <dlfcn.h>
#include <GL/glx.h>

void importOpenGL(void)
{
    uintptr_t i;
    void *libgl;
    
    libgl = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
    for(i = 0 ; i < EGLAPI_COUNT; i++){
        PGLAPI_PROC res = glXGetProcAddress((const GLubyte *) glapi[i].n);
        if (!res) {res = dlsym(libgl, glapi[i].n);}
        glapi[i].f = res;
    }
}
#endif

#ifdef __cplusplus
}
#endif
