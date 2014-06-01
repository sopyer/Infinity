/* WARNING: This file was automatically generated */
/* Do not edit. */

#include "opengl.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef void(__stdcall *GLPROC)();

static void   glextLoadFunctions(void);
static void   glextLoadModule(void);
static void   glextUnloadModule(void);
static GLPROC glextGetProc(const char *proc);
static void   glextAddExtension(const char* extension);

int importOpenGL(void)
{
    GLint minor, major;
    GLint num_extensions;
    int i;
    GLint profile;

    glextLoadModule();
    glextLoadFunctions();
    glextUnloadModule();

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    if (10 * major + minor < 43) {
        fprintf(stderr, "Error: OpenGL version 4.3 not supported.\n");
        fprintf(stderr, "       Your version is %d.%d.\n", major, minor);
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }

    
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

    if ((profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) == 0) {
        fprintf(stderr, "Error: This application requires a compatibility profile\n");
        return GL_FALSE;
    }
    
    /* --- Check for minimal version and profile --- */

    if (major * 10 + minor < 43) {
    fprintf(stderr, "Error: OpenGL version 4.3 not supported.\n");
        fprintf(stderr, "       Your version is %d.%d.\n", major, minor);
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }

    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

    if ((profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) == 0) {
    fprintf(stderr, "Error: This application requires a compatibility profile\n");
        return GL_FALSE;
    }

    /* --- Check for extensions --- */

    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
    
    for (i = 0; i < num_extensions; i++) {
        glextAddExtension((const char*)glGetStringi(GL_EXTENSIONS, i));
    }

    if (!GLEXT_ARB_buffer_storage) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_buffer_storage not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_seamless_cubemap_per_texture) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_seamless_cubemap_per_texture not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_bindless_texture) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_bindless_texture not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_sparse_texture) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_sparse_texture not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_shader_draw_parameters) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_shader_draw_parameters not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_indirect_parameters) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_indirect_parameters not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_shader_group_vote) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_shader_group_vote not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_multi_bind) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_multi_bind not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_clear_texture) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_clear_texture not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_query_buffer_object) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_query_buffer_object not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_enhanced_layouts) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_enhanced_layouts not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_texture_mirror_clamp_to_edge) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_texture_mirror_clamp_to_edge not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_texture_stencil8) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_texture_stencil8 not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_compute_variable_group_size) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_compute_variable_group_size not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_ARB_vertex_type_10f_11f_11f_rev) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_vertex_type_10f_11f_11f_rev not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }
    if (!GLEXT_EXT_direct_state_access) {
        fprintf(stderr, "Error: OpenGL extension GL_EXT_direct_state_access not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }

    return GL_TRUE;
}



void glextLoadFunctions(void)
{
    /* --- Function pointer loading --- */


    /* GL_ARB_ES2_compatibility */

    glpfReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILER_PROC*)glextGetProc("glReleaseShaderCompiler");
    glpfShaderBinary = (PFNGLSHADERBINARY_PROC*)glextGetProc("glShaderBinary");
    glpfGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMAT_PROC*)glextGetProc("glGetShaderPrecisionFormat");
    glpfDepthRangef = (PFNGLDEPTHRANGEF_PROC*)glextGetProc("glDepthRangef");
    glpfClearDepthf = (PFNGLCLEARDEPTHF_PROC*)glextGetProc("glClearDepthf");


    /* GL_ARB_base_instance */

    glpfDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCE_PROC*)glextGetProc("glDrawArraysInstancedBaseInstance");
    glpfDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCE_PROC*)glextGetProc("glDrawElementsInstancedBaseInstance");
    glpfDrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCE_PROC*)glextGetProc("glDrawElementsInstancedBaseVertexBaseInstance");


    /* GL_ARB_blend_func_extended */

    glpfBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXED_PROC*)glextGetProc("glBindFragDataLocationIndexed");
    glpfGetFragDataIndex = (PFNGLGETFRAGDATAINDEX_PROC*)glextGetProc("glGetFragDataIndex");


    /* GL_ARB_clear_buffer_object */

    glpfClearBufferData = (PFNGLCLEARBUFFERDATA_PROC*)glextGetProc("glClearBufferData");
    glpfClearBufferSubData = (PFNGLCLEARBUFFERSUBDATA_PROC*)glextGetProc("glClearBufferSubData");
    glpfClearNamedBufferDataEXT = (PFNGLCLEARNAMEDBUFFERDATAEXT_PROC*)glextGetProc("glClearNamedBufferDataEXT");
    glpfClearNamedBufferSubDataEXT = (PFNGLCLEARNAMEDBUFFERSUBDATAEXT_PROC*)glextGetProc("glClearNamedBufferSubDataEXT");


    /* GL_ARB_compute_shader */

    glpfDispatchCompute = (PFNGLDISPATCHCOMPUTE_PROC*)glextGetProc("glDispatchCompute");
    glpfDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECT_PROC*)glextGetProc("glDispatchComputeIndirect");


    /* GL_ARB_copy_buffer */

    glpfCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATA_PROC*)glextGetProc("glCopyBufferSubData");


    /* GL_ARB_copy_image */

    glpfCopyImageSubData = (PFNGLCOPYIMAGESUBDATA_PROC*)glextGetProc("glCopyImageSubData");


    /* GL_ARB_draw_elements_base_vertex */

    glpfDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEX_PROC*)glextGetProc("glDrawElementsBaseVertex");
    glpfDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEX_PROC*)glextGetProc("glDrawRangeElementsBaseVertex");
    glpfDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEX_PROC*)glextGetProc("glDrawElementsInstancedBaseVertex");
    glpfMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEX_PROC*)glextGetProc("glMultiDrawElementsBaseVertex");


    /* GL_ARB_draw_indirect */

    glpfDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECT_PROC*)glextGetProc("glDrawArraysIndirect");
    glpfDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECT_PROC*)glextGetProc("glDrawElementsIndirect");


    /* GL_ARB_framebuffer_no_attachments */

    glpfFramebufferParameteri = (PFNGLFRAMEBUFFERPARAMETERI_PROC*)glextGetProc("glFramebufferParameteri");
    glpfGetFramebufferParameteriv = (PFNGLGETFRAMEBUFFERPARAMETERIV_PROC*)glextGetProc("glGetFramebufferParameteriv");
    glpfNamedFramebufferParameteriEXT = (PFNGLNAMEDFRAMEBUFFERPARAMETERIEXT_PROC*)glextGetProc("glNamedFramebufferParameteriEXT");
    glpfGetNamedFramebufferParameterivEXT = (PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXT_PROC*)glextGetProc("glGetNamedFramebufferParameterivEXT");


    /* GL_ARB_framebuffer_object */

    glpfIsRenderbuffer = (PFNGLISRENDERBUFFER_PROC*)glextGetProc("glIsRenderbuffer");
    glpfBindRenderbuffer = (PFNGLBINDRENDERBUFFER_PROC*)glextGetProc("glBindRenderbuffer");
    glpfDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERS_PROC*)glextGetProc("glDeleteRenderbuffers");
    glpfGenRenderbuffers = (PFNGLGENRENDERBUFFERS_PROC*)glextGetProc("glGenRenderbuffers");
    glpfRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGE_PROC*)glextGetProc("glRenderbufferStorage");
    glpfGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIV_PROC*)glextGetProc("glGetRenderbufferParameteriv");
    glpfIsFramebuffer = (PFNGLISFRAMEBUFFER_PROC*)glextGetProc("glIsFramebuffer");
    glpfBindFramebuffer = (PFNGLBINDFRAMEBUFFER_PROC*)glextGetProc("glBindFramebuffer");
    glpfDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERS_PROC*)glextGetProc("glDeleteFramebuffers");
    glpfGenFramebuffers = (PFNGLGENFRAMEBUFFERS_PROC*)glextGetProc("glGenFramebuffers");
    glpfCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUS_PROC*)glextGetProc("glCheckFramebufferStatus");
    glpfFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1D_PROC*)glextGetProc("glFramebufferTexture1D");
    glpfFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2D_PROC*)glextGetProc("glFramebufferTexture2D");
    glpfFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3D_PROC*)glextGetProc("glFramebufferTexture3D");
    glpfFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFER_PROC*)glextGetProc("glFramebufferRenderbuffer");
    glpfGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV_PROC*)glextGetProc("glGetFramebufferAttachmentParameteriv");
    glpfGenerateMipmap = (PFNGLGENERATEMIPMAP_PROC*)glextGetProc("glGenerateMipmap");
    glpfBlitFramebuffer = (PFNGLBLITFRAMEBUFFER_PROC*)glextGetProc("glBlitFramebuffer");
    glpfRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLE_PROC*)glextGetProc("glRenderbufferStorageMultisample");
    glpfFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYER_PROC*)glextGetProc("glFramebufferTextureLayer");


    /* GL_ARB_get_program_binary */

    glpfGetProgramBinary = (PFNGLGETPROGRAMBINARY_PROC*)glextGetProc("glGetProgramBinary");
    glpfProgramBinary = (PFNGLPROGRAMBINARY_PROC*)glextGetProc("glProgramBinary");
    glpfProgramParameteri = (PFNGLPROGRAMPARAMETERI_PROC*)glextGetProc("glProgramParameteri");


    /* GL_ARB_gpu_shader_fp64 */

    glpfUniform1d = (PFNGLUNIFORM1D_PROC*)glextGetProc("glUniform1d");
    glpfUniform2d = (PFNGLUNIFORM2D_PROC*)glextGetProc("glUniform2d");
    glpfUniform3d = (PFNGLUNIFORM3D_PROC*)glextGetProc("glUniform3d");
    glpfUniform4d = (PFNGLUNIFORM4D_PROC*)glextGetProc("glUniform4d");
    glpfUniform1dv = (PFNGLUNIFORM1DV_PROC*)glextGetProc("glUniform1dv");
    glpfUniform2dv = (PFNGLUNIFORM2DV_PROC*)glextGetProc("glUniform2dv");
    glpfUniform3dv = (PFNGLUNIFORM3DV_PROC*)glextGetProc("glUniform3dv");
    glpfUniform4dv = (PFNGLUNIFORM4DV_PROC*)glextGetProc("glUniform4dv");
    glpfUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DV_PROC*)glextGetProc("glUniformMatrix2dv");
    glpfUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DV_PROC*)glextGetProc("glUniformMatrix3dv");
    glpfUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DV_PROC*)glextGetProc("glUniformMatrix4dv");
    glpfUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DV_PROC*)glextGetProc("glUniformMatrix2x3dv");
    glpfUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DV_PROC*)glextGetProc("glUniformMatrix2x4dv");
    glpfUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DV_PROC*)glextGetProc("glUniformMatrix3x2dv");
    glpfUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DV_PROC*)glextGetProc("glUniformMatrix3x4dv");
    glpfUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DV_PROC*)glextGetProc("glUniformMatrix4x2dv");
    glpfUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DV_PROC*)glextGetProc("glUniformMatrix4x3dv");
    glpfGetUniformdv = (PFNGLGETUNIFORMDV_PROC*)glextGetProc("glGetUniformdv");


    /* GL_ARB_internalformat_query */

    glpfGetInternalformativ = (PFNGLGETINTERNALFORMATIV_PROC*)glextGetProc("glGetInternalformativ");


    /* GL_ARB_internalformat_query2 */

    glpfGetInternalformati64v = (PFNGLGETINTERNALFORMATI64V_PROC*)glextGetProc("glGetInternalformati64v");


    /* GL_ARB_invalidate_subdata */

    glpfInvalidateTexSubImage = (PFNGLINVALIDATETEXSUBIMAGE_PROC*)glextGetProc("glInvalidateTexSubImage");
    glpfInvalidateTexImage = (PFNGLINVALIDATETEXIMAGE_PROC*)glextGetProc("glInvalidateTexImage");
    glpfInvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATA_PROC*)glextGetProc("glInvalidateBufferSubData");
    glpfInvalidateBufferData = (PFNGLINVALIDATEBUFFERDATA_PROC*)glextGetProc("glInvalidateBufferData");
    glpfInvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFER_PROC*)glextGetProc("glInvalidateFramebuffer");
    glpfInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFER_PROC*)glextGetProc("glInvalidateSubFramebuffer");


    /* GL_ARB_map_buffer_range */

    glpfMapBufferRange = (PFNGLMAPBUFFERRANGE_PROC*)glextGetProc("glMapBufferRange");
    glpfFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGE_PROC*)glextGetProc("glFlushMappedBufferRange");


    /* GL_ARB_multi_draw_indirect */

    glpfMultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECT_PROC*)glextGetProc("glMultiDrawArraysIndirect");
    glpfMultiDrawElementsIndirect = (PFNGLMULTIDRAWELEMENTSINDIRECT_PROC*)glextGetProc("glMultiDrawElementsIndirect");


    /* GL_ARB_program_interface_query */

    glpfGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIV_PROC*)glextGetProc("glGetProgramInterfaceiv");
    glpfGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEX_PROC*)glextGetProc("glGetProgramResourceIndex");
    glpfGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAME_PROC*)glextGetProc("glGetProgramResourceName");
    glpfGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIV_PROC*)glextGetProc("glGetProgramResourceiv");
    glpfGetProgramResourceLocation = (PFNGLGETPROGRAMRESOURCELOCATION_PROC*)glextGetProc("glGetProgramResourceLocation");
    glpfGetProgramResourceLocationIndex = (PFNGLGETPROGRAMRESOURCELOCATIONINDEX_PROC*)glextGetProc("glGetProgramResourceLocationIndex");


    /* GL_ARB_provoking_vertex */

    glpfProvokingVertex = (PFNGLPROVOKINGVERTEX_PROC*)glextGetProc("glProvokingVertex");


    /* GL_ARB_sampler_objects */

    glpfGenSamplers = (PFNGLGENSAMPLERS_PROC*)glextGetProc("glGenSamplers");
    glpfDeleteSamplers = (PFNGLDELETESAMPLERS_PROC*)glextGetProc("glDeleteSamplers");
    glpfIsSampler = (PFNGLISSAMPLER_PROC*)glextGetProc("glIsSampler");
    glpfBindSampler = (PFNGLBINDSAMPLER_PROC*)glextGetProc("glBindSampler");
    glpfSamplerParameteri = (PFNGLSAMPLERPARAMETERI_PROC*)glextGetProc("glSamplerParameteri");
    glpfSamplerParameteriv = (PFNGLSAMPLERPARAMETERIV_PROC*)glextGetProc("glSamplerParameteriv");
    glpfSamplerParameterf = (PFNGLSAMPLERPARAMETERF_PROC*)glextGetProc("glSamplerParameterf");
    glpfSamplerParameterfv = (PFNGLSAMPLERPARAMETERFV_PROC*)glextGetProc("glSamplerParameterfv");
    glpfSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIV_PROC*)glextGetProc("glSamplerParameterIiv");
    glpfSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIV_PROC*)glextGetProc("glSamplerParameterIuiv");
    glpfGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIV_PROC*)glextGetProc("glGetSamplerParameteriv");
    glpfGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIV_PROC*)glextGetProc("glGetSamplerParameterIiv");
    glpfGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFV_PROC*)glextGetProc("glGetSamplerParameterfv");
    glpfGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIV_PROC*)glextGetProc("glGetSamplerParameterIuiv");


    /* GL_ARB_separate_shader_objects */

    glpfUseProgramStages = (PFNGLUSEPROGRAMSTAGES_PROC*)glextGetProc("glUseProgramStages");
    glpfActiveShaderProgram = (PFNGLACTIVESHADERPROGRAM_PROC*)glextGetProc("glActiveShaderProgram");
    glpfCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMV_PROC*)glextGetProc("glCreateShaderProgramv");
    glpfBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINE_PROC*)glextGetProc("glBindProgramPipeline");
    glpfDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINES_PROC*)glextGetProc("glDeleteProgramPipelines");
    glpfGenProgramPipelines = (PFNGLGENPROGRAMPIPELINES_PROC*)glextGetProc("glGenProgramPipelines");
    glpfIsProgramPipeline = (PFNGLISPROGRAMPIPELINE_PROC*)glextGetProc("glIsProgramPipeline");
    glpfGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIV_PROC*)glextGetProc("glGetProgramPipelineiv");
    glpfProgramUniform1i = (PFNGLPROGRAMUNIFORM1I_PROC*)glextGetProc("glProgramUniform1i");
    glpfProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IV_PROC*)glextGetProc("glProgramUniform1iv");
    glpfProgramUniform1f = (PFNGLPROGRAMUNIFORM1F_PROC*)glextGetProc("glProgramUniform1f");
    glpfProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FV_PROC*)glextGetProc("glProgramUniform1fv");
    glpfProgramUniform1d = (PFNGLPROGRAMUNIFORM1D_PROC*)glextGetProc("glProgramUniform1d");
    glpfProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DV_PROC*)glextGetProc("glProgramUniform1dv");
    glpfProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UI_PROC*)glextGetProc("glProgramUniform1ui");
    glpfProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIV_PROC*)glextGetProc("glProgramUniform1uiv");
    glpfProgramUniform2i = (PFNGLPROGRAMUNIFORM2I_PROC*)glextGetProc("glProgramUniform2i");
    glpfProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IV_PROC*)glextGetProc("glProgramUniform2iv");
    glpfProgramUniform2f = (PFNGLPROGRAMUNIFORM2F_PROC*)glextGetProc("glProgramUniform2f");
    glpfProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FV_PROC*)glextGetProc("glProgramUniform2fv");
    glpfProgramUniform2d = (PFNGLPROGRAMUNIFORM2D_PROC*)glextGetProc("glProgramUniform2d");
    glpfProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DV_PROC*)glextGetProc("glProgramUniform2dv");
    glpfProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UI_PROC*)glextGetProc("glProgramUniform2ui");
    glpfProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIV_PROC*)glextGetProc("glProgramUniform2uiv");
    glpfProgramUniform3i = (PFNGLPROGRAMUNIFORM3I_PROC*)glextGetProc("glProgramUniform3i");
    glpfProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IV_PROC*)glextGetProc("glProgramUniform3iv");
    glpfProgramUniform3f = (PFNGLPROGRAMUNIFORM3F_PROC*)glextGetProc("glProgramUniform3f");
    glpfProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FV_PROC*)glextGetProc("glProgramUniform3fv");
    glpfProgramUniform3d = (PFNGLPROGRAMUNIFORM3D_PROC*)glextGetProc("glProgramUniform3d");
    glpfProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DV_PROC*)glextGetProc("glProgramUniform3dv");
    glpfProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UI_PROC*)glextGetProc("glProgramUniform3ui");
    glpfProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIV_PROC*)glextGetProc("glProgramUniform3uiv");
    glpfProgramUniform4i = (PFNGLPROGRAMUNIFORM4I_PROC*)glextGetProc("glProgramUniform4i");
    glpfProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IV_PROC*)glextGetProc("glProgramUniform4iv");
    glpfProgramUniform4f = (PFNGLPROGRAMUNIFORM4F_PROC*)glextGetProc("glProgramUniform4f");
    glpfProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FV_PROC*)glextGetProc("glProgramUniform4fv");
    glpfProgramUniform4d = (PFNGLPROGRAMUNIFORM4D_PROC*)glextGetProc("glProgramUniform4d");
    glpfProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DV_PROC*)glextGetProc("glProgramUniform4dv");
    glpfProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UI_PROC*)glextGetProc("glProgramUniform4ui");
    glpfProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIV_PROC*)glextGetProc("glProgramUniform4uiv");
    glpfProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FV_PROC*)glextGetProc("glProgramUniformMatrix2fv");
    glpfProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FV_PROC*)glextGetProc("glProgramUniformMatrix3fv");
    glpfProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FV_PROC*)glextGetProc("glProgramUniformMatrix4fv");
    glpfProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DV_PROC*)glextGetProc("glProgramUniformMatrix2dv");
    glpfProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DV_PROC*)glextGetProc("glProgramUniformMatrix3dv");
    glpfProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DV_PROC*)glextGetProc("glProgramUniformMatrix4dv");
    glpfProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FV_PROC*)glextGetProc("glProgramUniformMatrix2x3fv");
    glpfProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FV_PROC*)glextGetProc("glProgramUniformMatrix3x2fv");
    glpfProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FV_PROC*)glextGetProc("glProgramUniformMatrix2x4fv");
    glpfProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FV_PROC*)glextGetProc("glProgramUniformMatrix4x2fv");
    glpfProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FV_PROC*)glextGetProc("glProgramUniformMatrix3x4fv");
    glpfProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FV_PROC*)glextGetProc("glProgramUniformMatrix4x3fv");
    glpfProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DV_PROC*)glextGetProc("glProgramUniformMatrix2x3dv");
    glpfProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DV_PROC*)glextGetProc("glProgramUniformMatrix3x2dv");
    glpfProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DV_PROC*)glextGetProc("glProgramUniformMatrix2x4dv");
    glpfProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DV_PROC*)glextGetProc("glProgramUniformMatrix4x2dv");
    glpfProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DV_PROC*)glextGetProc("glProgramUniformMatrix3x4dv");
    glpfProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DV_PROC*)glextGetProc("glProgramUniformMatrix4x3dv");
    glpfValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINE_PROC*)glextGetProc("glValidateProgramPipeline");
    glpfGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOG_PROC*)glextGetProc("glGetProgramPipelineInfoLog");


    /* GL_ARB_shader_atomic_counters */

    glpfGetActiveAtomicCounterBufferiv = (PFNGLGETACTIVEATOMICCOUNTERBUFFERIV_PROC*)glextGetProc("glGetActiveAtomicCounterBufferiv");


    /* GL_ARB_shader_image_load_store */

    glpfBindImageTexture = (PFNGLBINDIMAGETEXTURE_PROC*)glextGetProc("glBindImageTexture");
    glpfMemoryBarrier = (PFNGLMEMORYBARRIER_PROC*)glextGetProc("glMemoryBarrier");


    /* GL_ARB_shader_storage_buffer_object */

    glpfShaderStorageBlockBinding = (PFNGLSHADERSTORAGEBLOCKBINDING_PROC*)glextGetProc("glShaderStorageBlockBinding");


    /* GL_ARB_shader_subroutine */

    glpfGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATION_PROC*)glextGetProc("glGetSubroutineUniformLocation");
    glpfGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEX_PROC*)glextGetProc("glGetSubroutineIndex");
    glpfGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIV_PROC*)glextGetProc("glGetActiveSubroutineUniformiv");
    glpfGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAME_PROC*)glextGetProc("glGetActiveSubroutineUniformName");
    glpfGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAME_PROC*)glextGetProc("glGetActiveSubroutineName");
    glpfUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIV_PROC*)glextGetProc("glUniformSubroutinesuiv");
    glpfGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIV_PROC*)glextGetProc("glGetUniformSubroutineuiv");
    glpfGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIV_PROC*)glextGetProc("glGetProgramStageiv");


    /* GL_ARB_sync */

    glpfFenceSync = (PFNGLFENCESYNC_PROC*)glextGetProc("glFenceSync");
    glpfIsSync = (PFNGLISSYNC_PROC*)glextGetProc("glIsSync");
    glpfDeleteSync = (PFNGLDELETESYNC_PROC*)glextGetProc("glDeleteSync");
    glpfClientWaitSync = (PFNGLCLIENTWAITSYNC_PROC*)glextGetProc("glClientWaitSync");
    glpfWaitSync = (PFNGLWAITSYNC_PROC*)glextGetProc("glWaitSync");
    glpfGetInteger64v = (PFNGLGETINTEGER64V_PROC*)glextGetProc("glGetInteger64v");
    glpfGetSynciv = (PFNGLGETSYNCIV_PROC*)glextGetProc("glGetSynciv");


    /* GL_ARB_tessellation_shader */

    glpfPatchParameteri = (PFNGLPATCHPARAMETERI_PROC*)glextGetProc("glPatchParameteri");
    glpfPatchParameterfv = (PFNGLPATCHPARAMETERFV_PROC*)glextGetProc("glPatchParameterfv");


    /* GL_ARB_texture_buffer_range */

    glpfTexBufferRange = (PFNGLTEXBUFFERRANGE_PROC*)glextGetProc("glTexBufferRange");
    glpfTextureBufferRangeEXT = (PFNGLTEXTUREBUFFERRANGEEXT_PROC*)glextGetProc("glTextureBufferRangeEXT");


    /* GL_ARB_texture_multisample */

    glpfTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLE_PROC*)glextGetProc("glTexImage2DMultisample");
    glpfTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLE_PROC*)glextGetProc("glTexImage3DMultisample");
    glpfGetMultisamplefv = (PFNGLGETMULTISAMPLEFV_PROC*)glextGetProc("glGetMultisamplefv");
    glpfSampleMaski = (PFNGLSAMPLEMASKI_PROC*)glextGetProc("glSampleMaski");


    /* GL_ARB_texture_storage */

    glpfTexStorage1D = (PFNGLTEXSTORAGE1D_PROC*)glextGetProc("glTexStorage1D");
    glpfTexStorage2D = (PFNGLTEXSTORAGE2D_PROC*)glextGetProc("glTexStorage2D");
    glpfTexStorage3D = (PFNGLTEXSTORAGE3D_PROC*)glextGetProc("glTexStorage3D");
    glpfTextureStorage1DEXT = (PFNGLTEXTURESTORAGE1DEXT_PROC*)glextGetProc("glTextureStorage1DEXT");
    glpfTextureStorage2DEXT = (PFNGLTEXTURESTORAGE2DEXT_PROC*)glextGetProc("glTextureStorage2DEXT");
    glpfTextureStorage3DEXT = (PFNGLTEXTURESTORAGE3DEXT_PROC*)glextGetProc("glTextureStorage3DEXT");


    /* GL_ARB_texture_storage_multisample */

    glpfTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLE_PROC*)glextGetProc("glTexStorage2DMultisample");
    glpfTexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLE_PROC*)glextGetProc("glTexStorage3DMultisample");
    glpfTextureStorage2DMultisampleEXT = (PFNGLTEXTURESTORAGE2DMULTISAMPLEEXT_PROC*)glextGetProc("glTextureStorage2DMultisampleEXT");
    glpfTextureStorage3DMultisampleEXT = (PFNGLTEXTURESTORAGE3DMULTISAMPLEEXT_PROC*)glextGetProc("glTextureStorage3DMultisampleEXT");


    /* GL_ARB_texture_view */

    glpfTextureView = (PFNGLTEXTUREVIEW_PROC*)glextGetProc("glTextureView");


    /* GL_ARB_timer_query */

    glpfQueryCounter = (PFNGLQUERYCOUNTER_PROC*)glextGetProc("glQueryCounter");
    glpfGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64V_PROC*)glextGetProc("glGetQueryObjecti64v");
    glpfGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64V_PROC*)glextGetProc("glGetQueryObjectui64v");


    /* GL_ARB_transform_feedback2 */

    glpfBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACK_PROC*)glextGetProc("glBindTransformFeedback");
    glpfDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKS_PROC*)glextGetProc("glDeleteTransformFeedbacks");
    glpfGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKS_PROC*)glextGetProc("glGenTransformFeedbacks");
    glpfIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACK_PROC*)glextGetProc("glIsTransformFeedback");
    glpfPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACK_PROC*)glextGetProc("glPauseTransformFeedback");
    glpfResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACK_PROC*)glextGetProc("glResumeTransformFeedback");
    glpfDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACK_PROC*)glextGetProc("glDrawTransformFeedback");


    /* GL_ARB_transform_feedback3 */

    glpfDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAM_PROC*)glextGetProc("glDrawTransformFeedbackStream");
    glpfBeginQueryIndexed = (PFNGLBEGINQUERYINDEXED_PROC*)glextGetProc("glBeginQueryIndexed");
    glpfEndQueryIndexed = (PFNGLENDQUERYINDEXED_PROC*)glextGetProc("glEndQueryIndexed");
    glpfGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIV_PROC*)glextGetProc("glGetQueryIndexediv");


    /* GL_ARB_transform_feedback_instanced */

    glpfDrawTransformFeedbackInstanced = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCED_PROC*)glextGetProc("glDrawTransformFeedbackInstanced");
    glpfDrawTransformFeedbackStreamInstanced = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCED_PROC*)glextGetProc("glDrawTransformFeedbackStreamInstanced");


    /* GL_ARB_uniform_buffer_object */

    glpfGetUniformIndices = (PFNGLGETUNIFORMINDICES_PROC*)glextGetProc("glGetUniformIndices");
    glpfGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIV_PROC*)glextGetProc("glGetActiveUniformsiv");
    glpfGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAME_PROC*)glextGetProc("glGetActiveUniformName");
    glpfGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEX_PROC*)glextGetProc("glGetUniformBlockIndex");
    glpfGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIV_PROC*)glextGetProc("glGetActiveUniformBlockiv");
    glpfGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAME_PROC*)glextGetProc("glGetActiveUniformBlockName");
    glpfUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDING_PROC*)glextGetProc("glUniformBlockBinding");


    /* GL_ARB_vertex_array_object */

    glpfBindVertexArray = (PFNGLBINDVERTEXARRAY_PROC*)glextGetProc("glBindVertexArray");
    glpfDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYS_PROC*)glextGetProc("glDeleteVertexArrays");
    glpfGenVertexArrays = (PFNGLGENVERTEXARRAYS_PROC*)glextGetProc("glGenVertexArrays");
    glpfIsVertexArray = (PFNGLISVERTEXARRAY_PROC*)glextGetProc("glIsVertexArray");


    /* GL_ARB_vertex_attrib_64bit */

    glpfVertexAttribL1d = (PFNGLVERTEXATTRIBL1D_PROC*)glextGetProc("glVertexAttribL1d");
    glpfVertexAttribL2d = (PFNGLVERTEXATTRIBL2D_PROC*)glextGetProc("glVertexAttribL2d");
    glpfVertexAttribL3d = (PFNGLVERTEXATTRIBL3D_PROC*)glextGetProc("glVertexAttribL3d");
    glpfVertexAttribL4d = (PFNGLVERTEXATTRIBL4D_PROC*)glextGetProc("glVertexAttribL4d");
    glpfVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DV_PROC*)glextGetProc("glVertexAttribL1dv");
    glpfVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DV_PROC*)glextGetProc("glVertexAttribL2dv");
    glpfVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DV_PROC*)glextGetProc("glVertexAttribL3dv");
    glpfVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DV_PROC*)glextGetProc("glVertexAttribL4dv");
    glpfVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTER_PROC*)glextGetProc("glVertexAttribLPointer");
    glpfGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDV_PROC*)glextGetProc("glGetVertexAttribLdv");


    /* GL_ARB_vertex_attrib_binding */

    glpfBindVertexBuffer = (PFNGLBINDVERTEXBUFFER_PROC*)glextGetProc("glBindVertexBuffer");
    glpfVertexAttribFormat = (PFNGLVERTEXATTRIBFORMAT_PROC*)glextGetProc("glVertexAttribFormat");
    glpfVertexAttribIFormat = (PFNGLVERTEXATTRIBIFORMAT_PROC*)glextGetProc("glVertexAttribIFormat");
    glpfVertexAttribLFormat = (PFNGLVERTEXATTRIBLFORMAT_PROC*)glextGetProc("glVertexAttribLFormat");
    glpfVertexAttribBinding = (PFNGLVERTEXATTRIBBINDING_PROC*)glextGetProc("glVertexAttribBinding");
    glpfVertexBindingDivisor = (PFNGLVERTEXBINDINGDIVISOR_PROC*)glextGetProc("glVertexBindingDivisor");
    glpfVertexArrayBindVertexBufferEXT = (PFNGLVERTEXARRAYBINDVERTEXBUFFEREXT_PROC*)glextGetProc("glVertexArrayBindVertexBufferEXT");
    glpfVertexArrayVertexAttribFormatEXT = (PFNGLVERTEXARRAYVERTEXATTRIBFORMATEXT_PROC*)glextGetProc("glVertexArrayVertexAttribFormatEXT");
    glpfVertexArrayVertexAttribIFormatEXT = (PFNGLVERTEXARRAYVERTEXATTRIBIFORMATEXT_PROC*)glextGetProc("glVertexArrayVertexAttribIFormatEXT");
    glpfVertexArrayVertexAttribLFormatEXT = (PFNGLVERTEXARRAYVERTEXATTRIBLFORMATEXT_PROC*)glextGetProc("glVertexArrayVertexAttribLFormatEXT");
    glpfVertexArrayVertexAttribBindingEXT = (PFNGLVERTEXARRAYVERTEXATTRIBBINDINGEXT_PROC*)glextGetProc("glVertexArrayVertexAttribBindingEXT");
    glpfVertexArrayVertexBindingDivisorEXT = (PFNGLVERTEXARRAYVERTEXBINDINGDIVISOREXT_PROC*)glextGetProc("glVertexArrayVertexBindingDivisorEXT");


    /* GL_ARB_vertex_type_2_10_10_10_rev */

    glpfVertexP2ui = (PFNGLVERTEXP2UI_PROC*)glextGetProc("glVertexP2ui");
    glpfVertexP2uiv = (PFNGLVERTEXP2UIV_PROC*)glextGetProc("glVertexP2uiv");
    glpfVertexP3ui = (PFNGLVERTEXP3UI_PROC*)glextGetProc("glVertexP3ui");
    glpfVertexP3uiv = (PFNGLVERTEXP3UIV_PROC*)glextGetProc("glVertexP3uiv");
    glpfVertexP4ui = (PFNGLVERTEXP4UI_PROC*)glextGetProc("glVertexP4ui");
    glpfVertexP4uiv = (PFNGLVERTEXP4UIV_PROC*)glextGetProc("glVertexP4uiv");
    glpfTexCoordP1ui = (PFNGLTEXCOORDP1UI_PROC*)glextGetProc("glTexCoordP1ui");
    glpfTexCoordP1uiv = (PFNGLTEXCOORDP1UIV_PROC*)glextGetProc("glTexCoordP1uiv");
    glpfTexCoordP2ui = (PFNGLTEXCOORDP2UI_PROC*)glextGetProc("glTexCoordP2ui");
    glpfTexCoordP2uiv = (PFNGLTEXCOORDP2UIV_PROC*)glextGetProc("glTexCoordP2uiv");
    glpfTexCoordP3ui = (PFNGLTEXCOORDP3UI_PROC*)glextGetProc("glTexCoordP3ui");
    glpfTexCoordP3uiv = (PFNGLTEXCOORDP3UIV_PROC*)glextGetProc("glTexCoordP3uiv");
    glpfTexCoordP4ui = (PFNGLTEXCOORDP4UI_PROC*)glextGetProc("glTexCoordP4ui");
    glpfTexCoordP4uiv = (PFNGLTEXCOORDP4UIV_PROC*)glextGetProc("glTexCoordP4uiv");
    glpfMultiTexCoordP1ui = (PFNGLMULTITEXCOORDP1UI_PROC*)glextGetProc("glMultiTexCoordP1ui");
    glpfMultiTexCoordP1uiv = (PFNGLMULTITEXCOORDP1UIV_PROC*)glextGetProc("glMultiTexCoordP1uiv");
    glpfMultiTexCoordP2ui = (PFNGLMULTITEXCOORDP2UI_PROC*)glextGetProc("glMultiTexCoordP2ui");
    glpfMultiTexCoordP2uiv = (PFNGLMULTITEXCOORDP2UIV_PROC*)glextGetProc("glMultiTexCoordP2uiv");
    glpfMultiTexCoordP3ui = (PFNGLMULTITEXCOORDP3UI_PROC*)glextGetProc("glMultiTexCoordP3ui");
    glpfMultiTexCoordP3uiv = (PFNGLMULTITEXCOORDP3UIV_PROC*)glextGetProc("glMultiTexCoordP3uiv");
    glpfMultiTexCoordP4ui = (PFNGLMULTITEXCOORDP4UI_PROC*)glextGetProc("glMultiTexCoordP4ui");
    glpfMultiTexCoordP4uiv = (PFNGLMULTITEXCOORDP4UIV_PROC*)glextGetProc("glMultiTexCoordP4uiv");
    glpfNormalP3ui = (PFNGLNORMALP3UI_PROC*)glextGetProc("glNormalP3ui");
    glpfNormalP3uiv = (PFNGLNORMALP3UIV_PROC*)glextGetProc("glNormalP3uiv");
    glpfColorP3ui = (PFNGLCOLORP3UI_PROC*)glextGetProc("glColorP3ui");
    glpfColorP3uiv = (PFNGLCOLORP3UIV_PROC*)glextGetProc("glColorP3uiv");
    glpfColorP4ui = (PFNGLCOLORP4UI_PROC*)glextGetProc("glColorP4ui");
    glpfColorP4uiv = (PFNGLCOLORP4UIV_PROC*)glextGetProc("glColorP4uiv");
    glpfSecondaryColorP3ui = (PFNGLSECONDARYCOLORP3UI_PROC*)glextGetProc("glSecondaryColorP3ui");
    glpfSecondaryColorP3uiv = (PFNGLSECONDARYCOLORP3UIV_PROC*)glextGetProc("glSecondaryColorP3uiv");
    glpfVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UI_PROC*)glextGetProc("glVertexAttribP1ui");
    glpfVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIV_PROC*)glextGetProc("glVertexAttribP1uiv");
    glpfVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UI_PROC*)glextGetProc("glVertexAttribP2ui");
    glpfVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIV_PROC*)glextGetProc("glVertexAttribP2uiv");
    glpfVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UI_PROC*)glextGetProc("glVertexAttribP3ui");
    glpfVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIV_PROC*)glextGetProc("glVertexAttribP3uiv");
    glpfVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UI_PROC*)glextGetProc("glVertexAttribP4ui");
    glpfVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIV_PROC*)glextGetProc("glVertexAttribP4uiv");


    /* GL_ARB_viewport_array */

    glpfViewportArrayv = (PFNGLVIEWPORTARRAYV_PROC*)glextGetProc("glViewportArrayv");
    glpfViewportIndexedf = (PFNGLVIEWPORTINDEXEDF_PROC*)glextGetProc("glViewportIndexedf");
    glpfViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFV_PROC*)glextGetProc("glViewportIndexedfv");
    glpfScissorArrayv = (PFNGLSCISSORARRAYV_PROC*)glextGetProc("glScissorArrayv");
    glpfScissorIndexed = (PFNGLSCISSORINDEXED_PROC*)glextGetProc("glScissorIndexed");
    glpfScissorIndexedv = (PFNGLSCISSORINDEXEDV_PROC*)glextGetProc("glScissorIndexedv");
    glpfDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYV_PROC*)glextGetProc("glDepthRangeArrayv");
    glpfDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXED_PROC*)glextGetProc("glDepthRangeIndexed");
    glpfGetFloati_v = (PFNGLGETFLOATI_V_PROC*)glextGetProc("glGetFloati_v");
    glpfGetDoublei_v = (PFNGLGETDOUBLEI_V_PROC*)glextGetProc("glGetDoublei_v");


    /* GL_EXT_direct_state_access */

    glpfClientAttribDefaultEXT = (PFNGLCLIENTATTRIBDEFAULTEXT_PROC*)glextGetProc("glClientAttribDefaultEXT");
    glpfPushClientAttribDefaultEXT = (PFNGLPUSHCLIENTATTRIBDEFAULTEXT_PROC*)glextGetProc("glPushClientAttribDefaultEXT");
    glpfMatrixLoadfEXT = (PFNGLMATRIXLOADFEXT_PROC*)glextGetProc("glMatrixLoadfEXT");
    glpfMatrixLoaddEXT = (PFNGLMATRIXLOADDEXT_PROC*)glextGetProc("glMatrixLoaddEXT");
    glpfMatrixMultfEXT = (PFNGLMATRIXMULTFEXT_PROC*)glextGetProc("glMatrixMultfEXT");
    glpfMatrixMultdEXT = (PFNGLMATRIXMULTDEXT_PROC*)glextGetProc("glMatrixMultdEXT");
    glpfMatrixLoadIdentityEXT = (PFNGLMATRIXLOADIDENTITYEXT_PROC*)glextGetProc("glMatrixLoadIdentityEXT");
    glpfMatrixRotatefEXT = (PFNGLMATRIXROTATEFEXT_PROC*)glextGetProc("glMatrixRotatefEXT");
    glpfMatrixRotatedEXT = (PFNGLMATRIXROTATEDEXT_PROC*)glextGetProc("glMatrixRotatedEXT");
    glpfMatrixScalefEXT = (PFNGLMATRIXSCALEFEXT_PROC*)glextGetProc("glMatrixScalefEXT");
    glpfMatrixScaledEXT = (PFNGLMATRIXSCALEDEXT_PROC*)glextGetProc("glMatrixScaledEXT");
    glpfMatrixTranslatefEXT = (PFNGLMATRIXTRANSLATEFEXT_PROC*)glextGetProc("glMatrixTranslatefEXT");
    glpfMatrixTranslatedEXT = (PFNGLMATRIXTRANSLATEDEXT_PROC*)glextGetProc("glMatrixTranslatedEXT");
    glpfMatrixFrustumEXT = (PFNGLMATRIXFRUSTUMEXT_PROC*)glextGetProc("glMatrixFrustumEXT");
    glpfMatrixOrthoEXT = (PFNGLMATRIXORTHOEXT_PROC*)glextGetProc("glMatrixOrthoEXT");
    glpfMatrixPopEXT = (PFNGLMATRIXPOPEXT_PROC*)glextGetProc("glMatrixPopEXT");
    glpfMatrixPushEXT = (PFNGLMATRIXPUSHEXT_PROC*)glextGetProc("glMatrixPushEXT");
    glpfMatrixLoadTransposefEXT = (PFNGLMATRIXLOADTRANSPOSEFEXT_PROC*)glextGetProc("glMatrixLoadTransposefEXT");
    glpfMatrixLoadTransposedEXT = (PFNGLMATRIXLOADTRANSPOSEDEXT_PROC*)glextGetProc("glMatrixLoadTransposedEXT");
    glpfMatrixMultTransposefEXT = (PFNGLMATRIXMULTTRANSPOSEFEXT_PROC*)glextGetProc("glMatrixMultTransposefEXT");
    glpfMatrixMultTransposedEXT = (PFNGLMATRIXMULTTRANSPOSEDEXT_PROC*)glextGetProc("glMatrixMultTransposedEXT");
    glpfTextureParameterfEXT = (PFNGLTEXTUREPARAMETERFEXT_PROC*)glextGetProc("glTextureParameterfEXT");
    glpfTextureParameterfvEXT = (PFNGLTEXTUREPARAMETERFVEXT_PROC*)glextGetProc("glTextureParameterfvEXT");
    glpfTextureParameteriEXT = (PFNGLTEXTUREPARAMETERIEXT_PROC*)glextGetProc("glTextureParameteriEXT");
    glpfTextureParameterivEXT = (PFNGLTEXTUREPARAMETERIVEXT_PROC*)glextGetProc("glTextureParameterivEXT");
    glpfTextureImage1DEXT = (PFNGLTEXTUREIMAGE1DEXT_PROC*)glextGetProc("glTextureImage1DEXT");
    glpfTextureImage2DEXT = (PFNGLTEXTUREIMAGE2DEXT_PROC*)glextGetProc("glTextureImage2DEXT");
    glpfTextureSubImage1DEXT = (PFNGLTEXTURESUBIMAGE1DEXT_PROC*)glextGetProc("glTextureSubImage1DEXT");
    glpfTextureSubImage2DEXT = (PFNGLTEXTURESUBIMAGE2DEXT_PROC*)glextGetProc("glTextureSubImage2DEXT");
    glpfCopyTextureImage1DEXT = (PFNGLCOPYTEXTUREIMAGE1DEXT_PROC*)glextGetProc("glCopyTextureImage1DEXT");
    glpfCopyTextureImage2DEXT = (PFNGLCOPYTEXTUREIMAGE2DEXT_PROC*)glextGetProc("glCopyTextureImage2DEXT");
    glpfCopyTextureSubImage1DEXT = (PFNGLCOPYTEXTURESUBIMAGE1DEXT_PROC*)glextGetProc("glCopyTextureSubImage1DEXT");
    glpfCopyTextureSubImage2DEXT = (PFNGLCOPYTEXTURESUBIMAGE2DEXT_PROC*)glextGetProc("glCopyTextureSubImage2DEXT");
    glpfGetTextureImageEXT = (PFNGLGETTEXTUREIMAGEEXT_PROC*)glextGetProc("glGetTextureImageEXT");
    glpfGetTextureParameterfvEXT = (PFNGLGETTEXTUREPARAMETERFVEXT_PROC*)glextGetProc("glGetTextureParameterfvEXT");
    glpfGetTextureParameterivEXT = (PFNGLGETTEXTUREPARAMETERIVEXT_PROC*)glextGetProc("glGetTextureParameterivEXT");
    glpfGetTextureLevelParameterfvEXT = (PFNGLGETTEXTURELEVELPARAMETERFVEXT_PROC*)glextGetProc("glGetTextureLevelParameterfvEXT");
    glpfGetTextureLevelParameterivEXT = (PFNGLGETTEXTURELEVELPARAMETERIVEXT_PROC*)glextGetProc("glGetTextureLevelParameterivEXT");
    glpfTextureImage3DEXT = (PFNGLTEXTUREIMAGE3DEXT_PROC*)glextGetProc("glTextureImage3DEXT");
    glpfTextureSubImage3DEXT = (PFNGLTEXTURESUBIMAGE3DEXT_PROC*)glextGetProc("glTextureSubImage3DEXT");
    glpfCopyTextureSubImage3DEXT = (PFNGLCOPYTEXTURESUBIMAGE3DEXT_PROC*)glextGetProc("glCopyTextureSubImage3DEXT");
    glpfMultiTexParameterfEXT = (PFNGLMULTITEXPARAMETERFEXT_PROC*)glextGetProc("glMultiTexParameterfEXT");
    glpfMultiTexParameterfvEXT = (PFNGLMULTITEXPARAMETERFVEXT_PROC*)glextGetProc("glMultiTexParameterfvEXT");
    glpfMultiTexParameteriEXT = (PFNGLMULTITEXPARAMETERIEXT_PROC*)glextGetProc("glMultiTexParameteriEXT");
    glpfMultiTexParameterivEXT = (PFNGLMULTITEXPARAMETERIVEXT_PROC*)glextGetProc("glMultiTexParameterivEXT");
    glpfMultiTexImage1DEXT = (PFNGLMULTITEXIMAGE1DEXT_PROC*)glextGetProc("glMultiTexImage1DEXT");
    glpfMultiTexImage2DEXT = (PFNGLMULTITEXIMAGE2DEXT_PROC*)glextGetProc("glMultiTexImage2DEXT");
    glpfMultiTexSubImage1DEXT = (PFNGLMULTITEXSUBIMAGE1DEXT_PROC*)glextGetProc("glMultiTexSubImage1DEXT");
    glpfMultiTexSubImage2DEXT = (PFNGLMULTITEXSUBIMAGE2DEXT_PROC*)glextGetProc("glMultiTexSubImage2DEXT");
    glpfCopyMultiTexImage1DEXT = (PFNGLCOPYMULTITEXIMAGE1DEXT_PROC*)glextGetProc("glCopyMultiTexImage1DEXT");
    glpfCopyMultiTexImage2DEXT = (PFNGLCOPYMULTITEXIMAGE2DEXT_PROC*)glextGetProc("glCopyMultiTexImage2DEXT");
    glpfCopyMultiTexSubImage1DEXT = (PFNGLCOPYMULTITEXSUBIMAGE1DEXT_PROC*)glextGetProc("glCopyMultiTexSubImage1DEXT");
    glpfCopyMultiTexSubImage2DEXT = (PFNGLCOPYMULTITEXSUBIMAGE2DEXT_PROC*)glextGetProc("glCopyMultiTexSubImage2DEXT");
    glpfGetMultiTexImageEXT = (PFNGLGETMULTITEXIMAGEEXT_PROC*)glextGetProc("glGetMultiTexImageEXT");
    glpfGetMultiTexParameterfvEXT = (PFNGLGETMULTITEXPARAMETERFVEXT_PROC*)glextGetProc("glGetMultiTexParameterfvEXT");
    glpfGetMultiTexParameterivEXT = (PFNGLGETMULTITEXPARAMETERIVEXT_PROC*)glextGetProc("glGetMultiTexParameterivEXT");
    glpfGetMultiTexLevelParameterfvEXT = (PFNGLGETMULTITEXLEVELPARAMETERFVEXT_PROC*)glextGetProc("glGetMultiTexLevelParameterfvEXT");
    glpfGetMultiTexLevelParameterivEXT = (PFNGLGETMULTITEXLEVELPARAMETERIVEXT_PROC*)glextGetProc("glGetMultiTexLevelParameterivEXT");
    glpfMultiTexImage3DEXT = (PFNGLMULTITEXIMAGE3DEXT_PROC*)glextGetProc("glMultiTexImage3DEXT");
    glpfMultiTexSubImage3DEXT = (PFNGLMULTITEXSUBIMAGE3DEXT_PROC*)glextGetProc("glMultiTexSubImage3DEXT");
    glpfCopyMultiTexSubImage3DEXT = (PFNGLCOPYMULTITEXSUBIMAGE3DEXT_PROC*)glextGetProc("glCopyMultiTexSubImage3DEXT");
    glpfBindMultiTextureEXT = (PFNGLBINDMULTITEXTUREEXT_PROC*)glextGetProc("glBindMultiTextureEXT");
    glpfEnableClientStateIndexedEXT = (PFNGLENABLECLIENTSTATEINDEXEDEXT_PROC*)glextGetProc("glEnableClientStateIndexedEXT");
    glpfDisableClientStateIndexedEXT = (PFNGLDISABLECLIENTSTATEINDEXEDEXT_PROC*)glextGetProc("glDisableClientStateIndexedEXT");
    glpfMultiTexCoordPointerEXT = (PFNGLMULTITEXCOORDPOINTEREXT_PROC*)glextGetProc("glMultiTexCoordPointerEXT");
    glpfMultiTexEnvfEXT = (PFNGLMULTITEXENVFEXT_PROC*)glextGetProc("glMultiTexEnvfEXT");
    glpfMultiTexEnvfvEXT = (PFNGLMULTITEXENVFVEXT_PROC*)glextGetProc("glMultiTexEnvfvEXT");
    glpfMultiTexEnviEXT = (PFNGLMULTITEXENVIEXT_PROC*)glextGetProc("glMultiTexEnviEXT");
    glpfMultiTexEnvivEXT = (PFNGLMULTITEXENVIVEXT_PROC*)glextGetProc("glMultiTexEnvivEXT");
    glpfMultiTexGendEXT = (PFNGLMULTITEXGENDEXT_PROC*)glextGetProc("glMultiTexGendEXT");
    glpfMultiTexGendvEXT = (PFNGLMULTITEXGENDVEXT_PROC*)glextGetProc("glMultiTexGendvEXT");
    glpfMultiTexGenfEXT = (PFNGLMULTITEXGENFEXT_PROC*)glextGetProc("glMultiTexGenfEXT");
    glpfMultiTexGenfvEXT = (PFNGLMULTITEXGENFVEXT_PROC*)glextGetProc("glMultiTexGenfvEXT");
    glpfMultiTexGeniEXT = (PFNGLMULTITEXGENIEXT_PROC*)glextGetProc("glMultiTexGeniEXT");
    glpfMultiTexGenivEXT = (PFNGLMULTITEXGENIVEXT_PROC*)glextGetProc("glMultiTexGenivEXT");
    glpfGetMultiTexEnvfvEXT = (PFNGLGETMULTITEXENVFVEXT_PROC*)glextGetProc("glGetMultiTexEnvfvEXT");
    glpfGetMultiTexEnvivEXT = (PFNGLGETMULTITEXENVIVEXT_PROC*)glextGetProc("glGetMultiTexEnvivEXT");
    glpfGetMultiTexGendvEXT = (PFNGLGETMULTITEXGENDVEXT_PROC*)glextGetProc("glGetMultiTexGendvEXT");
    glpfGetMultiTexGenfvEXT = (PFNGLGETMULTITEXGENFVEXT_PROC*)glextGetProc("glGetMultiTexGenfvEXT");
    glpfGetMultiTexGenivEXT = (PFNGLGETMULTITEXGENIVEXT_PROC*)glextGetProc("glGetMultiTexGenivEXT");
    glpfGetFloatIndexedvEXT = (PFNGLGETFLOATINDEXEDVEXT_PROC*)glextGetProc("glGetFloatIndexedvEXT");
    glpfGetDoubleIndexedvEXT = (PFNGLGETDOUBLEINDEXEDVEXT_PROC*)glextGetProc("glGetDoubleIndexedvEXT");
    glpfGetPointerIndexedvEXT = (PFNGLGETPOINTERINDEXEDVEXT_PROC*)glextGetProc("glGetPointerIndexedvEXT");
    glpfCompressedTextureImage3DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE3DEXT_PROC*)glextGetProc("glCompressedTextureImage3DEXT");
    glpfCompressedTextureImage2DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE2DEXT_PROC*)glextGetProc("glCompressedTextureImage2DEXT");
    glpfCompressedTextureImage1DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE1DEXT_PROC*)glextGetProc("glCompressedTextureImage1DEXT");
    glpfCompressedTextureSubImage3DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXT_PROC*)glextGetProc("glCompressedTextureSubImage3DEXT");
    glpfCompressedTextureSubImage2DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXT_PROC*)glextGetProc("glCompressedTextureSubImage2DEXT");
    glpfCompressedTextureSubImage1DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXT_PROC*)glextGetProc("glCompressedTextureSubImage1DEXT");
    glpfGetCompressedTextureImageEXT = (PFNGLGETCOMPRESSEDTEXTUREIMAGEEXT_PROC*)glextGetProc("glGetCompressedTextureImageEXT");
    glpfCompressedMultiTexImage3DEXT = (PFNGLCOMPRESSEDMULTITEXIMAGE3DEXT_PROC*)glextGetProc("glCompressedMultiTexImage3DEXT");
    glpfCompressedMultiTexImage2DEXT = (PFNGLCOMPRESSEDMULTITEXIMAGE2DEXT_PROC*)glextGetProc("glCompressedMultiTexImage2DEXT");
    glpfCompressedMultiTexImage1DEXT = (PFNGLCOMPRESSEDMULTITEXIMAGE1DEXT_PROC*)glextGetProc("glCompressedMultiTexImage1DEXT");
    glpfCompressedMultiTexSubImage3DEXT = (PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXT_PROC*)glextGetProc("glCompressedMultiTexSubImage3DEXT");
    glpfCompressedMultiTexSubImage2DEXT = (PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXT_PROC*)glextGetProc("glCompressedMultiTexSubImage2DEXT");
    glpfCompressedMultiTexSubImage1DEXT = (PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXT_PROC*)glextGetProc("glCompressedMultiTexSubImage1DEXT");
    glpfGetCompressedMultiTexImageEXT = (PFNGLGETCOMPRESSEDMULTITEXIMAGEEXT_PROC*)glextGetProc("glGetCompressedMultiTexImageEXT");
    glpfNamedProgramStringEXT = (PFNGLNAMEDPROGRAMSTRINGEXT_PROC*)glextGetProc("glNamedProgramStringEXT");
    glpfNamedProgramLocalParameter4dEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXT_PROC*)glextGetProc("glNamedProgramLocalParameter4dEXT");
    glpfNamedProgramLocalParameter4dvEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXT_PROC*)glextGetProc("glNamedProgramLocalParameter4dvEXT");
    glpfNamedProgramLocalParameter4fEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXT_PROC*)glextGetProc("glNamedProgramLocalParameter4fEXT");
    glpfNamedProgramLocalParameter4fvEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXT_PROC*)glextGetProc("glNamedProgramLocalParameter4fvEXT");
    glpfGetNamedProgramLocalParameterdvEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXT_PROC*)glextGetProc("glGetNamedProgramLocalParameterdvEXT");
    glpfGetNamedProgramLocalParameterfvEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXT_PROC*)glextGetProc("glGetNamedProgramLocalParameterfvEXT");
    glpfGetNamedProgramivEXT = (PFNGLGETNAMEDPROGRAMIVEXT_PROC*)glextGetProc("glGetNamedProgramivEXT");
    glpfGetNamedProgramStringEXT = (PFNGLGETNAMEDPROGRAMSTRINGEXT_PROC*)glextGetProc("glGetNamedProgramStringEXT");
    glpfNamedProgramLocalParameters4fvEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXT_PROC*)glextGetProc("glNamedProgramLocalParameters4fvEXT");
    glpfNamedProgramLocalParameterI4iEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXT_PROC*)glextGetProc("glNamedProgramLocalParameterI4iEXT");
    glpfNamedProgramLocalParameterI4ivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXT_PROC*)glextGetProc("glNamedProgramLocalParameterI4ivEXT");
    glpfNamedProgramLocalParametersI4ivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXT_PROC*)glextGetProc("glNamedProgramLocalParametersI4ivEXT");
    glpfNamedProgramLocalParameterI4uiEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXT_PROC*)glextGetProc("glNamedProgramLocalParameterI4uiEXT");
    glpfNamedProgramLocalParameterI4uivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXT_PROC*)glextGetProc("glNamedProgramLocalParameterI4uivEXT");
    glpfNamedProgramLocalParametersI4uivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXT_PROC*)glextGetProc("glNamedProgramLocalParametersI4uivEXT");
    glpfGetNamedProgramLocalParameterIivEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXT_PROC*)glextGetProc("glGetNamedProgramLocalParameterIivEXT");
    glpfGetNamedProgramLocalParameterIuivEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXT_PROC*)glextGetProc("glGetNamedProgramLocalParameterIuivEXT");
    glpfTextureParameterIivEXT = (PFNGLTEXTUREPARAMETERIIVEXT_PROC*)glextGetProc("glTextureParameterIivEXT");
    glpfTextureParameterIuivEXT = (PFNGLTEXTUREPARAMETERIUIVEXT_PROC*)glextGetProc("glTextureParameterIuivEXT");
    glpfGetTextureParameterIivEXT = (PFNGLGETTEXTUREPARAMETERIIVEXT_PROC*)glextGetProc("glGetTextureParameterIivEXT");
    glpfGetTextureParameterIuivEXT = (PFNGLGETTEXTUREPARAMETERIUIVEXT_PROC*)glextGetProc("glGetTextureParameterIuivEXT");
    glpfMultiTexParameterIivEXT = (PFNGLMULTITEXPARAMETERIIVEXT_PROC*)glextGetProc("glMultiTexParameterIivEXT");
    glpfMultiTexParameterIuivEXT = (PFNGLMULTITEXPARAMETERIUIVEXT_PROC*)glextGetProc("glMultiTexParameterIuivEXT");
    glpfGetMultiTexParameterIivEXT = (PFNGLGETMULTITEXPARAMETERIIVEXT_PROC*)glextGetProc("glGetMultiTexParameterIivEXT");
    glpfGetMultiTexParameterIuivEXT = (PFNGLGETMULTITEXPARAMETERIUIVEXT_PROC*)glextGetProc("glGetMultiTexParameterIuivEXT");
    glpfProgramUniform1fEXT = (PFNGLPROGRAMUNIFORM1FEXT_PROC*)glextGetProc("glProgramUniform1fEXT");
    glpfProgramUniform2fEXT = (PFNGLPROGRAMUNIFORM2FEXT_PROC*)glextGetProc("glProgramUniform2fEXT");
    glpfProgramUniform3fEXT = (PFNGLPROGRAMUNIFORM3FEXT_PROC*)glextGetProc("glProgramUniform3fEXT");
    glpfProgramUniform4fEXT = (PFNGLPROGRAMUNIFORM4FEXT_PROC*)glextGetProc("glProgramUniform4fEXT");
    glpfProgramUniform1iEXT = (PFNGLPROGRAMUNIFORM1IEXT_PROC*)glextGetProc("glProgramUniform1iEXT");
    glpfProgramUniform2iEXT = (PFNGLPROGRAMUNIFORM2IEXT_PROC*)glextGetProc("glProgramUniform2iEXT");
    glpfProgramUniform3iEXT = (PFNGLPROGRAMUNIFORM3IEXT_PROC*)glextGetProc("glProgramUniform3iEXT");
    glpfProgramUniform4iEXT = (PFNGLPROGRAMUNIFORM4IEXT_PROC*)glextGetProc("glProgramUniform4iEXT");
    glpfProgramUniform1fvEXT = (PFNGLPROGRAMUNIFORM1FVEXT_PROC*)glextGetProc("glProgramUniform1fvEXT");
    glpfProgramUniform2fvEXT = (PFNGLPROGRAMUNIFORM2FVEXT_PROC*)glextGetProc("glProgramUniform2fvEXT");
    glpfProgramUniform3fvEXT = (PFNGLPROGRAMUNIFORM3FVEXT_PROC*)glextGetProc("glProgramUniform3fvEXT");
    glpfProgramUniform4fvEXT = (PFNGLPROGRAMUNIFORM4FVEXT_PROC*)glextGetProc("glProgramUniform4fvEXT");
    glpfProgramUniform1ivEXT = (PFNGLPROGRAMUNIFORM1IVEXT_PROC*)glextGetProc("glProgramUniform1ivEXT");
    glpfProgramUniform2ivEXT = (PFNGLPROGRAMUNIFORM2IVEXT_PROC*)glextGetProc("glProgramUniform2ivEXT");
    glpfProgramUniform3ivEXT = (PFNGLPROGRAMUNIFORM3IVEXT_PROC*)glextGetProc("glProgramUniform3ivEXT");
    glpfProgramUniform4ivEXT = (PFNGLPROGRAMUNIFORM4IVEXT_PROC*)glextGetProc("glProgramUniform4ivEXT");
    glpfProgramUniformMatrix2fvEXT = (PFNGLPROGRAMUNIFORMMATRIX2FVEXT_PROC*)glextGetProc("glProgramUniformMatrix2fvEXT");
    glpfProgramUniformMatrix3fvEXT = (PFNGLPROGRAMUNIFORMMATRIX3FVEXT_PROC*)glextGetProc("glProgramUniformMatrix3fvEXT");
    glpfProgramUniformMatrix4fvEXT = (PFNGLPROGRAMUNIFORMMATRIX4FVEXT_PROC*)glextGetProc("glProgramUniformMatrix4fvEXT");
    glpfProgramUniformMatrix2x3fvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X3FVEXT_PROC*)glextGetProc("glProgramUniformMatrix2x3fvEXT");
    glpfProgramUniformMatrix3x2fvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X2FVEXT_PROC*)glextGetProc("glProgramUniformMatrix3x2fvEXT");
    glpfProgramUniformMatrix2x4fvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X4FVEXT_PROC*)glextGetProc("glProgramUniformMatrix2x4fvEXT");
    glpfProgramUniformMatrix4x2fvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X2FVEXT_PROC*)glextGetProc("glProgramUniformMatrix4x2fvEXT");
    glpfProgramUniformMatrix3x4fvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X4FVEXT_PROC*)glextGetProc("glProgramUniformMatrix3x4fvEXT");
    glpfProgramUniformMatrix4x3fvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X3FVEXT_PROC*)glextGetProc("glProgramUniformMatrix4x3fvEXT");
    glpfProgramUniform1uiEXT = (PFNGLPROGRAMUNIFORM1UIEXT_PROC*)glextGetProc("glProgramUniform1uiEXT");
    glpfProgramUniform2uiEXT = (PFNGLPROGRAMUNIFORM2UIEXT_PROC*)glextGetProc("glProgramUniform2uiEXT");
    glpfProgramUniform3uiEXT = (PFNGLPROGRAMUNIFORM3UIEXT_PROC*)glextGetProc("glProgramUniform3uiEXT");
    glpfProgramUniform4uiEXT = (PFNGLPROGRAMUNIFORM4UIEXT_PROC*)glextGetProc("glProgramUniform4uiEXT");
    glpfProgramUniform1uivEXT = (PFNGLPROGRAMUNIFORM1UIVEXT_PROC*)glextGetProc("glProgramUniform1uivEXT");
    glpfProgramUniform2uivEXT = (PFNGLPROGRAMUNIFORM2UIVEXT_PROC*)glextGetProc("glProgramUniform2uivEXT");
    glpfProgramUniform3uivEXT = (PFNGLPROGRAMUNIFORM3UIVEXT_PROC*)glextGetProc("glProgramUniform3uivEXT");
    glpfProgramUniform4uivEXT = (PFNGLPROGRAMUNIFORM4UIVEXT_PROC*)glextGetProc("glProgramUniform4uivEXT");
    glpfNamedBufferDataEXT = (PFNGLNAMEDBUFFERDATAEXT_PROC*)glextGetProc("glNamedBufferDataEXT");
    glpfNamedBufferSubDataEXT = (PFNGLNAMEDBUFFERSUBDATAEXT_PROC*)glextGetProc("glNamedBufferSubDataEXT");
    glpfMapNamedBufferEXT = (PFNGLMAPNAMEDBUFFEREXT_PROC*)glextGetProc("glMapNamedBufferEXT");
    glpfUnmapNamedBufferEXT = (PFNGLUNMAPNAMEDBUFFEREXT_PROC*)glextGetProc("glUnmapNamedBufferEXT");
    glpfMapNamedBufferRangeEXT = (PFNGLMAPNAMEDBUFFERRANGEEXT_PROC*)glextGetProc("glMapNamedBufferRangeEXT");
    glpfFlushMappedNamedBufferRangeEXT = (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXT_PROC*)glextGetProc("glFlushMappedNamedBufferRangeEXT");
    glpfNamedCopyBufferSubDataEXT = (PFNGLNAMEDCOPYBUFFERSUBDATAEXT_PROC*)glextGetProc("glNamedCopyBufferSubDataEXT");
    glpfGetNamedBufferParameterivEXT = (PFNGLGETNAMEDBUFFERPARAMETERIVEXT_PROC*)glextGetProc("glGetNamedBufferParameterivEXT");
    glpfGetNamedBufferPointervEXT = (PFNGLGETNAMEDBUFFERPOINTERVEXT_PROC*)glextGetProc("glGetNamedBufferPointervEXT");
    glpfGetNamedBufferSubDataEXT = (PFNGLGETNAMEDBUFFERSUBDATAEXT_PROC*)glextGetProc("glGetNamedBufferSubDataEXT");
    glpfTextureBufferEXT = (PFNGLTEXTUREBUFFEREXT_PROC*)glextGetProc("glTextureBufferEXT");
    glpfMultiTexBufferEXT = (PFNGLMULTITEXBUFFEREXT_PROC*)glextGetProc("glMultiTexBufferEXT");
    glpfNamedRenderbufferStorageEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEEXT_PROC*)glextGetProc("glNamedRenderbufferStorageEXT");
    glpfGetNamedRenderbufferParameterivEXT = (PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXT_PROC*)glextGetProc("glGetNamedRenderbufferParameterivEXT");
    glpfCheckNamedFramebufferStatusEXT = (PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXT_PROC*)glextGetProc("glCheckNamedFramebufferStatusEXT");
    glpfNamedFramebufferTexture1DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXT_PROC*)glextGetProc("glNamedFramebufferTexture1DEXT");
    glpfNamedFramebufferTexture2DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXT_PROC*)glextGetProc("glNamedFramebufferTexture2DEXT");
    glpfNamedFramebufferTexture3DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXT_PROC*)glextGetProc("glNamedFramebufferTexture3DEXT");
    glpfNamedFramebufferRenderbufferEXT = (PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXT_PROC*)glextGetProc("glNamedFramebufferRenderbufferEXT");
    glpfGetNamedFramebufferAttachmentParameterivEXT = (PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXT_PROC*)glextGetProc("glGetNamedFramebufferAttachmentParameterivEXT");
    glpfGenerateTextureMipmapEXT = (PFNGLGENERATETEXTUREMIPMAPEXT_PROC*)glextGetProc("glGenerateTextureMipmapEXT");
    glpfGenerateMultiTexMipmapEXT = (PFNGLGENERATEMULTITEXMIPMAPEXT_PROC*)glextGetProc("glGenerateMultiTexMipmapEXT");
    glpfFramebufferDrawBufferEXT = (PFNGLFRAMEBUFFERDRAWBUFFEREXT_PROC*)glextGetProc("glFramebufferDrawBufferEXT");
    glpfFramebufferDrawBuffersEXT = (PFNGLFRAMEBUFFERDRAWBUFFERSEXT_PROC*)glextGetProc("glFramebufferDrawBuffersEXT");
    glpfFramebufferReadBufferEXT = (PFNGLFRAMEBUFFERREADBUFFEREXT_PROC*)glextGetProc("glFramebufferReadBufferEXT");
    glpfGetFramebufferParameterivEXT = (PFNGLGETFRAMEBUFFERPARAMETERIVEXT_PROC*)glextGetProc("glGetFramebufferParameterivEXT");
    glpfNamedRenderbufferStorageMultisampleEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXT_PROC*)glextGetProc("glNamedRenderbufferStorageMultisampleEXT");
    glpfNamedRenderbufferStorageMultisampleCoverageEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXT_PROC*)glextGetProc("glNamedRenderbufferStorageMultisampleCoverageEXT");
    glpfNamedFramebufferTextureEXT = (PFNGLNAMEDFRAMEBUFFERTEXTUREEXT_PROC*)glextGetProc("glNamedFramebufferTextureEXT");
    glpfNamedFramebufferTextureLayerEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXT_PROC*)glextGetProc("glNamedFramebufferTextureLayerEXT");
    glpfNamedFramebufferTextureFaceEXT = (PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXT_PROC*)glextGetProc("glNamedFramebufferTextureFaceEXT");
    glpfTextureRenderbufferEXT = (PFNGLTEXTURERENDERBUFFEREXT_PROC*)glextGetProc("glTextureRenderbufferEXT");
    glpfMultiTexRenderbufferEXT = (PFNGLMULTITEXRENDERBUFFEREXT_PROC*)glextGetProc("glMultiTexRenderbufferEXT");
    glpfProgramUniform1dEXT = (PFNGLPROGRAMUNIFORM1DEXT_PROC*)glextGetProc("glProgramUniform1dEXT");
    glpfProgramUniform2dEXT = (PFNGLPROGRAMUNIFORM2DEXT_PROC*)glextGetProc("glProgramUniform2dEXT");
    glpfProgramUniform3dEXT = (PFNGLPROGRAMUNIFORM3DEXT_PROC*)glextGetProc("glProgramUniform3dEXT");
    glpfProgramUniform4dEXT = (PFNGLPROGRAMUNIFORM4DEXT_PROC*)glextGetProc("glProgramUniform4dEXT");
    glpfProgramUniform1dvEXT = (PFNGLPROGRAMUNIFORM1DVEXT_PROC*)glextGetProc("glProgramUniform1dvEXT");
    glpfProgramUniform2dvEXT = (PFNGLPROGRAMUNIFORM2DVEXT_PROC*)glextGetProc("glProgramUniform2dvEXT");
    glpfProgramUniform3dvEXT = (PFNGLPROGRAMUNIFORM3DVEXT_PROC*)glextGetProc("glProgramUniform3dvEXT");
    glpfProgramUniform4dvEXT = (PFNGLPROGRAMUNIFORM4DVEXT_PROC*)glextGetProc("glProgramUniform4dvEXT");
    glpfProgramUniformMatrix2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2DVEXT_PROC*)glextGetProc("glProgramUniformMatrix2dvEXT");
    glpfProgramUniformMatrix3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3DVEXT_PROC*)glextGetProc("glProgramUniformMatrix3dvEXT");
    glpfProgramUniformMatrix4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4DVEXT_PROC*)glextGetProc("glProgramUniformMatrix4dvEXT");
    glpfProgramUniformMatrix2x3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X3DVEXT_PROC*)glextGetProc("glProgramUniformMatrix2x3dvEXT");
    glpfProgramUniformMatrix2x4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X4DVEXT_PROC*)glextGetProc("glProgramUniformMatrix2x4dvEXT");
    glpfProgramUniformMatrix3x2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X2DVEXT_PROC*)glextGetProc("glProgramUniformMatrix3x2dvEXT");
    glpfProgramUniformMatrix3x4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X4DVEXT_PROC*)glextGetProc("glProgramUniformMatrix3x4dvEXT");
    glpfProgramUniformMatrix4x2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X2DVEXT_PROC*)glextGetProc("glProgramUniformMatrix4x2dvEXT");
    glpfProgramUniformMatrix4x3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X3DVEXT_PROC*)glextGetProc("glProgramUniformMatrix4x3dvEXT");


    /* GL_VERSION_1_2 */

    glpfBlendColor = (PFNGLBLENDCOLOR_PROC*)glextGetProc("glBlendColor");
    glpfBlendEquation = (PFNGLBLENDEQUATION_PROC*)glextGetProc("glBlendEquation");
    glpfDrawRangeElements = (PFNGLDRAWRANGEELEMENTS_PROC*)glextGetProc("glDrawRangeElements");
    glpfTexImage3D = (PFNGLTEXIMAGE3D_PROC*)glextGetProc("glTexImage3D");
    glpfTexSubImage3D = (PFNGLTEXSUBIMAGE3D_PROC*)glextGetProc("glTexSubImage3D");
    glpfCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3D_PROC*)glextGetProc("glCopyTexSubImage3D");
    glpfColorTable = (PFNGLCOLORTABLE_PROC*)glextGetProc("glColorTable");
    glpfColorTableParameterfv = (PFNGLCOLORTABLEPARAMETERFV_PROC*)glextGetProc("glColorTableParameterfv");
    glpfColorTableParameteriv = (PFNGLCOLORTABLEPARAMETERIV_PROC*)glextGetProc("glColorTableParameteriv");
    glpfCopyColorTable = (PFNGLCOPYCOLORTABLE_PROC*)glextGetProc("glCopyColorTable");
    glpfGetColorTable = (PFNGLGETCOLORTABLE_PROC*)glextGetProc("glGetColorTable");
    glpfGetColorTableParameterfv = (PFNGLGETCOLORTABLEPARAMETERFV_PROC*)glextGetProc("glGetColorTableParameterfv");
    glpfGetColorTableParameteriv = (PFNGLGETCOLORTABLEPARAMETERIV_PROC*)glextGetProc("glGetColorTableParameteriv");
    glpfColorSubTable = (PFNGLCOLORSUBTABLE_PROC*)glextGetProc("glColorSubTable");
    glpfCopyColorSubTable = (PFNGLCOPYCOLORSUBTABLE_PROC*)glextGetProc("glCopyColorSubTable");
    glpfConvolutionFilter1D = (PFNGLCONVOLUTIONFILTER1D_PROC*)glextGetProc("glConvolutionFilter1D");
    glpfConvolutionFilter2D = (PFNGLCONVOLUTIONFILTER2D_PROC*)glextGetProc("glConvolutionFilter2D");
    glpfConvolutionParameterf = (PFNGLCONVOLUTIONPARAMETERF_PROC*)glextGetProc("glConvolutionParameterf");
    glpfConvolutionParameterfv = (PFNGLCONVOLUTIONPARAMETERFV_PROC*)glextGetProc("glConvolutionParameterfv");
    glpfConvolutionParameteri = (PFNGLCONVOLUTIONPARAMETERI_PROC*)glextGetProc("glConvolutionParameteri");
    glpfConvolutionParameteriv = (PFNGLCONVOLUTIONPARAMETERIV_PROC*)glextGetProc("glConvolutionParameteriv");
    glpfCopyConvolutionFilter1D = (PFNGLCOPYCONVOLUTIONFILTER1D_PROC*)glextGetProc("glCopyConvolutionFilter1D");
    glpfCopyConvolutionFilter2D = (PFNGLCOPYCONVOLUTIONFILTER2D_PROC*)glextGetProc("glCopyConvolutionFilter2D");
    glpfGetConvolutionFilter = (PFNGLGETCONVOLUTIONFILTER_PROC*)glextGetProc("glGetConvolutionFilter");
    glpfGetConvolutionParameterfv = (PFNGLGETCONVOLUTIONPARAMETERFV_PROC*)glextGetProc("glGetConvolutionParameterfv");
    glpfGetConvolutionParameteriv = (PFNGLGETCONVOLUTIONPARAMETERIV_PROC*)glextGetProc("glGetConvolutionParameteriv");
    glpfGetSeparableFilter = (PFNGLGETSEPARABLEFILTER_PROC*)glextGetProc("glGetSeparableFilter");
    glpfSeparableFilter2D = (PFNGLSEPARABLEFILTER2D_PROC*)glextGetProc("glSeparableFilter2D");
    glpfGetHistogram = (PFNGLGETHISTOGRAM_PROC*)glextGetProc("glGetHistogram");
    glpfGetHistogramParameterfv = (PFNGLGETHISTOGRAMPARAMETERFV_PROC*)glextGetProc("glGetHistogramParameterfv");
    glpfGetHistogramParameteriv = (PFNGLGETHISTOGRAMPARAMETERIV_PROC*)glextGetProc("glGetHistogramParameteriv");
    glpfGetMinmax = (PFNGLGETMINMAX_PROC*)glextGetProc("glGetMinmax");
    glpfGetMinmaxParameterfv = (PFNGLGETMINMAXPARAMETERFV_PROC*)glextGetProc("glGetMinmaxParameterfv");
    glpfGetMinmaxParameteriv = (PFNGLGETMINMAXPARAMETERIV_PROC*)glextGetProc("glGetMinmaxParameteriv");
    glpfHistogram = (PFNGLHISTOGRAM_PROC*)glextGetProc("glHistogram");
    glpfMinmax = (PFNGLMINMAX_PROC*)glextGetProc("glMinmax");
    glpfResetHistogram = (PFNGLRESETHISTOGRAM_PROC*)glextGetProc("glResetHistogram");
    glpfResetMinmax = (PFNGLRESETMINMAX_PROC*)glextGetProc("glResetMinmax");


    /* GL_VERSION_1_3 */

    glpfActiveTexture = (PFNGLACTIVETEXTURE_PROC*)glextGetProc("glActiveTexture");
    glpfSampleCoverage = (PFNGLSAMPLECOVERAGE_PROC*)glextGetProc("glSampleCoverage");
    glpfCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3D_PROC*)glextGetProc("glCompressedTexImage3D");
    glpfCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2D_PROC*)glextGetProc("glCompressedTexImage2D");
    glpfCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1D_PROC*)glextGetProc("glCompressedTexImage1D");
    glpfCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3D_PROC*)glextGetProc("glCompressedTexSubImage3D");
    glpfCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2D_PROC*)glextGetProc("glCompressedTexSubImage2D");
    glpfCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1D_PROC*)glextGetProc("glCompressedTexSubImage1D");
    glpfGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGE_PROC*)glextGetProc("glGetCompressedTexImage");
    glpfClientActiveTexture = (PFNGLCLIENTACTIVETEXTURE_PROC*)glextGetProc("glClientActiveTexture");
    glpfMultiTexCoord1d = (PFNGLMULTITEXCOORD1D_PROC*)glextGetProc("glMultiTexCoord1d");
    glpfMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DV_PROC*)glextGetProc("glMultiTexCoord1dv");
    glpfMultiTexCoord1f = (PFNGLMULTITEXCOORD1F_PROC*)glextGetProc("glMultiTexCoord1f");
    glpfMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FV_PROC*)glextGetProc("glMultiTexCoord1fv");
    glpfMultiTexCoord1i = (PFNGLMULTITEXCOORD1I_PROC*)glextGetProc("glMultiTexCoord1i");
    glpfMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IV_PROC*)glextGetProc("glMultiTexCoord1iv");
    glpfMultiTexCoord1s = (PFNGLMULTITEXCOORD1S_PROC*)glextGetProc("glMultiTexCoord1s");
    glpfMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SV_PROC*)glextGetProc("glMultiTexCoord1sv");
    glpfMultiTexCoord2d = (PFNGLMULTITEXCOORD2D_PROC*)glextGetProc("glMultiTexCoord2d");
    glpfMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DV_PROC*)glextGetProc("glMultiTexCoord2dv");
    glpfMultiTexCoord2f = (PFNGLMULTITEXCOORD2F_PROC*)glextGetProc("glMultiTexCoord2f");
    glpfMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FV_PROC*)glextGetProc("glMultiTexCoord2fv");
    glpfMultiTexCoord2i = (PFNGLMULTITEXCOORD2I_PROC*)glextGetProc("glMultiTexCoord2i");
    glpfMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IV_PROC*)glextGetProc("glMultiTexCoord2iv");
    glpfMultiTexCoord2s = (PFNGLMULTITEXCOORD2S_PROC*)glextGetProc("glMultiTexCoord2s");
    glpfMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SV_PROC*)glextGetProc("glMultiTexCoord2sv");
    glpfMultiTexCoord3d = (PFNGLMULTITEXCOORD3D_PROC*)glextGetProc("glMultiTexCoord3d");
    glpfMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DV_PROC*)glextGetProc("glMultiTexCoord3dv");
    glpfMultiTexCoord3f = (PFNGLMULTITEXCOORD3F_PROC*)glextGetProc("glMultiTexCoord3f");
    glpfMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FV_PROC*)glextGetProc("glMultiTexCoord3fv");
    glpfMultiTexCoord3i = (PFNGLMULTITEXCOORD3I_PROC*)glextGetProc("glMultiTexCoord3i");
    glpfMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IV_PROC*)glextGetProc("glMultiTexCoord3iv");
    glpfMultiTexCoord3s = (PFNGLMULTITEXCOORD3S_PROC*)glextGetProc("glMultiTexCoord3s");
    glpfMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SV_PROC*)glextGetProc("glMultiTexCoord3sv");
    glpfMultiTexCoord4d = (PFNGLMULTITEXCOORD4D_PROC*)glextGetProc("glMultiTexCoord4d");
    glpfMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DV_PROC*)glextGetProc("glMultiTexCoord4dv");
    glpfMultiTexCoord4f = (PFNGLMULTITEXCOORD4F_PROC*)glextGetProc("glMultiTexCoord4f");
    glpfMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FV_PROC*)glextGetProc("glMultiTexCoord4fv");
    glpfMultiTexCoord4i = (PFNGLMULTITEXCOORD4I_PROC*)glextGetProc("glMultiTexCoord4i");
    glpfMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IV_PROC*)glextGetProc("glMultiTexCoord4iv");
    glpfMultiTexCoord4s = (PFNGLMULTITEXCOORD4S_PROC*)glextGetProc("glMultiTexCoord4s");
    glpfMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SV_PROC*)glextGetProc("glMultiTexCoord4sv");
    glpfLoadTransposeMatrixf = (PFNGLLOADTRANSPOSEMATRIXF_PROC*)glextGetProc("glLoadTransposeMatrixf");
    glpfLoadTransposeMatrixd = (PFNGLLOADTRANSPOSEMATRIXD_PROC*)glextGetProc("glLoadTransposeMatrixd");
    glpfMultTransposeMatrixf = (PFNGLMULTTRANSPOSEMATRIXF_PROC*)glextGetProc("glMultTransposeMatrixf");
    glpfMultTransposeMatrixd = (PFNGLMULTTRANSPOSEMATRIXD_PROC*)glextGetProc("glMultTransposeMatrixd");


    /* GL_VERSION_1_4 */

    glpfBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATE_PROC*)glextGetProc("glBlendFuncSeparate");
    glpfMultiDrawArrays = (PFNGLMULTIDRAWARRAYS_PROC*)glextGetProc("glMultiDrawArrays");
    glpfMultiDrawElements = (PFNGLMULTIDRAWELEMENTS_PROC*)glextGetProc("glMultiDrawElements");
    glpfPointParameterf = (PFNGLPOINTPARAMETERF_PROC*)glextGetProc("glPointParameterf");
    glpfPointParameterfv = (PFNGLPOINTPARAMETERFV_PROC*)glextGetProc("glPointParameterfv");
    glpfPointParameteri = (PFNGLPOINTPARAMETERI_PROC*)glextGetProc("glPointParameteri");
    glpfPointParameteriv = (PFNGLPOINTPARAMETERIV_PROC*)glextGetProc("glPointParameteriv");
    glpfFogCoordf = (PFNGLFOGCOORDF_PROC*)glextGetProc("glFogCoordf");
    glpfFogCoordfv = (PFNGLFOGCOORDFV_PROC*)glextGetProc("glFogCoordfv");
    glpfFogCoordd = (PFNGLFOGCOORDD_PROC*)glextGetProc("glFogCoordd");
    glpfFogCoorddv = (PFNGLFOGCOORDDV_PROC*)glextGetProc("glFogCoorddv");
    glpfFogCoordPointer = (PFNGLFOGCOORDPOINTER_PROC*)glextGetProc("glFogCoordPointer");
    glpfSecondaryColor3b = (PFNGLSECONDARYCOLOR3B_PROC*)glextGetProc("glSecondaryColor3b");
    glpfSecondaryColor3bv = (PFNGLSECONDARYCOLOR3BV_PROC*)glextGetProc("glSecondaryColor3bv");
    glpfSecondaryColor3d = (PFNGLSECONDARYCOLOR3D_PROC*)glextGetProc("glSecondaryColor3d");
    glpfSecondaryColor3dv = (PFNGLSECONDARYCOLOR3DV_PROC*)glextGetProc("glSecondaryColor3dv");
    glpfSecondaryColor3f = (PFNGLSECONDARYCOLOR3F_PROC*)glextGetProc("glSecondaryColor3f");
    glpfSecondaryColor3fv = (PFNGLSECONDARYCOLOR3FV_PROC*)glextGetProc("glSecondaryColor3fv");
    glpfSecondaryColor3i = (PFNGLSECONDARYCOLOR3I_PROC*)glextGetProc("glSecondaryColor3i");
    glpfSecondaryColor3iv = (PFNGLSECONDARYCOLOR3IV_PROC*)glextGetProc("glSecondaryColor3iv");
    glpfSecondaryColor3s = (PFNGLSECONDARYCOLOR3S_PROC*)glextGetProc("glSecondaryColor3s");
    glpfSecondaryColor3sv = (PFNGLSECONDARYCOLOR3SV_PROC*)glextGetProc("glSecondaryColor3sv");
    glpfSecondaryColor3ub = (PFNGLSECONDARYCOLOR3UB_PROC*)glextGetProc("glSecondaryColor3ub");
    glpfSecondaryColor3ubv = (PFNGLSECONDARYCOLOR3UBV_PROC*)glextGetProc("glSecondaryColor3ubv");
    glpfSecondaryColor3ui = (PFNGLSECONDARYCOLOR3UI_PROC*)glextGetProc("glSecondaryColor3ui");
    glpfSecondaryColor3uiv = (PFNGLSECONDARYCOLOR3UIV_PROC*)glextGetProc("glSecondaryColor3uiv");
    glpfSecondaryColor3us = (PFNGLSECONDARYCOLOR3US_PROC*)glextGetProc("glSecondaryColor3us");
    glpfSecondaryColor3usv = (PFNGLSECONDARYCOLOR3USV_PROC*)glextGetProc("glSecondaryColor3usv");
    glpfSecondaryColorPointer = (PFNGLSECONDARYCOLORPOINTER_PROC*)glextGetProc("glSecondaryColorPointer");
    glpfWindowPos2d = (PFNGLWINDOWPOS2D_PROC*)glextGetProc("glWindowPos2d");
    glpfWindowPos2dv = (PFNGLWINDOWPOS2DV_PROC*)glextGetProc("glWindowPos2dv");
    glpfWindowPos2f = (PFNGLWINDOWPOS2F_PROC*)glextGetProc("glWindowPos2f");
    glpfWindowPos2fv = (PFNGLWINDOWPOS2FV_PROC*)glextGetProc("glWindowPos2fv");
    glpfWindowPos2i = (PFNGLWINDOWPOS2I_PROC*)glextGetProc("glWindowPos2i");
    glpfWindowPos2iv = (PFNGLWINDOWPOS2IV_PROC*)glextGetProc("glWindowPos2iv");
    glpfWindowPos2s = (PFNGLWINDOWPOS2S_PROC*)glextGetProc("glWindowPos2s");
    glpfWindowPos2sv = (PFNGLWINDOWPOS2SV_PROC*)glextGetProc("glWindowPos2sv");
    glpfWindowPos3d = (PFNGLWINDOWPOS3D_PROC*)glextGetProc("glWindowPos3d");
    glpfWindowPos3dv = (PFNGLWINDOWPOS3DV_PROC*)glextGetProc("glWindowPos3dv");
    glpfWindowPos3f = (PFNGLWINDOWPOS3F_PROC*)glextGetProc("glWindowPos3f");
    glpfWindowPos3fv = (PFNGLWINDOWPOS3FV_PROC*)glextGetProc("glWindowPos3fv");
    glpfWindowPos3i = (PFNGLWINDOWPOS3I_PROC*)glextGetProc("glWindowPos3i");
    glpfWindowPos3iv = (PFNGLWINDOWPOS3IV_PROC*)glextGetProc("glWindowPos3iv");
    glpfWindowPos3s = (PFNGLWINDOWPOS3S_PROC*)glextGetProc("glWindowPos3s");
    glpfWindowPos3sv = (PFNGLWINDOWPOS3SV_PROC*)glextGetProc("glWindowPos3sv");


    /* GL_VERSION_1_5 */

    glpfGenQueries = (PFNGLGENQUERIES_PROC*)glextGetProc("glGenQueries");
    glpfDeleteQueries = (PFNGLDELETEQUERIES_PROC*)glextGetProc("glDeleteQueries");
    glpfIsQuery = (PFNGLISQUERY_PROC*)glextGetProc("glIsQuery");
    glpfBeginQuery = (PFNGLBEGINQUERY_PROC*)glextGetProc("glBeginQuery");
    glpfEndQuery = (PFNGLENDQUERY_PROC*)glextGetProc("glEndQuery");
    glpfGetQueryiv = (PFNGLGETQUERYIV_PROC*)glextGetProc("glGetQueryiv");
    glpfGetQueryObjectiv = (PFNGLGETQUERYOBJECTIV_PROC*)glextGetProc("glGetQueryObjectiv");
    glpfGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIV_PROC*)glextGetProc("glGetQueryObjectuiv");
    glpfBindBuffer = (PFNGLBINDBUFFER_PROC*)glextGetProc("glBindBuffer");
    glpfDeleteBuffers = (PFNGLDELETEBUFFERS_PROC*)glextGetProc("glDeleteBuffers");
    glpfGenBuffers = (PFNGLGENBUFFERS_PROC*)glextGetProc("glGenBuffers");
    glpfIsBuffer = (PFNGLISBUFFER_PROC*)glextGetProc("glIsBuffer");
    glpfBufferData = (PFNGLBUFFERDATA_PROC*)glextGetProc("glBufferData");
    glpfBufferSubData = (PFNGLBUFFERSUBDATA_PROC*)glextGetProc("glBufferSubData");
    glpfGetBufferSubData = (PFNGLGETBUFFERSUBDATA_PROC*)glextGetProc("glGetBufferSubData");
    glpfMapBuffer = (PFNGLMAPBUFFER_PROC*)glextGetProc("glMapBuffer");
    glpfUnmapBuffer = (PFNGLUNMAPBUFFER_PROC*)glextGetProc("glUnmapBuffer");
    glpfGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIV_PROC*)glextGetProc("glGetBufferParameteriv");
    glpfGetBufferPointerv = (PFNGLGETBUFFERPOINTERV_PROC*)glextGetProc("glGetBufferPointerv");


    /* GL_VERSION_2_0 */

    glpfBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATE_PROC*)glextGetProc("glBlendEquationSeparate");
    glpfDrawBuffers = (PFNGLDRAWBUFFERS_PROC*)glextGetProc("glDrawBuffers");
    glpfStencilOpSeparate = (PFNGLSTENCILOPSEPARATE_PROC*)glextGetProc("glStencilOpSeparate");
    glpfStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATE_PROC*)glextGetProc("glStencilFuncSeparate");
    glpfStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATE_PROC*)glextGetProc("glStencilMaskSeparate");
    glpfAttachShader = (PFNGLATTACHSHADER_PROC*)glextGetProc("glAttachShader");
    glpfBindAttribLocation = (PFNGLBINDATTRIBLOCATION_PROC*)glextGetProc("glBindAttribLocation");
    glpfCompileShader = (PFNGLCOMPILESHADER_PROC*)glextGetProc("glCompileShader");
    glpfCreateProgram = (PFNGLCREATEPROGRAM_PROC*)glextGetProc("glCreateProgram");
    glpfCreateShader = (PFNGLCREATESHADER_PROC*)glextGetProc("glCreateShader");
    glpfDeleteProgram = (PFNGLDELETEPROGRAM_PROC*)glextGetProc("glDeleteProgram");
    glpfDeleteShader = (PFNGLDELETESHADER_PROC*)glextGetProc("glDeleteShader");
    glpfDetachShader = (PFNGLDETACHSHADER_PROC*)glextGetProc("glDetachShader");
    glpfDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAY_PROC*)glextGetProc("glDisableVertexAttribArray");
    glpfEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAY_PROC*)glextGetProc("glEnableVertexAttribArray");
    glpfGetActiveAttrib = (PFNGLGETACTIVEATTRIB_PROC*)glextGetProc("glGetActiveAttrib");
    glpfGetActiveUniform = (PFNGLGETACTIVEUNIFORM_PROC*)glextGetProc("glGetActiveUniform");
    glpfGetAttachedShaders = (PFNGLGETATTACHEDSHADERS_PROC*)glextGetProc("glGetAttachedShaders");
    glpfGetAttribLocation = (PFNGLGETATTRIBLOCATION_PROC*)glextGetProc("glGetAttribLocation");
    glpfGetProgramiv = (PFNGLGETPROGRAMIV_PROC*)glextGetProc("glGetProgramiv");
    glpfGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOG_PROC*)glextGetProc("glGetProgramInfoLog");
    glpfGetShaderiv = (PFNGLGETSHADERIV_PROC*)glextGetProc("glGetShaderiv");
    glpfGetShaderInfoLog = (PFNGLGETSHADERINFOLOG_PROC*)glextGetProc("glGetShaderInfoLog");
    glpfGetShaderSource = (PFNGLGETSHADERSOURCE_PROC*)glextGetProc("glGetShaderSource");
    glpfGetUniformLocation = (PFNGLGETUNIFORMLOCATION_PROC*)glextGetProc("glGetUniformLocation");
    glpfGetUniformfv = (PFNGLGETUNIFORMFV_PROC*)glextGetProc("glGetUniformfv");
    glpfGetUniformiv = (PFNGLGETUNIFORMIV_PROC*)glextGetProc("glGetUniformiv");
    glpfGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDV_PROC*)glextGetProc("glGetVertexAttribdv");
    glpfGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFV_PROC*)glextGetProc("glGetVertexAttribfv");
    glpfGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIV_PROC*)glextGetProc("glGetVertexAttribiv");
    glpfGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERV_PROC*)glextGetProc("glGetVertexAttribPointerv");
    glpfIsProgram = (PFNGLISPROGRAM_PROC*)glextGetProc("glIsProgram");
    glpfIsShader = (PFNGLISSHADER_PROC*)glextGetProc("glIsShader");
    glpfLinkProgram = (PFNGLLINKPROGRAM_PROC*)glextGetProc("glLinkProgram");
    glpfShaderSource = (PFNGLSHADERSOURCE_PROC*)glextGetProc("glShaderSource");
    glpfUseProgram = (PFNGLUSEPROGRAM_PROC*)glextGetProc("glUseProgram");
    glpfUniform1f = (PFNGLUNIFORM1F_PROC*)glextGetProc("glUniform1f");
    glpfUniform2f = (PFNGLUNIFORM2F_PROC*)glextGetProc("glUniform2f");
    glpfUniform3f = (PFNGLUNIFORM3F_PROC*)glextGetProc("glUniform3f");
    glpfUniform4f = (PFNGLUNIFORM4F_PROC*)glextGetProc("glUniform4f");
    glpfUniform1i = (PFNGLUNIFORM1I_PROC*)glextGetProc("glUniform1i");
    glpfUniform2i = (PFNGLUNIFORM2I_PROC*)glextGetProc("glUniform2i");
    glpfUniform3i = (PFNGLUNIFORM3I_PROC*)glextGetProc("glUniform3i");
    glpfUniform4i = (PFNGLUNIFORM4I_PROC*)glextGetProc("glUniform4i");
    glpfUniform1fv = (PFNGLUNIFORM1FV_PROC*)glextGetProc("glUniform1fv");
    glpfUniform2fv = (PFNGLUNIFORM2FV_PROC*)glextGetProc("glUniform2fv");
    glpfUniform3fv = (PFNGLUNIFORM3FV_PROC*)glextGetProc("glUniform3fv");
    glpfUniform4fv = (PFNGLUNIFORM4FV_PROC*)glextGetProc("glUniform4fv");
    glpfUniform1iv = (PFNGLUNIFORM1IV_PROC*)glextGetProc("glUniform1iv");
    glpfUniform2iv = (PFNGLUNIFORM2IV_PROC*)glextGetProc("glUniform2iv");
    glpfUniform3iv = (PFNGLUNIFORM3IV_PROC*)glextGetProc("glUniform3iv");
    glpfUniform4iv = (PFNGLUNIFORM4IV_PROC*)glextGetProc("glUniform4iv");
    glpfUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FV_PROC*)glextGetProc("glUniformMatrix2fv");
    glpfUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FV_PROC*)glextGetProc("glUniformMatrix3fv");
    glpfUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FV_PROC*)glextGetProc("glUniformMatrix4fv");
    glpfValidateProgram = (PFNGLVALIDATEPROGRAM_PROC*)glextGetProc("glValidateProgram");
    glpfVertexAttrib1d = (PFNGLVERTEXATTRIB1D_PROC*)glextGetProc("glVertexAttrib1d");
    glpfVertexAttrib1dv = (PFNGLVERTEXATTRIB1DV_PROC*)glextGetProc("glVertexAttrib1dv");
    glpfVertexAttrib1f = (PFNGLVERTEXATTRIB1F_PROC*)glextGetProc("glVertexAttrib1f");
    glpfVertexAttrib1fv = (PFNGLVERTEXATTRIB1FV_PROC*)glextGetProc("glVertexAttrib1fv");
    glpfVertexAttrib1s = (PFNGLVERTEXATTRIB1S_PROC*)glextGetProc("glVertexAttrib1s");
    glpfVertexAttrib1sv = (PFNGLVERTEXATTRIB1SV_PROC*)glextGetProc("glVertexAttrib1sv");
    glpfVertexAttrib2d = (PFNGLVERTEXATTRIB2D_PROC*)glextGetProc("glVertexAttrib2d");
    glpfVertexAttrib2dv = (PFNGLVERTEXATTRIB2DV_PROC*)glextGetProc("glVertexAttrib2dv");
    glpfVertexAttrib2f = (PFNGLVERTEXATTRIB2F_PROC*)glextGetProc("glVertexAttrib2f");
    glpfVertexAttrib2fv = (PFNGLVERTEXATTRIB2FV_PROC*)glextGetProc("glVertexAttrib2fv");
    glpfVertexAttrib2s = (PFNGLVERTEXATTRIB2S_PROC*)glextGetProc("glVertexAttrib2s");
    glpfVertexAttrib2sv = (PFNGLVERTEXATTRIB2SV_PROC*)glextGetProc("glVertexAttrib2sv");
    glpfVertexAttrib3d = (PFNGLVERTEXATTRIB3D_PROC*)glextGetProc("glVertexAttrib3d");
    glpfVertexAttrib3dv = (PFNGLVERTEXATTRIB3DV_PROC*)glextGetProc("glVertexAttrib3dv");
    glpfVertexAttrib3f = (PFNGLVERTEXATTRIB3F_PROC*)glextGetProc("glVertexAttrib3f");
    glpfVertexAttrib3fv = (PFNGLVERTEXATTRIB3FV_PROC*)glextGetProc("glVertexAttrib3fv");
    glpfVertexAttrib3s = (PFNGLVERTEXATTRIB3S_PROC*)glextGetProc("glVertexAttrib3s");
    glpfVertexAttrib3sv = (PFNGLVERTEXATTRIB3SV_PROC*)glextGetProc("glVertexAttrib3sv");
    glpfVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBV_PROC*)glextGetProc("glVertexAttrib4Nbv");
    glpfVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIV_PROC*)glextGetProc("glVertexAttrib4Niv");
    glpfVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSV_PROC*)glextGetProc("glVertexAttrib4Nsv");
    glpfVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUB_PROC*)glextGetProc("glVertexAttrib4Nub");
    glpfVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBV_PROC*)glextGetProc("glVertexAttrib4Nubv");
    glpfVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIV_PROC*)glextGetProc("glVertexAttrib4Nuiv");
    glpfVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSV_PROC*)glextGetProc("glVertexAttrib4Nusv");
    glpfVertexAttrib4bv = (PFNGLVERTEXATTRIB4BV_PROC*)glextGetProc("glVertexAttrib4bv");
    glpfVertexAttrib4d = (PFNGLVERTEXATTRIB4D_PROC*)glextGetProc("glVertexAttrib4d");
    glpfVertexAttrib4dv = (PFNGLVERTEXATTRIB4DV_PROC*)glextGetProc("glVertexAttrib4dv");
    glpfVertexAttrib4f = (PFNGLVERTEXATTRIB4F_PROC*)glextGetProc("glVertexAttrib4f");
    glpfVertexAttrib4fv = (PFNGLVERTEXATTRIB4FV_PROC*)glextGetProc("glVertexAttrib4fv");
    glpfVertexAttrib4iv = (PFNGLVERTEXATTRIB4IV_PROC*)glextGetProc("glVertexAttrib4iv");
    glpfVertexAttrib4s = (PFNGLVERTEXATTRIB4S_PROC*)glextGetProc("glVertexAttrib4s");
    glpfVertexAttrib4sv = (PFNGLVERTEXATTRIB4SV_PROC*)glextGetProc("glVertexAttrib4sv");
    glpfVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBV_PROC*)glextGetProc("glVertexAttrib4ubv");
    glpfVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIV_PROC*)glextGetProc("glVertexAttrib4uiv");
    glpfVertexAttrib4usv = (PFNGLVERTEXATTRIB4USV_PROC*)glextGetProc("glVertexAttrib4usv");
    glpfVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTER_PROC*)glextGetProc("glVertexAttribPointer");


    /* GL_VERSION_2_1 */

    glpfUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FV_PROC*)glextGetProc("glUniformMatrix2x3fv");
    glpfUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FV_PROC*)glextGetProc("glUniformMatrix3x2fv");
    glpfUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FV_PROC*)glextGetProc("glUniformMatrix2x4fv");
    glpfUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FV_PROC*)glextGetProc("glUniformMatrix4x2fv");
    glpfUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FV_PROC*)glextGetProc("glUniformMatrix3x4fv");
    glpfUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FV_PROC*)glextGetProc("glUniformMatrix4x3fv");


    /* GL_VERSION_3_0 */

    glpfColorMaski = (PFNGLCOLORMASKI_PROC*)glextGetProc("glColorMaski");
    glpfGetBooleani_v = (PFNGLGETBOOLEANI_V_PROC*)glextGetProc("glGetBooleani_v");
    glpfGetIntegeri_v = (PFNGLGETINTEGERI_V_PROC*)glextGetProc("glGetIntegeri_v");
    glpfEnablei = (PFNGLENABLEI_PROC*)glextGetProc("glEnablei");
    glpfDisablei = (PFNGLDISABLEI_PROC*)glextGetProc("glDisablei");
    glpfIsEnabledi = (PFNGLISENABLEDI_PROC*)glextGetProc("glIsEnabledi");
    glpfBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACK_PROC*)glextGetProc("glBeginTransformFeedback");
    glpfEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACK_PROC*)glextGetProc("glEndTransformFeedback");
    glpfBindBufferRange = (PFNGLBINDBUFFERRANGE_PROC*)glextGetProc("glBindBufferRange");
    glpfBindBufferBase = (PFNGLBINDBUFFERBASE_PROC*)glextGetProc("glBindBufferBase");
    glpfTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGS_PROC*)glextGetProc("glTransformFeedbackVaryings");
    glpfGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYING_PROC*)glextGetProc("glGetTransformFeedbackVarying");
    glpfClampColor = (PFNGLCLAMPCOLOR_PROC*)glextGetProc("glClampColor");
    glpfBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDER_PROC*)glextGetProc("glBeginConditionalRender");
    glpfEndConditionalRender = (PFNGLENDCONDITIONALRENDER_PROC*)glextGetProc("glEndConditionalRender");
    glpfVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTER_PROC*)glextGetProc("glVertexAttribIPointer");
    glpfGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIV_PROC*)glextGetProc("glGetVertexAttribIiv");
    glpfGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIV_PROC*)glextGetProc("glGetVertexAttribIuiv");
    glpfVertexAttribI1i = (PFNGLVERTEXATTRIBI1I_PROC*)glextGetProc("glVertexAttribI1i");
    glpfVertexAttribI2i = (PFNGLVERTEXATTRIBI2I_PROC*)glextGetProc("glVertexAttribI2i");
    glpfVertexAttribI3i = (PFNGLVERTEXATTRIBI3I_PROC*)glextGetProc("glVertexAttribI3i");
    glpfVertexAttribI4i = (PFNGLVERTEXATTRIBI4I_PROC*)glextGetProc("glVertexAttribI4i");
    glpfVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UI_PROC*)glextGetProc("glVertexAttribI1ui");
    glpfVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UI_PROC*)glextGetProc("glVertexAttribI2ui");
    glpfVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UI_PROC*)glextGetProc("glVertexAttribI3ui");
    glpfVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UI_PROC*)glextGetProc("glVertexAttribI4ui");
    glpfVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IV_PROC*)glextGetProc("glVertexAttribI1iv");
    glpfVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IV_PROC*)glextGetProc("glVertexAttribI2iv");
    glpfVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IV_PROC*)glextGetProc("glVertexAttribI3iv");
    glpfVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IV_PROC*)glextGetProc("glVertexAttribI4iv");
    glpfVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIV_PROC*)glextGetProc("glVertexAttribI1uiv");
    glpfVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIV_PROC*)glextGetProc("glVertexAttribI2uiv");
    glpfVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIV_PROC*)glextGetProc("glVertexAttribI3uiv");
    glpfVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIV_PROC*)glextGetProc("glVertexAttribI4uiv");
    glpfVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BV_PROC*)glextGetProc("glVertexAttribI4bv");
    glpfVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SV_PROC*)glextGetProc("glVertexAttribI4sv");
    glpfVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBV_PROC*)glextGetProc("glVertexAttribI4ubv");
    glpfVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USV_PROC*)glextGetProc("glVertexAttribI4usv");
    glpfGetUniformuiv = (PFNGLGETUNIFORMUIV_PROC*)glextGetProc("glGetUniformuiv");
    glpfBindFragDataLocation = (PFNGLBINDFRAGDATALOCATION_PROC*)glextGetProc("glBindFragDataLocation");
    glpfGetFragDataLocation = (PFNGLGETFRAGDATALOCATION_PROC*)glextGetProc("glGetFragDataLocation");
    glpfUniform1ui = (PFNGLUNIFORM1UI_PROC*)glextGetProc("glUniform1ui");
    glpfUniform2ui = (PFNGLUNIFORM2UI_PROC*)glextGetProc("glUniform2ui");
    glpfUniform3ui = (PFNGLUNIFORM3UI_PROC*)glextGetProc("glUniform3ui");
    glpfUniform4ui = (PFNGLUNIFORM4UI_PROC*)glextGetProc("glUniform4ui");
    glpfUniform1uiv = (PFNGLUNIFORM1UIV_PROC*)glextGetProc("glUniform1uiv");
    glpfUniform2uiv = (PFNGLUNIFORM2UIV_PROC*)glextGetProc("glUniform2uiv");
    glpfUniform3uiv = (PFNGLUNIFORM3UIV_PROC*)glextGetProc("glUniform3uiv");
    glpfUniform4uiv = (PFNGLUNIFORM4UIV_PROC*)glextGetProc("glUniform4uiv");
    glpfTexParameterIiv = (PFNGLTEXPARAMETERIIV_PROC*)glextGetProc("glTexParameterIiv");
    glpfTexParameterIuiv = (PFNGLTEXPARAMETERIUIV_PROC*)glextGetProc("glTexParameterIuiv");
    glpfGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIV_PROC*)glextGetProc("glGetTexParameterIiv");
    glpfGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIV_PROC*)glextGetProc("glGetTexParameterIuiv");
    glpfClearBufferiv = (PFNGLCLEARBUFFERIV_PROC*)glextGetProc("glClearBufferiv");
    glpfClearBufferuiv = (PFNGLCLEARBUFFERUIV_PROC*)glextGetProc("glClearBufferuiv");
    glpfClearBufferfv = (PFNGLCLEARBUFFERFV_PROC*)glextGetProc("glClearBufferfv");
    glpfClearBufferfi = (PFNGLCLEARBUFFERFI_PROC*)glextGetProc("glClearBufferfi");
    glpfGetStringi = (PFNGLGETSTRINGI_PROC*)glextGetProc("glGetStringi");


    /* GL_VERSION_3_1 */

    glpfDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCED_PROC*)glextGetProc("glDrawArraysInstanced");
    glpfDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCED_PROC*)glextGetProc("glDrawElementsInstanced");
    glpfTexBuffer = (PFNGLTEXBUFFER_PROC*)glextGetProc("glTexBuffer");
    glpfPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEX_PROC*)glextGetProc("glPrimitiveRestartIndex");


    /* GL_VERSION_3_2 */

    glpfGetInteger64i_v = (PFNGLGETINTEGER64I_V_PROC*)glextGetProc("glGetInteger64i_v");
    glpfGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64V_PROC*)glextGetProc("glGetBufferParameteri64v");
    glpfFramebufferTexture = (PFNGLFRAMEBUFFERTEXTURE_PROC*)glextGetProc("glFramebufferTexture");


    /* GL_VERSION_3_3 */

    glpfVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISOR_PROC*)glextGetProc("glVertexAttribDivisor");


    /* GL_VERSION_4_0 */

    glpfMinSampleShading = (PFNGLMINSAMPLESHADING_PROC*)glextGetProc("glMinSampleShading");
    glpfBlendEquationi = (PFNGLBLENDEQUATIONI_PROC*)glextGetProc("glBlendEquationi");
    glpfBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEI_PROC*)glextGetProc("glBlendEquationSeparatei");
    glpfBlendFunci = (PFNGLBLENDFUNCI_PROC*)glextGetProc("glBlendFunci");
    glpfBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEI_PROC*)glextGetProc("glBlendFuncSeparatei");


}

/* ----------------------- Extension flag definitions ---------------------- */
int GLEXT_ARB_buffer_storage = GL_FALSE;
int GLEXT_ARB_seamless_cubemap_per_texture = GL_FALSE;
int GLEXT_ARB_bindless_texture = GL_FALSE;
int GLEXT_ARB_sparse_texture = GL_FALSE;
int GLEXT_ARB_shader_draw_parameters = GL_FALSE;
int GLEXT_ARB_indirect_parameters = GL_FALSE;
int GLEXT_ARB_shader_group_vote = GL_FALSE;
int GLEXT_ARB_multi_bind = GL_FALSE;
int GLEXT_ARB_clear_texture = GL_FALSE;
int GLEXT_ARB_query_buffer_object = GL_FALSE;
int GLEXT_ARB_enhanced_layouts = GL_FALSE;
int GLEXT_ARB_texture_mirror_clamp_to_edge = GL_FALSE;
int GLEXT_ARB_texture_stencil8 = GL_FALSE;
int GLEXT_ARB_compute_variable_group_size = GL_FALSE;
int GLEXT_ARB_vertex_type_10f_11f_11f_rev = GL_FALSE;
int GLEXT_EXT_direct_state_access = GL_FALSE;

/* ---------------------- Function pointer definitions --------------------- */

/* GL_ARB_ES2_compatibility */

PFNGLRELEASESHADERCOMPILER_PROC* glpfReleaseShaderCompiler = NULL;
PFNGLSHADERBINARY_PROC* glpfShaderBinary = NULL;
PFNGLGETSHADERPRECISIONFORMAT_PROC* glpfGetShaderPrecisionFormat = NULL;
PFNGLDEPTHRANGEF_PROC* glpfDepthRangef = NULL;
PFNGLCLEARDEPTHF_PROC* glpfClearDepthf = NULL;

/* GL_ARB_base_instance */

PFNGLDRAWARRAYSINSTANCEDBASEINSTANCE_PROC* glpfDrawArraysInstancedBaseInstance = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCE_PROC* glpfDrawElementsInstancedBaseInstance = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCE_PROC* glpfDrawElementsInstancedBaseVertexBaseInstance = NULL;

/* GL_ARB_blend_func_extended */

PFNGLBINDFRAGDATALOCATIONINDEXED_PROC* glpfBindFragDataLocationIndexed = NULL;
PFNGLGETFRAGDATAINDEX_PROC* glpfGetFragDataIndex = NULL;

/* GL_ARB_clear_buffer_object */

PFNGLCLEARBUFFERDATA_PROC* glpfClearBufferData = NULL;
PFNGLCLEARBUFFERSUBDATA_PROC* glpfClearBufferSubData = NULL;
PFNGLCLEARNAMEDBUFFERDATAEXT_PROC* glpfClearNamedBufferDataEXT = NULL;
PFNGLCLEARNAMEDBUFFERSUBDATAEXT_PROC* glpfClearNamedBufferSubDataEXT = NULL;

/* GL_ARB_compute_shader */

PFNGLDISPATCHCOMPUTE_PROC* glpfDispatchCompute = NULL;
PFNGLDISPATCHCOMPUTEINDIRECT_PROC* glpfDispatchComputeIndirect = NULL;

/* GL_ARB_copy_buffer */

PFNGLCOPYBUFFERSUBDATA_PROC* glpfCopyBufferSubData = NULL;

/* GL_ARB_copy_image */

PFNGLCOPYIMAGESUBDATA_PROC* glpfCopyImageSubData = NULL;

/* GL_ARB_draw_elements_base_vertex */

PFNGLDRAWELEMENTSBASEVERTEX_PROC* glpfDrawElementsBaseVertex = NULL;
PFNGLDRAWRANGEELEMENTSBASEVERTEX_PROC* glpfDrawRangeElementsBaseVertex = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEX_PROC* glpfDrawElementsInstancedBaseVertex = NULL;
PFNGLMULTIDRAWELEMENTSBASEVERTEX_PROC* glpfMultiDrawElementsBaseVertex = NULL;

/* GL_ARB_draw_indirect */

PFNGLDRAWARRAYSINDIRECT_PROC* glpfDrawArraysIndirect = NULL;
PFNGLDRAWELEMENTSINDIRECT_PROC* glpfDrawElementsIndirect = NULL;

/* GL_ARB_framebuffer_no_attachments */

PFNGLFRAMEBUFFERPARAMETERI_PROC* glpfFramebufferParameteri = NULL;
PFNGLGETFRAMEBUFFERPARAMETERIV_PROC* glpfGetFramebufferParameteriv = NULL;
PFNGLNAMEDFRAMEBUFFERPARAMETERIEXT_PROC* glpfNamedFramebufferParameteriEXT = NULL;
PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXT_PROC* glpfGetNamedFramebufferParameterivEXT = NULL;

/* GL_ARB_framebuffer_object */

PFNGLISRENDERBUFFER_PROC* glpfIsRenderbuffer = NULL;
PFNGLBINDRENDERBUFFER_PROC* glpfBindRenderbuffer = NULL;
PFNGLDELETERENDERBUFFERS_PROC* glpfDeleteRenderbuffers = NULL;
PFNGLGENRENDERBUFFERS_PROC* glpfGenRenderbuffers = NULL;
PFNGLRENDERBUFFERSTORAGE_PROC* glpfRenderbufferStorage = NULL;
PFNGLGETRENDERBUFFERPARAMETERIV_PROC* glpfGetRenderbufferParameteriv = NULL;
PFNGLISFRAMEBUFFER_PROC* glpfIsFramebuffer = NULL;
PFNGLBINDFRAMEBUFFER_PROC* glpfBindFramebuffer = NULL;
PFNGLDELETEFRAMEBUFFERS_PROC* glpfDeleteFramebuffers = NULL;
PFNGLGENFRAMEBUFFERS_PROC* glpfGenFramebuffers = NULL;
PFNGLCHECKFRAMEBUFFERSTATUS_PROC* glpfCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERTEXTURE1D_PROC* glpfFramebufferTexture1D = NULL;
PFNGLFRAMEBUFFERTEXTURE2D_PROC* glpfFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERTEXTURE3D_PROC* glpfFramebufferTexture3D = NULL;
PFNGLFRAMEBUFFERRENDERBUFFER_PROC* glpfFramebufferRenderbuffer = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV_PROC* glpfGetFramebufferAttachmentParameteriv = NULL;
PFNGLGENERATEMIPMAP_PROC* glpfGenerateMipmap = NULL;
PFNGLBLITFRAMEBUFFER_PROC* glpfBlitFramebuffer = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLE_PROC* glpfRenderbufferStorageMultisample = NULL;
PFNGLFRAMEBUFFERTEXTURELAYER_PROC* glpfFramebufferTextureLayer = NULL;

/* GL_ARB_get_program_binary */

PFNGLGETPROGRAMBINARY_PROC* glpfGetProgramBinary = NULL;
PFNGLPROGRAMBINARY_PROC* glpfProgramBinary = NULL;
PFNGLPROGRAMPARAMETERI_PROC* glpfProgramParameteri = NULL;

/* GL_ARB_gpu_shader_fp64 */

PFNGLUNIFORM1D_PROC* glpfUniform1d = NULL;
PFNGLUNIFORM2D_PROC* glpfUniform2d = NULL;
PFNGLUNIFORM3D_PROC* glpfUniform3d = NULL;
PFNGLUNIFORM4D_PROC* glpfUniform4d = NULL;
PFNGLUNIFORM1DV_PROC* glpfUniform1dv = NULL;
PFNGLUNIFORM2DV_PROC* glpfUniform2dv = NULL;
PFNGLUNIFORM3DV_PROC* glpfUniform3dv = NULL;
PFNGLUNIFORM4DV_PROC* glpfUniform4dv = NULL;
PFNGLUNIFORMMATRIX2DV_PROC* glpfUniformMatrix2dv = NULL;
PFNGLUNIFORMMATRIX3DV_PROC* glpfUniformMatrix3dv = NULL;
PFNGLUNIFORMMATRIX4DV_PROC* glpfUniformMatrix4dv = NULL;
PFNGLUNIFORMMATRIX2X3DV_PROC* glpfUniformMatrix2x3dv = NULL;
PFNGLUNIFORMMATRIX2X4DV_PROC* glpfUniformMatrix2x4dv = NULL;
PFNGLUNIFORMMATRIX3X2DV_PROC* glpfUniformMatrix3x2dv = NULL;
PFNGLUNIFORMMATRIX3X4DV_PROC* glpfUniformMatrix3x4dv = NULL;
PFNGLUNIFORMMATRIX4X2DV_PROC* glpfUniformMatrix4x2dv = NULL;
PFNGLUNIFORMMATRIX4X3DV_PROC* glpfUniformMatrix4x3dv = NULL;
PFNGLGETUNIFORMDV_PROC* glpfGetUniformdv = NULL;

/* GL_ARB_internalformat_query */

PFNGLGETINTERNALFORMATIV_PROC* glpfGetInternalformativ = NULL;

/* GL_ARB_internalformat_query2 */

PFNGLGETINTERNALFORMATI64V_PROC* glpfGetInternalformati64v = NULL;

/* GL_ARB_invalidate_subdata */

PFNGLINVALIDATETEXSUBIMAGE_PROC* glpfInvalidateTexSubImage = NULL;
PFNGLINVALIDATETEXIMAGE_PROC* glpfInvalidateTexImage = NULL;
PFNGLINVALIDATEBUFFERSUBDATA_PROC* glpfInvalidateBufferSubData = NULL;
PFNGLINVALIDATEBUFFERDATA_PROC* glpfInvalidateBufferData = NULL;
PFNGLINVALIDATEFRAMEBUFFER_PROC* glpfInvalidateFramebuffer = NULL;
PFNGLINVALIDATESUBFRAMEBUFFER_PROC* glpfInvalidateSubFramebuffer = NULL;

/* GL_ARB_map_buffer_range */

PFNGLMAPBUFFERRANGE_PROC* glpfMapBufferRange = NULL;
PFNGLFLUSHMAPPEDBUFFERRANGE_PROC* glpfFlushMappedBufferRange = NULL;

/* GL_ARB_multi_draw_indirect */

PFNGLMULTIDRAWARRAYSINDIRECT_PROC* glpfMultiDrawArraysIndirect = NULL;
PFNGLMULTIDRAWELEMENTSINDIRECT_PROC* glpfMultiDrawElementsIndirect = NULL;

/* GL_ARB_program_interface_query */

PFNGLGETPROGRAMINTERFACEIV_PROC* glpfGetProgramInterfaceiv = NULL;
PFNGLGETPROGRAMRESOURCEINDEX_PROC* glpfGetProgramResourceIndex = NULL;
PFNGLGETPROGRAMRESOURCENAME_PROC* glpfGetProgramResourceName = NULL;
PFNGLGETPROGRAMRESOURCEIV_PROC* glpfGetProgramResourceiv = NULL;
PFNGLGETPROGRAMRESOURCELOCATION_PROC* glpfGetProgramResourceLocation = NULL;
PFNGLGETPROGRAMRESOURCELOCATIONINDEX_PROC* glpfGetProgramResourceLocationIndex = NULL;

/* GL_ARB_provoking_vertex */

PFNGLPROVOKINGVERTEX_PROC* glpfProvokingVertex = NULL;

/* GL_ARB_sampler_objects */

PFNGLGENSAMPLERS_PROC* glpfGenSamplers = NULL;
PFNGLDELETESAMPLERS_PROC* glpfDeleteSamplers = NULL;
PFNGLISSAMPLER_PROC* glpfIsSampler = NULL;
PFNGLBINDSAMPLER_PROC* glpfBindSampler = NULL;
PFNGLSAMPLERPARAMETERI_PROC* glpfSamplerParameteri = NULL;
PFNGLSAMPLERPARAMETERIV_PROC* glpfSamplerParameteriv = NULL;
PFNGLSAMPLERPARAMETERF_PROC* glpfSamplerParameterf = NULL;
PFNGLSAMPLERPARAMETERFV_PROC* glpfSamplerParameterfv = NULL;
PFNGLSAMPLERPARAMETERIIV_PROC* glpfSamplerParameterIiv = NULL;
PFNGLSAMPLERPARAMETERIUIV_PROC* glpfSamplerParameterIuiv = NULL;
PFNGLGETSAMPLERPARAMETERIV_PROC* glpfGetSamplerParameteriv = NULL;
PFNGLGETSAMPLERPARAMETERIIV_PROC* glpfGetSamplerParameterIiv = NULL;
PFNGLGETSAMPLERPARAMETERFV_PROC* glpfGetSamplerParameterfv = NULL;
PFNGLGETSAMPLERPARAMETERIUIV_PROC* glpfGetSamplerParameterIuiv = NULL;

/* GL_ARB_separate_shader_objects */

PFNGLUSEPROGRAMSTAGES_PROC* glpfUseProgramStages = NULL;
PFNGLACTIVESHADERPROGRAM_PROC* glpfActiveShaderProgram = NULL;
PFNGLCREATESHADERPROGRAMV_PROC* glpfCreateShaderProgramv = NULL;
PFNGLBINDPROGRAMPIPELINE_PROC* glpfBindProgramPipeline = NULL;
PFNGLDELETEPROGRAMPIPELINES_PROC* glpfDeleteProgramPipelines = NULL;
PFNGLGENPROGRAMPIPELINES_PROC* glpfGenProgramPipelines = NULL;
PFNGLISPROGRAMPIPELINE_PROC* glpfIsProgramPipeline = NULL;
PFNGLGETPROGRAMPIPELINEIV_PROC* glpfGetProgramPipelineiv = NULL;
PFNGLPROGRAMUNIFORM1I_PROC* glpfProgramUniform1i = NULL;
PFNGLPROGRAMUNIFORM1IV_PROC* glpfProgramUniform1iv = NULL;
PFNGLPROGRAMUNIFORM1F_PROC* glpfProgramUniform1f = NULL;
PFNGLPROGRAMUNIFORM1FV_PROC* glpfProgramUniform1fv = NULL;
PFNGLPROGRAMUNIFORM1D_PROC* glpfProgramUniform1d = NULL;
PFNGLPROGRAMUNIFORM1DV_PROC* glpfProgramUniform1dv = NULL;
PFNGLPROGRAMUNIFORM1UI_PROC* glpfProgramUniform1ui = NULL;
PFNGLPROGRAMUNIFORM1UIV_PROC* glpfProgramUniform1uiv = NULL;
PFNGLPROGRAMUNIFORM2I_PROC* glpfProgramUniform2i = NULL;
PFNGLPROGRAMUNIFORM2IV_PROC* glpfProgramUniform2iv = NULL;
PFNGLPROGRAMUNIFORM2F_PROC* glpfProgramUniform2f = NULL;
PFNGLPROGRAMUNIFORM2FV_PROC* glpfProgramUniform2fv = NULL;
PFNGLPROGRAMUNIFORM2D_PROC* glpfProgramUniform2d = NULL;
PFNGLPROGRAMUNIFORM2DV_PROC* glpfProgramUniform2dv = NULL;
PFNGLPROGRAMUNIFORM2UI_PROC* glpfProgramUniform2ui = NULL;
PFNGLPROGRAMUNIFORM2UIV_PROC* glpfProgramUniform2uiv = NULL;
PFNGLPROGRAMUNIFORM3I_PROC* glpfProgramUniform3i = NULL;
PFNGLPROGRAMUNIFORM3IV_PROC* glpfProgramUniform3iv = NULL;
PFNGLPROGRAMUNIFORM3F_PROC* glpfProgramUniform3f = NULL;
PFNGLPROGRAMUNIFORM3FV_PROC* glpfProgramUniform3fv = NULL;
PFNGLPROGRAMUNIFORM3D_PROC* glpfProgramUniform3d = NULL;
PFNGLPROGRAMUNIFORM3DV_PROC* glpfProgramUniform3dv = NULL;
PFNGLPROGRAMUNIFORM3UI_PROC* glpfProgramUniform3ui = NULL;
PFNGLPROGRAMUNIFORM3UIV_PROC* glpfProgramUniform3uiv = NULL;
PFNGLPROGRAMUNIFORM4I_PROC* glpfProgramUniform4i = NULL;
PFNGLPROGRAMUNIFORM4IV_PROC* glpfProgramUniform4iv = NULL;
PFNGLPROGRAMUNIFORM4F_PROC* glpfProgramUniform4f = NULL;
PFNGLPROGRAMUNIFORM4FV_PROC* glpfProgramUniform4fv = NULL;
PFNGLPROGRAMUNIFORM4D_PROC* glpfProgramUniform4d = NULL;
PFNGLPROGRAMUNIFORM4DV_PROC* glpfProgramUniform4dv = NULL;
PFNGLPROGRAMUNIFORM4UI_PROC* glpfProgramUniform4ui = NULL;
PFNGLPROGRAMUNIFORM4UIV_PROC* glpfProgramUniform4uiv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2FV_PROC* glpfProgramUniformMatrix2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3FV_PROC* glpfProgramUniformMatrix3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4FV_PROC* glpfProgramUniformMatrix4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2DV_PROC* glpfProgramUniformMatrix2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3DV_PROC* glpfProgramUniformMatrix3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4DV_PROC* glpfProgramUniformMatrix4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3FV_PROC* glpfProgramUniformMatrix2x3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2FV_PROC* glpfProgramUniformMatrix3x2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4FV_PROC* glpfProgramUniformMatrix2x4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2FV_PROC* glpfProgramUniformMatrix4x2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4FV_PROC* glpfProgramUniformMatrix3x4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3FV_PROC* glpfProgramUniformMatrix4x3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3DV_PROC* glpfProgramUniformMatrix2x3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2DV_PROC* glpfProgramUniformMatrix3x2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4DV_PROC* glpfProgramUniformMatrix2x4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2DV_PROC* glpfProgramUniformMatrix4x2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4DV_PROC* glpfProgramUniformMatrix3x4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3DV_PROC* glpfProgramUniformMatrix4x3dv = NULL;
PFNGLVALIDATEPROGRAMPIPELINE_PROC* glpfValidateProgramPipeline = NULL;
PFNGLGETPROGRAMPIPELINEINFOLOG_PROC* glpfGetProgramPipelineInfoLog = NULL;

/* GL_ARB_shader_atomic_counters */

PFNGLGETACTIVEATOMICCOUNTERBUFFERIV_PROC* glpfGetActiveAtomicCounterBufferiv = NULL;

/* GL_ARB_shader_image_load_store */

PFNGLBINDIMAGETEXTURE_PROC* glpfBindImageTexture = NULL;
PFNGLMEMORYBARRIER_PROC* glpfMemoryBarrier = NULL;

/* GL_ARB_shader_storage_buffer_object */

PFNGLSHADERSTORAGEBLOCKBINDING_PROC* glpfShaderStorageBlockBinding = NULL;

/* GL_ARB_shader_subroutine */

PFNGLGETSUBROUTINEUNIFORMLOCATION_PROC* glpfGetSubroutineUniformLocation = NULL;
PFNGLGETSUBROUTINEINDEX_PROC* glpfGetSubroutineIndex = NULL;
PFNGLGETACTIVESUBROUTINEUNIFORMIV_PROC* glpfGetActiveSubroutineUniformiv = NULL;
PFNGLGETACTIVESUBROUTINEUNIFORMNAME_PROC* glpfGetActiveSubroutineUniformName = NULL;
PFNGLGETACTIVESUBROUTINENAME_PROC* glpfGetActiveSubroutineName = NULL;
PFNGLUNIFORMSUBROUTINESUIV_PROC* glpfUniformSubroutinesuiv = NULL;
PFNGLGETUNIFORMSUBROUTINEUIV_PROC* glpfGetUniformSubroutineuiv = NULL;
PFNGLGETPROGRAMSTAGEIV_PROC* glpfGetProgramStageiv = NULL;

/* GL_ARB_sync */

PFNGLFENCESYNC_PROC* glpfFenceSync = NULL;
PFNGLISSYNC_PROC* glpfIsSync = NULL;
PFNGLDELETESYNC_PROC* glpfDeleteSync = NULL;
PFNGLCLIENTWAITSYNC_PROC* glpfClientWaitSync = NULL;
PFNGLWAITSYNC_PROC* glpfWaitSync = NULL;
PFNGLGETINTEGER64V_PROC* glpfGetInteger64v = NULL;
PFNGLGETSYNCIV_PROC* glpfGetSynciv = NULL;

/* GL_ARB_tessellation_shader */

PFNGLPATCHPARAMETERI_PROC* glpfPatchParameteri = NULL;
PFNGLPATCHPARAMETERFV_PROC* glpfPatchParameterfv = NULL;

/* GL_ARB_texture_buffer_range */

PFNGLTEXBUFFERRANGE_PROC* glpfTexBufferRange = NULL;
PFNGLTEXTUREBUFFERRANGEEXT_PROC* glpfTextureBufferRangeEXT = NULL;

/* GL_ARB_texture_multisample */

PFNGLTEXIMAGE2DMULTISAMPLE_PROC* glpfTexImage2DMultisample = NULL;
PFNGLTEXIMAGE3DMULTISAMPLE_PROC* glpfTexImage3DMultisample = NULL;
PFNGLGETMULTISAMPLEFV_PROC* glpfGetMultisamplefv = NULL;
PFNGLSAMPLEMASKI_PROC* glpfSampleMaski = NULL;

/* GL_ARB_texture_storage */

PFNGLTEXSTORAGE1D_PROC* glpfTexStorage1D = NULL;
PFNGLTEXSTORAGE2D_PROC* glpfTexStorage2D = NULL;
PFNGLTEXSTORAGE3D_PROC* glpfTexStorage3D = NULL;
PFNGLTEXTURESTORAGE1DEXT_PROC* glpfTextureStorage1DEXT = NULL;
PFNGLTEXTURESTORAGE2DEXT_PROC* glpfTextureStorage2DEXT = NULL;
PFNGLTEXTURESTORAGE3DEXT_PROC* glpfTextureStorage3DEXT = NULL;

/* GL_ARB_texture_storage_multisample */

PFNGLTEXSTORAGE2DMULTISAMPLE_PROC* glpfTexStorage2DMultisample = NULL;
PFNGLTEXSTORAGE3DMULTISAMPLE_PROC* glpfTexStorage3DMultisample = NULL;
PFNGLTEXTURESTORAGE2DMULTISAMPLEEXT_PROC* glpfTextureStorage2DMultisampleEXT = NULL;
PFNGLTEXTURESTORAGE3DMULTISAMPLEEXT_PROC* glpfTextureStorage3DMultisampleEXT = NULL;

/* GL_ARB_texture_view */

PFNGLTEXTUREVIEW_PROC* glpfTextureView = NULL;

/* GL_ARB_timer_query */

PFNGLQUERYCOUNTER_PROC* glpfQueryCounter = NULL;
PFNGLGETQUERYOBJECTI64V_PROC* glpfGetQueryObjecti64v = NULL;
PFNGLGETQUERYOBJECTUI64V_PROC* glpfGetQueryObjectui64v = NULL;

/* GL_ARB_transform_feedback2 */

PFNGLBINDTRANSFORMFEEDBACK_PROC* glpfBindTransformFeedback = NULL;
PFNGLDELETETRANSFORMFEEDBACKS_PROC* glpfDeleteTransformFeedbacks = NULL;
PFNGLGENTRANSFORMFEEDBACKS_PROC* glpfGenTransformFeedbacks = NULL;
PFNGLISTRANSFORMFEEDBACK_PROC* glpfIsTransformFeedback = NULL;
PFNGLPAUSETRANSFORMFEEDBACK_PROC* glpfPauseTransformFeedback = NULL;
PFNGLRESUMETRANSFORMFEEDBACK_PROC* glpfResumeTransformFeedback = NULL;
PFNGLDRAWTRANSFORMFEEDBACK_PROC* glpfDrawTransformFeedback = NULL;

/* GL_ARB_transform_feedback3 */

PFNGLDRAWTRANSFORMFEEDBACKSTREAM_PROC* glpfDrawTransformFeedbackStream = NULL;
PFNGLBEGINQUERYINDEXED_PROC* glpfBeginQueryIndexed = NULL;
PFNGLENDQUERYINDEXED_PROC* glpfEndQueryIndexed = NULL;
PFNGLGETQUERYINDEXEDIV_PROC* glpfGetQueryIndexediv = NULL;

/* GL_ARB_transform_feedback_instanced */

PFNGLDRAWTRANSFORMFEEDBACKINSTANCED_PROC* glpfDrawTransformFeedbackInstanced = NULL;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCED_PROC* glpfDrawTransformFeedbackStreamInstanced = NULL;

/* GL_ARB_uniform_buffer_object */

PFNGLGETUNIFORMINDICES_PROC* glpfGetUniformIndices = NULL;
PFNGLGETACTIVEUNIFORMSIV_PROC* glpfGetActiveUniformsiv = NULL;
PFNGLGETACTIVEUNIFORMNAME_PROC* glpfGetActiveUniformName = NULL;
PFNGLGETUNIFORMBLOCKINDEX_PROC* glpfGetUniformBlockIndex = NULL;
PFNGLGETACTIVEUNIFORMBLOCKIV_PROC* glpfGetActiveUniformBlockiv = NULL;
PFNGLGETACTIVEUNIFORMBLOCKNAME_PROC* glpfGetActiveUniformBlockName = NULL;
PFNGLUNIFORMBLOCKBINDING_PROC* glpfUniformBlockBinding = NULL;

/* GL_ARB_vertex_array_object */

PFNGLBINDVERTEXARRAY_PROC* glpfBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYS_PROC* glpfDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYS_PROC* glpfGenVertexArrays = NULL;
PFNGLISVERTEXARRAY_PROC* glpfIsVertexArray = NULL;

/* GL_ARB_vertex_attrib_64bit */

PFNGLVERTEXATTRIBL1D_PROC* glpfVertexAttribL1d = NULL;
PFNGLVERTEXATTRIBL2D_PROC* glpfVertexAttribL2d = NULL;
PFNGLVERTEXATTRIBL3D_PROC* glpfVertexAttribL3d = NULL;
PFNGLVERTEXATTRIBL4D_PROC* glpfVertexAttribL4d = NULL;
PFNGLVERTEXATTRIBL1DV_PROC* glpfVertexAttribL1dv = NULL;
PFNGLVERTEXATTRIBL2DV_PROC* glpfVertexAttribL2dv = NULL;
PFNGLVERTEXATTRIBL3DV_PROC* glpfVertexAttribL3dv = NULL;
PFNGLVERTEXATTRIBL4DV_PROC* glpfVertexAttribL4dv = NULL;
PFNGLVERTEXATTRIBLPOINTER_PROC* glpfVertexAttribLPointer = NULL;
PFNGLGETVERTEXATTRIBLDV_PROC* glpfGetVertexAttribLdv = NULL;

/* GL_ARB_vertex_attrib_binding */

PFNGLBINDVERTEXBUFFER_PROC* glpfBindVertexBuffer = NULL;
PFNGLVERTEXATTRIBFORMAT_PROC* glpfVertexAttribFormat = NULL;
PFNGLVERTEXATTRIBIFORMAT_PROC* glpfVertexAttribIFormat = NULL;
PFNGLVERTEXATTRIBLFORMAT_PROC* glpfVertexAttribLFormat = NULL;
PFNGLVERTEXATTRIBBINDING_PROC* glpfVertexAttribBinding = NULL;
PFNGLVERTEXBINDINGDIVISOR_PROC* glpfVertexBindingDivisor = NULL;
PFNGLVERTEXARRAYBINDVERTEXBUFFEREXT_PROC* glpfVertexArrayBindVertexBufferEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBFORMATEXT_PROC* glpfVertexArrayVertexAttribFormatEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBIFORMATEXT_PROC* glpfVertexArrayVertexAttribIFormatEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBLFORMATEXT_PROC* glpfVertexArrayVertexAttribLFormatEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBBINDINGEXT_PROC* glpfVertexArrayVertexAttribBindingEXT = NULL;
PFNGLVERTEXARRAYVERTEXBINDINGDIVISOREXT_PROC* glpfVertexArrayVertexBindingDivisorEXT = NULL;

/* GL_ARB_vertex_type_2_10_10_10_rev */

PFNGLVERTEXP2UI_PROC* glpfVertexP2ui = NULL;
PFNGLVERTEXP2UIV_PROC* glpfVertexP2uiv = NULL;
PFNGLVERTEXP3UI_PROC* glpfVertexP3ui = NULL;
PFNGLVERTEXP3UIV_PROC* glpfVertexP3uiv = NULL;
PFNGLVERTEXP4UI_PROC* glpfVertexP4ui = NULL;
PFNGLVERTEXP4UIV_PROC* glpfVertexP4uiv = NULL;
PFNGLTEXCOORDP1UI_PROC* glpfTexCoordP1ui = NULL;
PFNGLTEXCOORDP1UIV_PROC* glpfTexCoordP1uiv = NULL;
PFNGLTEXCOORDP2UI_PROC* glpfTexCoordP2ui = NULL;
PFNGLTEXCOORDP2UIV_PROC* glpfTexCoordP2uiv = NULL;
PFNGLTEXCOORDP3UI_PROC* glpfTexCoordP3ui = NULL;
PFNGLTEXCOORDP3UIV_PROC* glpfTexCoordP3uiv = NULL;
PFNGLTEXCOORDP4UI_PROC* glpfTexCoordP4ui = NULL;
PFNGLTEXCOORDP4UIV_PROC* glpfTexCoordP4uiv = NULL;
PFNGLMULTITEXCOORDP1UI_PROC* glpfMultiTexCoordP1ui = NULL;
PFNGLMULTITEXCOORDP1UIV_PROC* glpfMultiTexCoordP1uiv = NULL;
PFNGLMULTITEXCOORDP2UI_PROC* glpfMultiTexCoordP2ui = NULL;
PFNGLMULTITEXCOORDP2UIV_PROC* glpfMultiTexCoordP2uiv = NULL;
PFNGLMULTITEXCOORDP3UI_PROC* glpfMultiTexCoordP3ui = NULL;
PFNGLMULTITEXCOORDP3UIV_PROC* glpfMultiTexCoordP3uiv = NULL;
PFNGLMULTITEXCOORDP4UI_PROC* glpfMultiTexCoordP4ui = NULL;
PFNGLMULTITEXCOORDP4UIV_PROC* glpfMultiTexCoordP4uiv = NULL;
PFNGLNORMALP3UI_PROC* glpfNormalP3ui = NULL;
PFNGLNORMALP3UIV_PROC* glpfNormalP3uiv = NULL;
PFNGLCOLORP3UI_PROC* glpfColorP3ui = NULL;
PFNGLCOLORP3UIV_PROC* glpfColorP3uiv = NULL;
PFNGLCOLORP4UI_PROC* glpfColorP4ui = NULL;
PFNGLCOLORP4UIV_PROC* glpfColorP4uiv = NULL;
PFNGLSECONDARYCOLORP3UI_PROC* glpfSecondaryColorP3ui = NULL;
PFNGLSECONDARYCOLORP3UIV_PROC* glpfSecondaryColorP3uiv = NULL;
PFNGLVERTEXATTRIBP1UI_PROC* glpfVertexAttribP1ui = NULL;
PFNGLVERTEXATTRIBP1UIV_PROC* glpfVertexAttribP1uiv = NULL;
PFNGLVERTEXATTRIBP2UI_PROC* glpfVertexAttribP2ui = NULL;
PFNGLVERTEXATTRIBP2UIV_PROC* glpfVertexAttribP2uiv = NULL;
PFNGLVERTEXATTRIBP3UI_PROC* glpfVertexAttribP3ui = NULL;
PFNGLVERTEXATTRIBP3UIV_PROC* glpfVertexAttribP3uiv = NULL;
PFNGLVERTEXATTRIBP4UI_PROC* glpfVertexAttribP4ui = NULL;
PFNGLVERTEXATTRIBP4UIV_PROC* glpfVertexAttribP4uiv = NULL;

/* GL_ARB_viewport_array */

PFNGLVIEWPORTARRAYV_PROC* glpfViewportArrayv = NULL;
PFNGLVIEWPORTINDEXEDF_PROC* glpfViewportIndexedf = NULL;
PFNGLVIEWPORTINDEXEDFV_PROC* glpfViewportIndexedfv = NULL;
PFNGLSCISSORARRAYV_PROC* glpfScissorArrayv = NULL;
PFNGLSCISSORINDEXED_PROC* glpfScissorIndexed = NULL;
PFNGLSCISSORINDEXEDV_PROC* glpfScissorIndexedv = NULL;
PFNGLDEPTHRANGEARRAYV_PROC* glpfDepthRangeArrayv = NULL;
PFNGLDEPTHRANGEINDEXED_PROC* glpfDepthRangeIndexed = NULL;
PFNGLGETFLOATI_V_PROC* glpfGetFloati_v = NULL;
PFNGLGETDOUBLEI_V_PROC* glpfGetDoublei_v = NULL;

/* GL_EXT_direct_state_access */

PFNGLCLIENTATTRIBDEFAULTEXT_PROC* glpfClientAttribDefaultEXT = NULL;
PFNGLPUSHCLIENTATTRIBDEFAULTEXT_PROC* glpfPushClientAttribDefaultEXT = NULL;
PFNGLMATRIXLOADFEXT_PROC* glpfMatrixLoadfEXT = NULL;
PFNGLMATRIXLOADDEXT_PROC* glpfMatrixLoaddEXT = NULL;
PFNGLMATRIXMULTFEXT_PROC* glpfMatrixMultfEXT = NULL;
PFNGLMATRIXMULTDEXT_PROC* glpfMatrixMultdEXT = NULL;
PFNGLMATRIXLOADIDENTITYEXT_PROC* glpfMatrixLoadIdentityEXT = NULL;
PFNGLMATRIXROTATEFEXT_PROC* glpfMatrixRotatefEXT = NULL;
PFNGLMATRIXROTATEDEXT_PROC* glpfMatrixRotatedEXT = NULL;
PFNGLMATRIXSCALEFEXT_PROC* glpfMatrixScalefEXT = NULL;
PFNGLMATRIXSCALEDEXT_PROC* glpfMatrixScaledEXT = NULL;
PFNGLMATRIXTRANSLATEFEXT_PROC* glpfMatrixTranslatefEXT = NULL;
PFNGLMATRIXTRANSLATEDEXT_PROC* glpfMatrixTranslatedEXT = NULL;
PFNGLMATRIXFRUSTUMEXT_PROC* glpfMatrixFrustumEXT = NULL;
PFNGLMATRIXORTHOEXT_PROC* glpfMatrixOrthoEXT = NULL;
PFNGLMATRIXPOPEXT_PROC* glpfMatrixPopEXT = NULL;
PFNGLMATRIXPUSHEXT_PROC* glpfMatrixPushEXT = NULL;
PFNGLMATRIXLOADTRANSPOSEFEXT_PROC* glpfMatrixLoadTransposefEXT = NULL;
PFNGLMATRIXLOADTRANSPOSEDEXT_PROC* glpfMatrixLoadTransposedEXT = NULL;
PFNGLMATRIXMULTTRANSPOSEFEXT_PROC* glpfMatrixMultTransposefEXT = NULL;
PFNGLMATRIXMULTTRANSPOSEDEXT_PROC* glpfMatrixMultTransposedEXT = NULL;
PFNGLTEXTUREPARAMETERFEXT_PROC* glpfTextureParameterfEXT = NULL;
PFNGLTEXTUREPARAMETERFVEXT_PROC* glpfTextureParameterfvEXT = NULL;
PFNGLTEXTUREPARAMETERIEXT_PROC* glpfTextureParameteriEXT = NULL;
PFNGLTEXTUREPARAMETERIVEXT_PROC* glpfTextureParameterivEXT = NULL;
PFNGLTEXTUREIMAGE1DEXT_PROC* glpfTextureImage1DEXT = NULL;
PFNGLTEXTUREIMAGE2DEXT_PROC* glpfTextureImage2DEXT = NULL;
PFNGLTEXTURESUBIMAGE1DEXT_PROC* glpfTextureSubImage1DEXT = NULL;
PFNGLTEXTURESUBIMAGE2DEXT_PROC* glpfTextureSubImage2DEXT = NULL;
PFNGLCOPYTEXTUREIMAGE1DEXT_PROC* glpfCopyTextureImage1DEXT = NULL;
PFNGLCOPYTEXTUREIMAGE2DEXT_PROC* glpfCopyTextureImage2DEXT = NULL;
PFNGLCOPYTEXTURESUBIMAGE1DEXT_PROC* glpfCopyTextureSubImage1DEXT = NULL;
PFNGLCOPYTEXTURESUBIMAGE2DEXT_PROC* glpfCopyTextureSubImage2DEXT = NULL;
PFNGLGETTEXTUREIMAGEEXT_PROC* glpfGetTextureImageEXT = NULL;
PFNGLGETTEXTUREPARAMETERFVEXT_PROC* glpfGetTextureParameterfvEXT = NULL;
PFNGLGETTEXTUREPARAMETERIVEXT_PROC* glpfGetTextureParameterivEXT = NULL;
PFNGLGETTEXTURELEVELPARAMETERFVEXT_PROC* glpfGetTextureLevelParameterfvEXT = NULL;
PFNGLGETTEXTURELEVELPARAMETERIVEXT_PROC* glpfGetTextureLevelParameterivEXT = NULL;
PFNGLTEXTUREIMAGE3DEXT_PROC* glpfTextureImage3DEXT = NULL;
PFNGLTEXTURESUBIMAGE3DEXT_PROC* glpfTextureSubImage3DEXT = NULL;
PFNGLCOPYTEXTURESUBIMAGE3DEXT_PROC* glpfCopyTextureSubImage3DEXT = NULL;
PFNGLMULTITEXPARAMETERFEXT_PROC* glpfMultiTexParameterfEXT = NULL;
PFNGLMULTITEXPARAMETERFVEXT_PROC* glpfMultiTexParameterfvEXT = NULL;
PFNGLMULTITEXPARAMETERIEXT_PROC* glpfMultiTexParameteriEXT = NULL;
PFNGLMULTITEXPARAMETERIVEXT_PROC* glpfMultiTexParameterivEXT = NULL;
PFNGLMULTITEXIMAGE1DEXT_PROC* glpfMultiTexImage1DEXT = NULL;
PFNGLMULTITEXIMAGE2DEXT_PROC* glpfMultiTexImage2DEXT = NULL;
PFNGLMULTITEXSUBIMAGE1DEXT_PROC* glpfMultiTexSubImage1DEXT = NULL;
PFNGLMULTITEXSUBIMAGE2DEXT_PROC* glpfMultiTexSubImage2DEXT = NULL;
PFNGLCOPYMULTITEXIMAGE1DEXT_PROC* glpfCopyMultiTexImage1DEXT = NULL;
PFNGLCOPYMULTITEXIMAGE2DEXT_PROC* glpfCopyMultiTexImage2DEXT = NULL;
PFNGLCOPYMULTITEXSUBIMAGE1DEXT_PROC* glpfCopyMultiTexSubImage1DEXT = NULL;
PFNGLCOPYMULTITEXSUBIMAGE2DEXT_PROC* glpfCopyMultiTexSubImage2DEXT = NULL;
PFNGLGETMULTITEXIMAGEEXT_PROC* glpfGetMultiTexImageEXT = NULL;
PFNGLGETMULTITEXPARAMETERFVEXT_PROC* glpfGetMultiTexParameterfvEXT = NULL;
PFNGLGETMULTITEXPARAMETERIVEXT_PROC* glpfGetMultiTexParameterivEXT = NULL;
PFNGLGETMULTITEXLEVELPARAMETERFVEXT_PROC* glpfGetMultiTexLevelParameterfvEXT = NULL;
PFNGLGETMULTITEXLEVELPARAMETERIVEXT_PROC* glpfGetMultiTexLevelParameterivEXT = NULL;
PFNGLMULTITEXIMAGE3DEXT_PROC* glpfMultiTexImage3DEXT = NULL;
PFNGLMULTITEXSUBIMAGE3DEXT_PROC* glpfMultiTexSubImage3DEXT = NULL;
PFNGLCOPYMULTITEXSUBIMAGE3DEXT_PROC* glpfCopyMultiTexSubImage3DEXT = NULL;
PFNGLBINDMULTITEXTUREEXT_PROC* glpfBindMultiTextureEXT = NULL;
PFNGLENABLECLIENTSTATEINDEXEDEXT_PROC* glpfEnableClientStateIndexedEXT = NULL;
PFNGLDISABLECLIENTSTATEINDEXEDEXT_PROC* glpfDisableClientStateIndexedEXT = NULL;
PFNGLMULTITEXCOORDPOINTEREXT_PROC* glpfMultiTexCoordPointerEXT = NULL;
PFNGLMULTITEXENVFEXT_PROC* glpfMultiTexEnvfEXT = NULL;
PFNGLMULTITEXENVFVEXT_PROC* glpfMultiTexEnvfvEXT = NULL;
PFNGLMULTITEXENVIEXT_PROC* glpfMultiTexEnviEXT = NULL;
PFNGLMULTITEXENVIVEXT_PROC* glpfMultiTexEnvivEXT = NULL;
PFNGLMULTITEXGENDEXT_PROC* glpfMultiTexGendEXT = NULL;
PFNGLMULTITEXGENDVEXT_PROC* glpfMultiTexGendvEXT = NULL;
PFNGLMULTITEXGENFEXT_PROC* glpfMultiTexGenfEXT = NULL;
PFNGLMULTITEXGENFVEXT_PROC* glpfMultiTexGenfvEXT = NULL;
PFNGLMULTITEXGENIEXT_PROC* glpfMultiTexGeniEXT = NULL;
PFNGLMULTITEXGENIVEXT_PROC* glpfMultiTexGenivEXT = NULL;
PFNGLGETMULTITEXENVFVEXT_PROC* glpfGetMultiTexEnvfvEXT = NULL;
PFNGLGETMULTITEXENVIVEXT_PROC* glpfGetMultiTexEnvivEXT = NULL;
PFNGLGETMULTITEXGENDVEXT_PROC* glpfGetMultiTexGendvEXT = NULL;
PFNGLGETMULTITEXGENFVEXT_PROC* glpfGetMultiTexGenfvEXT = NULL;
PFNGLGETMULTITEXGENIVEXT_PROC* glpfGetMultiTexGenivEXT = NULL;
PFNGLGETFLOATINDEXEDVEXT_PROC* glpfGetFloatIndexedvEXT = NULL;
PFNGLGETDOUBLEINDEXEDVEXT_PROC* glpfGetDoubleIndexedvEXT = NULL;
PFNGLGETPOINTERINDEXEDVEXT_PROC* glpfGetPointerIndexedvEXT = NULL;
PFNGLCOMPRESSEDTEXTUREIMAGE3DEXT_PROC* glpfCompressedTextureImage3DEXT = NULL;
PFNGLCOMPRESSEDTEXTUREIMAGE2DEXT_PROC* glpfCompressedTextureImage2DEXT = NULL;
PFNGLCOMPRESSEDTEXTUREIMAGE1DEXT_PROC* glpfCompressedTextureImage1DEXT = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXT_PROC* glpfCompressedTextureSubImage3DEXT = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXT_PROC* glpfCompressedTextureSubImage2DEXT = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXT_PROC* glpfCompressedTextureSubImage1DEXT = NULL;
PFNGLGETCOMPRESSEDTEXTUREIMAGEEXT_PROC* glpfGetCompressedTextureImageEXT = NULL;
PFNGLCOMPRESSEDMULTITEXIMAGE3DEXT_PROC* glpfCompressedMultiTexImage3DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXIMAGE2DEXT_PROC* glpfCompressedMultiTexImage2DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXIMAGE1DEXT_PROC* glpfCompressedMultiTexImage1DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXT_PROC* glpfCompressedMultiTexSubImage3DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXT_PROC* glpfCompressedMultiTexSubImage2DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXT_PROC* glpfCompressedMultiTexSubImage1DEXT = NULL;
PFNGLGETCOMPRESSEDMULTITEXIMAGEEXT_PROC* glpfGetCompressedMultiTexImageEXT = NULL;
PFNGLNAMEDPROGRAMSTRINGEXT_PROC* glpfNamedProgramStringEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXT_PROC* glpfNamedProgramLocalParameter4dEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXT_PROC* glpfNamedProgramLocalParameter4dvEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXT_PROC* glpfNamedProgramLocalParameter4fEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXT_PROC* glpfNamedProgramLocalParameter4fvEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXT_PROC* glpfGetNamedProgramLocalParameterdvEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXT_PROC* glpfGetNamedProgramLocalParameterfvEXT = NULL;
PFNGLGETNAMEDPROGRAMIVEXT_PROC* glpfGetNamedProgramivEXT = NULL;
PFNGLGETNAMEDPROGRAMSTRINGEXT_PROC* glpfGetNamedProgramStringEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXT_PROC* glpfNamedProgramLocalParameters4fvEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXT_PROC* glpfNamedProgramLocalParameterI4iEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXT_PROC* glpfNamedProgramLocalParameterI4ivEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXT_PROC* glpfNamedProgramLocalParametersI4ivEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXT_PROC* glpfNamedProgramLocalParameterI4uiEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXT_PROC* glpfNamedProgramLocalParameterI4uivEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXT_PROC* glpfNamedProgramLocalParametersI4uivEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXT_PROC* glpfGetNamedProgramLocalParameterIivEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXT_PROC* glpfGetNamedProgramLocalParameterIuivEXT = NULL;
PFNGLTEXTUREPARAMETERIIVEXT_PROC* glpfTextureParameterIivEXT = NULL;
PFNGLTEXTUREPARAMETERIUIVEXT_PROC* glpfTextureParameterIuivEXT = NULL;
PFNGLGETTEXTUREPARAMETERIIVEXT_PROC* glpfGetTextureParameterIivEXT = NULL;
PFNGLGETTEXTUREPARAMETERIUIVEXT_PROC* glpfGetTextureParameterIuivEXT = NULL;
PFNGLMULTITEXPARAMETERIIVEXT_PROC* glpfMultiTexParameterIivEXT = NULL;
PFNGLMULTITEXPARAMETERIUIVEXT_PROC* glpfMultiTexParameterIuivEXT = NULL;
PFNGLGETMULTITEXPARAMETERIIVEXT_PROC* glpfGetMultiTexParameterIivEXT = NULL;
PFNGLGETMULTITEXPARAMETERIUIVEXT_PROC* glpfGetMultiTexParameterIuivEXT = NULL;
PFNGLPROGRAMUNIFORM1FEXT_PROC* glpfProgramUniform1fEXT = NULL;
PFNGLPROGRAMUNIFORM2FEXT_PROC* glpfProgramUniform2fEXT = NULL;
PFNGLPROGRAMUNIFORM3FEXT_PROC* glpfProgramUniform3fEXT = NULL;
PFNGLPROGRAMUNIFORM4FEXT_PROC* glpfProgramUniform4fEXT = NULL;
PFNGLPROGRAMUNIFORM1IEXT_PROC* glpfProgramUniform1iEXT = NULL;
PFNGLPROGRAMUNIFORM2IEXT_PROC* glpfProgramUniform2iEXT = NULL;
PFNGLPROGRAMUNIFORM3IEXT_PROC* glpfProgramUniform3iEXT = NULL;
PFNGLPROGRAMUNIFORM4IEXT_PROC* glpfProgramUniform4iEXT = NULL;
PFNGLPROGRAMUNIFORM1FVEXT_PROC* glpfProgramUniform1fvEXT = NULL;
PFNGLPROGRAMUNIFORM2FVEXT_PROC* glpfProgramUniform2fvEXT = NULL;
PFNGLPROGRAMUNIFORM3FVEXT_PROC* glpfProgramUniform3fvEXT = NULL;
PFNGLPROGRAMUNIFORM4FVEXT_PROC* glpfProgramUniform4fvEXT = NULL;
PFNGLPROGRAMUNIFORM1IVEXT_PROC* glpfProgramUniform1ivEXT = NULL;
PFNGLPROGRAMUNIFORM2IVEXT_PROC* glpfProgramUniform2ivEXT = NULL;
PFNGLPROGRAMUNIFORM3IVEXT_PROC* glpfProgramUniform3ivEXT = NULL;
PFNGLPROGRAMUNIFORM4IVEXT_PROC* glpfProgramUniform4ivEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2FVEXT_PROC* glpfProgramUniformMatrix2fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3FVEXT_PROC* glpfProgramUniformMatrix3fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4FVEXT_PROC* glpfProgramUniformMatrix4fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3FVEXT_PROC* glpfProgramUniformMatrix2x3fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2FVEXT_PROC* glpfProgramUniformMatrix3x2fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4FVEXT_PROC* glpfProgramUniformMatrix2x4fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2FVEXT_PROC* glpfProgramUniformMatrix4x2fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4FVEXT_PROC* glpfProgramUniformMatrix3x4fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3FVEXT_PROC* glpfProgramUniformMatrix4x3fvEXT = NULL;
PFNGLPROGRAMUNIFORM1UIEXT_PROC* glpfProgramUniform1uiEXT = NULL;
PFNGLPROGRAMUNIFORM2UIEXT_PROC* glpfProgramUniform2uiEXT = NULL;
PFNGLPROGRAMUNIFORM3UIEXT_PROC* glpfProgramUniform3uiEXT = NULL;
PFNGLPROGRAMUNIFORM4UIEXT_PROC* glpfProgramUniform4uiEXT = NULL;
PFNGLPROGRAMUNIFORM1UIVEXT_PROC* glpfProgramUniform1uivEXT = NULL;
PFNGLPROGRAMUNIFORM2UIVEXT_PROC* glpfProgramUniform2uivEXT = NULL;
PFNGLPROGRAMUNIFORM3UIVEXT_PROC* glpfProgramUniform3uivEXT = NULL;
PFNGLPROGRAMUNIFORM4UIVEXT_PROC* glpfProgramUniform4uivEXT = NULL;
PFNGLNAMEDBUFFERDATAEXT_PROC* glpfNamedBufferDataEXT = NULL;
PFNGLNAMEDBUFFERSUBDATAEXT_PROC* glpfNamedBufferSubDataEXT = NULL;
PFNGLMAPNAMEDBUFFEREXT_PROC* glpfMapNamedBufferEXT = NULL;
PFNGLUNMAPNAMEDBUFFEREXT_PROC* glpfUnmapNamedBufferEXT = NULL;
PFNGLMAPNAMEDBUFFERRANGEEXT_PROC* glpfMapNamedBufferRangeEXT = NULL;
PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXT_PROC* glpfFlushMappedNamedBufferRangeEXT = NULL;
PFNGLNAMEDCOPYBUFFERSUBDATAEXT_PROC* glpfNamedCopyBufferSubDataEXT = NULL;
PFNGLGETNAMEDBUFFERPARAMETERIVEXT_PROC* glpfGetNamedBufferParameterivEXT = NULL;
PFNGLGETNAMEDBUFFERPOINTERVEXT_PROC* glpfGetNamedBufferPointervEXT = NULL;
PFNGLGETNAMEDBUFFERSUBDATAEXT_PROC* glpfGetNamedBufferSubDataEXT = NULL;
PFNGLTEXTUREBUFFEREXT_PROC* glpfTextureBufferEXT = NULL;
PFNGLMULTITEXBUFFEREXT_PROC* glpfMultiTexBufferEXT = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEEXT_PROC* glpfNamedRenderbufferStorageEXT = NULL;
PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXT_PROC* glpfGetNamedRenderbufferParameterivEXT = NULL;
PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXT_PROC* glpfCheckNamedFramebufferStatusEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXT_PROC* glpfNamedFramebufferTexture1DEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXT_PROC* glpfNamedFramebufferTexture2DEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXT_PROC* glpfNamedFramebufferTexture3DEXT = NULL;
PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXT_PROC* glpfNamedFramebufferRenderbufferEXT = NULL;
PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXT_PROC* glpfGetNamedFramebufferAttachmentParameterivEXT = NULL;
PFNGLGENERATETEXTUREMIPMAPEXT_PROC* glpfGenerateTextureMipmapEXT = NULL;
PFNGLGENERATEMULTITEXMIPMAPEXT_PROC* glpfGenerateMultiTexMipmapEXT = NULL;
PFNGLFRAMEBUFFERDRAWBUFFEREXT_PROC* glpfFramebufferDrawBufferEXT = NULL;
PFNGLFRAMEBUFFERDRAWBUFFERSEXT_PROC* glpfFramebufferDrawBuffersEXT = NULL;
PFNGLFRAMEBUFFERREADBUFFEREXT_PROC* glpfFramebufferReadBufferEXT = NULL;
PFNGLGETFRAMEBUFFERPARAMETERIVEXT_PROC* glpfGetFramebufferParameterivEXT = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXT_PROC* glpfNamedRenderbufferStorageMultisampleEXT = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXT_PROC* glpfNamedRenderbufferStorageMultisampleCoverageEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTUREEXT_PROC* glpfNamedFramebufferTextureEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXT_PROC* glpfNamedFramebufferTextureLayerEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXT_PROC* glpfNamedFramebufferTextureFaceEXT = NULL;
PFNGLTEXTURERENDERBUFFEREXT_PROC* glpfTextureRenderbufferEXT = NULL;
PFNGLMULTITEXRENDERBUFFEREXT_PROC* glpfMultiTexRenderbufferEXT = NULL;
PFNGLPROGRAMUNIFORM1DEXT_PROC* glpfProgramUniform1dEXT = NULL;
PFNGLPROGRAMUNIFORM2DEXT_PROC* glpfProgramUniform2dEXT = NULL;
PFNGLPROGRAMUNIFORM3DEXT_PROC* glpfProgramUniform3dEXT = NULL;
PFNGLPROGRAMUNIFORM4DEXT_PROC* glpfProgramUniform4dEXT = NULL;
PFNGLPROGRAMUNIFORM1DVEXT_PROC* glpfProgramUniform1dvEXT = NULL;
PFNGLPROGRAMUNIFORM2DVEXT_PROC* glpfProgramUniform2dvEXT = NULL;
PFNGLPROGRAMUNIFORM3DVEXT_PROC* glpfProgramUniform3dvEXT = NULL;
PFNGLPROGRAMUNIFORM4DVEXT_PROC* glpfProgramUniform4dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2DVEXT_PROC* glpfProgramUniformMatrix2dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3DVEXT_PROC* glpfProgramUniformMatrix3dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4DVEXT_PROC* glpfProgramUniformMatrix4dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3DVEXT_PROC* glpfProgramUniformMatrix2x3dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4DVEXT_PROC* glpfProgramUniformMatrix2x4dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2DVEXT_PROC* glpfProgramUniformMatrix3x2dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4DVEXT_PROC* glpfProgramUniformMatrix3x4dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2DVEXT_PROC* glpfProgramUniformMatrix4x2dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3DVEXT_PROC* glpfProgramUniformMatrix4x3dvEXT = NULL;

/* GL_VERSION_1_2 */

PFNGLBLENDCOLOR_PROC* glpfBlendColor = NULL;
PFNGLBLENDEQUATION_PROC* glpfBlendEquation = NULL;
PFNGLDRAWRANGEELEMENTS_PROC* glpfDrawRangeElements = NULL;
PFNGLTEXIMAGE3D_PROC* glpfTexImage3D = NULL;
PFNGLTEXSUBIMAGE3D_PROC* glpfTexSubImage3D = NULL;
PFNGLCOPYTEXSUBIMAGE3D_PROC* glpfCopyTexSubImage3D = NULL;
PFNGLCOLORTABLE_PROC* glpfColorTable = NULL;
PFNGLCOLORTABLEPARAMETERFV_PROC* glpfColorTableParameterfv = NULL;
PFNGLCOLORTABLEPARAMETERIV_PROC* glpfColorTableParameteriv = NULL;
PFNGLCOPYCOLORTABLE_PROC* glpfCopyColorTable = NULL;
PFNGLGETCOLORTABLE_PROC* glpfGetColorTable = NULL;
PFNGLGETCOLORTABLEPARAMETERFV_PROC* glpfGetColorTableParameterfv = NULL;
PFNGLGETCOLORTABLEPARAMETERIV_PROC* glpfGetColorTableParameteriv = NULL;
PFNGLCOLORSUBTABLE_PROC* glpfColorSubTable = NULL;
PFNGLCOPYCOLORSUBTABLE_PROC* glpfCopyColorSubTable = NULL;
PFNGLCONVOLUTIONFILTER1D_PROC* glpfConvolutionFilter1D = NULL;
PFNGLCONVOLUTIONFILTER2D_PROC* glpfConvolutionFilter2D = NULL;
PFNGLCONVOLUTIONPARAMETERF_PROC* glpfConvolutionParameterf = NULL;
PFNGLCONVOLUTIONPARAMETERFV_PROC* glpfConvolutionParameterfv = NULL;
PFNGLCONVOLUTIONPARAMETERI_PROC* glpfConvolutionParameteri = NULL;
PFNGLCONVOLUTIONPARAMETERIV_PROC* glpfConvolutionParameteriv = NULL;
PFNGLCOPYCONVOLUTIONFILTER1D_PROC* glpfCopyConvolutionFilter1D = NULL;
PFNGLCOPYCONVOLUTIONFILTER2D_PROC* glpfCopyConvolutionFilter2D = NULL;
PFNGLGETCONVOLUTIONFILTER_PROC* glpfGetConvolutionFilter = NULL;
PFNGLGETCONVOLUTIONPARAMETERFV_PROC* glpfGetConvolutionParameterfv = NULL;
PFNGLGETCONVOLUTIONPARAMETERIV_PROC* glpfGetConvolutionParameteriv = NULL;
PFNGLGETSEPARABLEFILTER_PROC* glpfGetSeparableFilter = NULL;
PFNGLSEPARABLEFILTER2D_PROC* glpfSeparableFilter2D = NULL;
PFNGLGETHISTOGRAM_PROC* glpfGetHistogram = NULL;
PFNGLGETHISTOGRAMPARAMETERFV_PROC* glpfGetHistogramParameterfv = NULL;
PFNGLGETHISTOGRAMPARAMETERIV_PROC* glpfGetHistogramParameteriv = NULL;
PFNGLGETMINMAX_PROC* glpfGetMinmax = NULL;
PFNGLGETMINMAXPARAMETERFV_PROC* glpfGetMinmaxParameterfv = NULL;
PFNGLGETMINMAXPARAMETERIV_PROC* glpfGetMinmaxParameteriv = NULL;
PFNGLHISTOGRAM_PROC* glpfHistogram = NULL;
PFNGLMINMAX_PROC* glpfMinmax = NULL;
PFNGLRESETHISTOGRAM_PROC* glpfResetHistogram = NULL;
PFNGLRESETMINMAX_PROC* glpfResetMinmax = NULL;

/* GL_VERSION_1_3 */

PFNGLACTIVETEXTURE_PROC* glpfActiveTexture = NULL;
PFNGLSAMPLECOVERAGE_PROC* glpfSampleCoverage = NULL;
PFNGLCOMPRESSEDTEXIMAGE3D_PROC* glpfCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2D_PROC* glpfCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE1D_PROC* glpfCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3D_PROC* glpfCompressedTexSubImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2D_PROC* glpfCompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1D_PROC* glpfCompressedTexSubImage1D = NULL;
PFNGLGETCOMPRESSEDTEXIMAGE_PROC* glpfGetCompressedTexImage = NULL;
PFNGLCLIENTACTIVETEXTURE_PROC* glpfClientActiveTexture = NULL;
PFNGLMULTITEXCOORD1D_PROC* glpfMultiTexCoord1d = NULL;
PFNGLMULTITEXCOORD1DV_PROC* glpfMultiTexCoord1dv = NULL;
PFNGLMULTITEXCOORD1F_PROC* glpfMultiTexCoord1f = NULL;
PFNGLMULTITEXCOORD1FV_PROC* glpfMultiTexCoord1fv = NULL;
PFNGLMULTITEXCOORD1I_PROC* glpfMultiTexCoord1i = NULL;
PFNGLMULTITEXCOORD1IV_PROC* glpfMultiTexCoord1iv = NULL;
PFNGLMULTITEXCOORD1S_PROC* glpfMultiTexCoord1s = NULL;
PFNGLMULTITEXCOORD1SV_PROC* glpfMultiTexCoord1sv = NULL;
PFNGLMULTITEXCOORD2D_PROC* glpfMultiTexCoord2d = NULL;
PFNGLMULTITEXCOORD2DV_PROC* glpfMultiTexCoord2dv = NULL;
PFNGLMULTITEXCOORD2F_PROC* glpfMultiTexCoord2f = NULL;
PFNGLMULTITEXCOORD2FV_PROC* glpfMultiTexCoord2fv = NULL;
PFNGLMULTITEXCOORD2I_PROC* glpfMultiTexCoord2i = NULL;
PFNGLMULTITEXCOORD2IV_PROC* glpfMultiTexCoord2iv = NULL;
PFNGLMULTITEXCOORD2S_PROC* glpfMultiTexCoord2s = NULL;
PFNGLMULTITEXCOORD2SV_PROC* glpfMultiTexCoord2sv = NULL;
PFNGLMULTITEXCOORD3D_PROC* glpfMultiTexCoord3d = NULL;
PFNGLMULTITEXCOORD3DV_PROC* glpfMultiTexCoord3dv = NULL;
PFNGLMULTITEXCOORD3F_PROC* glpfMultiTexCoord3f = NULL;
PFNGLMULTITEXCOORD3FV_PROC* glpfMultiTexCoord3fv = NULL;
PFNGLMULTITEXCOORD3I_PROC* glpfMultiTexCoord3i = NULL;
PFNGLMULTITEXCOORD3IV_PROC* glpfMultiTexCoord3iv = NULL;
PFNGLMULTITEXCOORD3S_PROC* glpfMultiTexCoord3s = NULL;
PFNGLMULTITEXCOORD3SV_PROC* glpfMultiTexCoord3sv = NULL;
PFNGLMULTITEXCOORD4D_PROC* glpfMultiTexCoord4d = NULL;
PFNGLMULTITEXCOORD4DV_PROC* glpfMultiTexCoord4dv = NULL;
PFNGLMULTITEXCOORD4F_PROC* glpfMultiTexCoord4f = NULL;
PFNGLMULTITEXCOORD4FV_PROC* glpfMultiTexCoord4fv = NULL;
PFNGLMULTITEXCOORD4I_PROC* glpfMultiTexCoord4i = NULL;
PFNGLMULTITEXCOORD4IV_PROC* glpfMultiTexCoord4iv = NULL;
PFNGLMULTITEXCOORD4S_PROC* glpfMultiTexCoord4s = NULL;
PFNGLMULTITEXCOORD4SV_PROC* glpfMultiTexCoord4sv = NULL;
PFNGLLOADTRANSPOSEMATRIXF_PROC* glpfLoadTransposeMatrixf = NULL;
PFNGLLOADTRANSPOSEMATRIXD_PROC* glpfLoadTransposeMatrixd = NULL;
PFNGLMULTTRANSPOSEMATRIXF_PROC* glpfMultTransposeMatrixf = NULL;
PFNGLMULTTRANSPOSEMATRIXD_PROC* glpfMultTransposeMatrixd = NULL;

/* GL_VERSION_1_4 */

PFNGLBLENDFUNCSEPARATE_PROC* glpfBlendFuncSeparate = NULL;
PFNGLMULTIDRAWARRAYS_PROC* glpfMultiDrawArrays = NULL;
PFNGLMULTIDRAWELEMENTS_PROC* glpfMultiDrawElements = NULL;
PFNGLPOINTPARAMETERF_PROC* glpfPointParameterf = NULL;
PFNGLPOINTPARAMETERFV_PROC* glpfPointParameterfv = NULL;
PFNGLPOINTPARAMETERI_PROC* glpfPointParameteri = NULL;
PFNGLPOINTPARAMETERIV_PROC* glpfPointParameteriv = NULL;
PFNGLFOGCOORDF_PROC* glpfFogCoordf = NULL;
PFNGLFOGCOORDFV_PROC* glpfFogCoordfv = NULL;
PFNGLFOGCOORDD_PROC* glpfFogCoordd = NULL;
PFNGLFOGCOORDDV_PROC* glpfFogCoorddv = NULL;
PFNGLFOGCOORDPOINTER_PROC* glpfFogCoordPointer = NULL;
PFNGLSECONDARYCOLOR3B_PROC* glpfSecondaryColor3b = NULL;
PFNGLSECONDARYCOLOR3BV_PROC* glpfSecondaryColor3bv = NULL;
PFNGLSECONDARYCOLOR3D_PROC* glpfSecondaryColor3d = NULL;
PFNGLSECONDARYCOLOR3DV_PROC* glpfSecondaryColor3dv = NULL;
PFNGLSECONDARYCOLOR3F_PROC* glpfSecondaryColor3f = NULL;
PFNGLSECONDARYCOLOR3FV_PROC* glpfSecondaryColor3fv = NULL;
PFNGLSECONDARYCOLOR3I_PROC* glpfSecondaryColor3i = NULL;
PFNGLSECONDARYCOLOR3IV_PROC* glpfSecondaryColor3iv = NULL;
PFNGLSECONDARYCOLOR3S_PROC* glpfSecondaryColor3s = NULL;
PFNGLSECONDARYCOLOR3SV_PROC* glpfSecondaryColor3sv = NULL;
PFNGLSECONDARYCOLOR3UB_PROC* glpfSecondaryColor3ub = NULL;
PFNGLSECONDARYCOLOR3UBV_PROC* glpfSecondaryColor3ubv = NULL;
PFNGLSECONDARYCOLOR3UI_PROC* glpfSecondaryColor3ui = NULL;
PFNGLSECONDARYCOLOR3UIV_PROC* glpfSecondaryColor3uiv = NULL;
PFNGLSECONDARYCOLOR3US_PROC* glpfSecondaryColor3us = NULL;
PFNGLSECONDARYCOLOR3USV_PROC* glpfSecondaryColor3usv = NULL;
PFNGLSECONDARYCOLORPOINTER_PROC* glpfSecondaryColorPointer = NULL;
PFNGLWINDOWPOS2D_PROC* glpfWindowPos2d = NULL;
PFNGLWINDOWPOS2DV_PROC* glpfWindowPos2dv = NULL;
PFNGLWINDOWPOS2F_PROC* glpfWindowPos2f = NULL;
PFNGLWINDOWPOS2FV_PROC* glpfWindowPos2fv = NULL;
PFNGLWINDOWPOS2I_PROC* glpfWindowPos2i = NULL;
PFNGLWINDOWPOS2IV_PROC* glpfWindowPos2iv = NULL;
PFNGLWINDOWPOS2S_PROC* glpfWindowPos2s = NULL;
PFNGLWINDOWPOS2SV_PROC* glpfWindowPos2sv = NULL;
PFNGLWINDOWPOS3D_PROC* glpfWindowPos3d = NULL;
PFNGLWINDOWPOS3DV_PROC* glpfWindowPos3dv = NULL;
PFNGLWINDOWPOS3F_PROC* glpfWindowPos3f = NULL;
PFNGLWINDOWPOS3FV_PROC* glpfWindowPos3fv = NULL;
PFNGLWINDOWPOS3I_PROC* glpfWindowPos3i = NULL;
PFNGLWINDOWPOS3IV_PROC* glpfWindowPos3iv = NULL;
PFNGLWINDOWPOS3S_PROC* glpfWindowPos3s = NULL;
PFNGLWINDOWPOS3SV_PROC* glpfWindowPos3sv = NULL;

/* GL_VERSION_1_5 */

PFNGLGENQUERIES_PROC* glpfGenQueries = NULL;
PFNGLDELETEQUERIES_PROC* glpfDeleteQueries = NULL;
PFNGLISQUERY_PROC* glpfIsQuery = NULL;
PFNGLBEGINQUERY_PROC* glpfBeginQuery = NULL;
PFNGLENDQUERY_PROC* glpfEndQuery = NULL;
PFNGLGETQUERYIV_PROC* glpfGetQueryiv = NULL;
PFNGLGETQUERYOBJECTIV_PROC* glpfGetQueryObjectiv = NULL;
PFNGLGETQUERYOBJECTUIV_PROC* glpfGetQueryObjectuiv = NULL;
PFNGLBINDBUFFER_PROC* glpfBindBuffer = NULL;
PFNGLDELETEBUFFERS_PROC* glpfDeleteBuffers = NULL;
PFNGLGENBUFFERS_PROC* glpfGenBuffers = NULL;
PFNGLISBUFFER_PROC* glpfIsBuffer = NULL;
PFNGLBUFFERDATA_PROC* glpfBufferData = NULL;
PFNGLBUFFERSUBDATA_PROC* glpfBufferSubData = NULL;
PFNGLGETBUFFERSUBDATA_PROC* glpfGetBufferSubData = NULL;
PFNGLMAPBUFFER_PROC* glpfMapBuffer = NULL;
PFNGLUNMAPBUFFER_PROC* glpfUnmapBuffer = NULL;
PFNGLGETBUFFERPARAMETERIV_PROC* glpfGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERV_PROC* glpfGetBufferPointerv = NULL;

/* GL_VERSION_2_0 */

PFNGLBLENDEQUATIONSEPARATE_PROC* glpfBlendEquationSeparate = NULL;
PFNGLDRAWBUFFERS_PROC* glpfDrawBuffers = NULL;
PFNGLSTENCILOPSEPARATE_PROC* glpfStencilOpSeparate = NULL;
PFNGLSTENCILFUNCSEPARATE_PROC* glpfStencilFuncSeparate = NULL;
PFNGLSTENCILMASKSEPARATE_PROC* glpfStencilMaskSeparate = NULL;
PFNGLATTACHSHADER_PROC* glpfAttachShader = NULL;
PFNGLBINDATTRIBLOCATION_PROC* glpfBindAttribLocation = NULL;
PFNGLCOMPILESHADER_PROC* glpfCompileShader = NULL;
PFNGLCREATEPROGRAM_PROC* glpfCreateProgram = NULL;
PFNGLCREATESHADER_PROC* glpfCreateShader = NULL;
PFNGLDELETEPROGRAM_PROC* glpfDeleteProgram = NULL;
PFNGLDELETESHADER_PROC* glpfDeleteShader = NULL;
PFNGLDETACHSHADER_PROC* glpfDetachShader = NULL;
PFNGLDISABLEVERTEXATTRIBARRAY_PROC* glpfDisableVertexAttribArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAY_PROC* glpfEnableVertexAttribArray = NULL;
PFNGLGETACTIVEATTRIB_PROC* glpfGetActiveAttrib = NULL;
PFNGLGETACTIVEUNIFORM_PROC* glpfGetActiveUniform = NULL;
PFNGLGETATTACHEDSHADERS_PROC* glpfGetAttachedShaders = NULL;
PFNGLGETATTRIBLOCATION_PROC* glpfGetAttribLocation = NULL;
PFNGLGETPROGRAMIV_PROC* glpfGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOG_PROC* glpfGetProgramInfoLog = NULL;
PFNGLGETSHADERIV_PROC* glpfGetShaderiv = NULL;
PFNGLGETSHADERINFOLOG_PROC* glpfGetShaderInfoLog = NULL;
PFNGLGETSHADERSOURCE_PROC* glpfGetShaderSource = NULL;
PFNGLGETUNIFORMLOCATION_PROC* glpfGetUniformLocation = NULL;
PFNGLGETUNIFORMFV_PROC* glpfGetUniformfv = NULL;
PFNGLGETUNIFORMIV_PROC* glpfGetUniformiv = NULL;
PFNGLGETVERTEXATTRIBDV_PROC* glpfGetVertexAttribdv = NULL;
PFNGLGETVERTEXATTRIBFV_PROC* glpfGetVertexAttribfv = NULL;
PFNGLGETVERTEXATTRIBIV_PROC* glpfGetVertexAttribiv = NULL;
PFNGLGETVERTEXATTRIBPOINTERV_PROC* glpfGetVertexAttribPointerv = NULL;
PFNGLISPROGRAM_PROC* glpfIsProgram = NULL;
PFNGLISSHADER_PROC* glpfIsShader = NULL;
PFNGLLINKPROGRAM_PROC* glpfLinkProgram = NULL;
PFNGLSHADERSOURCE_PROC* glpfShaderSource = NULL;
PFNGLUSEPROGRAM_PROC* glpfUseProgram = NULL;
PFNGLUNIFORM1F_PROC* glpfUniform1f = NULL;
PFNGLUNIFORM2F_PROC* glpfUniform2f = NULL;
PFNGLUNIFORM3F_PROC* glpfUniform3f = NULL;
PFNGLUNIFORM4F_PROC* glpfUniform4f = NULL;
PFNGLUNIFORM1I_PROC* glpfUniform1i = NULL;
PFNGLUNIFORM2I_PROC* glpfUniform2i = NULL;
PFNGLUNIFORM3I_PROC* glpfUniform3i = NULL;
PFNGLUNIFORM4I_PROC* glpfUniform4i = NULL;
PFNGLUNIFORM1FV_PROC* glpfUniform1fv = NULL;
PFNGLUNIFORM2FV_PROC* glpfUniform2fv = NULL;
PFNGLUNIFORM3FV_PROC* glpfUniform3fv = NULL;
PFNGLUNIFORM4FV_PROC* glpfUniform4fv = NULL;
PFNGLUNIFORM1IV_PROC* glpfUniform1iv = NULL;
PFNGLUNIFORM2IV_PROC* glpfUniform2iv = NULL;
PFNGLUNIFORM3IV_PROC* glpfUniform3iv = NULL;
PFNGLUNIFORM4IV_PROC* glpfUniform4iv = NULL;
PFNGLUNIFORMMATRIX2FV_PROC* glpfUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FV_PROC* glpfUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FV_PROC* glpfUniformMatrix4fv = NULL;
PFNGLVALIDATEPROGRAM_PROC* glpfValidateProgram = NULL;
PFNGLVERTEXATTRIB1D_PROC* glpfVertexAttrib1d = NULL;
PFNGLVERTEXATTRIB1DV_PROC* glpfVertexAttrib1dv = NULL;
PFNGLVERTEXATTRIB1F_PROC* glpfVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB1FV_PROC* glpfVertexAttrib1fv = NULL;
PFNGLVERTEXATTRIB1S_PROC* glpfVertexAttrib1s = NULL;
PFNGLVERTEXATTRIB1SV_PROC* glpfVertexAttrib1sv = NULL;
PFNGLVERTEXATTRIB2D_PROC* glpfVertexAttrib2d = NULL;
PFNGLVERTEXATTRIB2DV_PROC* glpfVertexAttrib2dv = NULL;
PFNGLVERTEXATTRIB2F_PROC* glpfVertexAttrib2f = NULL;
PFNGLVERTEXATTRIB2FV_PROC* glpfVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB2S_PROC* glpfVertexAttrib2s = NULL;
PFNGLVERTEXATTRIB2SV_PROC* glpfVertexAttrib2sv = NULL;
PFNGLVERTEXATTRIB3D_PROC* glpfVertexAttrib3d = NULL;
PFNGLVERTEXATTRIB3DV_PROC* glpfVertexAttrib3dv = NULL;
PFNGLVERTEXATTRIB3F_PROC* glpfVertexAttrib3f = NULL;
PFNGLVERTEXATTRIB3FV_PROC* glpfVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB3S_PROC* glpfVertexAttrib3s = NULL;
PFNGLVERTEXATTRIB3SV_PROC* glpfVertexAttrib3sv = NULL;
PFNGLVERTEXATTRIB4NBV_PROC* glpfVertexAttrib4Nbv = NULL;
PFNGLVERTEXATTRIB4NIV_PROC* glpfVertexAttrib4Niv = NULL;
PFNGLVERTEXATTRIB4NSV_PROC* glpfVertexAttrib4Nsv = NULL;
PFNGLVERTEXATTRIB4NUB_PROC* glpfVertexAttrib4Nub = NULL;
PFNGLVERTEXATTRIB4NUBV_PROC* glpfVertexAttrib4Nubv = NULL;
PFNGLVERTEXATTRIB4NUIV_PROC* glpfVertexAttrib4Nuiv = NULL;
PFNGLVERTEXATTRIB4NUSV_PROC* glpfVertexAttrib4Nusv = NULL;
PFNGLVERTEXATTRIB4BV_PROC* glpfVertexAttrib4bv = NULL;
PFNGLVERTEXATTRIB4D_PROC* glpfVertexAttrib4d = NULL;
PFNGLVERTEXATTRIB4DV_PROC* glpfVertexAttrib4dv = NULL;
PFNGLVERTEXATTRIB4F_PROC* glpfVertexAttrib4f = NULL;
PFNGLVERTEXATTRIB4FV_PROC* glpfVertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB4IV_PROC* glpfVertexAttrib4iv = NULL;
PFNGLVERTEXATTRIB4S_PROC* glpfVertexAttrib4s = NULL;
PFNGLVERTEXATTRIB4SV_PROC* glpfVertexAttrib4sv = NULL;
PFNGLVERTEXATTRIB4UBV_PROC* glpfVertexAttrib4ubv = NULL;
PFNGLVERTEXATTRIB4UIV_PROC* glpfVertexAttrib4uiv = NULL;
PFNGLVERTEXATTRIB4USV_PROC* glpfVertexAttrib4usv = NULL;
PFNGLVERTEXATTRIBPOINTER_PROC* glpfVertexAttribPointer = NULL;

/* GL_VERSION_2_1 */

PFNGLUNIFORMMATRIX2X3FV_PROC* glpfUniformMatrix2x3fv = NULL;
PFNGLUNIFORMMATRIX3X2FV_PROC* glpfUniformMatrix3x2fv = NULL;
PFNGLUNIFORMMATRIX2X4FV_PROC* glpfUniformMatrix2x4fv = NULL;
PFNGLUNIFORMMATRIX4X2FV_PROC* glpfUniformMatrix4x2fv = NULL;
PFNGLUNIFORMMATRIX3X4FV_PROC* glpfUniformMatrix3x4fv = NULL;
PFNGLUNIFORMMATRIX4X3FV_PROC* glpfUniformMatrix4x3fv = NULL;

/* GL_VERSION_3_0 */

PFNGLCOLORMASKI_PROC* glpfColorMaski = NULL;
PFNGLGETBOOLEANI_V_PROC* glpfGetBooleani_v = NULL;
PFNGLGETINTEGERI_V_PROC* glpfGetIntegeri_v = NULL;
PFNGLENABLEI_PROC* glpfEnablei = NULL;
PFNGLDISABLEI_PROC* glpfDisablei = NULL;
PFNGLISENABLEDI_PROC* glpfIsEnabledi = NULL;
PFNGLBEGINTRANSFORMFEEDBACK_PROC* glpfBeginTransformFeedback = NULL;
PFNGLENDTRANSFORMFEEDBACK_PROC* glpfEndTransformFeedback = NULL;
PFNGLBINDBUFFERRANGE_PROC* glpfBindBufferRange = NULL;
PFNGLBINDBUFFERBASE_PROC* glpfBindBufferBase = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGS_PROC* glpfTransformFeedbackVaryings = NULL;
PFNGLGETTRANSFORMFEEDBACKVARYING_PROC* glpfGetTransformFeedbackVarying = NULL;
PFNGLCLAMPCOLOR_PROC* glpfClampColor = NULL;
PFNGLBEGINCONDITIONALRENDER_PROC* glpfBeginConditionalRender = NULL;
PFNGLENDCONDITIONALRENDER_PROC* glpfEndConditionalRender = NULL;
PFNGLVERTEXATTRIBIPOINTER_PROC* glpfVertexAttribIPointer = NULL;
PFNGLGETVERTEXATTRIBIIV_PROC* glpfGetVertexAttribIiv = NULL;
PFNGLGETVERTEXATTRIBIUIV_PROC* glpfGetVertexAttribIuiv = NULL;
PFNGLVERTEXATTRIBI1I_PROC* glpfVertexAttribI1i = NULL;
PFNGLVERTEXATTRIBI2I_PROC* glpfVertexAttribI2i = NULL;
PFNGLVERTEXATTRIBI3I_PROC* glpfVertexAttribI3i = NULL;
PFNGLVERTEXATTRIBI4I_PROC* glpfVertexAttribI4i = NULL;
PFNGLVERTEXATTRIBI1UI_PROC* glpfVertexAttribI1ui = NULL;
PFNGLVERTEXATTRIBI2UI_PROC* glpfVertexAttribI2ui = NULL;
PFNGLVERTEXATTRIBI3UI_PROC* glpfVertexAttribI3ui = NULL;
PFNGLVERTEXATTRIBI4UI_PROC* glpfVertexAttribI4ui = NULL;
PFNGLVERTEXATTRIBI1IV_PROC* glpfVertexAttribI1iv = NULL;
PFNGLVERTEXATTRIBI2IV_PROC* glpfVertexAttribI2iv = NULL;
PFNGLVERTEXATTRIBI3IV_PROC* glpfVertexAttribI3iv = NULL;
PFNGLVERTEXATTRIBI4IV_PROC* glpfVertexAttribI4iv = NULL;
PFNGLVERTEXATTRIBI1UIV_PROC* glpfVertexAttribI1uiv = NULL;
PFNGLVERTEXATTRIBI2UIV_PROC* glpfVertexAttribI2uiv = NULL;
PFNGLVERTEXATTRIBI3UIV_PROC* glpfVertexAttribI3uiv = NULL;
PFNGLVERTEXATTRIBI4UIV_PROC* glpfVertexAttribI4uiv = NULL;
PFNGLVERTEXATTRIBI4BV_PROC* glpfVertexAttribI4bv = NULL;
PFNGLVERTEXATTRIBI4SV_PROC* glpfVertexAttribI4sv = NULL;
PFNGLVERTEXATTRIBI4UBV_PROC* glpfVertexAttribI4ubv = NULL;
PFNGLVERTEXATTRIBI4USV_PROC* glpfVertexAttribI4usv = NULL;
PFNGLGETUNIFORMUIV_PROC* glpfGetUniformuiv = NULL;
PFNGLBINDFRAGDATALOCATION_PROC* glpfBindFragDataLocation = NULL;
PFNGLGETFRAGDATALOCATION_PROC* glpfGetFragDataLocation = NULL;
PFNGLUNIFORM1UI_PROC* glpfUniform1ui = NULL;
PFNGLUNIFORM2UI_PROC* glpfUniform2ui = NULL;
PFNGLUNIFORM3UI_PROC* glpfUniform3ui = NULL;
PFNGLUNIFORM4UI_PROC* glpfUniform4ui = NULL;
PFNGLUNIFORM1UIV_PROC* glpfUniform1uiv = NULL;
PFNGLUNIFORM2UIV_PROC* glpfUniform2uiv = NULL;
PFNGLUNIFORM3UIV_PROC* glpfUniform3uiv = NULL;
PFNGLUNIFORM4UIV_PROC* glpfUniform4uiv = NULL;
PFNGLTEXPARAMETERIIV_PROC* glpfTexParameterIiv = NULL;
PFNGLTEXPARAMETERIUIV_PROC* glpfTexParameterIuiv = NULL;
PFNGLGETTEXPARAMETERIIV_PROC* glpfGetTexParameterIiv = NULL;
PFNGLGETTEXPARAMETERIUIV_PROC* glpfGetTexParameterIuiv = NULL;
PFNGLCLEARBUFFERIV_PROC* glpfClearBufferiv = NULL;
PFNGLCLEARBUFFERUIV_PROC* glpfClearBufferuiv = NULL;
PFNGLCLEARBUFFERFV_PROC* glpfClearBufferfv = NULL;
PFNGLCLEARBUFFERFI_PROC* glpfClearBufferfi = NULL;
PFNGLGETSTRINGI_PROC* glpfGetStringi = NULL;

/* GL_VERSION_3_1 */

PFNGLDRAWARRAYSINSTANCED_PROC* glpfDrawArraysInstanced = NULL;
PFNGLDRAWELEMENTSINSTANCED_PROC* glpfDrawElementsInstanced = NULL;
PFNGLTEXBUFFER_PROC* glpfTexBuffer = NULL;
PFNGLPRIMITIVERESTARTINDEX_PROC* glpfPrimitiveRestartIndex = NULL;

/* GL_VERSION_3_2 */

PFNGLGETINTEGER64I_V_PROC* glpfGetInteger64i_v = NULL;
PFNGLGETBUFFERPARAMETERI64V_PROC* glpfGetBufferParameteri64v = NULL;
PFNGLFRAMEBUFFERTEXTURE_PROC* glpfFramebufferTexture = NULL;

/* GL_VERSION_3_3 */

PFNGLVERTEXATTRIBDIVISOR_PROC* glpfVertexAttribDivisor = NULL;

/* GL_VERSION_4_0 */

PFNGLMINSAMPLESHADING_PROC* glpfMinSampleShading = NULL;
PFNGLBLENDEQUATIONI_PROC* glpfBlendEquationi = NULL;
PFNGLBLENDEQUATIONSEPARATEI_PROC* glpfBlendEquationSeparatei = NULL;
PFNGLBLENDFUNCI_PROC* glpfBlendFunci = NULL;
PFNGLBLENDFUNCSEPARATEI_PROC* glpfBlendFuncSeparatei = NULL;



static void glextAddExtension(const char* extension)
{
    if (strcmp("GL_ARB_buffer_storage", extension) == 0) {
        GLEXT_ARB_buffer_storage = GL_TRUE;
    }
    if (strcmp("GL_ARB_seamless_cubemap_per_texture", extension) == 0) {
        GLEXT_ARB_seamless_cubemap_per_texture = GL_TRUE;
    }
    if (strcmp("GL_ARB_bindless_texture", extension) == 0) {
        GLEXT_ARB_bindless_texture = GL_TRUE;
    }
    if (strcmp("GL_ARB_sparse_texture", extension) == 0) {
        GLEXT_ARB_sparse_texture = GL_TRUE;
    }
    if (strcmp("GL_ARB_shader_draw_parameters", extension) == 0) {
        GLEXT_ARB_shader_draw_parameters = GL_TRUE;
    }
    if (strcmp("GL_ARB_indirect_parameters", extension) == 0) {
        GLEXT_ARB_indirect_parameters = GL_TRUE;
    }
    if (strcmp("GL_ARB_shader_group_vote", extension) == 0) {
        GLEXT_ARB_shader_group_vote = GL_TRUE;
    }
    if (strcmp("GL_ARB_multi_bind", extension) == 0) {
        GLEXT_ARB_multi_bind = GL_TRUE;
    }
    if (strcmp("GL_ARB_clear_texture", extension) == 0) {
        GLEXT_ARB_clear_texture = GL_TRUE;
    }
    if (strcmp("GL_ARB_query_buffer_object", extension) == 0) {
        GLEXT_ARB_query_buffer_object = GL_TRUE;
    }
    if (strcmp("GL_ARB_enhanced_layouts", extension) == 0) {
        GLEXT_ARB_enhanced_layouts = GL_TRUE;
    }
    if (strcmp("GL_ARB_texture_mirror_clamp_to_edge", extension) == 0) {
        GLEXT_ARB_texture_mirror_clamp_to_edge = GL_TRUE;
    }
    if (strcmp("GL_ARB_texture_stencil8", extension) == 0) {
        GLEXT_ARB_texture_stencil8 = GL_TRUE;
    }
    if (strcmp("GL_ARB_compute_variable_group_size", extension) == 0) {
        GLEXT_ARB_compute_variable_group_size = GL_TRUE;
    }
    if (strcmp("GL_ARB_vertex_type_10f_11f_11f_rev", extension) == 0) {
        GLEXT_ARB_vertex_type_10f_11f_11f_rev = GL_TRUE;
    }
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

static GLPROC glextGetProc(const char *proc)
{
    GLPROC res;

    res = wglGetProcAddress(proc);
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

static GLPROC glextGetProc(const char *proc)
{
    GLPROC res;

    CFStringRef procname = CFStringCreateWithCString(kCFAllocatorDefault, proc,
                kCFStringEncodingASCII);
    res = CFBundleGetFunctionPointerForName(bundle, procname);
    CFRelease(procname);
    return res;
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

static GLPROC glextGetProc(const char *proc)
{
    GLPROC res;

    res = glXGetProcAddress((const GLubyte *) proc);
    if (!res)
        res = dlsym(libgl, proc);
    return res;
}
#endif

#ifdef __cplusplus
}
#endif
