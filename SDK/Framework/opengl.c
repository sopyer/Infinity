// WARNING: This file was automatically generated
// Do not edit.

#include <opengl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void loadOpenglFunctions(void);

static void open_libgl(void);
static void close_libgl(void);
static void *get_proc(const char *proc);
static void add_extension(const char* extension);

int importOpenGL(void) 
{
    GLint minor, major;
    GLint profile;
    GLint num_extensions;
    int i;

    open_libgl();
    loadOpenglFunctions();
    close_libgl();


    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    if (major < 3 || (major == 3 && minor < 3)) {
        fprintf(stderr, "Error: OpenGL version 3.3 not supported.\n");
        fprintf(stderr, "       Your version is %d.%d.\n", major, minor);
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }

    
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

    if ((profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) == 0) {
        fprintf(stderr, "Error: This application requires a compatibility profile\n");
        return GL_FALSE;
    }

    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
    
    for (i = 0; i < num_extensions; i++) {
        add_extension(glGetStringi(GL_EXTENSIONS, i));
    }


    return GL_TRUE;
}



void loadOpenglFunctions(void)
{
    // --- Function pointer loading

    // VERSION_1_2

    glpfBlendColor = (PFNGLBLENDCOLORPROC)get_proc("glBlendColor");
    glpfBlendEquation = (PFNGLBLENDEQUATIONPROC)get_proc("glBlendEquation");
    glpfDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)get_proc("glDrawRangeElements");
    glpfTexImage3D = (PFNGLTEXIMAGE3DPROC)get_proc("glTexImage3D");
    glpfTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)get_proc("glTexSubImage3D");
    glpfCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)get_proc("glCopyTexSubImage3D");

    // VERSION_1_2_DEPRECATED

    glpfColorTable = (PFNGLCOLORTABLEPROC)get_proc("glColorTable");
    glpfColorTableParameterfv = (PFNGLCOLORTABLEPARAMETERFVPROC)get_proc("glColorTableParameterfv");
    glpfColorTableParameteriv = (PFNGLCOLORTABLEPARAMETERIVPROC)get_proc("glColorTableParameteriv");
    glpfCopyColorTable = (PFNGLCOPYCOLORTABLEPROC)get_proc("glCopyColorTable");
    glpfGetColorTable = (PFNGLGETCOLORTABLEPROC)get_proc("glGetColorTable");
    glpfGetColorTableParameterfv = (PFNGLGETCOLORTABLEPARAMETERFVPROC)get_proc("glGetColorTableParameterfv");
    glpfGetColorTableParameteriv = (PFNGLGETCOLORTABLEPARAMETERIVPROC)get_proc("glGetColorTableParameteriv");
    glpfColorSubTable = (PFNGLCOLORSUBTABLEPROC)get_proc("glColorSubTable");
    glpfCopyColorSubTable = (PFNGLCOPYCOLORSUBTABLEPROC)get_proc("glCopyColorSubTable");
    glpfConvolutionFilter1D = (PFNGLCONVOLUTIONFILTER1DPROC)get_proc("glConvolutionFilter1D");
    glpfConvolutionFilter2D = (PFNGLCONVOLUTIONFILTER2DPROC)get_proc("glConvolutionFilter2D");
    glpfConvolutionParameterf = (PFNGLCONVOLUTIONPARAMETERFPROC)get_proc("glConvolutionParameterf");
    glpfConvolutionParameterfv = (PFNGLCONVOLUTIONPARAMETERFVPROC)get_proc("glConvolutionParameterfv");
    glpfConvolutionParameteri = (PFNGLCONVOLUTIONPARAMETERIPROC)get_proc("glConvolutionParameteri");
    glpfConvolutionParameteriv = (PFNGLCONVOLUTIONPARAMETERIVPROC)get_proc("glConvolutionParameteriv");
    glpfCopyConvolutionFilter1D = (PFNGLCOPYCONVOLUTIONFILTER1DPROC)get_proc("glCopyConvolutionFilter1D");
    glpfCopyConvolutionFilter2D = (PFNGLCOPYCONVOLUTIONFILTER2DPROC)get_proc("glCopyConvolutionFilter2D");
    glpfGetConvolutionFilter = (PFNGLGETCONVOLUTIONFILTERPROC)get_proc("glGetConvolutionFilter");
    glpfGetConvolutionParameterfv = (PFNGLGETCONVOLUTIONPARAMETERFVPROC)get_proc("glGetConvolutionParameterfv");
    glpfGetConvolutionParameteriv = (PFNGLGETCONVOLUTIONPARAMETERIVPROC)get_proc("glGetConvolutionParameteriv");
    glpfGetSeparableFilter = (PFNGLGETSEPARABLEFILTERPROC)get_proc("glGetSeparableFilter");
    glpfSeparableFilter2D = (PFNGLSEPARABLEFILTER2DPROC)get_proc("glSeparableFilter2D");
    glpfGetHistogram = (PFNGLGETHISTOGRAMPROC)get_proc("glGetHistogram");
    glpfGetHistogramParameterfv = (PFNGLGETHISTOGRAMPARAMETERFVPROC)get_proc("glGetHistogramParameterfv");
    glpfGetHistogramParameteriv = (PFNGLGETHISTOGRAMPARAMETERIVPROC)get_proc("glGetHistogramParameteriv");
    glpfGetMinmax = (PFNGLGETMINMAXPROC)get_proc("glGetMinmax");
    glpfGetMinmaxParameterfv = (PFNGLGETMINMAXPARAMETERFVPROC)get_proc("glGetMinmaxParameterfv");
    glpfGetMinmaxParameteriv = (PFNGLGETMINMAXPARAMETERIVPROC)get_proc("glGetMinmaxParameteriv");
    glpfHistogram = (PFNGLHISTOGRAMPROC)get_proc("glHistogram");
    glpfMinmax = (PFNGLMINMAXPROC)get_proc("glMinmax");
    glpfResetHistogram = (PFNGLRESETHISTOGRAMPROC)get_proc("glResetHistogram");
    glpfResetMinmax = (PFNGLRESETMINMAXPROC)get_proc("glResetMinmax");

    // VERSION_1_3

    glpfActiveTexture = (PFNGLACTIVETEXTUREPROC)get_proc("glActiveTexture");
    glpfSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)get_proc("glSampleCoverage");
    glpfCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)get_proc("glCompressedTexImage3D");
    glpfCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)get_proc("glCompressedTexImage2D");
    glpfCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)get_proc("glCompressedTexImage1D");
    glpfCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)get_proc("glCompressedTexSubImage3D");
    glpfCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)get_proc("glCompressedTexSubImage2D");
    glpfCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)get_proc("glCompressedTexSubImage1D");
    glpfGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)get_proc("glGetCompressedTexImage");

    // VERSION_1_3_DEPRECATED

    glpfClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC)get_proc("glClientActiveTexture");
    glpfMultiTexCoord1d = (PFNGLMULTITEXCOORD1DPROC)get_proc("glMultiTexCoord1d");
    glpfMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DVPROC)get_proc("glMultiTexCoord1dv");
    glpfMultiTexCoord1f = (PFNGLMULTITEXCOORD1FPROC)get_proc("glMultiTexCoord1f");
    glpfMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FVPROC)get_proc("glMultiTexCoord1fv");
    glpfMultiTexCoord1i = (PFNGLMULTITEXCOORD1IPROC)get_proc("glMultiTexCoord1i");
    glpfMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IVPROC)get_proc("glMultiTexCoord1iv");
    glpfMultiTexCoord1s = (PFNGLMULTITEXCOORD1SPROC)get_proc("glMultiTexCoord1s");
    glpfMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SVPROC)get_proc("glMultiTexCoord1sv");
    glpfMultiTexCoord2d = (PFNGLMULTITEXCOORD2DPROC)get_proc("glMultiTexCoord2d");
    glpfMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC)get_proc("glMultiTexCoord2dv");
    glpfMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC)get_proc("glMultiTexCoord2f");
    glpfMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC)get_proc("glMultiTexCoord2fv");
    glpfMultiTexCoord2i = (PFNGLMULTITEXCOORD2IPROC)get_proc("glMultiTexCoord2i");
    glpfMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IVPROC)get_proc("glMultiTexCoord2iv");
    glpfMultiTexCoord2s = (PFNGLMULTITEXCOORD2SPROC)get_proc("glMultiTexCoord2s");
    glpfMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SVPROC)get_proc("glMultiTexCoord2sv");
    glpfMultiTexCoord3d = (PFNGLMULTITEXCOORD3DPROC)get_proc("glMultiTexCoord3d");
    glpfMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DVPROC)get_proc("glMultiTexCoord3dv");
    glpfMultiTexCoord3f = (PFNGLMULTITEXCOORD3FPROC)get_proc("glMultiTexCoord3f");
    glpfMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FVPROC)get_proc("glMultiTexCoord3fv");
    glpfMultiTexCoord3i = (PFNGLMULTITEXCOORD3IPROC)get_proc("glMultiTexCoord3i");
    glpfMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IVPROC)get_proc("glMultiTexCoord3iv");
    glpfMultiTexCoord3s = (PFNGLMULTITEXCOORD3SPROC)get_proc("glMultiTexCoord3s");
    glpfMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SVPROC)get_proc("glMultiTexCoord3sv");
    glpfMultiTexCoord4d = (PFNGLMULTITEXCOORD4DPROC)get_proc("glMultiTexCoord4d");
    glpfMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DVPROC)get_proc("glMultiTexCoord4dv");
    glpfMultiTexCoord4f = (PFNGLMULTITEXCOORD4FPROC)get_proc("glMultiTexCoord4f");
    glpfMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FVPROC)get_proc("glMultiTexCoord4fv");
    glpfMultiTexCoord4i = (PFNGLMULTITEXCOORD4IPROC)get_proc("glMultiTexCoord4i");
    glpfMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IVPROC)get_proc("glMultiTexCoord4iv");
    glpfMultiTexCoord4s = (PFNGLMULTITEXCOORD4SPROC)get_proc("glMultiTexCoord4s");
    glpfMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SVPROC)get_proc("glMultiTexCoord4sv");
    glpfLoadTransposeMatrixf = (PFNGLLOADTRANSPOSEMATRIXFPROC)get_proc("glLoadTransposeMatrixf");
    glpfLoadTransposeMatrixd = (PFNGLLOADTRANSPOSEMATRIXDPROC)get_proc("glLoadTransposeMatrixd");
    glpfMultTransposeMatrixf = (PFNGLMULTTRANSPOSEMATRIXFPROC)get_proc("glMultTransposeMatrixf");
    glpfMultTransposeMatrixd = (PFNGLMULTTRANSPOSEMATRIXDPROC)get_proc("glMultTransposeMatrixd");

    // VERSION_1_4

    glpfBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)get_proc("glBlendFuncSeparate");
    glpfMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)get_proc("glMultiDrawArrays");
    glpfMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)get_proc("glMultiDrawElements");
    glpfPointParameterf = (PFNGLPOINTPARAMETERFPROC)get_proc("glPointParameterf");
    glpfPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)get_proc("glPointParameterfv");
    glpfPointParameteri = (PFNGLPOINTPARAMETERIPROC)get_proc("glPointParameteri");
    glpfPointParameteriv = (PFNGLPOINTPARAMETERIVPROC)get_proc("glPointParameteriv");

    // VERSION_1_4_DEPRECATED

    glpfFogCoordf = (PFNGLFOGCOORDFPROC)get_proc("glFogCoordf");
    glpfFogCoordfv = (PFNGLFOGCOORDFVPROC)get_proc("glFogCoordfv");
    glpfFogCoordd = (PFNGLFOGCOORDDPROC)get_proc("glFogCoordd");
    glpfFogCoorddv = (PFNGLFOGCOORDDVPROC)get_proc("glFogCoorddv");
    glpfFogCoordPointer = (PFNGLFOGCOORDPOINTERPROC)get_proc("glFogCoordPointer");
    glpfSecondaryColor3b = (PFNGLSECONDARYCOLOR3BPROC)get_proc("glSecondaryColor3b");
    glpfSecondaryColor3bv = (PFNGLSECONDARYCOLOR3BVPROC)get_proc("glSecondaryColor3bv");
    glpfSecondaryColor3d = (PFNGLSECONDARYCOLOR3DPROC)get_proc("glSecondaryColor3d");
    glpfSecondaryColor3dv = (PFNGLSECONDARYCOLOR3DVPROC)get_proc("glSecondaryColor3dv");
    glpfSecondaryColor3f = (PFNGLSECONDARYCOLOR3FPROC)get_proc("glSecondaryColor3f");
    glpfSecondaryColor3fv = (PFNGLSECONDARYCOLOR3FVPROC)get_proc("glSecondaryColor3fv");
    glpfSecondaryColor3i = (PFNGLSECONDARYCOLOR3IPROC)get_proc("glSecondaryColor3i");
    glpfSecondaryColor3iv = (PFNGLSECONDARYCOLOR3IVPROC)get_proc("glSecondaryColor3iv");
    glpfSecondaryColor3s = (PFNGLSECONDARYCOLOR3SPROC)get_proc("glSecondaryColor3s");
    glpfSecondaryColor3sv = (PFNGLSECONDARYCOLOR3SVPROC)get_proc("glSecondaryColor3sv");
    glpfSecondaryColor3ub = (PFNGLSECONDARYCOLOR3UBPROC)get_proc("glSecondaryColor3ub");
    glpfSecondaryColor3ubv = (PFNGLSECONDARYCOLOR3UBVPROC)get_proc("glSecondaryColor3ubv");
    glpfSecondaryColor3ui = (PFNGLSECONDARYCOLOR3UIPROC)get_proc("glSecondaryColor3ui");
    glpfSecondaryColor3uiv = (PFNGLSECONDARYCOLOR3UIVPROC)get_proc("glSecondaryColor3uiv");
    glpfSecondaryColor3us = (PFNGLSECONDARYCOLOR3USPROC)get_proc("glSecondaryColor3us");
    glpfSecondaryColor3usv = (PFNGLSECONDARYCOLOR3USVPROC)get_proc("glSecondaryColor3usv");
    glpfSecondaryColorPointer = (PFNGLSECONDARYCOLORPOINTERPROC)get_proc("glSecondaryColorPointer");
    glpfWindowPos2d = (PFNGLWINDOWPOS2DPROC)get_proc("glWindowPos2d");
    glpfWindowPos2dv = (PFNGLWINDOWPOS2DVPROC)get_proc("glWindowPos2dv");
    glpfWindowPos2f = (PFNGLWINDOWPOS2FPROC)get_proc("glWindowPos2f");
    glpfWindowPos2fv = (PFNGLWINDOWPOS2FVPROC)get_proc("glWindowPos2fv");
    glpfWindowPos2i = (PFNGLWINDOWPOS2IPROC)get_proc("glWindowPos2i");
    glpfWindowPos2iv = (PFNGLWINDOWPOS2IVPROC)get_proc("glWindowPos2iv");
    glpfWindowPos2s = (PFNGLWINDOWPOS2SPROC)get_proc("glWindowPos2s");
    glpfWindowPos2sv = (PFNGLWINDOWPOS2SVPROC)get_proc("glWindowPos2sv");
    glpfWindowPos3d = (PFNGLWINDOWPOS3DPROC)get_proc("glWindowPos3d");
    glpfWindowPos3dv = (PFNGLWINDOWPOS3DVPROC)get_proc("glWindowPos3dv");
    glpfWindowPos3f = (PFNGLWINDOWPOS3FPROC)get_proc("glWindowPos3f");
    glpfWindowPos3fv = (PFNGLWINDOWPOS3FVPROC)get_proc("glWindowPos3fv");
    glpfWindowPos3i = (PFNGLWINDOWPOS3IPROC)get_proc("glWindowPos3i");
    glpfWindowPos3iv = (PFNGLWINDOWPOS3IVPROC)get_proc("glWindowPos3iv");
    glpfWindowPos3s = (PFNGLWINDOWPOS3SPROC)get_proc("glWindowPos3s");
    glpfWindowPos3sv = (PFNGLWINDOWPOS3SVPROC)get_proc("glWindowPos3sv");

    // VERSION_1_5

    glpfGenQueries = (PFNGLGENQUERIESPROC)get_proc("glGenQueries");
    glpfDeleteQueries = (PFNGLDELETEQUERIESPROC)get_proc("glDeleteQueries");
    glpfIsQuery = (PFNGLISQUERYPROC)get_proc("glIsQuery");
    glpfBeginQuery = (PFNGLBEGINQUERYPROC)get_proc("glBeginQuery");
    glpfEndQuery = (PFNGLENDQUERYPROC)get_proc("glEndQuery");
    glpfGetQueryiv = (PFNGLGETQUERYIVPROC)get_proc("glGetQueryiv");
    glpfGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)get_proc("glGetQueryObjectiv");
    glpfGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)get_proc("glGetQueryObjectuiv");
    glpfBindBuffer = (PFNGLBINDBUFFERPROC)get_proc("glBindBuffer");
    glpfDeleteBuffers = (PFNGLDELETEBUFFERSPROC)get_proc("glDeleteBuffers");
    glpfGenBuffers = (PFNGLGENBUFFERSPROC)get_proc("glGenBuffers");
    glpfIsBuffer = (PFNGLISBUFFERPROC)get_proc("glIsBuffer");
    glpfBufferData = (PFNGLBUFFERDATAPROC)get_proc("glBufferData");
    glpfBufferSubData = (PFNGLBUFFERSUBDATAPROC)get_proc("glBufferSubData");
    glpfGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)get_proc("glGetBufferSubData");
    glpfMapBuffer = (PFNGLMAPBUFFERPROC)get_proc("glMapBuffer");
    glpfUnmapBuffer = (PFNGLUNMAPBUFFERPROC)get_proc("glUnmapBuffer");
    glpfGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)get_proc("glGetBufferParameteriv");
    glpfGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)get_proc("glGetBufferPointerv");

    // VERSION_2_0

    glpfBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)get_proc("glBlendEquationSeparate");
    glpfDrawBuffers = (PFNGLDRAWBUFFERSPROC)get_proc("glDrawBuffers");
    glpfStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)get_proc("glStencilOpSeparate");
    glpfStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)get_proc("glStencilFuncSeparate");
    glpfStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)get_proc("glStencilMaskSeparate");
    glpfAttachShader = (PFNGLATTACHSHADERPROC)get_proc("glAttachShader");
    glpfBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)get_proc("glBindAttribLocation");
    glpfCompileShader = (PFNGLCOMPILESHADERPROC)get_proc("glCompileShader");
    glpfCreateProgram = (PFNGLCREATEPROGRAMPROC)get_proc("glCreateProgram");
    glpfCreateShader = (PFNGLCREATESHADERPROC)get_proc("glCreateShader");
    glpfDeleteProgram = (PFNGLDELETEPROGRAMPROC)get_proc("glDeleteProgram");
    glpfDeleteShader = (PFNGLDELETESHADERPROC)get_proc("glDeleteShader");
    glpfDetachShader = (PFNGLDETACHSHADERPROC)get_proc("glDetachShader");
    glpfDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)get_proc("glDisableVertexAttribArray");
    glpfEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)get_proc("glEnableVertexAttribArray");
    glpfGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)get_proc("glGetActiveAttrib");
    glpfGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)get_proc("glGetActiveUniform");
    glpfGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)get_proc("glGetAttachedShaders");
    glpfGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)get_proc("glGetAttribLocation");
    glpfGetProgramiv = (PFNGLGETPROGRAMIVPROC)get_proc("glGetProgramiv");
    glpfGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)get_proc("glGetProgramInfoLog");
    glpfGetShaderiv = (PFNGLGETSHADERIVPROC)get_proc("glGetShaderiv");
    glpfGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)get_proc("glGetShaderInfoLog");
    glpfGetShaderSource = (PFNGLGETSHADERSOURCEPROC)get_proc("glGetShaderSource");
    glpfGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)get_proc("glGetUniformLocation");
    glpfGetUniformfv = (PFNGLGETUNIFORMFVPROC)get_proc("glGetUniformfv");
    glpfGetUniformiv = (PFNGLGETUNIFORMIVPROC)get_proc("glGetUniformiv");
    glpfGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)get_proc("glGetVertexAttribdv");
    glpfGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)get_proc("glGetVertexAttribfv");
    glpfGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)get_proc("glGetVertexAttribiv");
    glpfGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)get_proc("glGetVertexAttribPointerv");
    glpfIsProgram = (PFNGLISPROGRAMPROC)get_proc("glIsProgram");
    glpfIsShader = (PFNGLISSHADERPROC)get_proc("glIsShader");
    glpfLinkProgram = (PFNGLLINKPROGRAMPROC)get_proc("glLinkProgram");
    glpfShaderSource = (PFNGLSHADERSOURCEPROC)get_proc("glShaderSource");
    glpfUseProgram = (PFNGLUSEPROGRAMPROC)get_proc("glUseProgram");
    glpfUniform1f = (PFNGLUNIFORM1FPROC)get_proc("glUniform1f");
    glpfUniform2f = (PFNGLUNIFORM2FPROC)get_proc("glUniform2f");
    glpfUniform3f = (PFNGLUNIFORM3FPROC)get_proc("glUniform3f");
    glpfUniform4f = (PFNGLUNIFORM4FPROC)get_proc("glUniform4f");
    glpfUniform1i = (PFNGLUNIFORM1IPROC)get_proc("glUniform1i");
    glpfUniform2i = (PFNGLUNIFORM2IPROC)get_proc("glUniform2i");
    glpfUniform3i = (PFNGLUNIFORM3IPROC)get_proc("glUniform3i");
    glpfUniform4i = (PFNGLUNIFORM4IPROC)get_proc("glUniform4i");
    glpfUniform1fv = (PFNGLUNIFORM1FVPROC)get_proc("glUniform1fv");
    glpfUniform2fv = (PFNGLUNIFORM2FVPROC)get_proc("glUniform2fv");
    glpfUniform3fv = (PFNGLUNIFORM3FVPROC)get_proc("glUniform3fv");
    glpfUniform4fv = (PFNGLUNIFORM4FVPROC)get_proc("glUniform4fv");
    glpfUniform1iv = (PFNGLUNIFORM1IVPROC)get_proc("glUniform1iv");
    glpfUniform2iv = (PFNGLUNIFORM2IVPROC)get_proc("glUniform2iv");
    glpfUniform3iv = (PFNGLUNIFORM3IVPROC)get_proc("glUniform3iv");
    glpfUniform4iv = (PFNGLUNIFORM4IVPROC)get_proc("glUniform4iv");
    glpfUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)get_proc("glUniformMatrix2fv");
    glpfUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)get_proc("glUniformMatrix3fv");
    glpfUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)get_proc("glUniformMatrix4fv");
    glpfValidateProgram = (PFNGLVALIDATEPROGRAMPROC)get_proc("glValidateProgram");
    glpfVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC)get_proc("glVertexAttrib1d");
    glpfVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC)get_proc("glVertexAttrib1dv");
    glpfVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)get_proc("glVertexAttrib1f");
    glpfVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)get_proc("glVertexAttrib1fv");
    glpfVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC)get_proc("glVertexAttrib1s");
    glpfVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC)get_proc("glVertexAttrib1sv");
    glpfVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC)get_proc("glVertexAttrib2d");
    glpfVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC)get_proc("glVertexAttrib2dv");
    glpfVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)get_proc("glVertexAttrib2f");
    glpfVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)get_proc("glVertexAttrib2fv");
    glpfVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC)get_proc("glVertexAttrib2s");
    glpfVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC)get_proc("glVertexAttrib2sv");
    glpfVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC)get_proc("glVertexAttrib3d");
    glpfVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC)get_proc("glVertexAttrib3dv");
    glpfVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)get_proc("glVertexAttrib3f");
    glpfVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)get_proc("glVertexAttrib3fv");
    glpfVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC)get_proc("glVertexAttrib3s");
    glpfVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC)get_proc("glVertexAttrib3sv");
    glpfVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC)get_proc("glVertexAttrib4Nbv");
    glpfVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC)get_proc("glVertexAttrib4Niv");
    glpfVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC)get_proc("glVertexAttrib4Nsv");
    glpfVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC)get_proc("glVertexAttrib4Nub");
    glpfVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC)get_proc("glVertexAttrib4Nubv");
    glpfVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC)get_proc("glVertexAttrib4Nuiv");
    glpfVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC)get_proc("glVertexAttrib4Nusv");
    glpfVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC)get_proc("glVertexAttrib4bv");
    glpfVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC)get_proc("glVertexAttrib4d");
    glpfVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC)get_proc("glVertexAttrib4dv");
    glpfVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)get_proc("glVertexAttrib4f");
    glpfVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)get_proc("glVertexAttrib4fv");
    glpfVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC)get_proc("glVertexAttrib4iv");
    glpfVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC)get_proc("glVertexAttrib4s");
    glpfVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC)get_proc("glVertexAttrib4sv");
    glpfVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC)get_proc("glVertexAttrib4ubv");
    glpfVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC)get_proc("glVertexAttrib4uiv");
    glpfVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC)get_proc("glVertexAttrib4usv");
    glpfVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)get_proc("glVertexAttribPointer");

    // VERSION_2_1

    glpfUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)get_proc("glUniformMatrix2x3fv");
    glpfUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)get_proc("glUniformMatrix3x2fv");
    glpfUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)get_proc("glUniformMatrix2x4fv");
    glpfUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)get_proc("glUniformMatrix4x2fv");
    glpfUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)get_proc("glUniformMatrix3x4fv");
    glpfUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)get_proc("glUniformMatrix4x3fv");

    // VERSION_3_0

    glpfColorMaski = (PFNGLCOLORMASKIPROC)get_proc("glColorMaski");
    glpfGetBooleani_v = (PFNGLGETBOOLEANI_VPROC)get_proc("glGetBooleani_v");
    glpfGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)get_proc("glGetIntegeri_v");
    glpfEnablei = (PFNGLENABLEIPROC)get_proc("glEnablei");
    glpfDisablei = (PFNGLDISABLEIPROC)get_proc("glDisablei");
    glpfIsEnabledi = (PFNGLISENABLEDIPROC)get_proc("glIsEnabledi");
    glpfBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)get_proc("glBeginTransformFeedback");
    glpfEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)get_proc("glEndTransformFeedback");
    glpfBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)get_proc("glBindBufferRange");
    glpfBindBufferBase = (PFNGLBINDBUFFERBASEPROC)get_proc("glBindBufferBase");
    glpfTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)get_proc("glTransformFeedbackVaryings");
    glpfGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)get_proc("glGetTransformFeedbackVarying");
    glpfClampColor = (PFNGLCLAMPCOLORPROC)get_proc("glClampColor");
    glpfBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)get_proc("glBeginConditionalRender");
    glpfEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC)get_proc("glEndConditionalRender");
    glpfVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)get_proc("glVertexAttribIPointer");
    glpfGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)get_proc("glGetVertexAttribIiv");
    glpfGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)get_proc("glGetVertexAttribIuiv");
    glpfVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC)get_proc("glVertexAttribI1i");
    glpfVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC)get_proc("glVertexAttribI2i");
    glpfVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC)get_proc("glVertexAttribI3i");
    glpfVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)get_proc("glVertexAttribI4i");
    glpfVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC)get_proc("glVertexAttribI1ui");
    glpfVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC)get_proc("glVertexAttribI2ui");
    glpfVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC)get_proc("glVertexAttribI3ui");
    glpfVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)get_proc("glVertexAttribI4ui");
    glpfVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC)get_proc("glVertexAttribI1iv");
    glpfVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC)get_proc("glVertexAttribI2iv");
    glpfVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC)get_proc("glVertexAttribI3iv");
    glpfVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)get_proc("glVertexAttribI4iv");
    glpfVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC)get_proc("glVertexAttribI1uiv");
    glpfVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC)get_proc("glVertexAttribI2uiv");
    glpfVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC)get_proc("glVertexAttribI3uiv");
    glpfVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)get_proc("glVertexAttribI4uiv");
    glpfVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC)get_proc("glVertexAttribI4bv");
    glpfVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC)get_proc("glVertexAttribI4sv");
    glpfVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC)get_proc("glVertexAttribI4ubv");
    glpfVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC)get_proc("glVertexAttribI4usv");
    glpfGetUniformuiv = (PFNGLGETUNIFORMUIVPROC)get_proc("glGetUniformuiv");
    glpfBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)get_proc("glBindFragDataLocation");
    glpfGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)get_proc("glGetFragDataLocation");
    glpfUniform1ui = (PFNGLUNIFORM1UIPROC)get_proc("glUniform1ui");
    glpfUniform2ui = (PFNGLUNIFORM2UIPROC)get_proc("glUniform2ui");
    glpfUniform3ui = (PFNGLUNIFORM3UIPROC)get_proc("glUniform3ui");
    glpfUniform4ui = (PFNGLUNIFORM4UIPROC)get_proc("glUniform4ui");
    glpfUniform1uiv = (PFNGLUNIFORM1UIVPROC)get_proc("glUniform1uiv");
    glpfUniform2uiv = (PFNGLUNIFORM2UIVPROC)get_proc("glUniform2uiv");
    glpfUniform3uiv = (PFNGLUNIFORM3UIVPROC)get_proc("glUniform3uiv");
    glpfUniform4uiv = (PFNGLUNIFORM4UIVPROC)get_proc("glUniform4uiv");
    glpfTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC)get_proc("glTexParameterIiv");
    glpfTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC)get_proc("glTexParameterIuiv");
    glpfGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC)get_proc("glGetTexParameterIiv");
    glpfGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC)get_proc("glGetTexParameterIuiv");
    glpfClearBufferiv = (PFNGLCLEARBUFFERIVPROC)get_proc("glClearBufferiv");
    glpfClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)get_proc("glClearBufferuiv");
    glpfClearBufferfv = (PFNGLCLEARBUFFERFVPROC)get_proc("glClearBufferfv");
    glpfClearBufferfi = (PFNGLCLEARBUFFERFIPROC)get_proc("glClearBufferfi");
    glpfGetStringi = (PFNGLGETSTRINGIPROC)get_proc("glGetStringi");
    glpfIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)get_proc("glIsRenderbuffer");
    glpfBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)get_proc("glBindRenderbuffer");
    glpfDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)get_proc("glDeleteRenderbuffers");
    glpfGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)get_proc("glGenRenderbuffers");
    glpfRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)get_proc("glRenderbufferStorage");
    glpfGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)get_proc("glGetRenderbufferParameteriv");
    glpfIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)get_proc("glIsFramebuffer");
    glpfBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)get_proc("glBindFramebuffer");
    glpfDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)get_proc("glDeleteFramebuffers");
    glpfGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)get_proc("glGenFramebuffers");
    glpfCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)get_proc("glCheckFramebufferStatus");
    glpfFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)get_proc("glFramebufferTexture1D");
    glpfFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)get_proc("glFramebufferTexture2D");
    glpfFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)get_proc("glFramebufferTexture3D");
    glpfFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)get_proc("glFramebufferRenderbuffer");
    glpfGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)get_proc("glGetFramebufferAttachmentParameteriv");
    glpfGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)get_proc("glGenerateMipmap");
    glpfBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)get_proc("glBlitFramebuffer");
    glpfRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)get_proc("glRenderbufferStorageMultisample");
    glpfFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)get_proc("glFramebufferTextureLayer");
    glpfMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)get_proc("glMapBufferRange");
    glpfFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)get_proc("glFlushMappedBufferRange");
    glpfBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)get_proc("glBindVertexArray");
    glpfDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)get_proc("glDeleteVertexArrays");
    glpfGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)get_proc("glGenVertexArrays");
    glpfIsVertexArray = (PFNGLISVERTEXARRAYPROC)get_proc("glIsVertexArray");

    // VERSION_3_1

    glpfDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)get_proc("glDrawArraysInstanced");
    glpfDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)get_proc("glDrawElementsInstanced");
    glpfTexBuffer = (PFNGLTEXBUFFERPROC)get_proc("glTexBuffer");
    glpfPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)get_proc("glPrimitiveRestartIndex");
    glpfCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)get_proc("glCopyBufferSubData");
    glpfGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)get_proc("glGetUniformIndices");
    glpfGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)get_proc("glGetActiveUniformsiv");
    glpfGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)get_proc("glGetActiveUniformName");
    glpfGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)get_proc("glGetUniformBlockIndex");
    glpfGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)get_proc("glGetActiveUniformBlockiv");
    glpfGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)get_proc("glGetActiveUniformBlockName");
    glpfUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)get_proc("glUniformBlockBinding");

    // VERSION_3_2

    glpfGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)get_proc("glGetInteger64i_v");
    glpfGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)get_proc("glGetBufferParameteri64v");
    glpfFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)get_proc("glFramebufferTexture");
    glpfDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)get_proc("glDrawElementsBaseVertex");
    glpfDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)get_proc("glDrawRangeElementsBaseVertex");
    glpfDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)get_proc("glDrawElementsInstancedBaseVertex");
    glpfMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)get_proc("glMultiDrawElementsBaseVertex");
    glpfProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)get_proc("glProvokingVertex");
    glpfFenceSync = (PFNGLFENCESYNCPROC)get_proc("glFenceSync");
    glpfIsSync = (PFNGLISSYNCPROC)get_proc("glIsSync");
    glpfDeleteSync = (PFNGLDELETESYNCPROC)get_proc("glDeleteSync");
    glpfClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)get_proc("glClientWaitSync");
    glpfWaitSync = (PFNGLWAITSYNCPROC)get_proc("glWaitSync");
    glpfGetInteger64v = (PFNGLGETINTEGER64VPROC)get_proc("glGetInteger64v");
    glpfGetSynciv = (PFNGLGETSYNCIVPROC)get_proc("glGetSynciv");
    glpfTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)get_proc("glTexImage2DMultisample");
    glpfTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)get_proc("glTexImage3DMultisample");
    glpfGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC)get_proc("glGetMultisamplefv");
    glpfSampleMaski = (PFNGLSAMPLEMASKIPROC)get_proc("glSampleMaski");

    // VERSION_3_3

    glpfVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)get_proc("glVertexAttribDivisor");
    glpfBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)get_proc("glBindFragDataLocationIndexed");
    glpfGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC)get_proc("glGetFragDataIndex");
    glpfGenSamplers = (PFNGLGENSAMPLERSPROC)get_proc("glGenSamplers");
    glpfDeleteSamplers = (PFNGLDELETESAMPLERSPROC)get_proc("glDeleteSamplers");
    glpfIsSampler = (PFNGLISSAMPLERPROC)get_proc("glIsSampler");
    glpfBindSampler = (PFNGLBINDSAMPLERPROC)get_proc("glBindSampler");
    glpfSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)get_proc("glSamplerParameteri");
    glpfSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)get_proc("glSamplerParameteriv");
    glpfSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)get_proc("glSamplerParameterf");
    glpfSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)get_proc("glSamplerParameterfv");
    glpfSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)get_proc("glSamplerParameterIiv");
    glpfSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)get_proc("glSamplerParameterIuiv");
    glpfGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)get_proc("glGetSamplerParameteriv");
    glpfGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)get_proc("glGetSamplerParameterIiv");
    glpfGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)get_proc("glGetSamplerParameterfv");
    glpfGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)get_proc("glGetSamplerParameterIuiv");
    glpfQueryCounter = (PFNGLQUERYCOUNTERPROC)get_proc("glQueryCounter");
    glpfGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC)get_proc("glGetQueryObjecti64v");
    glpfGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC)get_proc("glGetQueryObjectui64v");
    glpfVertexP2ui = (PFNGLVERTEXP2UIPROC)get_proc("glVertexP2ui");
    glpfVertexP2uiv = (PFNGLVERTEXP2UIVPROC)get_proc("glVertexP2uiv");
    glpfVertexP3ui = (PFNGLVERTEXP3UIPROC)get_proc("glVertexP3ui");
    glpfVertexP3uiv = (PFNGLVERTEXP3UIVPROC)get_proc("glVertexP3uiv");
    glpfVertexP4ui = (PFNGLVERTEXP4UIPROC)get_proc("glVertexP4ui");
    glpfVertexP4uiv = (PFNGLVERTEXP4UIVPROC)get_proc("glVertexP4uiv");
    glpfTexCoordP1ui = (PFNGLTEXCOORDP1UIPROC)get_proc("glTexCoordP1ui");
    glpfTexCoordP1uiv = (PFNGLTEXCOORDP1UIVPROC)get_proc("glTexCoordP1uiv");
    glpfTexCoordP2ui = (PFNGLTEXCOORDP2UIPROC)get_proc("glTexCoordP2ui");
    glpfTexCoordP2uiv = (PFNGLTEXCOORDP2UIVPROC)get_proc("glTexCoordP2uiv");
    glpfTexCoordP3ui = (PFNGLTEXCOORDP3UIPROC)get_proc("glTexCoordP3ui");
    glpfTexCoordP3uiv = (PFNGLTEXCOORDP3UIVPROC)get_proc("glTexCoordP3uiv");
    glpfTexCoordP4ui = (PFNGLTEXCOORDP4UIPROC)get_proc("glTexCoordP4ui");
    glpfTexCoordP4uiv = (PFNGLTEXCOORDP4UIVPROC)get_proc("glTexCoordP4uiv");
    glpfMultiTexCoordP1ui = (PFNGLMULTITEXCOORDP1UIPROC)get_proc("glMultiTexCoordP1ui");
    glpfMultiTexCoordP1uiv = (PFNGLMULTITEXCOORDP1UIVPROC)get_proc("glMultiTexCoordP1uiv");
    glpfMultiTexCoordP2ui = (PFNGLMULTITEXCOORDP2UIPROC)get_proc("glMultiTexCoordP2ui");
    glpfMultiTexCoordP2uiv = (PFNGLMULTITEXCOORDP2UIVPROC)get_proc("glMultiTexCoordP2uiv");
    glpfMultiTexCoordP3ui = (PFNGLMULTITEXCOORDP3UIPROC)get_proc("glMultiTexCoordP3ui");
    glpfMultiTexCoordP3uiv = (PFNGLMULTITEXCOORDP3UIVPROC)get_proc("glMultiTexCoordP3uiv");
    glpfMultiTexCoordP4ui = (PFNGLMULTITEXCOORDP4UIPROC)get_proc("glMultiTexCoordP4ui");
    glpfMultiTexCoordP4uiv = (PFNGLMULTITEXCOORDP4UIVPROC)get_proc("glMultiTexCoordP4uiv");
    glpfNormalP3ui = (PFNGLNORMALP3UIPROC)get_proc("glNormalP3ui");
    glpfNormalP3uiv = (PFNGLNORMALP3UIVPROC)get_proc("glNormalP3uiv");
    glpfColorP3ui = (PFNGLCOLORP3UIPROC)get_proc("glColorP3ui");
    glpfColorP3uiv = (PFNGLCOLORP3UIVPROC)get_proc("glColorP3uiv");
    glpfColorP4ui = (PFNGLCOLORP4UIPROC)get_proc("glColorP4ui");
    glpfColorP4uiv = (PFNGLCOLORP4UIVPROC)get_proc("glColorP4uiv");
    glpfSecondaryColorP3ui = (PFNGLSECONDARYCOLORP3UIPROC)get_proc("glSecondaryColorP3ui");
    glpfSecondaryColorP3uiv = (PFNGLSECONDARYCOLORP3UIVPROC)get_proc("glSecondaryColorP3uiv");
    glpfVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC)get_proc("glVertexAttribP1ui");
    glpfVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC)get_proc("glVertexAttribP1uiv");
    glpfVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC)get_proc("glVertexAttribP2ui");
    glpfVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC)get_proc("glVertexAttribP2uiv");
    glpfVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC)get_proc("glVertexAttribP3ui");
    glpfVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC)get_proc("glVertexAttribP3uiv");
    glpfVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC)get_proc("glVertexAttribP4ui");
    glpfVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC)get_proc("glVertexAttribP4uiv");
}

// ----------------------- Extension flag definitions ---------------------- 

// ----------------- Function pointer definitions ----------------

PFNGLBLENDCOLORPROC glpfBlendColor = NULL;
PFNGLBLENDEQUATIONPROC glpfBlendEquation = NULL;
PFNGLDRAWRANGEELEMENTSPROC glpfDrawRangeElements = NULL;
PFNGLTEXIMAGE3DPROC glpfTexImage3D = NULL;
PFNGLTEXSUBIMAGE3DPROC glpfTexSubImage3D = NULL;
PFNGLCOPYTEXSUBIMAGE3DPROC glpfCopyTexSubImage3D = NULL;
PFNGLCOLORTABLEPROC glpfColorTable = NULL;
PFNGLCOLORTABLEPARAMETERFVPROC glpfColorTableParameterfv = NULL;
PFNGLCOLORTABLEPARAMETERIVPROC glpfColorTableParameteriv = NULL;
PFNGLCOPYCOLORTABLEPROC glpfCopyColorTable = NULL;
PFNGLGETCOLORTABLEPROC glpfGetColorTable = NULL;
PFNGLGETCOLORTABLEPARAMETERFVPROC glpfGetColorTableParameterfv = NULL;
PFNGLGETCOLORTABLEPARAMETERIVPROC glpfGetColorTableParameteriv = NULL;
PFNGLCOLORSUBTABLEPROC glpfColorSubTable = NULL;
PFNGLCOPYCOLORSUBTABLEPROC glpfCopyColorSubTable = NULL;
PFNGLCONVOLUTIONFILTER1DPROC glpfConvolutionFilter1D = NULL;
PFNGLCONVOLUTIONFILTER2DPROC glpfConvolutionFilter2D = NULL;
PFNGLCONVOLUTIONPARAMETERFPROC glpfConvolutionParameterf = NULL;
PFNGLCONVOLUTIONPARAMETERFVPROC glpfConvolutionParameterfv = NULL;
PFNGLCONVOLUTIONPARAMETERIPROC glpfConvolutionParameteri = NULL;
PFNGLCONVOLUTIONPARAMETERIVPROC glpfConvolutionParameteriv = NULL;
PFNGLCOPYCONVOLUTIONFILTER1DPROC glpfCopyConvolutionFilter1D = NULL;
PFNGLCOPYCONVOLUTIONFILTER2DPROC glpfCopyConvolutionFilter2D = NULL;
PFNGLGETCONVOLUTIONFILTERPROC glpfGetConvolutionFilter = NULL;
PFNGLGETCONVOLUTIONPARAMETERFVPROC glpfGetConvolutionParameterfv = NULL;
PFNGLGETCONVOLUTIONPARAMETERIVPROC glpfGetConvolutionParameteriv = NULL;
PFNGLGETSEPARABLEFILTERPROC glpfGetSeparableFilter = NULL;
PFNGLSEPARABLEFILTER2DPROC glpfSeparableFilter2D = NULL;
PFNGLGETHISTOGRAMPROC glpfGetHistogram = NULL;
PFNGLGETHISTOGRAMPARAMETERFVPROC glpfGetHistogramParameterfv = NULL;
PFNGLGETHISTOGRAMPARAMETERIVPROC glpfGetHistogramParameteriv = NULL;
PFNGLGETMINMAXPROC glpfGetMinmax = NULL;
PFNGLGETMINMAXPARAMETERFVPROC glpfGetMinmaxParameterfv = NULL;
PFNGLGETMINMAXPARAMETERIVPROC glpfGetMinmaxParameteriv = NULL;
PFNGLHISTOGRAMPROC glpfHistogram = NULL;
PFNGLMINMAXPROC glpfMinmax = NULL;
PFNGLRESETHISTOGRAMPROC glpfResetHistogram = NULL;
PFNGLRESETMINMAXPROC glpfResetMinmax = NULL;
PFNGLACTIVETEXTUREPROC glpfActiveTexture = NULL;
PFNGLSAMPLECOVERAGEPROC glpfSampleCoverage = NULL;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glpfCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glpfCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glpfCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glpfCompressedTexSubImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glpfCompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glpfCompressedTexSubImage1D = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glpfGetCompressedTexImage = NULL;
PFNGLCLIENTACTIVETEXTUREPROC glpfClientActiveTexture = NULL;
PFNGLMULTITEXCOORD1DPROC glpfMultiTexCoord1d = NULL;
PFNGLMULTITEXCOORD1DVPROC glpfMultiTexCoord1dv = NULL;
PFNGLMULTITEXCOORD1FPROC glpfMultiTexCoord1f = NULL;
PFNGLMULTITEXCOORD1FVPROC glpfMultiTexCoord1fv = NULL;
PFNGLMULTITEXCOORD1IPROC glpfMultiTexCoord1i = NULL;
PFNGLMULTITEXCOORD1IVPROC glpfMultiTexCoord1iv = NULL;
PFNGLMULTITEXCOORD1SPROC glpfMultiTexCoord1s = NULL;
PFNGLMULTITEXCOORD1SVPROC glpfMultiTexCoord1sv = NULL;
PFNGLMULTITEXCOORD2DPROC glpfMultiTexCoord2d = NULL;
PFNGLMULTITEXCOORD2DVPROC glpfMultiTexCoord2dv = NULL;
PFNGLMULTITEXCOORD2FPROC glpfMultiTexCoord2f = NULL;
PFNGLMULTITEXCOORD2FVPROC glpfMultiTexCoord2fv = NULL;
PFNGLMULTITEXCOORD2IPROC glpfMultiTexCoord2i = NULL;
PFNGLMULTITEXCOORD2IVPROC glpfMultiTexCoord2iv = NULL;
PFNGLMULTITEXCOORD2SPROC glpfMultiTexCoord2s = NULL;
PFNGLMULTITEXCOORD2SVPROC glpfMultiTexCoord2sv = NULL;
PFNGLMULTITEXCOORD3DPROC glpfMultiTexCoord3d = NULL;
PFNGLMULTITEXCOORD3DVPROC glpfMultiTexCoord3dv = NULL;
PFNGLMULTITEXCOORD3FPROC glpfMultiTexCoord3f = NULL;
PFNGLMULTITEXCOORD3FVPROC glpfMultiTexCoord3fv = NULL;
PFNGLMULTITEXCOORD3IPROC glpfMultiTexCoord3i = NULL;
PFNGLMULTITEXCOORD3IVPROC glpfMultiTexCoord3iv = NULL;
PFNGLMULTITEXCOORD3SPROC glpfMultiTexCoord3s = NULL;
PFNGLMULTITEXCOORD3SVPROC glpfMultiTexCoord3sv = NULL;
PFNGLMULTITEXCOORD4DPROC glpfMultiTexCoord4d = NULL;
PFNGLMULTITEXCOORD4DVPROC glpfMultiTexCoord4dv = NULL;
PFNGLMULTITEXCOORD4FPROC glpfMultiTexCoord4f = NULL;
PFNGLMULTITEXCOORD4FVPROC glpfMultiTexCoord4fv = NULL;
PFNGLMULTITEXCOORD4IPROC glpfMultiTexCoord4i = NULL;
PFNGLMULTITEXCOORD4IVPROC glpfMultiTexCoord4iv = NULL;
PFNGLMULTITEXCOORD4SPROC glpfMultiTexCoord4s = NULL;
PFNGLMULTITEXCOORD4SVPROC glpfMultiTexCoord4sv = NULL;
PFNGLLOADTRANSPOSEMATRIXFPROC glpfLoadTransposeMatrixf = NULL;
PFNGLLOADTRANSPOSEMATRIXDPROC glpfLoadTransposeMatrixd = NULL;
PFNGLMULTTRANSPOSEMATRIXFPROC glpfMultTransposeMatrixf = NULL;
PFNGLMULTTRANSPOSEMATRIXDPROC glpfMultTransposeMatrixd = NULL;
PFNGLBLENDFUNCSEPARATEPROC glpfBlendFuncSeparate = NULL;
PFNGLMULTIDRAWARRAYSPROC glpfMultiDrawArrays = NULL;
PFNGLMULTIDRAWELEMENTSPROC glpfMultiDrawElements = NULL;
PFNGLPOINTPARAMETERFPROC glpfPointParameterf = NULL;
PFNGLPOINTPARAMETERFVPROC glpfPointParameterfv = NULL;
PFNGLPOINTPARAMETERIPROC glpfPointParameteri = NULL;
PFNGLPOINTPARAMETERIVPROC glpfPointParameteriv = NULL;
PFNGLFOGCOORDFPROC glpfFogCoordf = NULL;
PFNGLFOGCOORDFVPROC glpfFogCoordfv = NULL;
PFNGLFOGCOORDDPROC glpfFogCoordd = NULL;
PFNGLFOGCOORDDVPROC glpfFogCoorddv = NULL;
PFNGLFOGCOORDPOINTERPROC glpfFogCoordPointer = NULL;
PFNGLSECONDARYCOLOR3BPROC glpfSecondaryColor3b = NULL;
PFNGLSECONDARYCOLOR3BVPROC glpfSecondaryColor3bv = NULL;
PFNGLSECONDARYCOLOR3DPROC glpfSecondaryColor3d = NULL;
PFNGLSECONDARYCOLOR3DVPROC glpfSecondaryColor3dv = NULL;
PFNGLSECONDARYCOLOR3FPROC glpfSecondaryColor3f = NULL;
PFNGLSECONDARYCOLOR3FVPROC glpfSecondaryColor3fv = NULL;
PFNGLSECONDARYCOLOR3IPROC glpfSecondaryColor3i = NULL;
PFNGLSECONDARYCOLOR3IVPROC glpfSecondaryColor3iv = NULL;
PFNGLSECONDARYCOLOR3SPROC glpfSecondaryColor3s = NULL;
PFNGLSECONDARYCOLOR3SVPROC glpfSecondaryColor3sv = NULL;
PFNGLSECONDARYCOLOR3UBPROC glpfSecondaryColor3ub = NULL;
PFNGLSECONDARYCOLOR3UBVPROC glpfSecondaryColor3ubv = NULL;
PFNGLSECONDARYCOLOR3UIPROC glpfSecondaryColor3ui = NULL;
PFNGLSECONDARYCOLOR3UIVPROC glpfSecondaryColor3uiv = NULL;
PFNGLSECONDARYCOLOR3USPROC glpfSecondaryColor3us = NULL;
PFNGLSECONDARYCOLOR3USVPROC glpfSecondaryColor3usv = NULL;
PFNGLSECONDARYCOLORPOINTERPROC glpfSecondaryColorPointer = NULL;
PFNGLWINDOWPOS2DPROC glpfWindowPos2d = NULL;
PFNGLWINDOWPOS2DVPROC glpfWindowPos2dv = NULL;
PFNGLWINDOWPOS2FPROC glpfWindowPos2f = NULL;
PFNGLWINDOWPOS2FVPROC glpfWindowPos2fv = NULL;
PFNGLWINDOWPOS2IPROC glpfWindowPos2i = NULL;
PFNGLWINDOWPOS2IVPROC glpfWindowPos2iv = NULL;
PFNGLWINDOWPOS2SPROC glpfWindowPos2s = NULL;
PFNGLWINDOWPOS2SVPROC glpfWindowPos2sv = NULL;
PFNGLWINDOWPOS3DPROC glpfWindowPos3d = NULL;
PFNGLWINDOWPOS3DVPROC glpfWindowPos3dv = NULL;
PFNGLWINDOWPOS3FPROC glpfWindowPos3f = NULL;
PFNGLWINDOWPOS3FVPROC glpfWindowPos3fv = NULL;
PFNGLWINDOWPOS3IPROC glpfWindowPos3i = NULL;
PFNGLWINDOWPOS3IVPROC glpfWindowPos3iv = NULL;
PFNGLWINDOWPOS3SPROC glpfWindowPos3s = NULL;
PFNGLWINDOWPOS3SVPROC glpfWindowPos3sv = NULL;
PFNGLGENQUERIESPROC glpfGenQueries = NULL;
PFNGLDELETEQUERIESPROC glpfDeleteQueries = NULL;
PFNGLISQUERYPROC glpfIsQuery = NULL;
PFNGLBEGINQUERYPROC glpfBeginQuery = NULL;
PFNGLENDQUERYPROC glpfEndQuery = NULL;
PFNGLGETQUERYIVPROC glpfGetQueryiv = NULL;
PFNGLGETQUERYOBJECTIVPROC glpfGetQueryObjectiv = NULL;
PFNGLGETQUERYOBJECTUIVPROC glpfGetQueryObjectuiv = NULL;
PFNGLBINDBUFFERPROC glpfBindBuffer = NULL;
PFNGLDELETEBUFFERSPROC glpfDeleteBuffers = NULL;
PFNGLGENBUFFERSPROC glpfGenBuffers = NULL;
PFNGLISBUFFERPROC glpfIsBuffer = NULL;
PFNGLBUFFERDATAPROC glpfBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glpfBufferSubData = NULL;
PFNGLGETBUFFERSUBDATAPROC glpfGetBufferSubData = NULL;
PFNGLMAPBUFFERPROC glpfMapBuffer = NULL;
PFNGLUNMAPBUFFERPROC glpfUnmapBuffer = NULL;
PFNGLGETBUFFERPARAMETERIVPROC glpfGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERVPROC glpfGetBufferPointerv = NULL;
PFNGLBLENDEQUATIONSEPARATEPROC glpfBlendEquationSeparate = NULL;
PFNGLDRAWBUFFERSPROC glpfDrawBuffers = NULL;
PFNGLSTENCILOPSEPARATEPROC glpfStencilOpSeparate = NULL;
PFNGLSTENCILFUNCSEPARATEPROC glpfStencilFuncSeparate = NULL;
PFNGLSTENCILMASKSEPARATEPROC glpfStencilMaskSeparate = NULL;
PFNGLATTACHSHADERPROC glpfAttachShader = NULL;
PFNGLBINDATTRIBLOCATIONPROC glpfBindAttribLocation = NULL;
PFNGLCOMPILESHADERPROC glpfCompileShader = NULL;
PFNGLCREATEPROGRAMPROC glpfCreateProgram = NULL;
PFNGLCREATESHADERPROC glpfCreateShader = NULL;
PFNGLDELETEPROGRAMPROC glpfDeleteProgram = NULL;
PFNGLDELETESHADERPROC glpfDeleteShader = NULL;
PFNGLDETACHSHADERPROC glpfDetachShader = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glpfDisableVertexAttribArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glpfEnableVertexAttribArray = NULL;
PFNGLGETACTIVEATTRIBPROC glpfGetActiveAttrib = NULL;
PFNGLGETACTIVEUNIFORMPROC glpfGetActiveUniform = NULL;
PFNGLGETATTACHEDSHADERSPROC glpfGetAttachedShaders = NULL;
PFNGLGETATTRIBLOCATIONPROC glpfGetAttribLocation = NULL;
PFNGLGETPROGRAMIVPROC glpfGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glpfGetProgramInfoLog = NULL;
PFNGLGETSHADERIVPROC glpfGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glpfGetShaderInfoLog = NULL;
PFNGLGETSHADERSOURCEPROC glpfGetShaderSource = NULL;
PFNGLGETUNIFORMLOCATIONPROC glpfGetUniformLocation = NULL;
PFNGLGETUNIFORMFVPROC glpfGetUniformfv = NULL;
PFNGLGETUNIFORMIVPROC glpfGetUniformiv = NULL;
PFNGLGETVERTEXATTRIBDVPROC glpfGetVertexAttribdv = NULL;
PFNGLGETVERTEXATTRIBFVPROC glpfGetVertexAttribfv = NULL;
PFNGLGETVERTEXATTRIBIVPROC glpfGetVertexAttribiv = NULL;
PFNGLGETVERTEXATTRIBPOINTERVPROC glpfGetVertexAttribPointerv = NULL;
PFNGLISPROGRAMPROC glpfIsProgram = NULL;
PFNGLISSHADERPROC glpfIsShader = NULL;
PFNGLLINKPROGRAMPROC glpfLinkProgram = NULL;
PFNGLSHADERSOURCEPROC glpfShaderSource = NULL;
PFNGLUSEPROGRAMPROC glpfUseProgram = NULL;
PFNGLUNIFORM1FPROC glpfUniform1f = NULL;
PFNGLUNIFORM2FPROC glpfUniform2f = NULL;
PFNGLUNIFORM3FPROC glpfUniform3f = NULL;
PFNGLUNIFORM4FPROC glpfUniform4f = NULL;
PFNGLUNIFORM1IPROC glpfUniform1i = NULL;
PFNGLUNIFORM2IPROC glpfUniform2i = NULL;
PFNGLUNIFORM3IPROC glpfUniform3i = NULL;
PFNGLUNIFORM4IPROC glpfUniform4i = NULL;
PFNGLUNIFORM1FVPROC glpfUniform1fv = NULL;
PFNGLUNIFORM2FVPROC glpfUniform2fv = NULL;
PFNGLUNIFORM3FVPROC glpfUniform3fv = NULL;
PFNGLUNIFORM4FVPROC glpfUniform4fv = NULL;
PFNGLUNIFORM1IVPROC glpfUniform1iv = NULL;
PFNGLUNIFORM2IVPROC glpfUniform2iv = NULL;
PFNGLUNIFORM3IVPROC glpfUniform3iv = NULL;
PFNGLUNIFORM4IVPROC glpfUniform4iv = NULL;
PFNGLUNIFORMMATRIX2FVPROC glpfUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC glpfUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC glpfUniformMatrix4fv = NULL;
PFNGLVALIDATEPROGRAMPROC glpfValidateProgram = NULL;
PFNGLVERTEXATTRIB1DPROC glpfVertexAttrib1d = NULL;
PFNGLVERTEXATTRIB1DVPROC glpfVertexAttrib1dv = NULL;
PFNGLVERTEXATTRIB1FPROC glpfVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB1FVPROC glpfVertexAttrib1fv = NULL;
PFNGLVERTEXATTRIB1SPROC glpfVertexAttrib1s = NULL;
PFNGLVERTEXATTRIB1SVPROC glpfVertexAttrib1sv = NULL;
PFNGLVERTEXATTRIB2DPROC glpfVertexAttrib2d = NULL;
PFNGLVERTEXATTRIB2DVPROC glpfVertexAttrib2dv = NULL;
PFNGLVERTEXATTRIB2FPROC glpfVertexAttrib2f = NULL;
PFNGLVERTEXATTRIB2FVPROC glpfVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB2SPROC glpfVertexAttrib2s = NULL;
PFNGLVERTEXATTRIB2SVPROC glpfVertexAttrib2sv = NULL;
PFNGLVERTEXATTRIB3DPROC glpfVertexAttrib3d = NULL;
PFNGLVERTEXATTRIB3DVPROC glpfVertexAttrib3dv = NULL;
PFNGLVERTEXATTRIB3FPROC glpfVertexAttrib3f = NULL;
PFNGLVERTEXATTRIB3FVPROC glpfVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB3SPROC glpfVertexAttrib3s = NULL;
PFNGLVERTEXATTRIB3SVPROC glpfVertexAttrib3sv = NULL;
PFNGLVERTEXATTRIB4NBVPROC glpfVertexAttrib4Nbv = NULL;
PFNGLVERTEXATTRIB4NIVPROC glpfVertexAttrib4Niv = NULL;
PFNGLVERTEXATTRIB4NSVPROC glpfVertexAttrib4Nsv = NULL;
PFNGLVERTEXATTRIB4NUBPROC glpfVertexAttrib4Nub = NULL;
PFNGLVERTEXATTRIB4NUBVPROC glpfVertexAttrib4Nubv = NULL;
PFNGLVERTEXATTRIB4NUIVPROC glpfVertexAttrib4Nuiv = NULL;
PFNGLVERTEXATTRIB4NUSVPROC glpfVertexAttrib4Nusv = NULL;
PFNGLVERTEXATTRIB4BVPROC glpfVertexAttrib4bv = NULL;
PFNGLVERTEXATTRIB4DPROC glpfVertexAttrib4d = NULL;
PFNGLVERTEXATTRIB4DVPROC glpfVertexAttrib4dv = NULL;
PFNGLVERTEXATTRIB4FPROC glpfVertexAttrib4f = NULL;
PFNGLVERTEXATTRIB4FVPROC glpfVertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB4IVPROC glpfVertexAttrib4iv = NULL;
PFNGLVERTEXATTRIB4SPROC glpfVertexAttrib4s = NULL;
PFNGLVERTEXATTRIB4SVPROC glpfVertexAttrib4sv = NULL;
PFNGLVERTEXATTRIB4UBVPROC glpfVertexAttrib4ubv = NULL;
PFNGLVERTEXATTRIB4UIVPROC glpfVertexAttrib4uiv = NULL;
PFNGLVERTEXATTRIB4USVPROC glpfVertexAttrib4usv = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glpfVertexAttribPointer = NULL;
PFNGLUNIFORMMATRIX2X3FVPROC glpfUniformMatrix2x3fv = NULL;
PFNGLUNIFORMMATRIX3X2FVPROC glpfUniformMatrix3x2fv = NULL;
PFNGLUNIFORMMATRIX2X4FVPROC glpfUniformMatrix2x4fv = NULL;
PFNGLUNIFORMMATRIX4X2FVPROC glpfUniformMatrix4x2fv = NULL;
PFNGLUNIFORMMATRIX3X4FVPROC glpfUniformMatrix3x4fv = NULL;
PFNGLUNIFORMMATRIX4X3FVPROC glpfUniformMatrix4x3fv = NULL;
PFNGLCOLORMASKIPROC glpfColorMaski = NULL;
PFNGLGETBOOLEANI_VPROC glpfGetBooleani_v = NULL;
PFNGLGETINTEGERI_VPROC glpfGetIntegeri_v = NULL;
PFNGLENABLEIPROC glpfEnablei = NULL;
PFNGLDISABLEIPROC glpfDisablei = NULL;
PFNGLISENABLEDIPROC glpfIsEnabledi = NULL;
PFNGLBEGINTRANSFORMFEEDBACKPROC glpfBeginTransformFeedback = NULL;
PFNGLENDTRANSFORMFEEDBACKPROC glpfEndTransformFeedback = NULL;
PFNGLBINDBUFFERRANGEPROC glpfBindBufferRange = NULL;
PFNGLBINDBUFFERBASEPROC glpfBindBufferBase = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glpfTransformFeedbackVaryings = NULL;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glpfGetTransformFeedbackVarying = NULL;
PFNGLCLAMPCOLORPROC glpfClampColor = NULL;
PFNGLBEGINCONDITIONALRENDERPROC glpfBeginConditionalRender = NULL;
PFNGLENDCONDITIONALRENDERPROC glpfEndConditionalRender = NULL;
PFNGLVERTEXATTRIBIPOINTERPROC glpfVertexAttribIPointer = NULL;
PFNGLGETVERTEXATTRIBIIVPROC glpfGetVertexAttribIiv = NULL;
PFNGLGETVERTEXATTRIBIUIVPROC glpfGetVertexAttribIuiv = NULL;
PFNGLVERTEXATTRIBI1IPROC glpfVertexAttribI1i = NULL;
PFNGLVERTEXATTRIBI2IPROC glpfVertexAttribI2i = NULL;
PFNGLVERTEXATTRIBI3IPROC glpfVertexAttribI3i = NULL;
PFNGLVERTEXATTRIBI4IPROC glpfVertexAttribI4i = NULL;
PFNGLVERTEXATTRIBI1UIPROC glpfVertexAttribI1ui = NULL;
PFNGLVERTEXATTRIBI2UIPROC glpfVertexAttribI2ui = NULL;
PFNGLVERTEXATTRIBI3UIPROC glpfVertexAttribI3ui = NULL;
PFNGLVERTEXATTRIBI4UIPROC glpfVertexAttribI4ui = NULL;
PFNGLVERTEXATTRIBI1IVPROC glpfVertexAttribI1iv = NULL;
PFNGLVERTEXATTRIBI2IVPROC glpfVertexAttribI2iv = NULL;
PFNGLVERTEXATTRIBI3IVPROC glpfVertexAttribI3iv = NULL;
PFNGLVERTEXATTRIBI4IVPROC glpfVertexAttribI4iv = NULL;
PFNGLVERTEXATTRIBI1UIVPROC glpfVertexAttribI1uiv = NULL;
PFNGLVERTEXATTRIBI2UIVPROC glpfVertexAttribI2uiv = NULL;
PFNGLVERTEXATTRIBI3UIVPROC glpfVertexAttribI3uiv = NULL;
PFNGLVERTEXATTRIBI4UIVPROC glpfVertexAttribI4uiv = NULL;
PFNGLVERTEXATTRIBI4BVPROC glpfVertexAttribI4bv = NULL;
PFNGLVERTEXATTRIBI4SVPROC glpfVertexAttribI4sv = NULL;
PFNGLVERTEXATTRIBI4UBVPROC glpfVertexAttribI4ubv = NULL;
PFNGLVERTEXATTRIBI4USVPROC glpfVertexAttribI4usv = NULL;
PFNGLGETUNIFORMUIVPROC glpfGetUniformuiv = NULL;
PFNGLBINDFRAGDATALOCATIONPROC glpfBindFragDataLocation = NULL;
PFNGLGETFRAGDATALOCATIONPROC glpfGetFragDataLocation = NULL;
PFNGLUNIFORM1UIPROC glpfUniform1ui = NULL;
PFNGLUNIFORM2UIPROC glpfUniform2ui = NULL;
PFNGLUNIFORM3UIPROC glpfUniform3ui = NULL;
PFNGLUNIFORM4UIPROC glpfUniform4ui = NULL;
PFNGLUNIFORM1UIVPROC glpfUniform1uiv = NULL;
PFNGLUNIFORM2UIVPROC glpfUniform2uiv = NULL;
PFNGLUNIFORM3UIVPROC glpfUniform3uiv = NULL;
PFNGLUNIFORM4UIVPROC glpfUniform4uiv = NULL;
PFNGLTEXPARAMETERIIVPROC glpfTexParameterIiv = NULL;
PFNGLTEXPARAMETERIUIVPROC glpfTexParameterIuiv = NULL;
PFNGLGETTEXPARAMETERIIVPROC glpfGetTexParameterIiv = NULL;
PFNGLGETTEXPARAMETERIUIVPROC glpfGetTexParameterIuiv = NULL;
PFNGLCLEARBUFFERIVPROC glpfClearBufferiv = NULL;
PFNGLCLEARBUFFERUIVPROC glpfClearBufferuiv = NULL;
PFNGLCLEARBUFFERFVPROC glpfClearBufferfv = NULL;
PFNGLCLEARBUFFERFIPROC glpfClearBufferfi = NULL;
PFNGLGETSTRINGIPROC glpfGetStringi = NULL;
PFNGLISRENDERBUFFERPROC glpfIsRenderbuffer = NULL;
PFNGLBINDRENDERBUFFERPROC glpfBindRenderbuffer = NULL;
PFNGLDELETERENDERBUFFERSPROC glpfDeleteRenderbuffers = NULL;
PFNGLGENRENDERBUFFERSPROC glpfGenRenderbuffers = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glpfRenderbufferStorage = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glpfGetRenderbufferParameteriv = NULL;
PFNGLISFRAMEBUFFERPROC glpfIsFramebuffer = NULL;
PFNGLBINDFRAMEBUFFERPROC glpfBindFramebuffer = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glpfDeleteFramebuffers = NULL;
PFNGLGENFRAMEBUFFERSPROC glpfGenFramebuffers = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glpfCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERTEXTURE1DPROC glpfFramebufferTexture1D = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glpfFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERTEXTURE3DPROC glpfFramebufferTexture3D = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glpfFramebufferRenderbuffer = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glpfGetFramebufferAttachmentParameteriv = NULL;
PFNGLGENERATEMIPMAPPROC glpfGenerateMipmap = NULL;
PFNGLBLITFRAMEBUFFERPROC glpfBlitFramebuffer = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glpfRenderbufferStorageMultisample = NULL;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glpfFramebufferTextureLayer = NULL;
PFNGLMAPBUFFERRANGEPROC glpfMapBufferRange = NULL;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glpfFlushMappedBufferRange = NULL;
PFNGLBINDVERTEXARRAYPROC glpfBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSPROC glpfDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSPROC glpfGenVertexArrays = NULL;
PFNGLISVERTEXARRAYPROC glpfIsVertexArray = NULL;
PFNGLDRAWARRAYSINSTANCEDPROC glpfDrawArraysInstanced = NULL;
PFNGLDRAWELEMENTSINSTANCEDPROC glpfDrawElementsInstanced = NULL;
PFNGLTEXBUFFERPROC glpfTexBuffer = NULL;
PFNGLPRIMITIVERESTARTINDEXPROC glpfPrimitiveRestartIndex = NULL;
PFNGLCOPYBUFFERSUBDATAPROC glpfCopyBufferSubData = NULL;
PFNGLGETUNIFORMINDICESPROC glpfGetUniformIndices = NULL;
PFNGLGETACTIVEUNIFORMSIVPROC glpfGetActiveUniformsiv = NULL;
PFNGLGETACTIVEUNIFORMNAMEPROC glpfGetActiveUniformName = NULL;
PFNGLGETUNIFORMBLOCKINDEXPROC glpfGetUniformBlockIndex = NULL;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glpfGetActiveUniformBlockiv = NULL;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glpfGetActiveUniformBlockName = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC glpfUniformBlockBinding = NULL;
PFNGLGETINTEGER64I_VPROC glpfGetInteger64i_v = NULL;
PFNGLGETBUFFERPARAMETERI64VPROC glpfGetBufferParameteri64v = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC glpfFramebufferTexture = NULL;
PFNGLDRAWELEMENTSBASEVERTEXPROC glpfDrawElementsBaseVertex = NULL;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glpfDrawRangeElementsBaseVertex = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glpfDrawElementsInstancedBaseVertex = NULL;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glpfMultiDrawElementsBaseVertex = NULL;
PFNGLPROVOKINGVERTEXPROC glpfProvokingVertex = NULL;
PFNGLFENCESYNCPROC glpfFenceSync = NULL;
PFNGLISSYNCPROC glpfIsSync = NULL;
PFNGLDELETESYNCPROC glpfDeleteSync = NULL;
PFNGLCLIENTWAITSYNCPROC glpfClientWaitSync = NULL;
PFNGLWAITSYNCPROC glpfWaitSync = NULL;
PFNGLGETINTEGER64VPROC glpfGetInteger64v = NULL;
PFNGLGETSYNCIVPROC glpfGetSynciv = NULL;
PFNGLTEXIMAGE2DMULTISAMPLEPROC glpfTexImage2DMultisample = NULL;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glpfTexImage3DMultisample = NULL;
PFNGLGETMULTISAMPLEFVPROC glpfGetMultisamplefv = NULL;
PFNGLSAMPLEMASKIPROC glpfSampleMaski = NULL;
PFNGLVERTEXATTRIBDIVISORPROC glpfVertexAttribDivisor = NULL;
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glpfBindFragDataLocationIndexed = NULL;
PFNGLGETFRAGDATAINDEXPROC glpfGetFragDataIndex = NULL;
PFNGLGENSAMPLERSPROC glpfGenSamplers = NULL;
PFNGLDELETESAMPLERSPROC glpfDeleteSamplers = NULL;
PFNGLISSAMPLERPROC glpfIsSampler = NULL;
PFNGLBINDSAMPLERPROC glpfBindSampler = NULL;
PFNGLSAMPLERPARAMETERIPROC glpfSamplerParameteri = NULL;
PFNGLSAMPLERPARAMETERIVPROC glpfSamplerParameteriv = NULL;
PFNGLSAMPLERPARAMETERFPROC glpfSamplerParameterf = NULL;
PFNGLSAMPLERPARAMETERFVPROC glpfSamplerParameterfv = NULL;
PFNGLSAMPLERPARAMETERIIVPROC glpfSamplerParameterIiv = NULL;
PFNGLSAMPLERPARAMETERIUIVPROC glpfSamplerParameterIuiv = NULL;
PFNGLGETSAMPLERPARAMETERIVPROC glpfGetSamplerParameteriv = NULL;
PFNGLGETSAMPLERPARAMETERIIVPROC glpfGetSamplerParameterIiv = NULL;
PFNGLGETSAMPLERPARAMETERFVPROC glpfGetSamplerParameterfv = NULL;
PFNGLGETSAMPLERPARAMETERIUIVPROC glpfGetSamplerParameterIuiv = NULL;
PFNGLQUERYCOUNTERPROC glpfQueryCounter = NULL;
PFNGLGETQUERYOBJECTI64VPROC glpfGetQueryObjecti64v = NULL;
PFNGLGETQUERYOBJECTUI64VPROC glpfGetQueryObjectui64v = NULL;
PFNGLVERTEXP2UIPROC glpfVertexP2ui = NULL;
PFNGLVERTEXP2UIVPROC glpfVertexP2uiv = NULL;
PFNGLVERTEXP3UIPROC glpfVertexP3ui = NULL;
PFNGLVERTEXP3UIVPROC glpfVertexP3uiv = NULL;
PFNGLVERTEXP4UIPROC glpfVertexP4ui = NULL;
PFNGLVERTEXP4UIVPROC glpfVertexP4uiv = NULL;
PFNGLTEXCOORDP1UIPROC glpfTexCoordP1ui = NULL;
PFNGLTEXCOORDP1UIVPROC glpfTexCoordP1uiv = NULL;
PFNGLTEXCOORDP2UIPROC glpfTexCoordP2ui = NULL;
PFNGLTEXCOORDP2UIVPROC glpfTexCoordP2uiv = NULL;
PFNGLTEXCOORDP3UIPROC glpfTexCoordP3ui = NULL;
PFNGLTEXCOORDP3UIVPROC glpfTexCoordP3uiv = NULL;
PFNGLTEXCOORDP4UIPROC glpfTexCoordP4ui = NULL;
PFNGLTEXCOORDP4UIVPROC glpfTexCoordP4uiv = NULL;
PFNGLMULTITEXCOORDP1UIPROC glpfMultiTexCoordP1ui = NULL;
PFNGLMULTITEXCOORDP1UIVPROC glpfMultiTexCoordP1uiv = NULL;
PFNGLMULTITEXCOORDP2UIPROC glpfMultiTexCoordP2ui = NULL;
PFNGLMULTITEXCOORDP2UIVPROC glpfMultiTexCoordP2uiv = NULL;
PFNGLMULTITEXCOORDP3UIPROC glpfMultiTexCoordP3ui = NULL;
PFNGLMULTITEXCOORDP3UIVPROC glpfMultiTexCoordP3uiv = NULL;
PFNGLMULTITEXCOORDP4UIPROC glpfMultiTexCoordP4ui = NULL;
PFNGLMULTITEXCOORDP4UIVPROC glpfMultiTexCoordP4uiv = NULL;
PFNGLNORMALP3UIPROC glpfNormalP3ui = NULL;
PFNGLNORMALP3UIVPROC glpfNormalP3uiv = NULL;
PFNGLCOLORP3UIPROC glpfColorP3ui = NULL;
PFNGLCOLORP3UIVPROC glpfColorP3uiv = NULL;
PFNGLCOLORP4UIPROC glpfColorP4ui = NULL;
PFNGLCOLORP4UIVPROC glpfColorP4uiv = NULL;
PFNGLSECONDARYCOLORP3UIPROC glpfSecondaryColorP3ui = NULL;
PFNGLSECONDARYCOLORP3UIVPROC glpfSecondaryColorP3uiv = NULL;
PFNGLVERTEXATTRIBP1UIPROC glpfVertexAttribP1ui = NULL;
PFNGLVERTEXATTRIBP1UIVPROC glpfVertexAttribP1uiv = NULL;
PFNGLVERTEXATTRIBP2UIPROC glpfVertexAttribP2ui = NULL;
PFNGLVERTEXATTRIBP2UIVPROC glpfVertexAttribP2uiv = NULL;
PFNGLVERTEXATTRIBP3UIPROC glpfVertexAttribP3ui = NULL;
PFNGLVERTEXATTRIBP3UIVPROC glpfVertexAttribP3uiv = NULL;
PFNGLVERTEXATTRIBP4UIPROC glpfVertexAttribP4ui = NULL;
PFNGLVERTEXATTRIBP4UIVPROC glpfVertexAttribP4uiv = NULL;

static void add_extension(const char* extension)
{
}


// get_proc from Slavomir Kaslev's gl3w.

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

static HMODULE libgl;

static void open_libgl(void)
{
    libgl = LoadLibraryA("opengl32.dll");
}

static void close_libgl(void)
{
    FreeLibrary(libgl);
}

static void *get_proc(const char *proc)
{
    void *res;

    res = wglGetProcAddress(proc);
    if (!res)
        res = GetProcAddress(libgl, proc);
    return res;
}
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#include <Carbon/Carbon.h>

CFBundleRef bundle;
CFURLRef bundleURL;

static void open_libgl(void)
{
    bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                CFSTR("/System/Library/Frameworks/OpenGL.framework"),
                kCFURLPOSIXPathStyle, true);

    bundle = CFBundleCreate(kCFAllocatorDefault, bundleURL);
    assert(bundle != NULL);
}

static void close_libgl(void)
{
    CFRelease(bundle);
    CFRelease(bundleURL);
}

static void *get_proc(const char *proc)
{
    void *res;

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

static void open_libgl(void)
{
    libgl = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
}

static void close_libgl(void)
{
    dlclose(libgl);
}

static void *get_proc(const char *proc)
{
    void *res;

    res = glXGetProcAddress((const GLubyte *) proc);
    if (!res)
        res = dlsym(libgl, proc);
    return res;
}
#endif

