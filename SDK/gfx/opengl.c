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
    if (10 * major + minor < 44) {
        fprintf(stderr, "Error: OpenGL version 4.4 not supported.\n");
        fprintf(stderr, "       Your version is %d.%d.\n", major, minor);
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }

    
    /* --- Check for extensions --- */

    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
    
    for (i = 0; i < num_extensions; i++) {
        glextAddExtension((const char*)glGetStringi(GL_EXTENSIONS, i));
    }

    if (!GLEXT_EXT_direct_state_access) {
        fprintf(stderr, "Error: OpenGL extension GL_EXT_direct_state_access not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }

    return GL_TRUE;
}

/* ----------------------- Extension flag definitions ---------------------- */
int GLEXT_EXT_direct_state_access = GL_FALSE;

/* ---------------------- Function pointer definitions --------------------- */
GLFP glfp = {
/* GL_EXT_direct_state_access */
/* 255 functions */
    "glBindMultiTextureEXT",
    "glCheckNamedFramebufferStatusEXT",
    "glClearNamedBufferDataEXT",
    "glClearNamedBufferSubDataEXT",
    "glClientAttribDefaultEXT",
    "glCompressedMultiTexImage1DEXT",
    "glCompressedMultiTexImage2DEXT",
    "glCompressedMultiTexImage3DEXT",
    "glCompressedMultiTexSubImage1DEXT",
    "glCompressedMultiTexSubImage2DEXT",
    "glCompressedMultiTexSubImage3DEXT",
    "glCompressedTextureImage1DEXT",
    "glCompressedTextureImage2DEXT",
    "glCompressedTextureImage3DEXT",
    "glCompressedTextureSubImage1DEXT",
    "glCompressedTextureSubImage2DEXT",
    "glCompressedTextureSubImage3DEXT",
    "glCopyMultiTexImage1DEXT",
    "glCopyMultiTexImage2DEXT",
    "glCopyMultiTexSubImage1DEXT",
    "glCopyMultiTexSubImage2DEXT",
    "glCopyMultiTexSubImage3DEXT",
    "glCopyTextureImage1DEXT",
    "glCopyTextureImage2DEXT",
    "glCopyTextureSubImage1DEXT",
    "glCopyTextureSubImage2DEXT",
    "glCopyTextureSubImage3DEXT",
    "glDisableClientStateIndexedEXT",
    "glDisableClientStateiEXT",
    "glDisableIndexedEXT",
    "glDisableVertexArrayAttribEXT",
    "glDisableVertexArrayEXT",
    "glEnableClientStateIndexedEXT",
    "glEnableClientStateiEXT",
    "glEnableIndexedEXT",
    "glEnableVertexArrayAttribEXT",
    "glEnableVertexArrayEXT",
    "glFlushMappedNamedBufferRangeEXT",
    "glFramebufferDrawBufferEXT",
    "glFramebufferDrawBuffersEXT",
    "glFramebufferReadBufferEXT",
    "glGenerateMultiTexMipmapEXT",
    "glGenerateTextureMipmapEXT",
    "glGetBooleanIndexedvEXT",
    "glGetCompressedMultiTexImageEXT",
    "glGetCompressedTextureImageEXT",
    "glGetDoubleIndexedvEXT",
    "glGetDoublei_vEXT",
    "glGetFloatIndexedvEXT",
    "glGetFloati_vEXT",
    "glGetFramebufferParameterivEXT",
    "glGetIntegerIndexedvEXT",
    "glGetMultiTexEnvfvEXT",
    "glGetMultiTexEnvivEXT",
    "glGetMultiTexGendvEXT",
    "glGetMultiTexGenfvEXT",
    "glGetMultiTexGenivEXT",
    "glGetMultiTexImageEXT",
    "glGetMultiTexLevelParameterfvEXT",
    "glGetMultiTexLevelParameterivEXT",
    "glGetMultiTexParameterIivEXT",
    "glGetMultiTexParameterIuivEXT",
    "glGetMultiTexParameterfvEXT",
    "glGetMultiTexParameterivEXT",
    "glGetNamedBufferParameterivEXT",
    "glGetNamedBufferPointervEXT",
    "glGetNamedBufferSubDataEXT",
    "glGetNamedFramebufferAttachmentParameterivEXT",
    "glGetNamedFramebufferParameterivEXT",
    "glGetNamedProgramLocalParameterIivEXT",
    "glGetNamedProgramLocalParameterIuivEXT",
    "glGetNamedProgramLocalParameterdvEXT",
    "glGetNamedProgramLocalParameterfvEXT",
    "glGetNamedProgramStringEXT",
    "glGetNamedProgramivEXT",
    "glGetNamedRenderbufferParameterivEXT",
    "glGetPointerIndexedvEXT",
    "glGetPointeri_vEXT",
    "glGetTextureImageEXT",
    "glGetTextureLevelParameterfvEXT",
    "glGetTextureLevelParameterivEXT",
    "glGetTextureParameterIivEXT",
    "glGetTextureParameterIuivEXT",
    "glGetTextureParameterfvEXT",
    "glGetTextureParameterivEXT",
    "glGetVertexArrayIntegeri_vEXT",
    "glGetVertexArrayIntegervEXT",
    "glGetVertexArrayPointeri_vEXT",
    "glGetVertexArrayPointervEXT",
    "glIsEnabledIndexedEXT",
    "glMapNamedBufferEXT",
    "glMapNamedBufferRangeEXT",
    "glMatrixFrustumEXT",
    "glMatrixLoadIdentityEXT",
    "glMatrixLoadTransposedEXT",
    "glMatrixLoadTransposefEXT",
    "glMatrixLoaddEXT",
    "glMatrixLoadfEXT",
    "glMatrixMultTransposedEXT",
    "glMatrixMultTransposefEXT",
    "glMatrixMultdEXT",
    "glMatrixMultfEXT",
    "glMatrixOrthoEXT",
    "glMatrixPopEXT",
    "glMatrixPushEXT",
    "glMatrixRotatedEXT",
    "glMatrixRotatefEXT",
    "glMatrixScaledEXT",
    "glMatrixScalefEXT",
    "glMatrixTranslatedEXT",
    "glMatrixTranslatefEXT",
    "glMultiTexBufferEXT",
    "glMultiTexCoordPointerEXT",
    "glMultiTexEnvfEXT",
    "glMultiTexEnvfvEXT",
    "glMultiTexEnviEXT",
    "glMultiTexEnvivEXT",
    "glMultiTexGendEXT",
    "glMultiTexGendvEXT",
    "glMultiTexGenfEXT",
    "glMultiTexGenfvEXT",
    "glMultiTexGeniEXT",
    "glMultiTexGenivEXT",
    "glMultiTexImage1DEXT",
    "glMultiTexImage2DEXT",
    "glMultiTexImage3DEXT",
    "glMultiTexParameterIivEXT",
    "glMultiTexParameterIuivEXT",
    "glMultiTexParameterfEXT",
    "glMultiTexParameterfvEXT",
    "glMultiTexParameteriEXT",
    "glMultiTexParameterivEXT",
    "glMultiTexRenderbufferEXT",
    "glMultiTexSubImage1DEXT",
    "glMultiTexSubImage2DEXT",
    "glMultiTexSubImage3DEXT",
    "glNamedBufferDataEXT",
    "glNamedBufferStorageEXT",
    "glNamedBufferSubDataEXT",
    "glNamedCopyBufferSubDataEXT",
    "glNamedFramebufferParameteriEXT",
    "glNamedFramebufferRenderbufferEXT",
    "glNamedFramebufferTexture1DEXT",
    "glNamedFramebufferTexture2DEXT",
    "glNamedFramebufferTexture3DEXT",
    "glNamedFramebufferTextureEXT",
    "glNamedFramebufferTextureFaceEXT",
    "glNamedFramebufferTextureLayerEXT",
    "glNamedProgramLocalParameter4dEXT",
    "glNamedProgramLocalParameter4dvEXT",
    "glNamedProgramLocalParameter4fEXT",
    "glNamedProgramLocalParameter4fvEXT",
    "glNamedProgramLocalParameterI4iEXT",
    "glNamedProgramLocalParameterI4ivEXT",
    "glNamedProgramLocalParameterI4uiEXT",
    "glNamedProgramLocalParameterI4uivEXT",
    "glNamedProgramLocalParameters4fvEXT",
    "glNamedProgramLocalParametersI4ivEXT",
    "glNamedProgramLocalParametersI4uivEXT",
    "glNamedProgramStringEXT",
    "glNamedRenderbufferStorageEXT",
    "glNamedRenderbufferStorageMultisampleCoverageEXT",
    "glNamedRenderbufferStorageMultisampleEXT",
    "glProgramUniform1dEXT",
    "glProgramUniform1dvEXT",
    "glProgramUniform1fEXT",
    "glProgramUniform1fvEXT",
    "glProgramUniform1iEXT",
    "glProgramUniform1ivEXT",
    "glProgramUniform1uiEXT",
    "glProgramUniform1uivEXT",
    "glProgramUniform2dEXT",
    "glProgramUniform2dvEXT",
    "glProgramUniform2fEXT",
    "glProgramUniform2fvEXT",
    "glProgramUniform2iEXT",
    "glProgramUniform2ivEXT",
    "glProgramUniform2uiEXT",
    "glProgramUniform2uivEXT",
    "glProgramUniform3dEXT",
    "glProgramUniform3dvEXT",
    "glProgramUniform3fEXT",
    "glProgramUniform3fvEXT",
    "glProgramUniform3iEXT",
    "glProgramUniform3ivEXT",
    "glProgramUniform3uiEXT",
    "glProgramUniform3uivEXT",
    "glProgramUniform4dEXT",
    "glProgramUniform4dvEXT",
    "glProgramUniform4fEXT",
    "glProgramUniform4fvEXT",
    "glProgramUniform4iEXT",
    "glProgramUniform4ivEXT",
    "glProgramUniform4uiEXT",
    "glProgramUniform4uivEXT",
    "glProgramUniformMatrix2dvEXT",
    "glProgramUniformMatrix2fvEXT",
    "glProgramUniformMatrix2x3dvEXT",
    "glProgramUniformMatrix2x3fvEXT",
    "glProgramUniformMatrix2x4dvEXT",
    "glProgramUniformMatrix2x4fvEXT",
    "glProgramUniformMatrix3dvEXT",
    "glProgramUniformMatrix3fvEXT",
    "glProgramUniformMatrix3x2dvEXT",
    "glProgramUniformMatrix3x2fvEXT",
    "glProgramUniformMatrix3x4dvEXT",
    "glProgramUniformMatrix3x4fvEXT",
    "glProgramUniformMatrix4dvEXT",
    "glProgramUniformMatrix4fvEXT",
    "glProgramUniformMatrix4x2dvEXT",
    "glProgramUniformMatrix4x2fvEXT",
    "glProgramUniformMatrix4x3dvEXT",
    "glProgramUniformMatrix4x3fvEXT",
    "glPushClientAttribDefaultEXT",
    "glTextureBufferEXT",
    "glTextureBufferRangeEXT",
    "glTextureImage1DEXT",
    "glTextureImage2DEXT",
    "glTextureImage3DEXT",
    "glTexturePageCommitmentEXT",
    "glTextureParameterIivEXT",
    "glTextureParameterIuivEXT",
    "glTextureParameterfEXT",
    "glTextureParameterfvEXT",
    "glTextureParameteriEXT",
    "glTextureParameterivEXT",
    "glTextureRenderbufferEXT",
    "glTextureStorage1DEXT",
    "glTextureStorage2DEXT",
    "glTextureStorage2DMultisampleEXT",
    "glTextureStorage3DEXT",
    "glTextureStorage3DMultisampleEXT",
    "glTextureSubImage1DEXT",
    "glTextureSubImage2DEXT",
    "glTextureSubImage3DEXT",
    "glUnmapNamedBufferEXT",
    "glVertexArrayBindVertexBufferEXT",
    "glVertexArrayColorOffsetEXT",
    "glVertexArrayEdgeFlagOffsetEXT",
    "glVertexArrayFogCoordOffsetEXT",
    "glVertexArrayIndexOffsetEXT",
    "glVertexArrayMultiTexCoordOffsetEXT",
    "glVertexArrayNormalOffsetEXT",
    "glVertexArraySecondaryColorOffsetEXT",
    "glVertexArrayTexCoordOffsetEXT",
    "glVertexArrayVertexAttribBindingEXT",
    "glVertexArrayVertexAttribDivisorEXT",
    "glVertexArrayVertexAttribFormatEXT",
    "glVertexArrayVertexAttribIFormatEXT",
    "glVertexArrayVertexAttribIOffsetEXT",
    "glVertexArrayVertexAttribLFormatEXT",
    "glVertexArrayVertexAttribLOffsetEXT",
    "glVertexArrayVertexAttribOffsetEXT",
    "glVertexArrayVertexBindingDivisorEXT",
    "glVertexArrayVertexOffsetEXT",
/* GL_VERSION_1_2 */
/* 4 functions */
    "glCopyTexSubImage3D",
    "glDrawRangeElements",
    "glTexImage3D",
    "glTexSubImage3D",
/* GL_VERSION_1_3 */
/* 9 functions */
    "glActiveTexture",
    "glCompressedTexImage1D",
    "glCompressedTexImage2D",
    "glCompressedTexImage3D",
    "glCompressedTexSubImage1D",
    "glCompressedTexSubImage2D",
    "glCompressedTexSubImage3D",
    "glGetCompressedTexImage",
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
/* 19 functions */
    "glBeginQuery",
    "glBindBuffer",
    "glBufferData",
    "glBufferSubData",
    "glDeleteBuffers",
    "glDeleteQueries",
    "glEndQuery",
    "glGenBuffers",
    "glGenQueries",
    "glGetBufferParameteriv",
    "glGetBufferPointerv",
    "glGetBufferSubData",
    "glGetQueryObjectiv",
    "glGetQueryObjectuiv",
    "glGetQueryiv",
    "glIsBuffer",
    "glIsQuery",
    "glMapBuffer",
    "glUnmapBuffer",
/* GL_VERSION_2_0 */
/* 93 functions */
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
    "glUniform1f",
    "glUniform1fv",
    "glUniform1i",
    "glUniform1iv",
    "glUniform2f",
    "glUniform2fv",
    "glUniform2i",
    "glUniform2iv",
    "glUniform3f",
    "glUniform3fv",
    "glUniform3i",
    "glUniform3iv",
    "glUniform4f",
    "glUniform4fv",
    "glUniform4i",
    "glUniform4iv",
    "glUniformMatrix2fv",
    "glUniformMatrix3fv",
    "glUniformMatrix4fv",
    "glUseProgram",
    "glValidateProgram",
    "glVertexAttrib1d",
    "glVertexAttrib1dv",
    "glVertexAttrib1f",
    "glVertexAttrib1fv",
    "glVertexAttrib1s",
    "glVertexAttrib1sv",
    "glVertexAttrib2d",
    "glVertexAttrib2dv",
    "glVertexAttrib2f",
    "glVertexAttrib2fv",
    "glVertexAttrib2s",
    "glVertexAttrib2sv",
    "glVertexAttrib3d",
    "glVertexAttrib3dv",
    "glVertexAttrib3f",
    "glVertexAttrib3fv",
    "glVertexAttrib3s",
    "glVertexAttrib3sv",
    "glVertexAttrib4Nbv",
    "glVertexAttrib4Niv",
    "glVertexAttrib4Nsv",
    "glVertexAttrib4Nub",
    "glVertexAttrib4Nubv",
    "glVertexAttrib4Nuiv",
    "glVertexAttrib4Nusv",
    "glVertexAttrib4bv",
    "glVertexAttrib4d",
    "glVertexAttrib4dv",
    "glVertexAttrib4f",
    "glVertexAttrib4fv",
    "glVertexAttrib4iv",
    "glVertexAttrib4s",
    "glVertexAttrib4sv",
    "glVertexAttrib4ubv",
    "glVertexAttrib4uiv",
    "glVertexAttrib4usv",
    "glVertexAttribPointer",
/* GL_VERSION_2_1 */
/* 6 functions */
    "glUniformMatrix2x3fv",
    "glUniformMatrix2x4fv",
    "glUniformMatrix3x2fv",
    "glUniformMatrix3x4fv",
    "glUniformMatrix4x2fv",
    "glUniformMatrix4x3fv",
/* GL_VERSION_3_0 */
/* 84 functions */
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
    "glUniform1ui",
    "glUniform1uiv",
    "glUniform2ui",
    "glUniform2uiv",
    "glUniform3ui",
    "glUniform3uiv",
    "glUniform4ui",
    "glUniform4uiv",
    "glVertexAttribI1i",
    "glVertexAttribI1iv",
    "glVertexAttribI1ui",
    "glVertexAttribI1uiv",
    "glVertexAttribI2i",
    "glVertexAttribI2iv",
    "glVertexAttribI2ui",
    "glVertexAttribI2uiv",
    "glVertexAttribI3i",
    "glVertexAttribI3iv",
    "glVertexAttribI3ui",
    "glVertexAttribI3uiv",
    "glVertexAttribI4bv",
    "glVertexAttribI4i",
    "glVertexAttribI4iv",
    "glVertexAttribI4sv",
    "glVertexAttribI4ubv",
    "glVertexAttribI4ui",
    "glVertexAttribI4uiv",
    "glVertexAttribI4usv",
    "glVertexAttribIPointer",
/* GL_VERSION_3_1 */
/* 12 functions */
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
    "glTexBuffer",
    "glUniformBlockBinding",
/* GL_VERSION_3_2 */
/* 19 functions */
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
    "glTexImage2DMultisample",
    "glTexImage3DMultisample",
    "glWaitSync",
/* GL_VERSION_3_3 */
/* 28 functions */
    "glBindFragDataLocationIndexed",
    "glBindSampler",
    "glDeleteSamplers",
    "glGenSamplers",
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
    "glVertexAttribDivisor",
    "glVertexAttribP1ui",
    "glVertexAttribP1uiv",
    "glVertexAttribP2ui",
    "glVertexAttribP2uiv",
    "glVertexAttribP3ui",
    "glVertexAttribP3uiv",
    "glVertexAttribP4ui",
    "glVertexAttribP4uiv",
/* GL_VERSION_4_0 */
/* 46 functions */
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
    "glUniform1d",
    "glUniform1dv",
    "glUniform2d",
    "glUniform2dv",
    "glUniform3d",
    "glUniform3dv",
    "glUniform4d",
    "glUniform4dv",
    "glUniformMatrix2dv",
    "glUniformMatrix2x3dv",
    "glUniformMatrix2x4dv",
    "glUniformMatrix3dv",
    "glUniformMatrix3x2dv",
    "glUniformMatrix3x4dv",
    "glUniformMatrix4dv",
    "glUniformMatrix4x2dv",
    "glUniformMatrix4x3dv",
    "glUniformSubroutinesuiv",
/* GL_VERSION_4_1 */
/* 88 functions */
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
    "glVertexAttribL1d",
    "glVertexAttribL1dv",
    "glVertexAttribL2d",
    "glVertexAttribL2dv",
    "glVertexAttribL3d",
    "glVertexAttribL3dv",
    "glVertexAttribL4d",
    "glVertexAttribL4dv",
    "glVertexAttribLPointer",
    "glViewportArrayv",
    "glViewportIndexedf",
    "glViewportIndexedfv",
/* GL_VERSION_4_2 */
/* 12 functions */
    "glBindImageTexture",
    "glDrawArraysInstancedBaseInstance",
    "glDrawElementsInstancedBaseInstance",
    "glDrawElementsInstancedBaseVertexBaseInstance",
    "glDrawTransformFeedbackInstanced",
    "glDrawTransformFeedbackStreamInstanced",
    "glGetActiveAtomicCounterBufferiv",
    "glGetInternalformativ",
    "glMemoryBarrier",
    "glTexStorage1D",
    "glTexStorage2D",
    "glTexStorage3D",
/* GL_VERSION_4_3 */
/* 44 functions */
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
    "glTexStorage2DMultisample",
    "glTexStorage3DMultisample",
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
};

void glextLoadFunctions(void)
{
    /* --- Function pointer loading --- */
    int i;
    for(i = 0 ; i < 737 ; i++){
        glfp.fp[i] = glextGetProc((const char*)glfp.fp[i]);
    }
}

static void glextAddExtension(const char* extension)
{
    if (strcmp("GL_EXT_direct_state_access", extension) == 0) {
        GLEXT_EXT_direct_state_access = GL_TRUE;
    }
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
