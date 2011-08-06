// WARNING: This file was automatically generated
// Do not edit.

#ifndef __gl_h_
#define __gl_h_

#ifdef __cplusplus
extern "C" {
#endif

/* Function declaration macros - to move into glplatform.h */

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif


//-------------------------------- DATA TYPES -------------------------------//

typedef unsigned int	GLenum;
typedef unsigned char	GLboolean;
typedef unsigned int	GLbitfield;
typedef void		GLvoid;
typedef signed char	GLbyte;		/* 1-byte signed */
typedef short		GLshort;	/* 2-byte signed */
typedef int		GLint;		/* 4-byte signed */
typedef unsigned char	GLubyte;	/* 1-byte unsigned */
typedef unsigned short	GLushort;	/* 2-byte unsigned */
typedef unsigned int	GLuint;		/* 4-byte unsigned */
typedef int		GLsizei;	/* 4-byte signed */
typedef float		GLfloat;	/* single precision float */
typedef float		GLclampf;	/* single precision float in [0,1] */
typedef double		GLdouble;	/* double precision float */
typedef double		GLclampd;	/* double precision float in [0,1] */

//--------------------------------- PASSTHRU --------------------------------//

#include <stddef.h>
#ifndef GL_VERSION_2_0
/* GL type for program/shader text */
typedef char GLchar;
#endif
#ifndef GL_VERSION_1_5
/* GL types for handling large vertex buffer objects */
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
#endif
#ifndef GL_ARB_vertex_buffer_object
/* GL types for handling large vertex buffer objects */
typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;
#endif
#ifndef GL_ARB_shader_objects
/* GL types for program/shader text and shader object handles */
typedef char GLcharARB;
typedef unsigned int GLhandleARB;
#endif
/* GL type for "half" precision (s10e5) float data in host memory */
#ifndef GL_ARB_half_float_pixel
typedef unsigned short GLhalfARB;
#endif
#ifndef GL_NV_half_float
typedef unsigned short GLhalfNV;
#endif
#ifndef GLEXT_64_TYPES_DEFINED
/* This code block is duplicated in glxext.h, so must be protected */
#define GLEXT_64_TYPES_DEFINED
/* Define int32_t, int64_t, and uint64_t types for UST/MSC */
/* (as used in the GL_EXT_timer_query extension). */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <inttypes.h>
#elif defined(__sun__) || defined(__digital__)
#include <inttypes.h>
#if defined(__STDC__)
#if defined(__arch64__) || defined(_LP64)
typedef long int int64_t;
typedef unsigned long int uint64_t;
#else
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#endif /* __arch64__ */
#endif /* __STDC__ */
#elif defined( __VMS ) || defined(__sgi)
#include <inttypes.h>
#elif defined(__SCO__) || defined(__USLC__)
#include <stdint.h>
#elif defined(__UNIXOS2__) || defined(__SOL64__)
typedef long int int32_t;
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#elif defined(_WIN32) && defined(__GNUC__)
#include <stdint.h>
#elif defined(_WIN32)
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
/* Fallback if nothing above works */
#include <inttypes.h>
#endif
#endif
#ifndef GL_EXT_timer_query
typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
#endif
#ifndef GL_ARB_sync
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef struct __GLsync *GLsync;
#endif
#ifndef GL_ARB_cl_event
/* These incomplete types let us declare types compatible with OpenCL's cl_context and cl_event */
struct _cl_context;
struct _cl_event;
#endif
#ifndef GL_ARB_debug_output
typedef void (APIENTRY *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,GLvoid *userParam);
#endif
#ifndef GL_AMD_debug_output
typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,GLvoid *userParam);
#endif
#ifndef GL_NV_vdpau_interop
typedef GLintptr GLvdpauSurfaceNV;
#endif
/* OpenGL 3.0 also reuses entry points from these extensions: */
/* ARB_framebuffer_object */
/* ARB_map_buffer_range */
/* ARB_vertex_array_object */
/* OpenGL 3.1 also reuses entry points from these extensions: */
/* ARB_copy_buffer */
/* ARB_uniform_buffer_object */
/* OpenGL 3.2 also reuses entry points from these extensions: */
/* ARB_draw_elements_base_vertex */
/* ARB_provoking_vertex */
/* ARB_sync */
/* ARB_texture_multisample */
/* OpenGL 3.3 also reuses entry points from these extensions: */
/* ARB_blend_func_extended */
/* ARB_sampler_objects */
/* ARB_explicit_attrib_location, but it has none */
/* ARB_occlusion_query2 (no entry points) */
/* ARB_shader_bit_encoding (no entry points) */
/* ARB_texture_rgb10_a2ui (no entry points) */
/* ARB_texture_swizzle (no entry points) */
/* ARB_timer_query */
/* ARB_vertex_type_2_10_10_10_rev */
/* OpenGL 4.0 also reuses entry points from these extensions: */
/* ARB_texture_query_lod (no entry points) */
/* ARB_draw_indirect */
/* ARB_gpu_shader5 (no entry points) */
/* ARB_gpu_shader_fp64 */
/* ARB_shader_subroutine */
/* ARB_tessellation_shader */
/* ARB_texture_buffer_object_rgb32 (no entry points) */
/* ARB_texture_cube_map_array (no entry points) */
/* ARB_texture_gather (no entry points) */
/* ARB_transform_feedback2 */
/* ARB_transform_feedback3 */
/* OpenGL 4.1 also reuses entry points from these extensions: */
/* ARB_ES2_compatibility */
/* ARB_get_program_binary */
/* ARB_separate_shader_objects */
/* ARB_shader_precision (no entry points) */
/* ARB_vertex_attrib_64bit */
/* ARB_viewport_array */
/* All ARB_fragment_program entry points are shared with ARB_vertex_program. */
/* This is really a WGL extension, but defines some associated GL enums.
 * ATI does not export "GL_ATI_pixel_format_float" in the GL_EXTENSIONS string.
 */
/* Some NV_fragment_program entry points are shared with ARB_vertex_program. */


//----------------------------------- ENUMS ---------------------------------//

#define GL_2D                                                  0x600
#define GL_2_BYTES                                             0x1407
#define GL_3D                                                  0x601
#define GL_3D_COLOR                                            0x602
#define GL_3D_COLOR_TEXTURE                                    0x603
#define GL_3_BYTES                                             0x1408
#define GL_4D_COLOR_TEXTURE                                    0x604
#define GL_4_BYTES                                             0x1409
#define GL_ACCUM                                               0x100
#define GL_ACCUM_ALPHA_BITS                                    0xd5b
#define GL_ACCUM_BLUE_BITS                                     0xd5a
#define GL_ACCUM_BUFFER_BIT                                    0x200
#define GL_ACCUM_CLEAR_VALUE                                   0xb80
#define GL_ACCUM_GREEN_BITS                                    0xd59
#define GL_ACCUM_RED_BITS                                      0xd58
#define GL_ACTIVE_ATTRIBUTES                                   0x8b89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH                         0x8b8a
#define GL_ACTIVE_TEXTURE                                      0x84e0
#define GL_ACTIVE_UNIFORMS                                     0x8b86
#define GL_ACTIVE_UNIFORM_BLOCKS                               0x8a36
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH                0x8a35
#define GL_ACTIVE_UNIFORM_MAX_LENGTH                           0x8b87
#define GL_ADD                                                 0x104
#define GL_ADD_SIGNED                                          0x8574
#define GL_ALIASED_LINE_WIDTH_RANGE                            0x846e
#define GL_ALIASED_POINT_SIZE_RANGE                            0x846d
#define GL_ALL_ATTRIB_BITS                                     0xffffffff
#define GL_ALPHA                                               0x1906
#define GL_ALPHA12                                             0x803d
#define GL_ALPHA16                                             0x803e
#define GL_ALPHA4                                              0x803b
#define GL_ALPHA8                                              0x803c
#define GL_ALPHA_BIAS                                          0xd1d
#define GL_ALPHA_BITS                                          0xd55
#define GL_ALPHA_INTEGER                                       0x8d97
#define GL_ALPHA_SCALE                                         0xd1c
#define GL_ALPHA_TEST                                          0xbc0
#define GL_ALPHA_TEST_FUNC                                     0xbc1
#define GL_ALPHA_TEST_REF                                      0xbc2
#define GL_ALREADY_SIGNALED                                    0x911a
#define GL_ALWAYS                                              0x207
#define GL_AMBIENT                                             0x1200
#define GL_AMBIENT_AND_DIFFUSE                                 0x1602
#define GL_AND                                                 0x1501
#define GL_AND_INVERTED                                        0x1504
#define GL_AND_REVERSE                                         0x1502
#define GL_ANY_SAMPLES_PASSED                                  0x8c2f
#define GL_ARRAY_BUFFER                                        0x8892
#define GL_ARRAY_BUFFER_BINDING                                0x8894
#define GL_ATTACHED_SHADERS                                    0x8b85
#define GL_ATTRIB_STACK_DEPTH                                  0xbb0
#define GL_AUTO_NORMAL                                         0xd80
#define GL_AUX0                                                0x409
#define GL_AUX1                                                0x40a
#define GL_AUX2                                                0x40b
#define GL_AUX3                                                0x40c
#define GL_AUX_BUFFERS                                         0xc00
#define GL_BACK                                                0x405
#define GL_BACK_LEFT                                           0x402
#define GL_BACK_RIGHT                                          0x403
#define GL_BGR                                                 0x80e0
#define GL_BGRA                                                0x80e1
#define GL_BGRA_INTEGER                                        0x8d9b
#define GL_BGR_INTEGER                                         0x8d9a
#define GL_BITMAP                                              0x1a00
#define GL_BITMAP_TOKEN                                        0x704
#define GL_BLEND                                               0xbe2
#define GL_BLEND_COLOR                                         0x8005
#define GL_BLEND_DST                                           0xbe0
#define GL_BLEND_DST_ALPHA                                     0x80ca
#define GL_BLEND_DST_RGB                                       0x80c8
#define GL_BLEND_EQUATION                                      0x8009
#define GL_BLEND_EQUATION_ALPHA                                0x883d
#define GL_BLEND_EQUATION_RGB                                  0x8009
#define GL_BLEND_SRC                                           0xbe1
#define GL_BLEND_SRC_ALPHA                                     0x80cb
#define GL_BLEND_SRC_RGB                                       0x80c9
#define GL_BLUE                                                0x1905
#define GL_BLUE_BIAS                                           0xd1b
#define GL_BLUE_BITS                                           0xd54
#define GL_BLUE_INTEGER                                        0x8d96
#define GL_BLUE_SCALE                                          0xd1a
#define GL_BOOL                                                0x8b56
#define GL_BOOL_VEC2                                           0x8b57
#define GL_BOOL_VEC3                                           0x8b58
#define GL_BOOL_VEC4                                           0x8b59
#define GL_BUFFER_ACCESS                                       0x88bb
#define GL_BUFFER_ACCESS_FLAGS                                 0x911f
#define GL_BUFFER_MAPPED                                       0x88bc
#define GL_BUFFER_MAP_LENGTH                                   0x9120
#define GL_BUFFER_MAP_OFFSET                                   0x9121
#define GL_BUFFER_MAP_POINTER                                  0x88bd
#define GL_BUFFER_SIZE                                         0x8764
#define GL_BUFFER_USAGE                                        0x8765
#define GL_BYTE                                                0x1400
#define GL_C3F_V3F                                             0x2a24
#define GL_C4F_N3F_V3F                                         0x2a26
#define GL_C4UB_V2F                                            0x2a22
#define GL_C4UB_V3F                                            0x2a23
#define GL_CCW                                                 0x901
#define GL_CLAMP                                               0x2900
#define GL_CLAMP_FRAGMENT_COLOR                                0x891b
#define GL_CLAMP_READ_COLOR                                    0x891c
#define GL_CLAMP_TO_BORDER                                     0x812d
#define GL_CLAMP_TO_EDGE                                       0x812f
#define GL_CLAMP_VERTEX_COLOR                                  0x891a
#define GL_CLEAR                                               0x1500
#define GL_CLIENT_ACTIVE_TEXTURE                               0x84e1
#define GL_CLIENT_ALL_ATTRIB_BITS                              0xffffffff
#define GL_CLIENT_ATTRIB_STACK_DEPTH                           0xbb1
#define GL_CLIENT_PIXEL_STORE_BIT                              0x1
#define GL_CLIENT_VERTEX_ARRAY_BIT                             0x2
#define GL_CLIP_DISTANCE0                                      0x3000
#define GL_CLIP_DISTANCE1                                      0x3001
#define GL_CLIP_DISTANCE2                                      0x3002
#define GL_CLIP_DISTANCE3                                      0x3003
#define GL_CLIP_DISTANCE4                                      0x3004
#define GL_CLIP_DISTANCE5                                      0x3005
#define GL_CLIP_DISTANCE6                                      0x3006
#define GL_CLIP_DISTANCE7                                      0x3007
#define GL_CLIP_PLANE0                                         0x3000
#define GL_CLIP_PLANE1                                         0x3001
#define GL_CLIP_PLANE2                                         0x3002
#define GL_CLIP_PLANE3                                         0x3003
#define GL_CLIP_PLANE4                                         0x3004
#define GL_CLIP_PLANE5                                         0x3005
#define GL_COEFF                                               0xa00
#define GL_COLOR                                               0x1800
#define GL_COLOR_ARRAY                                         0x8076
#define GL_COLOR_ARRAY_BUFFER_BINDING                          0x8898
#define GL_COLOR_ARRAY_POINTER                                 0x8090
#define GL_COLOR_ARRAY_SIZE                                    0x8081
#define GL_COLOR_ARRAY_STRIDE                                  0x8083
#define GL_COLOR_ARRAY_TYPE                                    0x8082
#define GL_COLOR_ATTACHMENT0                                   0x8ce0
#define GL_COLOR_ATTACHMENT1                                   0x8ce1
#define GL_COLOR_ATTACHMENT10                                  0x8cea
#define GL_COLOR_ATTACHMENT11                                  0x8ceb
#define GL_COLOR_ATTACHMENT12                                  0x8cec
#define GL_COLOR_ATTACHMENT13                                  0x8ced
#define GL_COLOR_ATTACHMENT14                                  0x8cee
#define GL_COLOR_ATTACHMENT15                                  0x8cef
#define GL_COLOR_ATTACHMENT2                                   0x8ce2
#define GL_COLOR_ATTACHMENT3                                   0x8ce3
#define GL_COLOR_ATTACHMENT4                                   0x8ce4
#define GL_COLOR_ATTACHMENT5                                   0x8ce5
#define GL_COLOR_ATTACHMENT6                                   0x8ce6
#define GL_COLOR_ATTACHMENT7                                   0x8ce7
#define GL_COLOR_ATTACHMENT8                                   0x8ce8
#define GL_COLOR_ATTACHMENT9                                   0x8ce9
#define GL_COLOR_BUFFER_BIT                                    0x4000
#define GL_COLOR_CLEAR_VALUE                                   0xc22
#define GL_COLOR_INDEX                                         0x1900
#define GL_COLOR_INDEXES                                       0x1603
#define GL_COLOR_LOGIC_OP                                      0xbf2
#define GL_COLOR_MATERIAL                                      0xb57
#define GL_COLOR_MATERIAL_FACE                                 0xb55
#define GL_COLOR_MATERIAL_PARAMETER                            0xb56
#define GL_COLOR_SUM                                           0x8458
#define GL_COLOR_WRITEMASK                                     0xc23
#define GL_COMBINE                                             0x8570
#define GL_COMBINE_ALPHA                                       0x8572
#define GL_COMBINE_RGB                                         0x8571
#define GL_COMPARE_REF_TO_TEXTURE                              0x884e
#define GL_COMPARE_R_TO_TEXTURE                                0x884e
#define GL_COMPILE                                             0x1300
#define GL_COMPILE_AND_EXECUTE                                 0x1301
#define GL_COMPILE_STATUS                                      0x8b81
#define GL_COMPRESSED_ALPHA                                    0x84e9
#define GL_COMPRESSED_INTENSITY                                0x84ec
#define GL_COMPRESSED_LUMINANCE                                0x84ea
#define GL_COMPRESSED_LUMINANCE_ALPHA                          0x84eb
#define GL_COMPRESSED_RED                                      0x8225
#define GL_COMPRESSED_RED_RGTC1                                0x8dbb
#define GL_COMPRESSED_RG                                       0x8226
#define GL_COMPRESSED_RGB                                      0x84ed
#define GL_COMPRESSED_RGBA                                     0x84ee
#define GL_COMPRESSED_RG_RGTC2                                 0x8dbd
#define GL_COMPRESSED_SIGNED_RED_RGTC1                         0x8dbc
#define GL_COMPRESSED_SIGNED_RG_RGTC2                          0x8dbe
#define GL_COMPRESSED_SLUMINANCE                               0x8c4a
#define GL_COMPRESSED_SLUMINANCE_ALPHA                         0x8c4b
#define GL_COMPRESSED_SRGB                                     0x8c48
#define GL_COMPRESSED_SRGB_ALPHA                               0x8c49
#define GL_COMPRESSED_TEXTURE_FORMATS                          0x86a3
#define GL_CONDITION_SATISFIED                                 0x911c
#define GL_CONSTANT                                            0x8576
#define GL_CONSTANT_ALPHA                                      0x8003
#define GL_CONSTANT_ATTENUATION                                0x1207
#define GL_CONSTANT_COLOR                                      0x8001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT                   0x2
#define GL_CONTEXT_CORE_PROFILE_BIT                            0x1
#define GL_CONTEXT_FLAGS                                       0x821e
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT                 0x1
#define GL_CONTEXT_PROFILE_MASK                                0x9126
#define GL_COORD_REPLACE                                       0x8862
#define GL_COPY                                                0x1503
#define GL_COPY_INVERTED                                       0x150c
#define GL_COPY_PIXEL_TOKEN                                    0x706
#define GL_COPY_READ_BUFFER                                    0x8f36
#define GL_COPY_WRITE_BUFFER                                   0x8f37
#define GL_CULL_FACE                                           0xb44
#define GL_CULL_FACE_MODE                                      0xb45
#define GL_CURRENT_BIT                                         0x1
#define GL_CURRENT_COLOR                                       0xb00
#define GL_CURRENT_FOG_COORD                                   0x8453
#define GL_CURRENT_FOG_COORDINATE                              0x8453
#define GL_CURRENT_INDEX                                       0xb01
#define GL_CURRENT_NORMAL                                      0xb02
#define GL_CURRENT_PROGRAM                                     0x8b8d
#define GL_CURRENT_QUERY                                       0x8865
#define GL_CURRENT_RASTER_COLOR                                0xb04
#define GL_CURRENT_RASTER_DISTANCE                             0xb09
#define GL_CURRENT_RASTER_INDEX                                0xb05
#define GL_CURRENT_RASTER_POSITION                             0xb07
#define GL_CURRENT_RASTER_POSITION_VALID                       0xb08
#define GL_CURRENT_RASTER_SECONDARY_COLOR                      0x845f
#define GL_CURRENT_RASTER_TEXTURE_COORDS                       0xb06
#define GL_CURRENT_SECONDARY_COLOR                             0x8459
#define GL_CURRENT_TEXTURE_COORDS                              0xb03
#define GL_CURRENT_VERTEX_ATTRIB                               0x8626
#define GL_CW                                                  0x900
#define GL_DECAL                                               0x2101
#define GL_DECR                                                0x1e03
#define GL_DECR_WRAP                                           0x8508
#define GL_DELETE_STATUS                                       0x8b80
#define GL_DEPTH                                               0x1801
#define GL_DEPTH24_STENCIL8                                    0x88f0
#define GL_DEPTH32F_STENCIL8                                   0x8cad
#define GL_DEPTH_ATTACHMENT                                    0x8d00
#define GL_DEPTH_BIAS                                          0xd1f
#define GL_DEPTH_BITS                                          0xd56
#define GL_DEPTH_BUFFER                                        0x8223
#define GL_DEPTH_BUFFER_BIT                                    0x100
#define GL_DEPTH_CLAMP                                         0x864f
#define GL_DEPTH_CLEAR_VALUE                                   0xb73
#define GL_DEPTH_COMPONENT                                     0x1902
#define GL_DEPTH_COMPONENT16                                   0x81a5
#define GL_DEPTH_COMPONENT24                                   0x81a6
#define GL_DEPTH_COMPONENT32                                   0x81a7
#define GL_DEPTH_COMPONENT32F                                  0x8cac
#define GL_DEPTH_FUNC                                          0xb74
#define GL_DEPTH_RANGE                                         0xb70
#define GL_DEPTH_SCALE                                         0xd1e
#define GL_DEPTH_STENCIL                                       0x84f9
#define GL_DEPTH_STENCIL_ATTACHMENT                            0x821a
#define GL_DEPTH_TEST                                          0xb71
#define GL_DEPTH_TEXTURE_MODE                                  0x884b
#define GL_DEPTH_WRITEMASK                                     0xb72
#define GL_DIFFUSE                                             0x1201
#define GL_DITHER                                              0xbd0
#define GL_DOMAIN                                              0xa02
#define GL_DONT_CARE                                           0x1100
#define GL_DOT3_RGB                                            0x86ae
#define GL_DOT3_RGBA                                           0x86af
#define GL_DOUBLE                                              0x140a
#define GL_DOUBLEBUFFER                                        0xc32
#define GL_DRAW_BUFFER                                         0xc01
#define GL_DRAW_BUFFER0                                        0x8825
#define GL_DRAW_BUFFER1                                        0x8826
#define GL_DRAW_BUFFER10                                       0x882f
#define GL_DRAW_BUFFER11                                       0x8830
#define GL_DRAW_BUFFER12                                       0x8831
#define GL_DRAW_BUFFER13                                       0x8832
#define GL_DRAW_BUFFER14                                       0x8833
#define GL_DRAW_BUFFER15                                       0x8834
#define GL_DRAW_BUFFER2                                        0x8827
#define GL_DRAW_BUFFER3                                        0x8828
#define GL_DRAW_BUFFER4                                        0x8829
#define GL_DRAW_BUFFER5                                        0x882a
#define GL_DRAW_BUFFER6                                        0x882b
#define GL_DRAW_BUFFER7                                        0x882c
#define GL_DRAW_BUFFER8                                        0x882d
#define GL_DRAW_BUFFER9                                        0x882e
#define GL_DRAW_FRAMEBUFFER                                    0x8ca9
#define GL_DRAW_FRAMEBUFFER_BINDING                            0x8ca6
#define GL_DRAW_PIXEL_TOKEN                                    0x705
#define GL_DST_ALPHA                                           0x304
#define GL_DST_COLOR                                           0x306
#define GL_DYNAMIC_COPY                                        0x88ea
#define GL_DYNAMIC_DRAW                                        0x88e8
#define GL_DYNAMIC_READ                                        0x88e9
#define GL_EDGE_FLAG                                           0xb43
#define GL_EDGE_FLAG_ARRAY                                     0x8079
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING                      0x889b
#define GL_EDGE_FLAG_ARRAY_POINTER                             0x8093
#define GL_EDGE_FLAG_ARRAY_STRIDE                              0x808c
#define GL_ELEMENT_ARRAY_BUFFER                                0x8893
#define GL_ELEMENT_ARRAY_BUFFER_BINDING                        0x8895
#define GL_EMISSION                                            0x1600
#define GL_ENABLE_BIT                                          0x2000
#define GL_EQUAL                                               0x202
#define GL_EQUIV                                               0x1509
#define GL_EVAL_BIT                                            0x10000
#define GL_EXP                                                 0x800
#define GL_EXP2                                                0x801
#define GL_EXTENSIONS                                          0x1f03
#define GL_EYE_LINEAR                                          0x2400
#define GL_EYE_PLANE                                           0x2502
#define GL_FALSE                                               0x0
#define GL_FASTEST                                             0x1101
#define GL_FEEDBACK                                            0x1c01
#define GL_FEEDBACK_BUFFER_POINTER                             0xdf0
#define GL_FEEDBACK_BUFFER_SIZE                                0xdf1
#define GL_FEEDBACK_BUFFER_TYPE                                0xdf2
#define GL_FILL                                                0x1b02
#define GL_FIRST_VERTEX_CONVENTION                             0x8e4d
#define GL_FIXED_ONLY                                          0x891d
#define GL_FLAT                                                0x1d00
#define GL_FLOAT                                               0x1406
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV                      0x8dad
#define GL_FLOAT_MAT2                                          0x8b5a
#define GL_FLOAT_MAT2x3                                        0x8b65
#define GL_FLOAT_MAT2x4                                        0x8b66
#define GL_FLOAT_MAT3                                          0x8b5b
#define GL_FLOAT_MAT3x2                                        0x8b67
#define GL_FLOAT_MAT3x4                                        0x8b68
#define GL_FLOAT_MAT4                                          0x8b5c
#define GL_FLOAT_MAT4x2                                        0x8b69
#define GL_FLOAT_MAT4x3                                        0x8b6a
#define GL_FLOAT_VEC2                                          0x8b50
#define GL_FLOAT_VEC3                                          0x8b51
#define GL_FLOAT_VEC4                                          0x8b52
#define GL_FOG                                                 0xb60
#define GL_FOG_BIT                                             0x80
#define GL_FOG_COLOR                                           0xb66
#define GL_FOG_COORD                                           0x8451
#define GL_FOG_COORDINATE                                      0x8451
#define GL_FOG_COORDINATE_ARRAY                                0x8457
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING                 0x889d
#define GL_FOG_COORDINATE_ARRAY_POINTER                        0x8456
#define GL_FOG_COORDINATE_ARRAY_STRIDE                         0x8455
#define GL_FOG_COORDINATE_ARRAY_TYPE                           0x8454
#define GL_FOG_COORDINATE_SOURCE                               0x8450
#define GL_FOG_COORD_ARRAY                                     0x8457
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING                      0x889d
#define GL_FOG_COORD_ARRAY_POINTER                             0x8456
#define GL_FOG_COORD_ARRAY_STRIDE                              0x8455
#define GL_FOG_COORD_ARRAY_TYPE                                0x8454
#define GL_FOG_COORD_SRC                                       0x8450
#define GL_FOG_DENSITY                                         0xb62
#define GL_FOG_END                                             0xb64
#define GL_FOG_HINT                                            0xc54
#define GL_FOG_INDEX                                           0xb61
#define GL_FOG_MODE                                            0xb65
#define GL_FOG_START                                           0xb63
#define GL_FRAGMENT_DEPTH                                      0x8452
#define GL_FRAGMENT_SHADER                                     0x8b30
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT                     0x8b8b
#define GL_FRAMEBUFFER                                         0x8d40
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE                   0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE                    0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING               0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE               0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE                   0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE                   0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED                      0x8da7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME                  0x8cd1
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE                  0x8cd0
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE                     0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE                 0x8217
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE        0x8cd3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER                0x8cd4
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL                0x8cd2
#define GL_FRAMEBUFFER_BINDING                                 0x8ca6
#define GL_FRAMEBUFFER_COMPLETE                                0x8cd5
#define GL_FRAMEBUFFER_DEFAULT                                 0x8218
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT                   0x8cd6
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER                  0x8cdb
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS                0x8da8
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT           0x8cd7
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE                  0x8d56
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER                  0x8cdc
#define GL_FRAMEBUFFER_SRGB                                    0x8db9
#define GL_FRAMEBUFFER_UNDEFINED                               0x8219
#define GL_FRAMEBUFFER_UNSUPPORTED                             0x8cdd
#define GL_FRONT                                               0x404
#define GL_FRONT_AND_BACK                                      0x408
#define GL_FRONT_FACE                                          0xb46
#define GL_FRONT_LEFT                                          0x400
#define GL_FRONT_RIGHT                                         0x401
#define GL_FUNC_ADD                                            0x8006
#define GL_FUNC_REVERSE_SUBTRACT                               0x800b
#define GL_FUNC_SUBTRACT                                       0x800a
#define GL_GENERATE_MIPMAP                                     0x8191
#define GL_GENERATE_MIPMAP_HINT                                0x8192
#define GL_GEOMETRY_INPUT_TYPE                                 0x8917
#define GL_GEOMETRY_OUTPUT_TYPE                                0x8918
#define GL_GEOMETRY_SHADER                                     0x8dd9
#define GL_GEOMETRY_VERTICES_OUT                               0x8916
#define GL_GEQUAL                                              0x206
#define GL_GREATER                                             0x204
#define GL_GREEN                                               0x1904
#define GL_GREEN_BIAS                                          0xd19
#define GL_GREEN_BITS                                          0xd53
#define GL_GREEN_INTEGER                                       0x8d95
#define GL_GREEN_SCALE                                         0xd18
#define GL_HALF_FLOAT                                          0x140b
#define GL_HINT_BIT                                            0x8000
#define GL_INCR                                                0x1e02
#define GL_INCR_WRAP                                           0x8507
#define GL_INDEX                                               0x8222
#define GL_INDEX_ARRAY                                         0x8077
#define GL_INDEX_ARRAY_BUFFER_BINDING                          0x8899
#define GL_INDEX_ARRAY_POINTER                                 0x8091
#define GL_INDEX_ARRAY_STRIDE                                  0x8086
#define GL_INDEX_ARRAY_TYPE                                    0x8085
#define GL_INDEX_BITS                                          0xd51
#define GL_INDEX_CLEAR_VALUE                                   0xc20
#define GL_INDEX_LOGIC_OP                                      0xbf1
#define GL_INDEX_MODE                                          0xc30
#define GL_INDEX_OFFSET                                        0xd13
#define GL_INDEX_SHIFT                                         0xd12
#define GL_INDEX_WRITEMASK                                     0xc21
#define GL_INFO_LOG_LENGTH                                     0x8b84
#define GL_INT                                                 0x1404
#define GL_INTENSITY                                           0x8049
#define GL_INTENSITY12                                         0x804c
#define GL_INTENSITY16                                         0x804d
#define GL_INTENSITY4                                          0x804a
#define GL_INTENSITY8                                          0x804b
#define GL_INTERLEAVED_ATTRIBS                                 0x8c8c
#define GL_INTERPOLATE                                         0x8575
#define GL_INT_2_10_10_10_REV                                  0x8d9f
#define GL_INT_SAMPLER_1D                                      0x8dc9
#define GL_INT_SAMPLER_1D_ARRAY                                0x8dce
#define GL_INT_SAMPLER_2D                                      0x8dca
#define GL_INT_SAMPLER_2D_ARRAY                                0x8dcf
#define GL_INT_SAMPLER_2D_MULTISAMPLE                          0x9109
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY                    0x910c
#define GL_INT_SAMPLER_2D_RECT                                 0x8dcd
#define GL_INT_SAMPLER_3D                                      0x8dcb
#define GL_INT_SAMPLER_BUFFER                                  0x8dd0
#define GL_INT_SAMPLER_CUBE                                    0x8dcc
#define GL_INT_VEC2                                            0x8b53
#define GL_INT_VEC3                                            0x8b54
#define GL_INT_VEC4                                            0x8b55
#define GL_INVALID_ENUM                                        0x500
#define GL_INVALID_FRAMEBUFFER_OPERATION                       0x506
#define GL_INVALID_INDEX                                       0xffffffff
#define GL_INVALID_OPERATION                                   0x502
#define GL_INVALID_VALUE                                       0x501
#define GL_INVERT                                              0x150a
#define GL_KEEP                                                0x1e00
#define GL_LAST_VERTEX_CONVENTION                              0x8e4e
#define GL_LEFT                                                0x406
#define GL_LEQUAL                                              0x203
#define GL_LESS                                                0x201
#define GL_LIGHT0                                              0x4000
#define GL_LIGHT1                                              0x4001
#define GL_LIGHT2                                              0x4002
#define GL_LIGHT3                                              0x4003
#define GL_LIGHT4                                              0x4004
#define GL_LIGHT5                                              0x4005
#define GL_LIGHT6                                              0x4006
#define GL_LIGHT7                                              0x4007
#define GL_LIGHTING                                            0xb50
#define GL_LIGHTING_BIT                                        0x40
#define GL_LIGHT_MODEL_AMBIENT                                 0xb53
#define GL_LIGHT_MODEL_COLOR_CONTROL                           0x81f8
#define GL_LIGHT_MODEL_LOCAL_VIEWER                            0xb51
#define GL_LIGHT_MODEL_TWO_SIDE                                0xb52
#define GL_LINE                                                0x1b01
#define GL_LINEAR                                              0x2601
#define GL_LINEAR_ATTENUATION                                  0x1208
#define GL_LINEAR_MIPMAP_LINEAR                                0x2703
#define GL_LINEAR_MIPMAP_NEAREST                               0x2701
#define GL_LINES                                               0x1
#define GL_LINES_ADJACENCY                                     0xa
#define GL_LINE_BIT                                            0x4
#define GL_LINE_LOOP                                           0x2
#define GL_LINE_RESET_TOKEN                                    0x707
#define GL_LINE_SMOOTH                                         0xb20
#define GL_LINE_SMOOTH_HINT                                    0xc52
#define GL_LINE_STIPPLE                                        0xb24
#define GL_LINE_STIPPLE_PATTERN                                0xb25
#define GL_LINE_STIPPLE_REPEAT                                 0xb26
#define GL_LINE_STRIP                                          0x3
#define GL_LINE_STRIP_ADJACENCY                                0xb
#define GL_LINE_TOKEN                                          0x702
#define GL_LINE_WIDTH                                          0xb21
#define GL_LINE_WIDTH_GRANULARITY                              0xb23
#define GL_LINE_WIDTH_RANGE                                    0xb22
#define GL_LINK_STATUS                                         0x8b82
#define GL_LIST_BASE                                           0xb32
#define GL_LIST_BIT                                            0x20000
#define GL_LIST_INDEX                                          0xb33
#define GL_LIST_MODE                                           0xb30
#define GL_LOAD                                                0x101
#define GL_LOGIC_OP                                            0xbf1
#define GL_LOGIC_OP_MODE                                       0xbf0
#define GL_LOWER_LEFT                                          0x8ca1
#define GL_LUMINANCE                                           0x1909
#define GL_LUMINANCE12                                         0x8041
#define GL_LUMINANCE12_ALPHA12                                 0x8047
#define GL_LUMINANCE12_ALPHA4                                  0x8046
#define GL_LUMINANCE16                                         0x8042
#define GL_LUMINANCE16_ALPHA16                                 0x8048
#define GL_LUMINANCE4                                          0x803f
#define GL_LUMINANCE4_ALPHA4                                   0x8043
#define GL_LUMINANCE6_ALPHA2                                   0x8044
#define GL_LUMINANCE8                                          0x8040
#define GL_LUMINANCE8_ALPHA8                                   0x8045
#define GL_LUMINANCE_ALPHA                                     0x190a
#define GL_MAJOR_VERSION                                       0x821b
#define GL_MAP1_COLOR_4                                        0xd90
#define GL_MAP1_GRID_DOMAIN                                    0xdd0
#define GL_MAP1_GRID_SEGMENTS                                  0xdd1
#define GL_MAP1_INDEX                                          0xd91
#define GL_MAP1_NORMAL                                         0xd92
#define GL_MAP1_TEXTURE_COORD_1                                0xd93
#define GL_MAP1_TEXTURE_COORD_2                                0xd94
#define GL_MAP1_TEXTURE_COORD_3                                0xd95
#define GL_MAP1_TEXTURE_COORD_4                                0xd96
#define GL_MAP1_VERTEX_3                                       0xd97
#define GL_MAP1_VERTEX_4                                       0xd98
#define GL_MAP2_COLOR_4                                        0xdb0
#define GL_MAP2_GRID_DOMAIN                                    0xdd2
#define GL_MAP2_GRID_SEGMENTS                                  0xdd3
#define GL_MAP2_INDEX                                          0xdb1
#define GL_MAP2_NORMAL                                         0xdb2
#define GL_MAP2_TEXTURE_COORD_1                                0xdb3
#define GL_MAP2_TEXTURE_COORD_2                                0xdb4
#define GL_MAP2_TEXTURE_COORD_3                                0xdb5
#define GL_MAP2_TEXTURE_COORD_4                                0xdb6
#define GL_MAP2_VERTEX_3                                       0xdb7
#define GL_MAP2_VERTEX_4                                       0xdb8
#define GL_MAP_COLOR                                           0xd10
#define GL_MAP_FLUSH_EXPLICIT_BIT                              0x10
#define GL_MAP_INVALIDATE_BUFFER_BIT                           0x8
#define GL_MAP_INVALIDATE_RANGE_BIT                            0x4
#define GL_MAP_READ_BIT                                        0x1
#define GL_MAP_STENCIL                                         0xd11
#define GL_MAP_UNSYNCHRONIZED_BIT                              0x20
#define GL_MAP_WRITE_BIT                                       0x2
#define GL_MATRIX_MODE                                         0xba0
#define GL_MAX                                                 0x8008
#define GL_MAX_3D_TEXTURE_SIZE                                 0x8073
#define GL_MAX_ARRAY_TEXTURE_LAYERS                            0x88ff
#define GL_MAX_ATTRIB_STACK_DEPTH                              0xd35
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH                       0xd3b
#define GL_MAX_CLIP_DISTANCES                                  0xd32
#define GL_MAX_CLIP_PLANES                                     0xd32
#define GL_MAX_COLOR_ATTACHMENTS                               0x8cdf
#define GL_MAX_COLOR_TEXTURE_SAMPLES                           0x910e
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS            0x8a33
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS            0x8a32
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS                    0x8b4d
#define GL_MAX_COMBINED_UNIFORM_BLOCKS                         0x8a2e
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS              0x8a31
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE                           0x851c
#define GL_MAX_DEPTH_TEXTURE_SAMPLES                           0x910f
#define GL_MAX_DRAW_BUFFERS                                    0x8824
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS                        0x88fc
#define GL_MAX_ELEMENTS_INDICES                                0x80e9
#define GL_MAX_ELEMENTS_VERTICES                               0x80e8
#define GL_MAX_EVAL_ORDER                                      0xd30
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS                       0x9125
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS                         0x8a2d
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS                     0x8b49
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS                       0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS                      0x9124
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES                        0x8de0
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS                    0x8c29
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS                0x8de1
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS                         0x8a2c
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS                     0x8ddf
#define GL_MAX_INTEGER_SAMPLES                                 0x9110
#define GL_MAX_LIGHTS                                          0xd31
#define GL_MAX_LIST_NESTING                                    0xb31
#define GL_MAX_MODELVIEW_STACK_DEPTH                           0xd36
#define GL_MAX_NAME_STACK_DEPTH                                0xd37
#define GL_MAX_PIXEL_MAP_TABLE                                 0xd34
#define GL_MAX_PROGRAM_TEXEL_OFFSET                            0x8905
#define GL_MAX_PROJECTION_STACK_DEPTH                          0xd38
#define GL_MAX_RECTANGLE_TEXTURE_SIZE                          0x84f8
#define GL_MAX_RENDERBUFFER_SIZE                               0x84e8
#define GL_MAX_SAMPLES                                         0x8d57
#define GL_MAX_SAMPLE_MASK_WORDS                               0x8e59
#define GL_MAX_SERVER_WAIT_TIMEOUT                             0x9111
#define GL_MAX_TEXTURE_BUFFER_SIZE                             0x8c2b
#define GL_MAX_TEXTURE_COORDS                                  0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS                             0x8872
#define GL_MAX_TEXTURE_LOD_BIAS                                0x84fd
#define GL_MAX_TEXTURE_SIZE                                    0xd33
#define GL_MAX_TEXTURE_STACK_DEPTH                             0xd39
#define GL_MAX_TEXTURE_UNITS                                   0x84e2
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS       0x8c8a
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS             0x8c8b
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS          0x8c80
#define GL_MAX_UNIFORM_BLOCK_SIZE                              0x8a30
#define GL_MAX_UNIFORM_BUFFER_BINDINGS                         0x8a2f
#define GL_MAX_VARYING_COMPONENTS                              0x8b4b
#define GL_MAX_VARYING_FLOATS                                  0x8b4b
#define GL_MAX_VERTEX_ATTRIBS                                  0x8869
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS                        0x9122
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS                      0x8b4c
#define GL_MAX_VERTEX_UNIFORM_BLOCKS                           0x8a2b
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS                       0x8b4a
#define GL_MAX_VIEWPORT_DIMS                                   0xd3a
#define GL_MIN                                                 0x8007
#define GL_MINOR_VERSION                                       0x821c
#define GL_MIN_PROGRAM_TEXEL_OFFSET                            0x8904
#define GL_MIRRORED_REPEAT                                     0x8370
#define GL_MODELVIEW                                           0x1700
#define GL_MODELVIEW_MATRIX                                    0xba6
#define GL_MODELVIEW_STACK_DEPTH                               0xba3
#define GL_MODULATE                                            0x2100
#define GL_MULT                                                0x103
#define GL_MULTISAMPLE                                         0x809d
#define GL_MULTISAMPLE_BIT                                     0x20000000
#define GL_N3F_V3F                                             0x2a25
#define GL_NAME_STACK_DEPTH                                    0xd70
#define GL_NAND                                                0x150e
#define GL_NEAREST                                             0x2600
#define GL_NEAREST_MIPMAP_LINEAR                               0x2702
#define GL_NEAREST_MIPMAP_NEAREST                              0x2700
#define GL_NEVER                                               0x200
#define GL_NICEST                                              0x1102
#define GL_NONE                                                0x0
#define GL_NOOP                                                0x1505
#define GL_NOR                                                 0x1508
#define GL_NORMALIZE                                           0xba1
#define GL_NORMAL_ARRAY                                        0x8075
#define GL_NORMAL_ARRAY_BUFFER_BINDING                         0x8897
#define GL_NORMAL_ARRAY_POINTER                                0x808f
#define GL_NORMAL_ARRAY_STRIDE                                 0x807f
#define GL_NORMAL_ARRAY_TYPE                                   0x807e
#define GL_NORMAL_MAP                                          0x8511
#define GL_NOTEQUAL                                            0x205
#define GL_NO_ERROR                                            0x0
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS                      0x86a2
#define GL_NUM_EXTENSIONS                                      0x821d
#define GL_OBJECT_LINEAR                                       0x2401
#define GL_OBJECT_PLANE                                        0x2501
#define GL_OBJECT_TYPE                                         0x9112
#define GL_ONE                                                 0x1
#define GL_ONE_MINUS_CONSTANT_ALPHA                            0x8004
#define GL_ONE_MINUS_CONSTANT_COLOR                            0x8002
#define GL_ONE_MINUS_DST_ALPHA                                 0x305
#define GL_ONE_MINUS_DST_COLOR                                 0x307
#define GL_ONE_MINUS_SRC1_ALPHA                                0x88fb
#define GL_ONE_MINUS_SRC1_COLOR                                0x88fa
#define GL_ONE_MINUS_SRC_ALPHA                                 0x303
#define GL_ONE_MINUS_SRC_COLOR                                 0x301
#define GL_OPERAND0_ALPHA                                      0x8598
#define GL_OPERAND0_RGB                                        0x8590
#define GL_OPERAND1_ALPHA                                      0x8599
#define GL_OPERAND1_RGB                                        0x8591
#define GL_OPERAND2_ALPHA                                      0x859a
#define GL_OPERAND2_RGB                                        0x8592
#define GL_OR                                                  0x1507
#define GL_ORDER                                               0xa01
#define GL_OR_INVERTED                                         0x150d
#define GL_OR_REVERSE                                          0x150b
#define GL_OUT_OF_MEMORY                                       0x505
#define GL_PACK_ALIGNMENT                                      0xd05
#define GL_PACK_IMAGE_HEIGHT                                   0x806c
#define GL_PACK_LSB_FIRST                                      0xd01
#define GL_PACK_ROW_LENGTH                                     0xd02
#define GL_PACK_SKIP_IMAGES                                    0x806b
#define GL_PACK_SKIP_PIXELS                                    0xd04
#define GL_PACK_SKIP_ROWS                                      0xd03
#define GL_PACK_SWAP_BYTES                                     0xd00
#define GL_PASS_THROUGH_TOKEN                                  0x700
#define GL_PERSPECTIVE_CORRECTION_HINT                         0xc50
#define GL_PIXEL_MAP_A_TO_A                                    0xc79
#define GL_PIXEL_MAP_A_TO_A_SIZE                               0xcb9
#define GL_PIXEL_MAP_B_TO_B                                    0xc78
#define GL_PIXEL_MAP_B_TO_B_SIZE                               0xcb8
#define GL_PIXEL_MAP_G_TO_G                                    0xc77
#define GL_PIXEL_MAP_G_TO_G_SIZE                               0xcb7
#define GL_PIXEL_MAP_I_TO_A                                    0xc75
#define GL_PIXEL_MAP_I_TO_A_SIZE                               0xcb5
#define GL_PIXEL_MAP_I_TO_B                                    0xc74
#define GL_PIXEL_MAP_I_TO_B_SIZE                               0xcb4
#define GL_PIXEL_MAP_I_TO_G                                    0xc73
#define GL_PIXEL_MAP_I_TO_G_SIZE                               0xcb3
#define GL_PIXEL_MAP_I_TO_I                                    0xc70
#define GL_PIXEL_MAP_I_TO_I_SIZE                               0xcb0
#define GL_PIXEL_MAP_I_TO_R                                    0xc72
#define GL_PIXEL_MAP_I_TO_R_SIZE                               0xcb2
#define GL_PIXEL_MAP_R_TO_R                                    0xc76
#define GL_PIXEL_MAP_R_TO_R_SIZE                               0xcb6
#define GL_PIXEL_MAP_S_TO_S                                    0xc71
#define GL_PIXEL_MAP_S_TO_S_SIZE                               0xcb1
#define GL_PIXEL_MODE_BIT                                      0x20
#define GL_PIXEL_PACK_BUFFER                                   0x88eb
#define GL_PIXEL_PACK_BUFFER_BINDING                           0x88ed
#define GL_PIXEL_UNPACK_BUFFER                                 0x88ec
#define GL_PIXEL_UNPACK_BUFFER_BINDING                         0x88ef
#define GL_POINT                                               0x1b00
#define GL_POINTS                                              0x0
#define GL_POINT_BIT                                           0x2
#define GL_POINT_DISTANCE_ATTENUATION                          0x8129
#define GL_POINT_FADE_THRESHOLD_SIZE                           0x8128
#define GL_POINT_SIZE                                          0xb11
#define GL_POINT_SIZE_GRANULARITY                              0xb13
#define GL_POINT_SIZE_MAX                                      0x8127
#define GL_POINT_SIZE_MIN                                      0x8126
#define GL_POINT_SIZE_RANGE                                    0xb12
#define GL_POINT_SMOOTH                                        0xb10
#define GL_POINT_SMOOTH_HINT                                   0xc51
#define GL_POINT_SPRITE                                        0x8861
#define GL_POINT_SPRITE_COORD_ORIGIN                           0x8ca0
#define GL_POINT_TOKEN                                         0x701
#define GL_POLYGON                                             0x9
#define GL_POLYGON_BIT                                         0x8
#define GL_POLYGON_MODE                                        0xb40
#define GL_POLYGON_OFFSET_FACTOR                               0x8038
#define GL_POLYGON_OFFSET_FILL                                 0x8037
#define GL_POLYGON_OFFSET_LINE                                 0x2a02
#define GL_POLYGON_OFFSET_POINT                                0x2a01
#define GL_POLYGON_OFFSET_UNITS                                0x2a00
#define GL_POLYGON_SMOOTH                                      0xb41
#define GL_POLYGON_SMOOTH_HINT                                 0xc53
#define GL_POLYGON_STIPPLE                                     0xb42
#define GL_POLYGON_STIPPLE_BIT                                 0x10
#define GL_POLYGON_TOKEN                                       0x703
#define GL_POSITION                                            0x1203
#define GL_PREVIOUS                                            0x8578
#define GL_PRIMARY_COLOR                                       0x8577
#define GL_PRIMITIVES_GENERATED                                0x8c87
#define GL_PRIMITIVE_RESTART                                   0x8f9d
#define GL_PRIMITIVE_RESTART_INDEX                             0x8f9e
#define GL_PROGRAM_POINT_SIZE                                  0x8642
#define GL_PROJECTION                                          0x1701
#define GL_PROJECTION_MATRIX                                   0xba7
#define GL_PROJECTION_STACK_DEPTH                              0xba4
#define GL_PROVOKING_VERTEX                                    0x8e4f
#define GL_PROXY_TEXTURE_1D                                    0x8063
#define GL_PROXY_TEXTURE_1D_ARRAY                              0x8c19
#define GL_PROXY_TEXTURE_2D                                    0x8064
#define GL_PROXY_TEXTURE_2D_ARRAY                              0x8c1b
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE                        0x9101
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY                  0x9103
#define GL_PROXY_TEXTURE_3D                                    0x8070
#define GL_PROXY_TEXTURE_CUBE_MAP                              0x851b
#define GL_PROXY_TEXTURE_RECTANGLE                             0x84f7
#define GL_Q                                                   0x2003
#define GL_QUADRATIC_ATTENUATION                               0x1209
#define GL_QUADS                                               0x7
#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION            0x8e4c
#define GL_QUAD_STRIP                                          0x8
#define GL_QUERY_BY_REGION_NO_WAIT                             0x8e16
#define GL_QUERY_BY_REGION_WAIT                                0x8e15
#define GL_QUERY_COUNTER_BITS                                  0x8864
#define GL_QUERY_NO_WAIT                                       0x8e14
#define GL_QUERY_RESULT                                        0x8866
#define GL_QUERY_RESULT_AVAILABLE                              0x8867
#define GL_QUERY_WAIT                                          0x8e13
#define GL_R                                                   0x2002
#define GL_R11F_G11F_B10F                                      0x8c3a
#define GL_R16                                                 0x822a
#define GL_R16F                                                0x822d
#define GL_R16I                                                0x8233
#define GL_R16UI                                               0x8234
#define GL_R16_SNORM                                           0x8f98
#define GL_R32F                                                0x822e
#define GL_R32I                                                0x8235
#define GL_R32UI                                               0x8236
#define GL_R3_G3_B2                                            0x2a10
#define GL_R8                                                  0x8229
#define GL_R8I                                                 0x8231
#define GL_R8UI                                                0x8232
#define GL_R8_SNORM                                            0x8f94
#define GL_RASTERIZER_DISCARD                                  0x8c89
#define GL_READ_BUFFER                                         0xc02
#define GL_READ_FRAMEBUFFER                                    0x8ca8
#define GL_READ_FRAMEBUFFER_BINDING                            0x8caa
#define GL_READ_ONLY                                           0x88b8
#define GL_READ_WRITE                                          0x88ba
#define GL_RED                                                 0x1903
#define GL_RED_BIAS                                            0xd15
#define GL_RED_BITS                                            0xd52
#define GL_RED_INTEGER                                         0x8d94
#define GL_RED_SCALE                                           0xd14
#define GL_RED_SNORM                                           0x8f90
#define GL_REFLECTION_MAP                                      0x8512
#define GL_RENDER                                              0x1c00
#define GL_RENDERBUFFER                                        0x8d41
#define GL_RENDERBUFFER_ALPHA_SIZE                             0x8d53
#define GL_RENDERBUFFER_BINDING                                0x8ca7
#define GL_RENDERBUFFER_BLUE_SIZE                              0x8d52
#define GL_RENDERBUFFER_DEPTH_SIZE                             0x8d54
#define GL_RENDERBUFFER_GREEN_SIZE                             0x8d51
#define GL_RENDERBUFFER_HEIGHT                                 0x8d43
#define GL_RENDERBUFFER_INTERNAL_FORMAT                        0x8d44
#define GL_RENDERBUFFER_RED_SIZE                               0x8d50
#define GL_RENDERBUFFER_SAMPLES                                0x8cab
#define GL_RENDERBUFFER_STENCIL_SIZE                           0x8d55
#define GL_RENDERBUFFER_WIDTH                                  0x8d42
#define GL_RENDERER                                            0x1f01
#define GL_RENDER_MODE                                         0xc40
#define GL_REPEAT                                              0x2901
#define GL_REPLACE                                             0x1e01
#define GL_RESCALE_NORMAL                                      0x803a
#define GL_RETURN                                              0x102
#define GL_RG                                                  0x8227
#define GL_RG16                                                0x822c
#define GL_RG16F                                               0x822f
#define GL_RG16I                                               0x8239
#define GL_RG16UI                                              0x823a
#define GL_RG16_SNORM                                          0x8f99
#define GL_RG32F                                               0x8230
#define GL_RG32I                                               0x823b
#define GL_RG32UI                                              0x823c
#define GL_RG8                                                 0x822b
#define GL_RG8I                                                0x8237
#define GL_RG8UI                                               0x8238
#define GL_RG8_SNORM                                           0x8f95
#define GL_RGB                                                 0x1907
#define GL_RGB10                                               0x8052
#define GL_RGB10_A2                                            0x8059
#define GL_RGB10_A2UI                                          0x906f
#define GL_RGB12                                               0x8053
#define GL_RGB16                                               0x8054
#define GL_RGB16F                                              0x881b
#define GL_RGB16I                                              0x8d89
#define GL_RGB16UI                                             0x8d77
#define GL_RGB16_SNORM                                         0x8f9a
#define GL_RGB32F                                              0x8815
#define GL_RGB32I                                              0x8d83
#define GL_RGB32UI                                             0x8d71
#define GL_RGB4                                                0x804f
#define GL_RGB5                                                0x8050
#define GL_RGB5_A1                                             0x8057
#define GL_RGB8                                                0x8051
#define GL_RGB8I                                               0x8d8f
#define GL_RGB8UI                                              0x8d7d
#define GL_RGB8_SNORM                                          0x8f96
#define GL_RGB9_E5                                             0x8c3d
#define GL_RGBA                                                0x1908
#define GL_RGBA12                                              0x805a
#define GL_RGBA16                                              0x805b
#define GL_RGBA16F                                             0x881a
#define GL_RGBA16I                                             0x8d88
#define GL_RGBA16UI                                            0x8d76
#define GL_RGBA16_SNORM                                        0x8f9b
#define GL_RGBA2                                               0x8055
#define GL_RGBA32F                                             0x8814
#define GL_RGBA32I                                             0x8d82
#define GL_RGBA32UI                                            0x8d70
#define GL_RGBA4                                               0x8056
#define GL_RGBA8                                               0x8058
#define GL_RGBA8I                                              0x8d8e
#define GL_RGBA8UI                                             0x8d7c
#define GL_RGBA8_SNORM                                         0x8f97
#define GL_RGBA_INTEGER                                        0x8d99
#define GL_RGBA_MODE                                           0xc31
#define GL_RGBA_SNORM                                          0x8f93
#define GL_RGB_INTEGER                                         0x8d98
#define GL_RGB_SCALE                                           0x8573
#define GL_RGB_SNORM                                           0x8f92
#define GL_RG_INTEGER                                          0x8228
#define GL_RG_SNORM                                            0x8f91
#define GL_RIGHT                                               0x407
#define GL_S                                                   0x2000
#define GL_SAMPLER_1D                                          0x8b5d
#define GL_SAMPLER_1D_ARRAY                                    0x8dc0
#define GL_SAMPLER_1D_ARRAY_SHADOW                             0x8dc3
#define GL_SAMPLER_1D_SHADOW                                   0x8b61
#define GL_SAMPLER_2D                                          0x8b5e
#define GL_SAMPLER_2D_ARRAY                                    0x8dc1
#define GL_SAMPLER_2D_ARRAY_SHADOW                             0x8dc4
#define GL_SAMPLER_2D_MULTISAMPLE                              0x9108
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY                        0x910b
#define GL_SAMPLER_2D_RECT                                     0x8b63
#define GL_SAMPLER_2D_RECT_SHADOW                              0x8b64
#define GL_SAMPLER_2D_SHADOW                                   0x8b62
#define GL_SAMPLER_3D                                          0x8b5f
#define GL_SAMPLER_BINDING                                     0x8919
#define GL_SAMPLER_BUFFER                                      0x8dc2
#define GL_SAMPLER_CUBE                                        0x8b60
#define GL_SAMPLER_CUBE_SHADOW                                 0x8dc5
#define GL_SAMPLES                                             0x80a9
#define GL_SAMPLES_PASSED                                      0x8914
#define GL_SAMPLE_ALPHA_TO_COVERAGE                            0x809e
#define GL_SAMPLE_ALPHA_TO_ONE                                 0x809f
#define GL_SAMPLE_BUFFERS                                      0x80a8
#define GL_SAMPLE_COVERAGE                                     0x80a0
#define GL_SAMPLE_COVERAGE_INVERT                              0x80ab
#define GL_SAMPLE_COVERAGE_VALUE                               0x80aa
#define GL_SAMPLE_MASK                                         0x8e51
#define GL_SAMPLE_MASK_VALUE                                   0x8e52
#define GL_SAMPLE_POSITION                                     0x8e50
#define GL_SCISSOR_BIT                                         0x80000
#define GL_SCISSOR_BOX                                         0xc10
#define GL_SCISSOR_TEST                                        0xc11
#define GL_SECONDARY_COLOR_ARRAY                               0x845e
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING                0x889c
#define GL_SECONDARY_COLOR_ARRAY_POINTER                       0x845d
#define GL_SECONDARY_COLOR_ARRAY_SIZE                          0x845a
#define GL_SECONDARY_COLOR_ARRAY_STRIDE                        0x845c
#define GL_SECONDARY_COLOR_ARRAY_TYPE                          0x845b
#define GL_SELECT                                              0x1c02
#define GL_SELECTION_BUFFER_POINTER                            0xdf3
#define GL_SELECTION_BUFFER_SIZE                               0xdf4
#define GL_SEPARATE_ATTRIBS                                    0x8c8d
#define GL_SEPARATE_SPECULAR_COLOR                             0x81fa
#define GL_SET                                                 0x150f
#define GL_SHADER_SOURCE_LENGTH                                0x8b88
#define GL_SHADER_TYPE                                         0x8b4f
#define GL_SHADE_MODEL                                         0xb54
#define GL_SHADING_LANGUAGE_VERSION                            0x8b8c
#define GL_SHININESS                                           0x1601
#define GL_SHORT                                               0x1402
#define GL_SIGNALED                                            0x9119
#define GL_SIGNED_NORMALIZED                                   0x8f9c
#define GL_SINGLE_COLOR                                        0x81f9
#define GL_SLUMINANCE                                          0x8c46
#define GL_SLUMINANCE8                                         0x8c47
#define GL_SLUMINANCE8_ALPHA8                                  0x8c45
#define GL_SLUMINANCE_ALPHA                                    0x8c44
#define GL_SMOOTH                                              0x1d01
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY                       0xb23
#define GL_SMOOTH_LINE_WIDTH_RANGE                             0xb22
#define GL_SMOOTH_POINT_SIZE_GRANULARITY                       0xb13
#define GL_SMOOTH_POINT_SIZE_RANGE                             0xb12
#define GL_SOURCE0_ALPHA                                       0x8588
#define GL_SOURCE0_RGB                                         0x8580
#define GL_SOURCE1_ALPHA                                       0x8589
#define GL_SOURCE1_RGB                                         0x8581
#define GL_SOURCE2_ALPHA                                       0x858a
#define GL_SOURCE2_RGB                                         0x8582
#define GL_SPECULAR                                            0x1202
#define GL_SPHERE_MAP                                          0x2402
#define GL_SPOT_CUTOFF                                         0x1206
#define GL_SPOT_DIRECTION                                      0x1204
#define GL_SPOT_EXPONENT                                       0x1205
#define GL_SRC0_ALPHA                                          0x8588
#define GL_SRC0_RGB                                            0x8580
#define GL_SRC1_ALPHA                                          0x8589
#define GL_SRC1_COLOR                                          0x88f9
#define GL_SRC1_RGB                                            0x8581
#define GL_SRC2_ALPHA                                          0x858a
#define GL_SRC2_RGB                                            0x8582
#define GL_SRC_ALPHA                                           0x302
#define GL_SRC_ALPHA_SATURATE                                  0x308
#define GL_SRC_COLOR                                           0x300
#define GL_SRGB                                                0x8c40
#define GL_SRGB8                                               0x8c41
#define GL_SRGB8_ALPHA8                                        0x8c43
#define GL_SRGB_ALPHA                                          0x8c42
#define GL_STACK_OVERFLOW                                      0x503
#define GL_STACK_UNDERFLOW                                     0x504
#define GL_STATIC_COPY                                         0x88e6
#define GL_STATIC_DRAW                                         0x88e4
#define GL_STATIC_READ                                         0x88e5
#define GL_STENCIL                                             0x1802
#define GL_STENCIL_ATTACHMENT                                  0x8d20
#define GL_STENCIL_BACK_FAIL                                   0x8801
#define GL_STENCIL_BACK_FUNC                                   0x8800
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL                        0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS                        0x8803
#define GL_STENCIL_BACK_REF                                    0x8ca3
#define GL_STENCIL_BACK_VALUE_MASK                             0x8ca4
#define GL_STENCIL_BACK_WRITEMASK                              0x8ca5
#define GL_STENCIL_BITS                                        0xd57
#define GL_STENCIL_BUFFER                                      0x8224
#define GL_STENCIL_BUFFER_BIT                                  0x400
#define GL_STENCIL_CLEAR_VALUE                                 0xb91
#define GL_STENCIL_FAIL                                        0xb94
#define GL_STENCIL_FUNC                                        0xb92
#define GL_STENCIL_INDEX                                       0x1901
#define GL_STENCIL_INDEX1                                      0x8d46
#define GL_STENCIL_INDEX16                                     0x8d49
#define GL_STENCIL_INDEX4                                      0x8d47
#define GL_STENCIL_INDEX8                                      0x8d48
#define GL_STENCIL_PASS_DEPTH_FAIL                             0xb95
#define GL_STENCIL_PASS_DEPTH_PASS                             0xb96
#define GL_STENCIL_REF                                         0xb97
#define GL_STENCIL_TEST                                        0xb90
#define GL_STENCIL_VALUE_MASK                                  0xb93
#define GL_STENCIL_WRITEMASK                                   0xb98
#define GL_STEREO                                              0xc33
#define GL_STREAM_COPY                                         0x88e2
#define GL_STREAM_DRAW                                         0x88e0
#define GL_STREAM_READ                                         0x88e1
#define GL_SUBPIXEL_BITS                                       0xd50
#define GL_SUBTRACT                                            0x84e7
#define GL_SYNC_CONDITION                                      0x9113
#define GL_SYNC_FENCE                                          0x9116
#define GL_SYNC_FLAGS                                          0x9115
#define GL_SYNC_FLUSH_COMMANDS_BIT                             0x1
#define GL_SYNC_GPU_COMMANDS_COMPLETE                          0x9117
#define GL_SYNC_STATUS                                         0x9114
#define GL_T                                                   0x2001
#define GL_T2F_C3F_V3F                                         0x2a2a
#define GL_T2F_C4F_N3F_V3F                                     0x2a2c
#define GL_T2F_C4UB_V3F                                        0x2a29
#define GL_T2F_N3F_V3F                                         0x2a2b
#define GL_T2F_V3F                                             0x2a27
#define GL_T4F_C4F_N3F_V4F                                     0x2a2d
#define GL_T4F_V4F                                             0x2a28
#define GL_TEXTURE                                             0x1702
#define GL_TEXTURE0                                            0x84c0
#define GL_TEXTURE1                                            0x84c1
#define GL_TEXTURE10                                           0x84ca
#define GL_TEXTURE11                                           0x84cb
#define GL_TEXTURE12                                           0x84cc
#define GL_TEXTURE13                                           0x84cd
#define GL_TEXTURE14                                           0x84ce
#define GL_TEXTURE15                                           0x84cf
#define GL_TEXTURE16                                           0x84d0
#define GL_TEXTURE17                                           0x84d1
#define GL_TEXTURE18                                           0x84d2
#define GL_TEXTURE19                                           0x84d3
#define GL_TEXTURE2                                            0x84c2
#define GL_TEXTURE20                                           0x84d4
#define GL_TEXTURE21                                           0x84d5
#define GL_TEXTURE22                                           0x84d6
#define GL_TEXTURE23                                           0x84d7
#define GL_TEXTURE24                                           0x84d8
#define GL_TEXTURE25                                           0x84d9
#define GL_TEXTURE26                                           0x84da
#define GL_TEXTURE27                                           0x84db
#define GL_TEXTURE28                                           0x84dc
#define GL_TEXTURE29                                           0x84dd
#define GL_TEXTURE3                                            0x84c3
#define GL_TEXTURE30                                           0x84de
#define GL_TEXTURE31                                           0x84df
#define GL_TEXTURE4                                            0x84c4
#define GL_TEXTURE5                                            0x84c5
#define GL_TEXTURE6                                            0x84c6
#define GL_TEXTURE7                                            0x84c7
#define GL_TEXTURE8                                            0x84c8
#define GL_TEXTURE9                                            0x84c9
#define GL_TEXTURE_1D                                          0xde0
#define GL_TEXTURE_1D_ARRAY                                    0x8c18
#define GL_TEXTURE_2D                                          0xde1
#define GL_TEXTURE_2D_ARRAY                                    0x8c1a
#define GL_TEXTURE_2D_MULTISAMPLE                              0x9100
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY                        0x9102
#define GL_TEXTURE_3D                                          0x806f
#define GL_TEXTURE_ALPHA_SIZE                                  0x805f
#define GL_TEXTURE_ALPHA_TYPE                                  0x8c13
#define GL_TEXTURE_BASE_LEVEL                                  0x813c
#define GL_TEXTURE_BINDING_1D                                  0x8068
#define GL_TEXTURE_BINDING_1D_ARRAY                            0x8c1c
#define GL_TEXTURE_BINDING_2D                                  0x8069
#define GL_TEXTURE_BINDING_2D_ARRAY                            0x8c1d
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE                      0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY                0x9105
#define GL_TEXTURE_BINDING_3D                                  0x806a
#define GL_TEXTURE_BINDING_BUFFER                              0x8c2c
#define GL_TEXTURE_BINDING_CUBE_MAP                            0x8514
#define GL_TEXTURE_BINDING_RECTANGLE                           0x84f6
#define GL_TEXTURE_BIT                                         0x40000
#define GL_TEXTURE_BLUE_SIZE                                   0x805e
#define GL_TEXTURE_BLUE_TYPE                                   0x8c12
#define GL_TEXTURE_BORDER                                      0x1005
#define GL_TEXTURE_BORDER_COLOR                                0x1004
#define GL_TEXTURE_BUFFER                                      0x8c2a
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING                   0x8c2d
#define GL_TEXTURE_BUFFER_FORMAT                               0x8c2e
#define GL_TEXTURE_COMPARE_FUNC                                0x884d
#define GL_TEXTURE_COMPARE_MODE                                0x884c
#define GL_TEXTURE_COMPONENTS                                  0x1003
#define GL_TEXTURE_COMPRESSED                                  0x86a1
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE                       0x86a0
#define GL_TEXTURE_COMPRESSION_HINT                            0x84ef
#define GL_TEXTURE_COORD_ARRAY                                 0x8078
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING                  0x889a
#define GL_TEXTURE_COORD_ARRAY_POINTER                         0x8092
#define GL_TEXTURE_COORD_ARRAY_SIZE                            0x8088
#define GL_TEXTURE_COORD_ARRAY_STRIDE                          0x808a
#define GL_TEXTURE_COORD_ARRAY_TYPE                            0x8089
#define GL_TEXTURE_CUBE_MAP                                    0x8513
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X                         0x8516
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y                         0x8518
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z                         0x851a
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X                         0x8515
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y                         0x8517
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z                         0x8519
#define GL_TEXTURE_CUBE_MAP_SEAMLESS                           0x884f
#define GL_TEXTURE_DEPTH                                       0x8071
#define GL_TEXTURE_DEPTH_SIZE                                  0x884a
#define GL_TEXTURE_DEPTH_TYPE                                  0x8c16
#define GL_TEXTURE_ENV                                         0x2300
#define GL_TEXTURE_ENV_COLOR                                   0x2201
#define GL_TEXTURE_ENV_MODE                                    0x2200
#define GL_TEXTURE_FILTER_CONTROL                              0x8500
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS                      0x9107
#define GL_TEXTURE_GEN_MODE                                    0x2500
#define GL_TEXTURE_GEN_Q                                       0xc63
#define GL_TEXTURE_GEN_R                                       0xc62
#define GL_TEXTURE_GEN_S                                       0xc60
#define GL_TEXTURE_GEN_T                                       0xc61
#define GL_TEXTURE_GREEN_SIZE                                  0x805d
#define GL_TEXTURE_GREEN_TYPE                                  0x8c11
#define GL_TEXTURE_HEIGHT                                      0x1001
#define GL_TEXTURE_INTENSITY_SIZE                              0x8061
#define GL_TEXTURE_INTENSITY_TYPE                              0x8c15
#define GL_TEXTURE_INTERNAL_FORMAT                             0x1003
#define GL_TEXTURE_LOD_BIAS                                    0x8501
#define GL_TEXTURE_LUMINANCE_SIZE                              0x8060
#define GL_TEXTURE_LUMINANCE_TYPE                              0x8c14
#define GL_TEXTURE_MAG_FILTER                                  0x2800
#define GL_TEXTURE_MATRIX                                      0xba8
#define GL_TEXTURE_MAX_LEVEL                                   0x813d
#define GL_TEXTURE_MAX_LOD                                     0x813b
#define GL_TEXTURE_MIN_FILTER                                  0x2801
#define GL_TEXTURE_MIN_LOD                                     0x813a
#define GL_TEXTURE_PRIORITY                                    0x8066
#define GL_TEXTURE_RECTANGLE                                   0x84f5
#define GL_TEXTURE_RED_SIZE                                    0x805c
#define GL_TEXTURE_RED_TYPE                                    0x8c10
#define GL_TEXTURE_RESIDENT                                    0x8067
#define GL_TEXTURE_SAMPLES                                     0x9106
#define GL_TEXTURE_SHARED_SIZE                                 0x8c3f
#define GL_TEXTURE_STACK_DEPTH                                 0xba5
#define GL_TEXTURE_STENCIL_SIZE                                0x88f1
#define GL_TEXTURE_SWIZZLE_A                                   0x8e45
#define GL_TEXTURE_SWIZZLE_B                                   0x8e44
#define GL_TEXTURE_SWIZZLE_G                                   0x8e43
#define GL_TEXTURE_SWIZZLE_R                                   0x8e42
#define GL_TEXTURE_SWIZZLE_RGBA                                0x8e46
#define GL_TEXTURE_WIDTH                                       0x1000
#define GL_TEXTURE_WRAP_R                                      0x8072
#define GL_TEXTURE_WRAP_S                                      0x2802
#define GL_TEXTURE_WRAP_T                                      0x2803
#define GL_TIMEOUT_EXPIRED                                     0x911b
#define GL_TIMEOUT_IGNORED                                     0x7fffffffffffffff
#define GL_TIMESTAMP                                           0x8e28
#define GL_TIME_ELAPSED                                        0x88bf
#define GL_TRANSFORM_BIT                                       0x1000
#define GL_TRANSFORM_FEEDBACK_BUFFER                           0x8c8e
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING                   0x8c8f
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE                      0x8c7f
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE                      0x8c85
#define GL_TRANSFORM_FEEDBACK_BUFFER_START                     0x8c84
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN               0x8c88
#define GL_TRANSFORM_FEEDBACK_VARYINGS                         0x8c83
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH               0x8c76
#define GL_TRANSPOSE_COLOR_MATRIX                              0x84e6
#define GL_TRANSPOSE_MODELVIEW_MATRIX                          0x84e3
#define GL_TRANSPOSE_PROJECTION_MATRIX                         0x84e4
#define GL_TRANSPOSE_TEXTURE_MATRIX                            0x84e5
#define GL_TRIANGLES                                           0x4
#define GL_TRIANGLES_ADJACENCY                                 0xc
#define GL_TRIANGLE_FAN                                        0x6
#define GL_TRIANGLE_STRIP                                      0x5
#define GL_TRIANGLE_STRIP_ADJACENCY                            0xd
#define GL_TRUE                                                0x1
#define GL_UNIFORM_ARRAY_STRIDE                                0x8a3c
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS                       0x8a42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES                0x8a43
#define GL_UNIFORM_BLOCK_BINDING                               0x8a3f
#define GL_UNIFORM_BLOCK_DATA_SIZE                             0x8a40
#define GL_UNIFORM_BLOCK_INDEX                                 0x8a3a
#define GL_UNIFORM_BLOCK_NAME_LENGTH                           0x8a41
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER         0x8a46
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER         0x8a45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER           0x8a44
#define GL_UNIFORM_BUFFER                                      0x8a11
#define GL_UNIFORM_BUFFER_BINDING                              0x8a28
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT                     0x8a34
#define GL_UNIFORM_BUFFER_SIZE                                 0x8a2a
#define GL_UNIFORM_BUFFER_START                                0x8a29
#define GL_UNIFORM_IS_ROW_MAJOR                                0x8a3e
#define GL_UNIFORM_MATRIX_STRIDE                               0x8a3d
#define GL_UNIFORM_NAME_LENGTH                                 0x8a39
#define GL_UNIFORM_OFFSET                                      0x8a3b
#define GL_UNIFORM_SIZE                                        0x8a38
#define GL_UNIFORM_TYPE                                        0x8a37
#define GL_UNPACK_ALIGNMENT                                    0xcf5
#define GL_UNPACK_IMAGE_HEIGHT                                 0x806e
#define GL_UNPACK_LSB_FIRST                                    0xcf1
#define GL_UNPACK_ROW_LENGTH                                   0xcf2
#define GL_UNPACK_SKIP_IMAGES                                  0x806d
#define GL_UNPACK_SKIP_PIXELS                                  0xcf4
#define GL_UNPACK_SKIP_ROWS                                    0xcf3
#define GL_UNPACK_SWAP_BYTES                                   0xcf0
#define GL_UNSIGNALED                                          0x9118
#define GL_UNSIGNED_BYTE                                       0x1401
#define GL_UNSIGNED_BYTE_2_3_3_REV                             0x8362
#define GL_UNSIGNED_BYTE_3_3_2                                 0x8032
#define GL_UNSIGNED_INT                                        0x1405
#define GL_UNSIGNED_INT_10F_11F_11F_REV                        0x8c3b
#define GL_UNSIGNED_INT_10_10_10_2                             0x8036
#define GL_UNSIGNED_INT_24_8                                   0x84fa
#define GL_UNSIGNED_INT_2_10_10_10_REV                         0x8368
#define GL_UNSIGNED_INT_5_9_9_9_REV                            0x8c3e
#define GL_UNSIGNED_INT_8_8_8_8                                0x8035
#define GL_UNSIGNED_INT_8_8_8_8_REV                            0x8367
#define GL_UNSIGNED_INT_SAMPLER_1D                             0x8dd1
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY                       0x8dd6
#define GL_UNSIGNED_INT_SAMPLER_2D                             0x8dd2
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY                       0x8dd7
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE                 0x910a
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY           0x910d
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT                        0x8dd5
#define GL_UNSIGNED_INT_SAMPLER_3D                             0x8dd3
#define GL_UNSIGNED_INT_SAMPLER_BUFFER                         0x8dd8
#define GL_UNSIGNED_INT_SAMPLER_CUBE                           0x8dd4
#define GL_UNSIGNED_INT_VEC2                                   0x8dc6
#define GL_UNSIGNED_INT_VEC3                                   0x8dc7
#define GL_UNSIGNED_INT_VEC4                                   0x8dc8
#define GL_UNSIGNED_NORMALIZED                                 0x8c17
#define GL_UNSIGNED_SHORT                                      0x1403
#define GL_UNSIGNED_SHORT_1_5_5_5_REV                          0x8366
#define GL_UNSIGNED_SHORT_4_4_4_4                              0x8033
#define GL_UNSIGNED_SHORT_4_4_4_4_REV                          0x8365
#define GL_UNSIGNED_SHORT_5_5_5_1                              0x8034
#define GL_UNSIGNED_SHORT_5_6_5                                0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV                            0x8364
#define GL_UPPER_LEFT                                          0x8ca2
#define GL_V2F                                                 0x2a20
#define GL_V3F                                                 0x2a21
#define GL_VALIDATE_STATUS                                     0x8b83
#define GL_VENDOR                                              0x1f00
#define GL_VERSION                                             0x1f02
#define GL_VERTEX_ARRAY                                        0x8074
#define GL_VERTEX_ARRAY_BINDING                                0x85b5
#define GL_VERTEX_ARRAY_BUFFER_BINDING                         0x8896
#define GL_VERTEX_ARRAY_POINTER                                0x808e
#define GL_VERTEX_ARRAY_SIZE                                   0x807a
#define GL_VERTEX_ARRAY_STRIDE                                 0x807c
#define GL_VERTEX_ARRAY_TYPE                                   0x807b
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING                  0x889f
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR                         0x88fe
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED                         0x8622
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER                         0x88fd
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED                      0x886a
#define GL_VERTEX_ATTRIB_ARRAY_POINTER                         0x8645
#define GL_VERTEX_ATTRIB_ARRAY_SIZE                            0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE                          0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE                            0x8625
#define GL_VERTEX_PROGRAM_POINT_SIZE                           0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE                             0x8643
#define GL_VERTEX_SHADER                                       0x8b31
#define GL_VIEWPORT                                            0xba2
#define GL_VIEWPORT_BIT                                        0x800
#define GL_WAIT_FAILED                                         0x911d
#define GL_WEIGHT_ARRAY_BUFFER_BINDING                         0x889e
#define GL_WRITE_ONLY                                          0x88b9
#define GL_XOR                                                 0x1506
#define GL_ZERO                                                0x0
#define GL_ZOOM_X                                              0xd16
#define GL_ZOOM_Y                                              0xd17
//------------------------ FUNCTION PROTOTYPES -----------------------------//


// VERSION_1_0

GLAPI void APIENTRY glCullFace (const GLenum mode);
GLAPI void APIENTRY glFrontFace (const GLenum mode);
GLAPI void APIENTRY glHint (const GLenum target, const GLenum mode);
GLAPI void APIENTRY glLineWidth (const GLfloat width);
GLAPI void APIENTRY glPointSize (const GLfloat size);
GLAPI void APIENTRY glPolygonMode (const GLenum face, const GLenum mode);
GLAPI void APIENTRY glScissor (const GLint x, const GLint y, const GLsizei width, const GLsizei height);
GLAPI void APIENTRY glTexParameterf (const GLenum target, const GLenum pname, const GLfloat param);
GLAPI void APIENTRY glTexParameterfv (const GLenum target, const GLenum pname, const GLfloat* params);
GLAPI void APIENTRY glTexParameteri (const GLenum target, const GLenum pname, const GLint param);
GLAPI void APIENTRY glTexParameteriv (const GLenum target, const GLenum pname, const GLint* params);
GLAPI void APIENTRY glTexImage1D (const GLenum target, const GLint level, const GLint internalformat, const GLsizei width, const GLint border, const GLenum format, const GLenum type, const GLvoid* pixels);
GLAPI void APIENTRY glTexImage2D (const GLenum target, const GLint level, const GLint internalformat, const GLsizei width, const GLsizei height, const GLint border, const GLenum format, const GLenum type, const GLvoid* pixels);
GLAPI void APIENTRY glDrawBuffer (const GLenum mode);
GLAPI void APIENTRY glClear (const GLbitfield mask);
GLAPI void APIENTRY glClearColor (const GLclampf red, const GLclampf green, const GLclampf blue, const GLclampf alpha);
GLAPI void APIENTRY glClearStencil (const GLint s);
GLAPI void APIENTRY glClearDepth (const GLclampd depth);
GLAPI void APIENTRY glStencilMask (const GLuint mask);
GLAPI void APIENTRY glColorMask (const GLboolean red, const GLboolean green, const GLboolean blue, const GLboolean alpha);
GLAPI void APIENTRY glDepthMask (const GLboolean flag);
GLAPI void APIENTRY glDisable (const GLenum cap);
GLAPI void APIENTRY glEnable (const GLenum cap);
GLAPI void APIENTRY glFinish ();
GLAPI void APIENTRY glFlush ();
GLAPI void APIENTRY glBlendFunc (const GLenum sfactor, const GLenum dfactor);
GLAPI void APIENTRY glLogicOp (const GLenum opcode);
GLAPI void APIENTRY glStencilFunc (const GLenum func, const GLint ref, const GLuint mask);
GLAPI void APIENTRY glStencilOp (const GLenum fail, const GLenum zfail, const GLenum zpass);
GLAPI void APIENTRY glDepthFunc (const GLenum func);
GLAPI void APIENTRY glPixelStoref (const GLenum pname, const GLfloat param);
GLAPI void APIENTRY glPixelStorei (const GLenum pname, const GLint param);
GLAPI void APIENTRY glReadBuffer (const GLenum mode);
GLAPI void APIENTRY glReadPixels (const GLint x, const GLint y, const GLsizei width, const GLsizei height, const GLenum format, const GLenum type, GLvoid* pixels);
GLAPI void APIENTRY glGetBooleanv (const GLenum pname, GLboolean* params);
GLAPI void APIENTRY glGetDoublev (const GLenum pname, GLdouble* params);
GLAPI GLenum APIENTRY glGetError ();
GLAPI void APIENTRY glGetFloatv (const GLenum pname, GLfloat* params);
GLAPI void APIENTRY glGetIntegerv (const GLenum pname, GLint* params);
GLAPI const GLubyte * APIENTRY glGetString (const GLenum name);
GLAPI void APIENTRY glGetTexImage (const GLenum target, const GLint level, const GLenum format, const GLenum type, GLvoid* pixels);
GLAPI void APIENTRY glGetTexParameterfv (const GLenum target, const GLenum pname, GLfloat* params);
GLAPI void APIENTRY glGetTexParameteriv (const GLenum target, const GLenum pname, GLint* params);
GLAPI void APIENTRY glGetTexLevelParameterfv (const GLenum target, const GLint level, const GLenum pname, GLfloat* params);
GLAPI void APIENTRY glGetTexLevelParameteriv (const GLenum target, const GLint level, const GLenum pname, GLint* params);
GLAPI GLboolean APIENTRY glIsEnabled (const GLenum cap);
GLAPI void APIENTRY glDepthRange (const GLclampd near, const GLclampd far);
GLAPI void APIENTRY glViewport (const GLint x, const GLint y, const GLsizei width, const GLsizei height);

// VERSION_1_0_DEPRECATED

GLAPI void APIENTRY glNewList (const GLuint list, const GLenum mode);
GLAPI void APIENTRY glEndList ();
GLAPI void APIENTRY glCallList (const GLuint list);
GLAPI void APIENTRY glCallLists (const GLsizei n, const GLenum type, const GLvoid* lists);
GLAPI void APIENTRY glDeleteLists (const GLuint list, const GLsizei range);
GLAPI GLuint APIENTRY glGenLists (const GLsizei range);
GLAPI void APIENTRY glListBase (const GLuint base);
GLAPI void APIENTRY glBegin (const GLenum mode);
GLAPI void APIENTRY glBitmap (const GLsizei width, const GLsizei height, const GLfloat xorig, const GLfloat yorig, const GLfloat xmove, const GLfloat ymove, const GLubyte* bitmap);
GLAPI void APIENTRY glColor3b (const GLbyte red, const GLbyte green, const GLbyte blue);
GLAPI void APIENTRY glColor3bv (const GLbyte* v);
GLAPI void APIENTRY glColor3d (const GLdouble red, const GLdouble green, const GLdouble blue);
GLAPI void APIENTRY glColor3dv (const GLdouble* v);
GLAPI void APIENTRY glColor3f (const GLfloat red, const GLfloat green, const GLfloat blue);
GLAPI void APIENTRY glColor3fv (const GLfloat* v);
GLAPI void APIENTRY glColor3i (const GLint red, const GLint green, const GLint blue);
GLAPI void APIENTRY glColor3iv (const GLint* v);
GLAPI void APIENTRY glColor3s (const GLshort red, const GLshort green, const GLshort blue);
GLAPI void APIENTRY glColor3sv (const GLshort* v);
GLAPI void APIENTRY glColor3ub (const GLubyte red, const GLubyte green, const GLubyte blue);
GLAPI void APIENTRY glColor3ubv (const GLubyte* v);
GLAPI void APIENTRY glColor3ui (const GLuint red, const GLuint green, const GLuint blue);
GLAPI void APIENTRY glColor3uiv (const GLuint* v);
GLAPI void APIENTRY glColor3us (const GLushort red, const GLushort green, const GLushort blue);
GLAPI void APIENTRY glColor3usv (const GLushort* v);
GLAPI void APIENTRY glColor4b (const GLbyte red, const GLbyte green, const GLbyte blue, const GLbyte alpha);
GLAPI void APIENTRY glColor4bv (const GLbyte* v);
GLAPI void APIENTRY glColor4d (const GLdouble red, const GLdouble green, const GLdouble blue, const GLdouble alpha);
GLAPI void APIENTRY glColor4dv (const GLdouble* v);
GLAPI void APIENTRY glColor4f (const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha);
GLAPI void APIENTRY glColor4fv (const GLfloat* v);
GLAPI void APIENTRY glColor4i (const GLint red, const GLint green, const GLint blue, const GLint alpha);
GLAPI void APIENTRY glColor4iv (const GLint* v);
GLAPI void APIENTRY glColor4s (const GLshort red, const GLshort green, const GLshort blue, const GLshort alpha);
GLAPI void APIENTRY glColor4sv (const GLshort* v);
GLAPI void APIENTRY glColor4ub (const GLubyte red, const GLubyte green, const GLubyte blue, const GLubyte alpha);
GLAPI void APIENTRY glColor4ubv (const GLubyte* v);
GLAPI void APIENTRY glColor4ui (const GLuint red, const GLuint green, const GLuint blue, const GLuint alpha);
GLAPI void APIENTRY glColor4uiv (const GLuint* v);
GLAPI void APIENTRY glColor4us (const GLushort red, const GLushort green, const GLushort blue, const GLushort alpha);
GLAPI void APIENTRY glColor4usv (const GLushort* v);
GLAPI void APIENTRY glEdgeFlag (const GLboolean flag);
GLAPI void APIENTRY glEdgeFlagv (const GLboolean* flag);
GLAPI void APIENTRY glEnd ();
GLAPI void APIENTRY glIndexd (const GLdouble c);
GLAPI void APIENTRY glIndexdv (const GLdouble* c);
GLAPI void APIENTRY glIndexf (const GLfloat c);
GLAPI void APIENTRY glIndexfv (const GLfloat* c);
GLAPI void APIENTRY glIndexi (const GLint c);
GLAPI void APIENTRY glIndexiv (const GLint* c);
GLAPI void APIENTRY glIndexs (const GLshort c);
GLAPI void APIENTRY glIndexsv (const GLshort* c);
GLAPI void APIENTRY glNormal3b (const GLbyte nx, const GLbyte ny, const GLbyte nz);
GLAPI void APIENTRY glNormal3bv (const GLbyte* v);
GLAPI void APIENTRY glNormal3d (const GLdouble nx, const GLdouble ny, const GLdouble nz);
GLAPI void APIENTRY glNormal3dv (const GLdouble* v);
GLAPI void APIENTRY glNormal3f (const GLfloat nx, const GLfloat ny, const GLfloat nz);
GLAPI void APIENTRY glNormal3fv (const GLfloat* v);
GLAPI void APIENTRY glNormal3i (const GLint nx, const GLint ny, const GLint nz);
GLAPI void APIENTRY glNormal3iv (const GLint* v);
GLAPI void APIENTRY glNormal3s (const GLshort nx, const GLshort ny, const GLshort nz);
GLAPI void APIENTRY glNormal3sv (const GLshort* v);
GLAPI void APIENTRY glRasterPos2d (const GLdouble x, const GLdouble y);
GLAPI void APIENTRY glRasterPos2dv (const GLdouble* v);
GLAPI void APIENTRY glRasterPos2f (const GLfloat x, const GLfloat y);
GLAPI void APIENTRY glRasterPos2fv (const GLfloat* v);
GLAPI void APIENTRY glRasterPos2i (const GLint x, const GLint y);
GLAPI void APIENTRY glRasterPos2iv (const GLint* v);
GLAPI void APIENTRY glRasterPos2s (const GLshort x, const GLshort y);
GLAPI void APIENTRY glRasterPos2sv (const GLshort* v);
GLAPI void APIENTRY glRasterPos3d (const GLdouble x, const GLdouble y, const GLdouble z);
GLAPI void APIENTRY glRasterPos3dv (const GLdouble* v);
GLAPI void APIENTRY glRasterPos3f (const GLfloat x, const GLfloat y, const GLfloat z);
GLAPI void APIENTRY glRasterPos3fv (const GLfloat* v);
GLAPI void APIENTRY glRasterPos3i (const GLint x, const GLint y, const GLint z);
GLAPI void APIENTRY glRasterPos3iv (const GLint* v);
GLAPI void APIENTRY glRasterPos3s (const GLshort x, const GLshort y, const GLshort z);
GLAPI void APIENTRY glRasterPos3sv (const GLshort* v);
GLAPI void APIENTRY glRasterPos4d (const GLdouble x, const GLdouble y, const GLdouble z, const GLdouble w);
GLAPI void APIENTRY glRasterPos4dv (const GLdouble* v);
GLAPI void APIENTRY glRasterPos4f (const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w);
GLAPI void APIENTRY glRasterPos4fv (const GLfloat* v);
GLAPI void APIENTRY glRasterPos4i (const GLint x, const GLint y, const GLint z, const GLint w);
GLAPI void APIENTRY glRasterPos4iv (const GLint* v);
GLAPI void APIENTRY glRasterPos4s (const GLshort x, const GLshort y, const GLshort z, const GLshort w);
GLAPI void APIENTRY glRasterPos4sv (const GLshort* v);
GLAPI void APIENTRY glRectd (const GLdouble x1, const GLdouble y1, const GLdouble x2, const GLdouble y2);
GLAPI void APIENTRY glRectdv (const GLdouble* v1, const GLdouble* v2);
GLAPI void APIENTRY glRectf (const GLfloat x1, const GLfloat y1, const GLfloat x2, const GLfloat y2);
GLAPI void APIENTRY glRectfv (const GLfloat* v1, const GLfloat* v2);
GLAPI void APIENTRY glRecti (const GLint x1, const GLint y1, const GLint x2, const GLint y2);
GLAPI void APIENTRY glRectiv (const GLint* v1, const GLint* v2);
GLAPI void APIENTRY glRects (const GLshort x1, const GLshort y1, const GLshort x2, const GLshort y2);
GLAPI void APIENTRY glRectsv (const GLshort* v1, const GLshort* v2);
GLAPI void APIENTRY glTexCoord1d (const GLdouble s);
GLAPI void APIENTRY glTexCoord1dv (const GLdouble* v);
GLAPI void APIENTRY glTexCoord1f (const GLfloat s);
GLAPI void APIENTRY glTexCoord1fv (const GLfloat* v);
GLAPI void APIENTRY glTexCoord1i (const GLint s);
GLAPI void APIENTRY glTexCoord1iv (const GLint* v);
GLAPI void APIENTRY glTexCoord1s (const GLshort s);
GLAPI void APIENTRY glTexCoord1sv (const GLshort* v);
GLAPI void APIENTRY glTexCoord2d (const GLdouble s, const GLdouble t);
GLAPI void APIENTRY glTexCoord2dv (const GLdouble* v);
GLAPI void APIENTRY glTexCoord2f (const GLfloat s, const GLfloat t);
GLAPI void APIENTRY glTexCoord2fv (const GLfloat* v);
GLAPI void APIENTRY glTexCoord2i (const GLint s, const GLint t);
GLAPI void APIENTRY glTexCoord2iv (const GLint* v);
GLAPI void APIENTRY glTexCoord2s (const GLshort s, const GLshort t);
GLAPI void APIENTRY glTexCoord2sv (const GLshort* v);
GLAPI void APIENTRY glTexCoord3d (const GLdouble s, const GLdouble t, const GLdouble r);
GLAPI void APIENTRY glTexCoord3dv (const GLdouble* v);
GLAPI void APIENTRY glTexCoord3f (const GLfloat s, const GLfloat t, const GLfloat r);
GLAPI void APIENTRY glTexCoord3fv (const GLfloat* v);
GLAPI void APIENTRY glTexCoord3i (const GLint s, const GLint t, const GLint r);
GLAPI void APIENTRY glTexCoord3iv (const GLint* v);
GLAPI void APIENTRY glTexCoord3s (const GLshort s, const GLshort t, const GLshort r);
GLAPI void APIENTRY glTexCoord3sv (const GLshort* v);
GLAPI void APIENTRY glTexCoord4d (const GLdouble s, const GLdouble t, const GLdouble r, const GLdouble q);
GLAPI void APIENTRY glTexCoord4dv (const GLdouble* v);
GLAPI void APIENTRY glTexCoord4f (const GLfloat s, const GLfloat t, const GLfloat r, const GLfloat q);
GLAPI void APIENTRY glTexCoord4fv (const GLfloat* v);
GLAPI void APIENTRY glTexCoord4i (const GLint s, const GLint t, const GLint r, const GLint q);
GLAPI void APIENTRY glTexCoord4iv (const GLint* v);
GLAPI void APIENTRY glTexCoord4s (const GLshort s, const GLshort t, const GLshort r, const GLshort q);
GLAPI void APIENTRY glTexCoord4sv (const GLshort* v);
GLAPI void APIENTRY glVertex2d (const GLdouble x, const GLdouble y);
GLAPI void APIENTRY glVertex2dv (const GLdouble* v);
GLAPI void APIENTRY glVertex2f (const GLfloat x, const GLfloat y);
GLAPI void APIENTRY glVertex2fv (const GLfloat* v);
GLAPI void APIENTRY glVertex2i (const GLint x, const GLint y);
GLAPI void APIENTRY glVertex2iv (const GLint* v);
GLAPI void APIENTRY glVertex2s (const GLshort x, const GLshort y);
GLAPI void APIENTRY glVertex2sv (const GLshort* v);
GLAPI void APIENTRY glVertex3d (const GLdouble x, const GLdouble y, const GLdouble z);
GLAPI void APIENTRY glVertex3dv (const GLdouble* v);
GLAPI void APIENTRY glVertex3f (const GLfloat x, const GLfloat y, const GLfloat z);
GLAPI void APIENTRY glVertex3fv (const GLfloat* v);
GLAPI void APIENTRY glVertex3i (const GLint x, const GLint y, const GLint z);
GLAPI void APIENTRY glVertex3iv (const GLint* v);
GLAPI void APIENTRY glVertex3s (const GLshort x, const GLshort y, const GLshort z);
GLAPI void APIENTRY glVertex3sv (const GLshort* v);
GLAPI void APIENTRY glVertex4d (const GLdouble x, const GLdouble y, const GLdouble z, const GLdouble w);
GLAPI void APIENTRY glVertex4dv (const GLdouble* v);
GLAPI void APIENTRY glVertex4f (const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w);
GLAPI void APIENTRY glVertex4fv (const GLfloat* v);
GLAPI void APIENTRY glVertex4i (const GLint x, const GLint y, const GLint z, const GLint w);
GLAPI void APIENTRY glVertex4iv (const GLint* v);
GLAPI void APIENTRY glVertex4s (const GLshort x, const GLshort y, const GLshort z, const GLshort w);
GLAPI void APIENTRY glVertex4sv (const GLshort* v);
GLAPI void APIENTRY glClipPlane (const GLenum plane, const GLdouble* equation);
GLAPI void APIENTRY glColorMaterial (const GLenum face, const GLenum mode);
GLAPI void APIENTRY glFogf (const GLenum pname, const GLfloat param);
GLAPI void APIENTRY glFogfv (const GLenum pname, const GLfloat* params);
GLAPI void APIENTRY glFogi (const GLenum pname, const GLint param);
GLAPI void APIENTRY glFogiv (const GLenum pname, const GLint* params);
GLAPI void APIENTRY glLightf (const GLenum light, const GLenum pname, const GLfloat param);
GLAPI void APIENTRY glLightfv (const GLenum light, const GLenum pname, const GLfloat* params);
GLAPI void APIENTRY glLighti (const GLenum light, const GLenum pname, const GLint param);
GLAPI void APIENTRY glLightiv (const GLenum light, const GLenum pname, const GLint* params);
GLAPI void APIENTRY glLightModelf (const GLenum pname, const GLfloat param);
GLAPI void APIENTRY glLightModelfv (const GLenum pname, const GLfloat* params);
GLAPI void APIENTRY glLightModeli (const GLenum pname, const GLint param);
GLAPI void APIENTRY glLightModeliv (const GLenum pname, const GLint* params);
GLAPI void APIENTRY glLineStipple (const GLint factor, const GLushort pattern);
GLAPI void APIENTRY glMaterialf (const GLenum face, const GLenum pname, const GLfloat param);
GLAPI void APIENTRY glMaterialfv (const GLenum face, const GLenum pname, const GLfloat* params);
GLAPI void APIENTRY glMateriali (const GLenum face, const GLenum pname, const GLint param);
GLAPI void APIENTRY glMaterialiv (const GLenum face, const GLenum pname, const GLint* params);
GLAPI void APIENTRY glPolygonStipple (const GLubyte* mask);
GLAPI void APIENTRY glShadeModel (const GLenum mode);
GLAPI void APIENTRY glTexEnvf (const GLenum target, const GLenum pname, const GLfloat param);
GLAPI void APIENTRY glTexEnvfv (const GLenum target, const GLenum pname, const GLfloat* params);
GLAPI void APIENTRY glTexEnvi (const GLenum target, const GLenum pname, const GLint param);
GLAPI void APIENTRY glTexEnviv (const GLenum target, const GLenum pname, const GLint* params);
GLAPI void APIENTRY glTexGend (const GLenum coord, const GLenum pname, const GLdouble param);
GLAPI void APIENTRY glTexGendv (const GLenum coord, const GLenum pname, const GLdouble* params);
GLAPI void APIENTRY glTexGenf (const GLenum coord, const GLenum pname, const GLfloat param);
GLAPI void APIENTRY glTexGenfv (const GLenum coord, const GLenum pname, const GLfloat* params);
GLAPI void APIENTRY glTexGeni (const GLenum coord, const GLenum pname, const GLint param);
GLAPI void APIENTRY glTexGeniv (const GLenum coord, const GLenum pname, const GLint* params);
GLAPI void APIENTRY glFeedbackBuffer (const GLsizei size, const GLenum type, GLfloat* buffer);
GLAPI void APIENTRY glSelectBuffer (const GLsizei size, GLuint* buffer);
GLAPI GLint APIENTRY glRenderMode (const GLenum mode);
GLAPI void APIENTRY glInitNames ();
GLAPI void APIENTRY glLoadName (const GLuint name);
GLAPI void APIENTRY glPassThrough (const GLfloat token);
GLAPI void APIENTRY glPopName ();
GLAPI void APIENTRY glPushName (const GLuint name);
GLAPI void APIENTRY glClearAccum (const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha);
GLAPI void APIENTRY glClearIndex (const GLfloat c);
GLAPI void APIENTRY glIndexMask (const GLuint mask);
GLAPI void APIENTRY glAccum (const GLenum op, const GLfloat value);
GLAPI void APIENTRY glPopAttrib ();
GLAPI void APIENTRY glPushAttrib (const GLbitfield mask);
GLAPI void APIENTRY glMap1d (const GLenum target, const GLdouble u1, const GLdouble u2, const GLint stride, const GLint order, const GLdouble* points);
GLAPI void APIENTRY glMap1f (const GLenum target, const GLfloat u1, const GLfloat u2, const GLint stride, const GLint order, const GLfloat* points);
GLAPI void APIENTRY glMap2d (const GLenum target, const GLdouble u1, const GLdouble u2, const GLint ustride, const GLint uorder, const GLdouble v1, const GLdouble v2, const GLint vstride, const GLint vorder, const GLdouble* points);
GLAPI void APIENTRY glMap2f (const GLenum target, const GLfloat u1, const GLfloat u2, const GLint ustride, const GLint uorder, const GLfloat v1, const GLfloat v2, const GLint vstride, const GLint vorder, const GLfloat* points);
GLAPI void APIENTRY glMapGrid1d (const GLint un, const GLdouble u1, const GLdouble u2);
GLAPI void APIENTRY glMapGrid1f (const GLint un, const GLfloat u1, const GLfloat u2);
GLAPI void APIENTRY glMapGrid2d (const GLint un, const GLdouble u1, const GLdouble u2, const GLint vn, const GLdouble v1, const GLdouble v2);
GLAPI void APIENTRY glMapGrid2f (const GLint un, const GLfloat u1, const GLfloat u2, const GLint vn, const GLfloat v1, const GLfloat v2);
GLAPI void APIENTRY glEvalCoord1d (const GLdouble u);
GLAPI void APIENTRY glEvalCoord1dv (const GLdouble* u);
GLAPI void APIENTRY glEvalCoord1f (const GLfloat u);
GLAPI void APIENTRY glEvalCoord1fv (const GLfloat* u);
GLAPI void APIENTRY glEvalCoord2d (const GLdouble u, const GLdouble v);
GLAPI void APIENTRY glEvalCoord2dv (const GLdouble* u);
GLAPI void APIENTRY glEvalCoord2f (const GLfloat u, const GLfloat v);
GLAPI void APIENTRY glEvalCoord2fv (const GLfloat* u);
GLAPI void APIENTRY glEvalMesh1 (const GLenum mode, const GLint i1, const GLint i2);
GLAPI void APIENTRY glEvalPoint1 (const GLint i);
GLAPI void APIENTRY glEvalMesh2 (const GLenum mode, const GLint i1, const GLint i2, const GLint j1, const GLint j2);
GLAPI void APIENTRY glEvalPoint2 (const GLint i, const GLint j);
GLAPI void APIENTRY glAlphaFunc (const GLenum func, const GLclampf ref);
GLAPI void APIENTRY glPixelZoom (const GLfloat xfactor, const GLfloat yfactor);
GLAPI void APIENTRY glPixelTransferf (const GLenum pname, const GLfloat param);
GLAPI void APIENTRY glPixelTransferi (const GLenum pname, const GLint param);
GLAPI void APIENTRY glPixelMapfv (const GLenum map, const GLint mapsize, const GLfloat* values);
GLAPI void APIENTRY glPixelMapuiv (const GLenum map, const GLint mapsize, const GLuint* values);
GLAPI void APIENTRY glPixelMapusv (const GLenum map, const GLint mapsize, const GLushort* values);
GLAPI void APIENTRY glCopyPixels (const GLint x, const GLint y, const GLsizei width, const GLsizei height, const GLenum type);
GLAPI void APIENTRY glDrawPixels (const GLsizei width, const GLsizei height, const GLenum format, const GLenum type, const GLvoid* pixels);
GLAPI void APIENTRY glGetClipPlane (const GLenum plane, GLdouble* equation);
GLAPI void APIENTRY glGetLightfv (const GLenum light, const GLenum pname, GLfloat* params);
GLAPI void APIENTRY glGetLightiv (const GLenum light, const GLenum pname, GLint* params);
GLAPI void APIENTRY glGetMapdv (const GLenum target, const GLenum query, GLdouble* v);
GLAPI void APIENTRY glGetMapfv (const GLenum target, const GLenum query, GLfloat* v);
GLAPI void APIENTRY glGetMapiv (const GLenum target, const GLenum query, GLint* v);
GLAPI void APIENTRY glGetMaterialfv (const GLenum face, const GLenum pname, GLfloat* params);
GLAPI void APIENTRY glGetMaterialiv (const GLenum face, const GLenum pname, GLint* params);
GLAPI void APIENTRY glGetPixelMapfv (const GLenum map, GLfloat* values);
GLAPI void APIENTRY glGetPixelMapuiv (const GLenum map, GLuint* values);
GLAPI void APIENTRY glGetPixelMapusv (const GLenum map, GLushort* values);
GLAPI void APIENTRY glGetPolygonStipple (GLubyte* mask);
GLAPI void APIENTRY glGetTexEnvfv (const GLenum target, const GLenum pname, GLfloat* params);
GLAPI void APIENTRY glGetTexEnviv (const GLenum target, const GLenum pname, GLint* params);
GLAPI void APIENTRY glGetTexGendv (const GLenum coord, const GLenum pname, GLdouble* params);
GLAPI void APIENTRY glGetTexGenfv (const GLenum coord, const GLenum pname, GLfloat* params);
GLAPI void APIENTRY glGetTexGeniv (const GLenum coord, const GLenum pname, GLint* params);
GLAPI GLboolean APIENTRY glIsList (const GLuint list);
GLAPI void APIENTRY glFrustum (const GLdouble left, const GLdouble right, const GLdouble bottom, const GLdouble top, const GLdouble zNear, const GLdouble zFar);
GLAPI void APIENTRY glLoadIdentity ();
GLAPI void APIENTRY glLoadMatrixf (const GLfloat* m);
GLAPI void APIENTRY glLoadMatrixd (const GLdouble* m);
GLAPI void APIENTRY glMatrixMode (const GLenum mode);
GLAPI void APIENTRY glMultMatrixf (const GLfloat* m);
GLAPI void APIENTRY glMultMatrixd (const GLdouble* m);
GLAPI void APIENTRY glOrtho (const GLdouble left, const GLdouble right, const GLdouble bottom, const GLdouble top, const GLdouble zNear, const GLdouble zFar);
GLAPI void APIENTRY glPopMatrix ();
GLAPI void APIENTRY glPushMatrix ();
GLAPI void APIENTRY glRotated (const GLdouble angle, const GLdouble x, const GLdouble y, const GLdouble z);
GLAPI void APIENTRY glRotatef (const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z);
GLAPI void APIENTRY glScaled (const GLdouble x, const GLdouble y, const GLdouble z);
GLAPI void APIENTRY glScalef (const GLfloat x, const GLfloat y, const GLfloat z);
GLAPI void APIENTRY glTranslated (const GLdouble x, const GLdouble y, const GLdouble z);
GLAPI void APIENTRY glTranslatef (const GLfloat x, const GLfloat y, const GLfloat z);

// VERSION_1_1

GLAPI void APIENTRY glDrawArrays (const GLenum mode, const GLint first, const GLsizei count);
GLAPI void APIENTRY glDrawElements (const GLenum mode, const GLsizei count, const GLenum type, const GLvoid* indices);
GLAPI void APIENTRY glGetPointerv (const GLenum pname, GLvoid** params);
GLAPI void APIENTRY glPolygonOffset (const GLfloat factor, const GLfloat units);
GLAPI void APIENTRY glCopyTexImage1D (const GLenum target, const GLint level, const GLenum internalformat, const GLint x, const GLint y, const GLsizei width, const GLint border);
GLAPI void APIENTRY glCopyTexImage2D (const GLenum target, const GLint level, const GLenum internalformat, const GLint x, const GLint y, const GLsizei width, const GLsizei height, const GLint border);
GLAPI void APIENTRY glCopyTexSubImage1D (const GLenum target, const GLint level, const GLint xoffset, const GLint x, const GLint y, const GLsizei width);
GLAPI void APIENTRY glCopyTexSubImage2D (const GLenum target, const GLint level, const GLint xoffset, const GLint yoffset, const GLint x, const GLint y, const GLsizei width, const GLsizei height);
GLAPI void APIENTRY glTexSubImage1D (const GLenum target, const GLint level, const GLint xoffset, const GLsizei width, const GLenum format, const GLenum type, const GLvoid* pixels);
GLAPI void APIENTRY glTexSubImage2D (const GLenum target, const GLint level, const GLint xoffset, const GLint yoffset, const GLsizei width, const GLsizei height, const GLenum format, const GLenum type, const GLvoid* pixels);
GLAPI void APIENTRY glBindTexture (const GLenum target, const GLuint texture);
GLAPI void APIENTRY glDeleteTextures (const GLsizei n, const GLuint* textures);
GLAPI void APIENTRY glGenTextures (const GLsizei n, GLuint* textures);
GLAPI GLboolean APIENTRY glIsTexture (const GLuint texture);

// VERSION_1_1_DEPRECATED

GLAPI void APIENTRY glArrayElement (const GLint i);
GLAPI void APIENTRY glColorPointer (const GLint size, const GLenum type, const GLsizei stride, const GLvoid* pointer);
GLAPI void APIENTRY glDisableClientState (const GLenum array);
GLAPI void APIENTRY glEdgeFlagPointer (const GLsizei stride, const GLvoid* pointer);
GLAPI void APIENTRY glEnableClientState (const GLenum array);
GLAPI void APIENTRY glIndexPointer (const GLenum type, const GLsizei stride, const GLvoid* pointer);
GLAPI void APIENTRY glInterleavedArrays (const GLenum format, const GLsizei stride, const GLvoid* pointer);
GLAPI void APIENTRY glNormalPointer (const GLenum type, const GLsizei stride, const GLvoid* pointer);
GLAPI void APIENTRY glTexCoordPointer (const GLint size, const GLenum type, const GLsizei stride, const GLvoid* pointer);
GLAPI void APIENTRY glVertexPointer (const GLint size, const GLenum type, const GLsizei stride, const GLvoid* pointer);
GLAPI GLboolean APIENTRY glAreTexturesResident (const GLsizei n, const GLuint* textures, GLboolean* residences);
GLAPI void APIENTRY glPrioritizeTextures (const GLsizei n, const GLuint* textures, const GLclampf* priorities);
GLAPI void APIENTRY glIndexub (const GLubyte c);
GLAPI void APIENTRY glIndexubv (const GLubyte* c);
GLAPI void APIENTRY glPopClientAttrib ();
GLAPI void APIENTRY glPushClientAttrib (const GLbitfield mask);

// VERSION_1_2

typedef void (APIENTRYP PFNGLBLENDCOLORPROC)(const GLclampf red, const GLclampf green, const GLclampf blue, const GLclampf alpha);
typedef void (APIENTRYP PFNGLBLENDEQUATIONPROC)(const GLenum mode);
typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSPROC)(const GLenum mode, const GLuint start, const GLuint end, const GLsizei count, const GLenum type, const GLvoid* indices);
typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC)(const GLenum target, const GLint level, const GLint internalformat, const GLsizei width, const GLsizei height, const GLsizei depth, const GLint border, const GLenum format, const GLenum type, const GLvoid* pixels);
typedef void (APIENTRYP PFNGLTEXSUBIMAGE3DPROC)(const GLenum target, const GLint level, const GLint xoffset, const GLint yoffset, const GLint zoffset, const GLsizei width, const GLsizei height, const GLsizei depth, const GLenum format, const GLenum type, const GLvoid* pixels);
typedef void (APIENTRYP PFNGLCOPYTEXSUBIMAGE3DPROC)(const GLenum target, const GLint level, const GLint xoffset, const GLint yoffset, const GLint zoffset, const GLint x, const GLint y, const GLsizei width, const GLsizei height);

GLAPI PFNGLBLENDCOLORPROC glpfBlendColor;
GLAPI PFNGLBLENDEQUATIONPROC glpfBlendEquation;
GLAPI PFNGLDRAWRANGEELEMENTSPROC glpfDrawRangeElements;
GLAPI PFNGLTEXIMAGE3DPROC glpfTexImage3D;
GLAPI PFNGLTEXSUBIMAGE3DPROC glpfTexSubImage3D;
GLAPI PFNGLCOPYTEXSUBIMAGE3DPROC glpfCopyTexSubImage3D;

#define glBlendColor glpfBlendColor
#define glBlendEquation glpfBlendEquation
#define glDrawRangeElements glpfDrawRangeElements
#define glTexImage3D glpfTexImage3D
#define glTexSubImage3D glpfTexSubImage3D
#define glCopyTexSubImage3D glpfCopyTexSubImage3D

// VERSION_1_2_DEPRECATED

typedef void (APIENTRYP PFNGLCOLORTABLEPROC)(const GLenum target, const GLenum internalformat, const GLsizei width, const GLenum format, const GLenum type, const GLvoid* table);
typedef void (APIENTRYP PFNGLCOLORTABLEPARAMETERFVPROC)(const GLenum target, const GLenum pname, const GLfloat* params);
typedef void (APIENTRYP PFNGLCOLORTABLEPARAMETERIVPROC)(const GLenum target, const GLenum pname, const GLint* params);
typedef void (APIENTRYP PFNGLCOPYCOLORTABLEPROC)(const GLenum target, const GLenum internalformat, const GLint x, const GLint y, const GLsizei width);
typedef void (APIENTRYP PFNGLGETCOLORTABLEPROC)(const GLenum target, const GLenum format, const GLenum type, GLvoid* table);
typedef void (APIENTRYP PFNGLGETCOLORTABLEPARAMETERFVPROC)(const GLenum target, const GLenum pname, GLfloat* params);
typedef void (APIENTRYP PFNGLGETCOLORTABLEPARAMETERIVPROC)(const GLenum target, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLCOLORSUBTABLEPROC)(const GLenum target, const GLsizei start, const GLsizei count, const GLenum format, const GLenum type, const GLvoid* data);
typedef void (APIENTRYP PFNGLCOPYCOLORSUBTABLEPROC)(const GLenum target, const GLsizei start, const GLint x, const GLint y, const GLsizei width);
typedef void (APIENTRYP PFNGLCONVOLUTIONFILTER1DPROC)(const GLenum target, const GLenum internalformat, const GLsizei width, const GLenum format, const GLenum type, const GLvoid* image);
typedef void (APIENTRYP PFNGLCONVOLUTIONFILTER2DPROC)(const GLenum target, const GLenum internalformat, const GLsizei width, const GLsizei height, const GLenum format, const GLenum type, const GLvoid* image);
typedef void (APIENTRYP PFNGLCONVOLUTIONPARAMETERFPROC)(const GLenum target, const GLenum pname, const GLfloat params);
typedef void (APIENTRYP PFNGLCONVOLUTIONPARAMETERFVPROC)(const GLenum target, const GLenum pname, const GLfloat* params);
typedef void (APIENTRYP PFNGLCONVOLUTIONPARAMETERIPROC)(const GLenum target, const GLenum pname, const GLint params);
typedef void (APIENTRYP PFNGLCONVOLUTIONPARAMETERIVPROC)(const GLenum target, const GLenum pname, const GLint* params);
typedef void (APIENTRYP PFNGLCOPYCONVOLUTIONFILTER1DPROC)(const GLenum target, const GLenum internalformat, const GLint x, const GLint y, const GLsizei width);
typedef void (APIENTRYP PFNGLCOPYCONVOLUTIONFILTER2DPROC)(const GLenum target, const GLenum internalformat, const GLint x, const GLint y, const GLsizei width, const GLsizei height);
typedef void (APIENTRYP PFNGLGETCONVOLUTIONFILTERPROC)(const GLenum target, const GLenum format, const GLenum type, GLvoid* image);
typedef void (APIENTRYP PFNGLGETCONVOLUTIONPARAMETERFVPROC)(const GLenum target, const GLenum pname, GLfloat* params);
typedef void (APIENTRYP PFNGLGETCONVOLUTIONPARAMETERIVPROC)(const GLenum target, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETSEPARABLEFILTERPROC)(const GLenum target, const GLenum format, const GLenum type, GLvoid* row, GLvoid* column, GLvoid* span);
typedef void (APIENTRYP PFNGLSEPARABLEFILTER2DPROC)(const GLenum target, const GLenum internalformat, const GLsizei width, const GLsizei height, const GLenum format, const GLenum type, const GLvoid* row, const GLvoid* column);
typedef void (APIENTRYP PFNGLGETHISTOGRAMPROC)(const GLenum target, const GLboolean reset, const GLenum format, const GLenum type, GLvoid* values);
typedef void (APIENTRYP PFNGLGETHISTOGRAMPARAMETERFVPROC)(const GLenum target, const GLenum pname, GLfloat* params);
typedef void (APIENTRYP PFNGLGETHISTOGRAMPARAMETERIVPROC)(const GLenum target, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETMINMAXPROC)(const GLenum target, const GLboolean reset, const GLenum format, const GLenum type, GLvoid* values);
typedef void (APIENTRYP PFNGLGETMINMAXPARAMETERFVPROC)(const GLenum target, const GLenum pname, GLfloat* params);
typedef void (APIENTRYP PFNGLGETMINMAXPARAMETERIVPROC)(const GLenum target, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLHISTOGRAMPROC)(const GLenum target, const GLsizei width, const GLenum internalformat, const GLboolean sink);
typedef void (APIENTRYP PFNGLMINMAXPROC)(const GLenum target, const GLenum internalformat, const GLboolean sink);
typedef void (APIENTRYP PFNGLRESETHISTOGRAMPROC)(const GLenum target);
typedef void (APIENTRYP PFNGLRESETMINMAXPROC)(const GLenum target);

GLAPI PFNGLCOLORTABLEPROC glpfColorTable;
GLAPI PFNGLCOLORTABLEPARAMETERFVPROC glpfColorTableParameterfv;
GLAPI PFNGLCOLORTABLEPARAMETERIVPROC glpfColorTableParameteriv;
GLAPI PFNGLCOPYCOLORTABLEPROC glpfCopyColorTable;
GLAPI PFNGLGETCOLORTABLEPROC glpfGetColorTable;
GLAPI PFNGLGETCOLORTABLEPARAMETERFVPROC glpfGetColorTableParameterfv;
GLAPI PFNGLGETCOLORTABLEPARAMETERIVPROC glpfGetColorTableParameteriv;
GLAPI PFNGLCOLORSUBTABLEPROC glpfColorSubTable;
GLAPI PFNGLCOPYCOLORSUBTABLEPROC glpfCopyColorSubTable;
GLAPI PFNGLCONVOLUTIONFILTER1DPROC glpfConvolutionFilter1D;
GLAPI PFNGLCONVOLUTIONFILTER2DPROC glpfConvolutionFilter2D;
GLAPI PFNGLCONVOLUTIONPARAMETERFPROC glpfConvolutionParameterf;
GLAPI PFNGLCONVOLUTIONPARAMETERFVPROC glpfConvolutionParameterfv;
GLAPI PFNGLCONVOLUTIONPARAMETERIPROC glpfConvolutionParameteri;
GLAPI PFNGLCONVOLUTIONPARAMETERIVPROC glpfConvolutionParameteriv;
GLAPI PFNGLCOPYCONVOLUTIONFILTER1DPROC glpfCopyConvolutionFilter1D;
GLAPI PFNGLCOPYCONVOLUTIONFILTER2DPROC glpfCopyConvolutionFilter2D;
GLAPI PFNGLGETCONVOLUTIONFILTERPROC glpfGetConvolutionFilter;
GLAPI PFNGLGETCONVOLUTIONPARAMETERFVPROC glpfGetConvolutionParameterfv;
GLAPI PFNGLGETCONVOLUTIONPARAMETERIVPROC glpfGetConvolutionParameteriv;
GLAPI PFNGLGETSEPARABLEFILTERPROC glpfGetSeparableFilter;
GLAPI PFNGLSEPARABLEFILTER2DPROC glpfSeparableFilter2D;
GLAPI PFNGLGETHISTOGRAMPROC glpfGetHistogram;
GLAPI PFNGLGETHISTOGRAMPARAMETERFVPROC glpfGetHistogramParameterfv;
GLAPI PFNGLGETHISTOGRAMPARAMETERIVPROC glpfGetHistogramParameteriv;
GLAPI PFNGLGETMINMAXPROC glpfGetMinmax;
GLAPI PFNGLGETMINMAXPARAMETERFVPROC glpfGetMinmaxParameterfv;
GLAPI PFNGLGETMINMAXPARAMETERIVPROC glpfGetMinmaxParameteriv;
GLAPI PFNGLHISTOGRAMPROC glpfHistogram;
GLAPI PFNGLMINMAXPROC glpfMinmax;
GLAPI PFNGLRESETHISTOGRAMPROC glpfResetHistogram;
GLAPI PFNGLRESETMINMAXPROC glpfResetMinmax;

#define glColorTable glpfColorTable
#define glColorTableParameterfv glpfColorTableParameterfv
#define glColorTableParameteriv glpfColorTableParameteriv
#define glCopyColorTable glpfCopyColorTable
#define glGetColorTable glpfGetColorTable
#define glGetColorTableParameterfv glpfGetColorTableParameterfv
#define glGetColorTableParameteriv glpfGetColorTableParameteriv
#define glColorSubTable glpfColorSubTable
#define glCopyColorSubTable glpfCopyColorSubTable
#define glConvolutionFilter1D glpfConvolutionFilter1D
#define glConvolutionFilter2D glpfConvolutionFilter2D
#define glConvolutionParameterf glpfConvolutionParameterf
#define glConvolutionParameterfv glpfConvolutionParameterfv
#define glConvolutionParameteri glpfConvolutionParameteri
#define glConvolutionParameteriv glpfConvolutionParameteriv
#define glCopyConvolutionFilter1D glpfCopyConvolutionFilter1D
#define glCopyConvolutionFilter2D glpfCopyConvolutionFilter2D
#define glGetConvolutionFilter glpfGetConvolutionFilter
#define glGetConvolutionParameterfv glpfGetConvolutionParameterfv
#define glGetConvolutionParameteriv glpfGetConvolutionParameteriv
#define glGetSeparableFilter glpfGetSeparableFilter
#define glSeparableFilter2D glpfSeparableFilter2D
#define glGetHistogram glpfGetHistogram
#define glGetHistogramParameterfv glpfGetHistogramParameterfv
#define glGetHistogramParameteriv glpfGetHistogramParameteriv
#define glGetMinmax glpfGetMinmax
#define glGetMinmaxParameterfv glpfGetMinmaxParameterfv
#define glGetMinmaxParameteriv glpfGetMinmaxParameteriv
#define glHistogram glpfHistogram
#define glMinmax glpfMinmax
#define glResetHistogram glpfResetHistogram
#define glResetMinmax glpfResetMinmax

// VERSION_1_3

typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC)(const GLenum texture);
typedef void (APIENTRYP PFNGLSAMPLECOVERAGEPROC)(const GLclampf value, const GLboolean invert);
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE3DPROC)(const GLenum target, const GLint level, const GLenum internalformat, const GLsizei width, const GLsizei height, const GLsizei depth, const GLint border, const GLsizei imageSize, const GLvoid* data);
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE2DPROC)(const GLenum target, const GLint level, const GLenum internalformat, const GLsizei width, const GLsizei height, const GLint border, const GLsizei imageSize, const GLvoid* data);
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE1DPROC)(const GLenum target, const GLint level, const GLenum internalformat, const GLsizei width, const GLint border, const GLsizei imageSize, const GLvoid* data);
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)(const GLenum target, const GLint level, const GLint xoffset, const GLint yoffset, const GLint zoffset, const GLsizei width, const GLsizei height, const GLsizei depth, const GLenum format, const GLsizei imageSize, const GLvoid* data);
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)(const GLenum target, const GLint level, const GLint xoffset, const GLint yoffset, const GLsizei width, const GLsizei height, const GLenum format, const GLsizei imageSize, const GLvoid* data);
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)(const GLenum target, const GLint level, const GLint xoffset, const GLsizei width, const GLenum format, const GLsizei imageSize, const GLvoid* data);
typedef void (APIENTRYP PFNGLGETCOMPRESSEDTEXIMAGEPROC)(const GLenum target, const GLint level, GLvoid* img);

GLAPI PFNGLACTIVETEXTUREPROC glpfActiveTexture;
GLAPI PFNGLSAMPLECOVERAGEPROC glpfSampleCoverage;
GLAPI PFNGLCOMPRESSEDTEXIMAGE3DPROC glpfCompressedTexImage3D;
GLAPI PFNGLCOMPRESSEDTEXIMAGE2DPROC glpfCompressedTexImage2D;
GLAPI PFNGLCOMPRESSEDTEXIMAGE1DPROC glpfCompressedTexImage1D;
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glpfCompressedTexSubImage3D;
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glpfCompressedTexSubImage2D;
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glpfCompressedTexSubImage1D;
GLAPI PFNGLGETCOMPRESSEDTEXIMAGEPROC glpfGetCompressedTexImage;

#define glActiveTexture glpfActiveTexture
#define glSampleCoverage glpfSampleCoverage
#define glCompressedTexImage3D glpfCompressedTexImage3D
#define glCompressedTexImage2D glpfCompressedTexImage2D
#define glCompressedTexImage1D glpfCompressedTexImage1D
#define glCompressedTexSubImage3D glpfCompressedTexSubImage3D
#define glCompressedTexSubImage2D glpfCompressedTexSubImage2D
#define glCompressedTexSubImage1D glpfCompressedTexSubImage1D
#define glGetCompressedTexImage glpfGetCompressedTexImage

// VERSION_1_3_DEPRECATED

typedef void (APIENTRYP PFNGLCLIENTACTIVETEXTUREPROC)(const GLenum texture);
typedef void (APIENTRYP PFNGLMULTITEXCOORD1DPROC)(const GLenum target, const GLdouble s);
typedef void (APIENTRYP PFNGLMULTITEXCOORD1DVPROC)(const GLenum target, const GLdouble* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD1FPROC)(const GLenum target, const GLfloat s);
typedef void (APIENTRYP PFNGLMULTITEXCOORD1FVPROC)(const GLenum target, const GLfloat* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD1IPROC)(const GLenum target, const GLint s);
typedef void (APIENTRYP PFNGLMULTITEXCOORD1IVPROC)(const GLenum target, const GLint* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD1SPROC)(const GLenum target, const GLshort s);
typedef void (APIENTRYP PFNGLMULTITEXCOORD1SVPROC)(const GLenum target, const GLshort* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD2DPROC)(const GLenum target, const GLdouble s, const GLdouble t);
typedef void (APIENTRYP PFNGLMULTITEXCOORD2DVPROC)(const GLenum target, const GLdouble* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD2FPROC)(const GLenum target, const GLfloat s, const GLfloat t);
typedef void (APIENTRYP PFNGLMULTITEXCOORD2FVPROC)(const GLenum target, const GLfloat* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD2IPROC)(const GLenum target, const GLint s, const GLint t);
typedef void (APIENTRYP PFNGLMULTITEXCOORD2IVPROC)(const GLenum target, const GLint* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD2SPROC)(const GLenum target, const GLshort s, const GLshort t);
typedef void (APIENTRYP PFNGLMULTITEXCOORD2SVPROC)(const GLenum target, const GLshort* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD3DPROC)(const GLenum target, const GLdouble s, const GLdouble t, const GLdouble r);
typedef void (APIENTRYP PFNGLMULTITEXCOORD3DVPROC)(const GLenum target, const GLdouble* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD3FPROC)(const GLenum target, const GLfloat s, const GLfloat t, const GLfloat r);
typedef void (APIENTRYP PFNGLMULTITEXCOORD3FVPROC)(const GLenum target, const GLfloat* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD3IPROC)(const GLenum target, const GLint s, const GLint t, const GLint r);
typedef void (APIENTRYP PFNGLMULTITEXCOORD3IVPROC)(const GLenum target, const GLint* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD3SPROC)(const GLenum target, const GLshort s, const GLshort t, const GLshort r);
typedef void (APIENTRYP PFNGLMULTITEXCOORD3SVPROC)(const GLenum target, const GLshort* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD4DPROC)(const GLenum target, const GLdouble s, const GLdouble t, const GLdouble r, const GLdouble q);
typedef void (APIENTRYP PFNGLMULTITEXCOORD4DVPROC)(const GLenum target, const GLdouble* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD4FPROC)(const GLenum target, const GLfloat s, const GLfloat t, const GLfloat r, const GLfloat q);
typedef void (APIENTRYP PFNGLMULTITEXCOORD4FVPROC)(const GLenum target, const GLfloat* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD4IPROC)(const GLenum target, const GLint s, const GLint t, const GLint r, const GLint q);
typedef void (APIENTRYP PFNGLMULTITEXCOORD4IVPROC)(const GLenum target, const GLint* v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD4SPROC)(const GLenum target, const GLshort s, const GLshort t, const GLshort r, const GLshort q);
typedef void (APIENTRYP PFNGLMULTITEXCOORD4SVPROC)(const GLenum target, const GLshort* v);
typedef void (APIENTRYP PFNGLLOADTRANSPOSEMATRIXFPROC)(const GLfloat* m);
typedef void (APIENTRYP PFNGLLOADTRANSPOSEMATRIXDPROC)(const GLdouble* m);
typedef void (APIENTRYP PFNGLMULTTRANSPOSEMATRIXFPROC)(const GLfloat* m);
typedef void (APIENTRYP PFNGLMULTTRANSPOSEMATRIXDPROC)(const GLdouble* m);

GLAPI PFNGLCLIENTACTIVETEXTUREPROC glpfClientActiveTexture;
GLAPI PFNGLMULTITEXCOORD1DPROC glpfMultiTexCoord1d;
GLAPI PFNGLMULTITEXCOORD1DVPROC glpfMultiTexCoord1dv;
GLAPI PFNGLMULTITEXCOORD1FPROC glpfMultiTexCoord1f;
GLAPI PFNGLMULTITEXCOORD1FVPROC glpfMultiTexCoord1fv;
GLAPI PFNGLMULTITEXCOORD1IPROC glpfMultiTexCoord1i;
GLAPI PFNGLMULTITEXCOORD1IVPROC glpfMultiTexCoord1iv;
GLAPI PFNGLMULTITEXCOORD1SPROC glpfMultiTexCoord1s;
GLAPI PFNGLMULTITEXCOORD1SVPROC glpfMultiTexCoord1sv;
GLAPI PFNGLMULTITEXCOORD2DPROC glpfMultiTexCoord2d;
GLAPI PFNGLMULTITEXCOORD2DVPROC glpfMultiTexCoord2dv;
GLAPI PFNGLMULTITEXCOORD2FPROC glpfMultiTexCoord2f;
GLAPI PFNGLMULTITEXCOORD2FVPROC glpfMultiTexCoord2fv;
GLAPI PFNGLMULTITEXCOORD2IPROC glpfMultiTexCoord2i;
GLAPI PFNGLMULTITEXCOORD2IVPROC glpfMultiTexCoord2iv;
GLAPI PFNGLMULTITEXCOORD2SPROC glpfMultiTexCoord2s;
GLAPI PFNGLMULTITEXCOORD2SVPROC glpfMultiTexCoord2sv;
GLAPI PFNGLMULTITEXCOORD3DPROC glpfMultiTexCoord3d;
GLAPI PFNGLMULTITEXCOORD3DVPROC glpfMultiTexCoord3dv;
GLAPI PFNGLMULTITEXCOORD3FPROC glpfMultiTexCoord3f;
GLAPI PFNGLMULTITEXCOORD3FVPROC glpfMultiTexCoord3fv;
GLAPI PFNGLMULTITEXCOORD3IPROC glpfMultiTexCoord3i;
GLAPI PFNGLMULTITEXCOORD3IVPROC glpfMultiTexCoord3iv;
GLAPI PFNGLMULTITEXCOORD3SPROC glpfMultiTexCoord3s;
GLAPI PFNGLMULTITEXCOORD3SVPROC glpfMultiTexCoord3sv;
GLAPI PFNGLMULTITEXCOORD4DPROC glpfMultiTexCoord4d;
GLAPI PFNGLMULTITEXCOORD4DVPROC glpfMultiTexCoord4dv;
GLAPI PFNGLMULTITEXCOORD4FPROC glpfMultiTexCoord4f;
GLAPI PFNGLMULTITEXCOORD4FVPROC glpfMultiTexCoord4fv;
GLAPI PFNGLMULTITEXCOORD4IPROC glpfMultiTexCoord4i;
GLAPI PFNGLMULTITEXCOORD4IVPROC glpfMultiTexCoord4iv;
GLAPI PFNGLMULTITEXCOORD4SPROC glpfMultiTexCoord4s;
GLAPI PFNGLMULTITEXCOORD4SVPROC glpfMultiTexCoord4sv;
GLAPI PFNGLLOADTRANSPOSEMATRIXFPROC glpfLoadTransposeMatrixf;
GLAPI PFNGLLOADTRANSPOSEMATRIXDPROC glpfLoadTransposeMatrixd;
GLAPI PFNGLMULTTRANSPOSEMATRIXFPROC glpfMultTransposeMatrixf;
GLAPI PFNGLMULTTRANSPOSEMATRIXDPROC glpfMultTransposeMatrixd;

#define glClientActiveTexture glpfClientActiveTexture
#define glMultiTexCoord1d glpfMultiTexCoord1d
#define glMultiTexCoord1dv glpfMultiTexCoord1dv
#define glMultiTexCoord1f glpfMultiTexCoord1f
#define glMultiTexCoord1fv glpfMultiTexCoord1fv
#define glMultiTexCoord1i glpfMultiTexCoord1i
#define glMultiTexCoord1iv glpfMultiTexCoord1iv
#define glMultiTexCoord1s glpfMultiTexCoord1s
#define glMultiTexCoord1sv glpfMultiTexCoord1sv
#define glMultiTexCoord2d glpfMultiTexCoord2d
#define glMultiTexCoord2dv glpfMultiTexCoord2dv
#define glMultiTexCoord2f glpfMultiTexCoord2f
#define glMultiTexCoord2fv glpfMultiTexCoord2fv
#define glMultiTexCoord2i glpfMultiTexCoord2i
#define glMultiTexCoord2iv glpfMultiTexCoord2iv
#define glMultiTexCoord2s glpfMultiTexCoord2s
#define glMultiTexCoord2sv glpfMultiTexCoord2sv
#define glMultiTexCoord3d glpfMultiTexCoord3d
#define glMultiTexCoord3dv glpfMultiTexCoord3dv
#define glMultiTexCoord3f glpfMultiTexCoord3f
#define glMultiTexCoord3fv glpfMultiTexCoord3fv
#define glMultiTexCoord3i glpfMultiTexCoord3i
#define glMultiTexCoord3iv glpfMultiTexCoord3iv
#define glMultiTexCoord3s glpfMultiTexCoord3s
#define glMultiTexCoord3sv glpfMultiTexCoord3sv
#define glMultiTexCoord4d glpfMultiTexCoord4d
#define glMultiTexCoord4dv glpfMultiTexCoord4dv
#define glMultiTexCoord4f glpfMultiTexCoord4f
#define glMultiTexCoord4fv glpfMultiTexCoord4fv
#define glMultiTexCoord4i glpfMultiTexCoord4i
#define glMultiTexCoord4iv glpfMultiTexCoord4iv
#define glMultiTexCoord4s glpfMultiTexCoord4s
#define glMultiTexCoord4sv glpfMultiTexCoord4sv
#define glLoadTransposeMatrixf glpfLoadTransposeMatrixf
#define glLoadTransposeMatrixd glpfLoadTransposeMatrixd
#define glMultTransposeMatrixf glpfMultTransposeMatrixf
#define glMultTransposeMatrixd glpfMultTransposeMatrixd

// VERSION_1_4

typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEPROC)(const GLenum sfactorRGB, const GLenum dfactorRGB, const GLenum sfactorAlpha, const GLenum dfactorAlpha);
typedef void (APIENTRYP PFNGLMULTIDRAWARRAYSPROC)(const GLenum mode, const GLint* first, const GLsizei* count, const GLsizei primcount);
typedef void (APIENTRYP PFNGLMULTIDRAWELEMENTSPROC)(const GLenum mode, const GLsizei* count, const GLenum type, const GLvoid** indices, const GLsizei primcount);
typedef void (APIENTRYP PFNGLPOINTPARAMETERFPROC)(const GLenum pname, const GLfloat param);
typedef void (APIENTRYP PFNGLPOINTPARAMETERFVPROC)(const GLenum pname, const GLfloat* params);
typedef void (APIENTRYP PFNGLPOINTPARAMETERIPROC)(const GLenum pname, const GLint param);
typedef void (APIENTRYP PFNGLPOINTPARAMETERIVPROC)(const GLenum pname, const GLint* params);

GLAPI PFNGLBLENDFUNCSEPARATEPROC glpfBlendFuncSeparate;
GLAPI PFNGLMULTIDRAWARRAYSPROC glpfMultiDrawArrays;
GLAPI PFNGLMULTIDRAWELEMENTSPROC glpfMultiDrawElements;
GLAPI PFNGLPOINTPARAMETERFPROC glpfPointParameterf;
GLAPI PFNGLPOINTPARAMETERFVPROC glpfPointParameterfv;
GLAPI PFNGLPOINTPARAMETERIPROC glpfPointParameteri;
GLAPI PFNGLPOINTPARAMETERIVPROC glpfPointParameteriv;

#define glBlendFuncSeparate glpfBlendFuncSeparate
#define glMultiDrawArrays glpfMultiDrawArrays
#define glMultiDrawElements glpfMultiDrawElements
#define glPointParameterf glpfPointParameterf
#define glPointParameterfv glpfPointParameterfv
#define glPointParameteri glpfPointParameteri
#define glPointParameteriv glpfPointParameteriv

// VERSION_1_4_DEPRECATED

typedef void (APIENTRYP PFNGLFOGCOORDFPROC)(const GLfloat coord);
typedef void (APIENTRYP PFNGLFOGCOORDFVPROC)(const GLfloat* coord);
typedef void (APIENTRYP PFNGLFOGCOORDDPROC)(const GLdouble coord);
typedef void (APIENTRYP PFNGLFOGCOORDDVPROC)(const GLdouble* coord);
typedef void (APIENTRYP PFNGLFOGCOORDPOINTERPROC)(const GLenum type, const GLsizei stride, const GLvoid* pointer);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3BPROC)(const GLbyte red, const GLbyte green, const GLbyte blue);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3BVPROC)(const GLbyte* v);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3DPROC)(const GLdouble red, const GLdouble green, const GLdouble blue);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3DVPROC)(const GLdouble* v);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3FPROC)(const GLfloat red, const GLfloat green, const GLfloat blue);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3FVPROC)(const GLfloat* v);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3IPROC)(const GLint red, const GLint green, const GLint blue);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3IVPROC)(const GLint* v);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3SPROC)(const GLshort red, const GLshort green, const GLshort blue);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3SVPROC)(const GLshort* v);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3UBPROC)(const GLubyte red, const GLubyte green, const GLubyte blue);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3UBVPROC)(const GLubyte* v);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3UIPROC)(const GLuint red, const GLuint green, const GLuint blue);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3UIVPROC)(const GLuint* v);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3USPROC)(const GLushort red, const GLushort green, const GLushort blue);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3USVPROC)(const GLushort* v);
typedef void (APIENTRYP PFNGLSECONDARYCOLORPOINTERPROC)(const GLint size, const GLenum type, const GLsizei stride, const GLvoid* pointer);
typedef void (APIENTRYP PFNGLWINDOWPOS2DPROC)(const GLdouble x, const GLdouble y);
typedef void (APIENTRYP PFNGLWINDOWPOS2DVPROC)(const GLdouble* v);
typedef void (APIENTRYP PFNGLWINDOWPOS2FPROC)(const GLfloat x, const GLfloat y);
typedef void (APIENTRYP PFNGLWINDOWPOS2FVPROC)(const GLfloat* v);
typedef void (APIENTRYP PFNGLWINDOWPOS2IPROC)(const GLint x, const GLint y);
typedef void (APIENTRYP PFNGLWINDOWPOS2IVPROC)(const GLint* v);
typedef void (APIENTRYP PFNGLWINDOWPOS2SPROC)(const GLshort x, const GLshort y);
typedef void (APIENTRYP PFNGLWINDOWPOS2SVPROC)(const GLshort* v);
typedef void (APIENTRYP PFNGLWINDOWPOS3DPROC)(const GLdouble x, const GLdouble y, const GLdouble z);
typedef void (APIENTRYP PFNGLWINDOWPOS3DVPROC)(const GLdouble* v);
typedef void (APIENTRYP PFNGLWINDOWPOS3FPROC)(const GLfloat x, const GLfloat y, const GLfloat z);
typedef void (APIENTRYP PFNGLWINDOWPOS3FVPROC)(const GLfloat* v);
typedef void (APIENTRYP PFNGLWINDOWPOS3IPROC)(const GLint x, const GLint y, const GLint z);
typedef void (APIENTRYP PFNGLWINDOWPOS3IVPROC)(const GLint* v);
typedef void (APIENTRYP PFNGLWINDOWPOS3SPROC)(const GLshort x, const GLshort y, const GLshort z);
typedef void (APIENTRYP PFNGLWINDOWPOS3SVPROC)(const GLshort* v);

GLAPI PFNGLFOGCOORDFPROC glpfFogCoordf;
GLAPI PFNGLFOGCOORDFVPROC glpfFogCoordfv;
GLAPI PFNGLFOGCOORDDPROC glpfFogCoordd;
GLAPI PFNGLFOGCOORDDVPROC glpfFogCoorddv;
GLAPI PFNGLFOGCOORDPOINTERPROC glpfFogCoordPointer;
GLAPI PFNGLSECONDARYCOLOR3BPROC glpfSecondaryColor3b;
GLAPI PFNGLSECONDARYCOLOR3BVPROC glpfSecondaryColor3bv;
GLAPI PFNGLSECONDARYCOLOR3DPROC glpfSecondaryColor3d;
GLAPI PFNGLSECONDARYCOLOR3DVPROC glpfSecondaryColor3dv;
GLAPI PFNGLSECONDARYCOLOR3FPROC glpfSecondaryColor3f;
GLAPI PFNGLSECONDARYCOLOR3FVPROC glpfSecondaryColor3fv;
GLAPI PFNGLSECONDARYCOLOR3IPROC glpfSecondaryColor3i;
GLAPI PFNGLSECONDARYCOLOR3IVPROC glpfSecondaryColor3iv;
GLAPI PFNGLSECONDARYCOLOR3SPROC glpfSecondaryColor3s;
GLAPI PFNGLSECONDARYCOLOR3SVPROC glpfSecondaryColor3sv;
GLAPI PFNGLSECONDARYCOLOR3UBPROC glpfSecondaryColor3ub;
GLAPI PFNGLSECONDARYCOLOR3UBVPROC glpfSecondaryColor3ubv;
GLAPI PFNGLSECONDARYCOLOR3UIPROC glpfSecondaryColor3ui;
GLAPI PFNGLSECONDARYCOLOR3UIVPROC glpfSecondaryColor3uiv;
GLAPI PFNGLSECONDARYCOLOR3USPROC glpfSecondaryColor3us;
GLAPI PFNGLSECONDARYCOLOR3USVPROC glpfSecondaryColor3usv;
GLAPI PFNGLSECONDARYCOLORPOINTERPROC glpfSecondaryColorPointer;
GLAPI PFNGLWINDOWPOS2DPROC glpfWindowPos2d;
GLAPI PFNGLWINDOWPOS2DVPROC glpfWindowPos2dv;
GLAPI PFNGLWINDOWPOS2FPROC glpfWindowPos2f;
GLAPI PFNGLWINDOWPOS2FVPROC glpfWindowPos2fv;
GLAPI PFNGLWINDOWPOS2IPROC glpfWindowPos2i;
GLAPI PFNGLWINDOWPOS2IVPROC glpfWindowPos2iv;
GLAPI PFNGLWINDOWPOS2SPROC glpfWindowPos2s;
GLAPI PFNGLWINDOWPOS2SVPROC glpfWindowPos2sv;
GLAPI PFNGLWINDOWPOS3DPROC glpfWindowPos3d;
GLAPI PFNGLWINDOWPOS3DVPROC glpfWindowPos3dv;
GLAPI PFNGLWINDOWPOS3FPROC glpfWindowPos3f;
GLAPI PFNGLWINDOWPOS3FVPROC glpfWindowPos3fv;
GLAPI PFNGLWINDOWPOS3IPROC glpfWindowPos3i;
GLAPI PFNGLWINDOWPOS3IVPROC glpfWindowPos3iv;
GLAPI PFNGLWINDOWPOS3SPROC glpfWindowPos3s;
GLAPI PFNGLWINDOWPOS3SVPROC glpfWindowPos3sv;

#define glFogCoordf glpfFogCoordf
#define glFogCoordfv glpfFogCoordfv
#define glFogCoordd glpfFogCoordd
#define glFogCoorddv glpfFogCoorddv
#define glFogCoordPointer glpfFogCoordPointer
#define glSecondaryColor3b glpfSecondaryColor3b
#define glSecondaryColor3bv glpfSecondaryColor3bv
#define glSecondaryColor3d glpfSecondaryColor3d
#define glSecondaryColor3dv glpfSecondaryColor3dv
#define glSecondaryColor3f glpfSecondaryColor3f
#define glSecondaryColor3fv glpfSecondaryColor3fv
#define glSecondaryColor3i glpfSecondaryColor3i
#define glSecondaryColor3iv glpfSecondaryColor3iv
#define glSecondaryColor3s glpfSecondaryColor3s
#define glSecondaryColor3sv glpfSecondaryColor3sv
#define glSecondaryColor3ub glpfSecondaryColor3ub
#define glSecondaryColor3ubv glpfSecondaryColor3ubv
#define glSecondaryColor3ui glpfSecondaryColor3ui
#define glSecondaryColor3uiv glpfSecondaryColor3uiv
#define glSecondaryColor3us glpfSecondaryColor3us
#define glSecondaryColor3usv glpfSecondaryColor3usv
#define glSecondaryColorPointer glpfSecondaryColorPointer
#define glWindowPos2d glpfWindowPos2d
#define glWindowPos2dv glpfWindowPos2dv
#define glWindowPos2f glpfWindowPos2f
#define glWindowPos2fv glpfWindowPos2fv
#define glWindowPos2i glpfWindowPos2i
#define glWindowPos2iv glpfWindowPos2iv
#define glWindowPos2s glpfWindowPos2s
#define glWindowPos2sv glpfWindowPos2sv
#define glWindowPos3d glpfWindowPos3d
#define glWindowPos3dv glpfWindowPos3dv
#define glWindowPos3f glpfWindowPos3f
#define glWindowPos3fv glpfWindowPos3fv
#define glWindowPos3i glpfWindowPos3i
#define glWindowPos3iv glpfWindowPos3iv
#define glWindowPos3s glpfWindowPos3s
#define glWindowPos3sv glpfWindowPos3sv

// VERSION_1_5

typedef void (APIENTRYP PFNGLGENQUERIESPROC)(const GLsizei n, GLuint* ids);
typedef void (APIENTRYP PFNGLDELETEQUERIESPROC)(const GLsizei n, const GLuint* ids);
typedef GLboolean (APIENTRYP PFNGLISQUERYPROC)(const GLuint id);
typedef void (APIENTRYP PFNGLBEGINQUERYPROC)(const GLenum target, const GLuint id);
typedef void (APIENTRYP PFNGLENDQUERYPROC)(const GLenum target);
typedef void (APIENTRYP PFNGLGETQUERYIVPROC)(const GLenum target, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETQUERYOBJECTIVPROC)(const GLuint id, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETQUERYOBJECTUIVPROC)(const GLuint id, const GLenum pname, GLuint* params);
typedef void (APIENTRYP PFNGLBINDBUFFERPROC)(const GLenum target, const GLuint buffer);
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC)(const GLsizei n, const GLuint* buffers);
typedef void (APIENTRYP PFNGLGENBUFFERSPROC)(const GLsizei n, GLuint* buffers);
typedef GLboolean (APIENTRYP PFNGLISBUFFERPROC)(const GLuint buffer);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC)(const GLenum target, const GLsizeiptr size, const GLvoid* data, const GLenum usage);
typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC)(const GLenum target, const GLintptr offset, const GLsizeiptr size, const GLvoid* data);
typedef void (APIENTRYP PFNGLGETBUFFERSUBDATAPROC)(const GLenum target, const GLintptr offset, const GLsizeiptr size, GLvoid* data);
typedef GLvoid* (APIENTRYP PFNGLMAPBUFFERPROC)(const GLenum target, const GLenum access);
typedef GLboolean (APIENTRYP PFNGLUNMAPBUFFERPROC)(const GLenum target);
typedef void (APIENTRYP PFNGLGETBUFFERPARAMETERIVPROC)(const GLenum target, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETBUFFERPOINTERVPROC)(const GLenum target, const GLenum pname, GLvoid** params);

GLAPI PFNGLGENQUERIESPROC glpfGenQueries;
GLAPI PFNGLDELETEQUERIESPROC glpfDeleteQueries;
GLAPI PFNGLISQUERYPROC glpfIsQuery;
GLAPI PFNGLBEGINQUERYPROC glpfBeginQuery;
GLAPI PFNGLENDQUERYPROC glpfEndQuery;
GLAPI PFNGLGETQUERYIVPROC glpfGetQueryiv;
GLAPI PFNGLGETQUERYOBJECTIVPROC glpfGetQueryObjectiv;
GLAPI PFNGLGETQUERYOBJECTUIVPROC glpfGetQueryObjectuiv;
GLAPI PFNGLBINDBUFFERPROC glpfBindBuffer;
GLAPI PFNGLDELETEBUFFERSPROC glpfDeleteBuffers;
GLAPI PFNGLGENBUFFERSPROC glpfGenBuffers;
GLAPI PFNGLISBUFFERPROC glpfIsBuffer;
GLAPI PFNGLBUFFERDATAPROC glpfBufferData;
GLAPI PFNGLBUFFERSUBDATAPROC glpfBufferSubData;
GLAPI PFNGLGETBUFFERSUBDATAPROC glpfGetBufferSubData;
GLAPI PFNGLMAPBUFFERPROC glpfMapBuffer;
GLAPI PFNGLUNMAPBUFFERPROC glpfUnmapBuffer;
GLAPI PFNGLGETBUFFERPARAMETERIVPROC glpfGetBufferParameteriv;
GLAPI PFNGLGETBUFFERPOINTERVPROC glpfGetBufferPointerv;

#define glGenQueries glpfGenQueries
#define glDeleteQueries glpfDeleteQueries
#define glIsQuery glpfIsQuery
#define glBeginQuery glpfBeginQuery
#define glEndQuery glpfEndQuery
#define glGetQueryiv glpfGetQueryiv
#define glGetQueryObjectiv glpfGetQueryObjectiv
#define glGetQueryObjectuiv glpfGetQueryObjectuiv
#define glBindBuffer glpfBindBuffer
#define glDeleteBuffers glpfDeleteBuffers
#define glGenBuffers glpfGenBuffers
#define glIsBuffer glpfIsBuffer
#define glBufferData glpfBufferData
#define glBufferSubData glpfBufferSubData
#define glGetBufferSubData glpfGetBufferSubData
#define glMapBuffer glpfMapBuffer
#define glUnmapBuffer glpfUnmapBuffer
#define glGetBufferParameteriv glpfGetBufferParameteriv
#define glGetBufferPointerv glpfGetBufferPointerv

// VERSION_2_0

typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEPROC)(const GLenum modeRGB, const GLenum modeAlpha);
typedef void (APIENTRYP PFNGLDRAWBUFFERSPROC)(const GLsizei n, const GLenum* bufs);
typedef void (APIENTRYP PFNGLSTENCILOPSEPARATEPROC)(const GLenum face, const GLenum sfail, const GLenum dpfail, const GLenum dppass);
typedef void (APIENTRYP PFNGLSTENCILFUNCSEPARATEPROC)(const GLenum face, const GLenum func, const GLint ref, const GLuint mask);
typedef void (APIENTRYP PFNGLSTENCILMASKSEPARATEPROC)(const GLenum face, const GLuint mask);
typedef void (APIENTRYP PFNGLATTACHSHADERPROC)(const GLuint program, const GLuint shader);
typedef void (APIENTRYP PFNGLBINDATTRIBLOCATIONPROC)(const GLuint program, const GLuint index, const GLchar* name);
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC)(const GLuint shader);
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC)();
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC)(const GLenum type);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC)(const GLuint program);
typedef void (APIENTRYP PFNGLDELETESHADERPROC)(const GLuint shader);
typedef void (APIENTRYP PFNGLDETACHSHADERPROC)(const GLuint program, const GLuint shader);
typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC)(const GLuint index);
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(const GLuint index);
typedef void (APIENTRYP PFNGLGETACTIVEATTRIBPROC)(const GLuint program, const GLuint index, const GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMPROC)(const GLuint program, const GLuint index, const GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef void (APIENTRYP PFNGLGETATTACHEDSHADERSPROC)(const GLuint program, const GLsizei maxCount, GLsizei* count, GLuint* obj);
typedef GLint (APIENTRYP PFNGLGETATTRIBLOCATIONPROC)(const GLuint program, const GLchar* name);
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC)(const GLuint program, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC)(const GLuint program, const GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (APIENTRYP PFNGLGETSHADERIVPROC)(const GLuint shader, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC)(const GLuint shader, const GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (APIENTRYP PFNGLGETSHADERSOURCEPROC)(const GLuint shader, const GLsizei bufSize, GLsizei* length, GLchar* source);
typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(const GLuint program, const GLchar* name);
typedef void (APIENTRYP PFNGLGETUNIFORMFVPROC)(const GLuint program, const GLint location, GLfloat* params);
typedef void (APIENTRYP PFNGLGETUNIFORMIVPROC)(const GLuint program, const GLint location, GLint* params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBDVPROC)(const GLuint index, const GLenum pname, GLdouble* params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBFVPROC)(const GLuint index, const GLenum pname, GLfloat* params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIVPROC)(const GLuint index, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBPOINTERVPROC)(const GLuint index, const GLenum pname, GLvoid** pointer);
typedef GLboolean (APIENTRYP PFNGLISPROGRAMPROC)(const GLuint program);
typedef GLboolean (APIENTRYP PFNGLISSHADERPROC)(const GLuint shader);
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC)(const GLuint program);
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC)(const GLuint shader, const GLsizei count, const GLchar** string, const GLint* length);
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC)(const GLuint program);
typedef void (APIENTRYP PFNGLUNIFORM1FPROC)(const GLint location, const GLfloat v0);
typedef void (APIENTRYP PFNGLUNIFORM2FPROC)(const GLint location, const GLfloat v0, const GLfloat v1);
typedef void (APIENTRYP PFNGLUNIFORM3FPROC)(const GLint location, const GLfloat v0, const GLfloat v1, const GLfloat v2);
typedef void (APIENTRYP PFNGLUNIFORM4FPROC)(const GLint location, const GLfloat v0, const GLfloat v1, const GLfloat v2, const GLfloat v3);
typedef void (APIENTRYP PFNGLUNIFORM1IPROC)(const GLint location, const GLint v0);
typedef void (APIENTRYP PFNGLUNIFORM2IPROC)(const GLint location, const GLint v0, const GLint v1);
typedef void (APIENTRYP PFNGLUNIFORM3IPROC)(const GLint location, const GLint v0, const GLint v1, const GLint v2);
typedef void (APIENTRYP PFNGLUNIFORM4IPROC)(const GLint location, const GLint v0, const GLint v1, const GLint v2, const GLint v3);
typedef void (APIENTRYP PFNGLUNIFORM1FVPROC)(const GLint location, const GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM2FVPROC)(const GLint location, const GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM3FVPROC)(const GLint location, const GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM4FVPROC)(const GLint location, const GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM1IVPROC)(const GLint location, const GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORM2IVPROC)(const GLint location, const GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORM3IVPROC)(const GLint location, const GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORM4IVPROC)(const GLint location, const GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2FVPROC)(const GLint location, const GLsizei count, const GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3FVPROC)(const GLint location, const GLsizei count, const GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)(const GLint location, const GLsizei count, const GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPROC)(const GLuint program);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1DPROC)(const GLuint index, const GLdouble x);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1DVPROC)(const GLuint index, const GLdouble* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1FPROC)(const GLuint index, const GLfloat x);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1FVPROC)(const GLuint index, const GLfloat* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1SPROC)(const GLuint index, const GLshort x);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1SVPROC)(const GLuint index, const GLshort* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2DPROC)(const GLuint index, const GLdouble x, const GLdouble y);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2DVPROC)(const GLuint index, const GLdouble* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2FPROC)(const GLuint index, const GLfloat x, const GLfloat y);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2FVPROC)(const GLuint index, const GLfloat* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2SPROC)(const GLuint index, const GLshort x, const GLshort y);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2SVPROC)(const GLuint index, const GLshort* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3DPROC)(const GLuint index, const GLdouble x, const GLdouble y, const GLdouble z);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3DVPROC)(const GLuint index, const GLdouble* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3FPROC)(const GLuint index, const GLfloat x, const GLfloat y, const GLfloat z);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3FVPROC)(const GLuint index, const GLfloat* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3SPROC)(const GLuint index, const GLshort x, const GLshort y, const GLshort z);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3SVPROC)(const GLuint index, const GLshort* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NBVPROC)(const GLuint index, const GLbyte* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NIVPROC)(const GLuint index, const GLint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NSVPROC)(const GLuint index, const GLshort* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NUBPROC)(const GLuint index, const GLubyte x, const GLubyte y, const GLubyte z, const GLubyte w);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NUBVPROC)(const GLuint index, const GLubyte* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NUIVPROC)(const GLuint index, const GLuint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NUSVPROC)(const GLuint index, const GLushort* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4BVPROC)(const GLuint index, const GLbyte* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4DPROC)(const GLuint index, const GLdouble x, const GLdouble y, const GLdouble z, const GLdouble w);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4DVPROC)(const GLuint index, const GLdouble* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4FPROC)(const GLuint index, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4FVPROC)(const GLuint index, const GLfloat* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4IVPROC)(const GLuint index, const GLint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4SPROC)(const GLuint index, const GLshort x, const GLshort y, const GLshort z, const GLshort w);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4SVPROC)(const GLuint index, const GLshort* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4UBVPROC)(const GLuint index, const GLubyte* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4UIVPROC)(const GLuint index, const GLuint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4USVPROC)(const GLuint index, const GLushort* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(const GLuint index, const GLint size, const GLenum type, const GLboolean normalized, const GLsizei stride, const GLvoid* pointer);

GLAPI PFNGLBLENDEQUATIONSEPARATEPROC glpfBlendEquationSeparate;
GLAPI PFNGLDRAWBUFFERSPROC glpfDrawBuffers;
GLAPI PFNGLSTENCILOPSEPARATEPROC glpfStencilOpSeparate;
GLAPI PFNGLSTENCILFUNCSEPARATEPROC glpfStencilFuncSeparate;
GLAPI PFNGLSTENCILMASKSEPARATEPROC glpfStencilMaskSeparate;
GLAPI PFNGLATTACHSHADERPROC glpfAttachShader;
GLAPI PFNGLBINDATTRIBLOCATIONPROC glpfBindAttribLocation;
GLAPI PFNGLCOMPILESHADERPROC glpfCompileShader;
GLAPI PFNGLCREATEPROGRAMPROC glpfCreateProgram;
GLAPI PFNGLCREATESHADERPROC glpfCreateShader;
GLAPI PFNGLDELETEPROGRAMPROC glpfDeleteProgram;
GLAPI PFNGLDELETESHADERPROC glpfDeleteShader;
GLAPI PFNGLDETACHSHADERPROC glpfDetachShader;
GLAPI PFNGLDISABLEVERTEXATTRIBARRAYPROC glpfDisableVertexAttribArray;
GLAPI PFNGLENABLEVERTEXATTRIBARRAYPROC glpfEnableVertexAttribArray;
GLAPI PFNGLGETACTIVEATTRIBPROC glpfGetActiveAttrib;
GLAPI PFNGLGETACTIVEUNIFORMPROC glpfGetActiveUniform;
GLAPI PFNGLGETATTACHEDSHADERSPROC glpfGetAttachedShaders;
GLAPI PFNGLGETATTRIBLOCATIONPROC glpfGetAttribLocation;
GLAPI PFNGLGETPROGRAMIVPROC glpfGetProgramiv;
GLAPI PFNGLGETPROGRAMINFOLOGPROC glpfGetProgramInfoLog;
GLAPI PFNGLGETSHADERIVPROC glpfGetShaderiv;
GLAPI PFNGLGETSHADERINFOLOGPROC glpfGetShaderInfoLog;
GLAPI PFNGLGETSHADERSOURCEPROC glpfGetShaderSource;
GLAPI PFNGLGETUNIFORMLOCATIONPROC glpfGetUniformLocation;
GLAPI PFNGLGETUNIFORMFVPROC glpfGetUniformfv;
GLAPI PFNGLGETUNIFORMIVPROC glpfGetUniformiv;
GLAPI PFNGLGETVERTEXATTRIBDVPROC glpfGetVertexAttribdv;
GLAPI PFNGLGETVERTEXATTRIBFVPROC glpfGetVertexAttribfv;
GLAPI PFNGLGETVERTEXATTRIBIVPROC glpfGetVertexAttribiv;
GLAPI PFNGLGETVERTEXATTRIBPOINTERVPROC glpfGetVertexAttribPointerv;
GLAPI PFNGLISPROGRAMPROC glpfIsProgram;
GLAPI PFNGLISSHADERPROC glpfIsShader;
GLAPI PFNGLLINKPROGRAMPROC glpfLinkProgram;
GLAPI PFNGLSHADERSOURCEPROC glpfShaderSource;
GLAPI PFNGLUSEPROGRAMPROC glpfUseProgram;
GLAPI PFNGLUNIFORM1FPROC glpfUniform1f;
GLAPI PFNGLUNIFORM2FPROC glpfUniform2f;
GLAPI PFNGLUNIFORM3FPROC glpfUniform3f;
GLAPI PFNGLUNIFORM4FPROC glpfUniform4f;
GLAPI PFNGLUNIFORM1IPROC glpfUniform1i;
GLAPI PFNGLUNIFORM2IPROC glpfUniform2i;
GLAPI PFNGLUNIFORM3IPROC glpfUniform3i;
GLAPI PFNGLUNIFORM4IPROC glpfUniform4i;
GLAPI PFNGLUNIFORM1FVPROC glpfUniform1fv;
GLAPI PFNGLUNIFORM2FVPROC glpfUniform2fv;
GLAPI PFNGLUNIFORM3FVPROC glpfUniform3fv;
GLAPI PFNGLUNIFORM4FVPROC glpfUniform4fv;
GLAPI PFNGLUNIFORM1IVPROC glpfUniform1iv;
GLAPI PFNGLUNIFORM2IVPROC glpfUniform2iv;
GLAPI PFNGLUNIFORM3IVPROC glpfUniform3iv;
GLAPI PFNGLUNIFORM4IVPROC glpfUniform4iv;
GLAPI PFNGLUNIFORMMATRIX2FVPROC glpfUniformMatrix2fv;
GLAPI PFNGLUNIFORMMATRIX3FVPROC glpfUniformMatrix3fv;
GLAPI PFNGLUNIFORMMATRIX4FVPROC glpfUniformMatrix4fv;
GLAPI PFNGLVALIDATEPROGRAMPROC glpfValidateProgram;
GLAPI PFNGLVERTEXATTRIB1DPROC glpfVertexAttrib1d;
GLAPI PFNGLVERTEXATTRIB1DVPROC glpfVertexAttrib1dv;
GLAPI PFNGLVERTEXATTRIB1FPROC glpfVertexAttrib1f;
GLAPI PFNGLVERTEXATTRIB1FVPROC glpfVertexAttrib1fv;
GLAPI PFNGLVERTEXATTRIB1SPROC glpfVertexAttrib1s;
GLAPI PFNGLVERTEXATTRIB1SVPROC glpfVertexAttrib1sv;
GLAPI PFNGLVERTEXATTRIB2DPROC glpfVertexAttrib2d;
GLAPI PFNGLVERTEXATTRIB2DVPROC glpfVertexAttrib2dv;
GLAPI PFNGLVERTEXATTRIB2FPROC glpfVertexAttrib2f;
GLAPI PFNGLVERTEXATTRIB2FVPROC glpfVertexAttrib2fv;
GLAPI PFNGLVERTEXATTRIB2SPROC glpfVertexAttrib2s;
GLAPI PFNGLVERTEXATTRIB2SVPROC glpfVertexAttrib2sv;
GLAPI PFNGLVERTEXATTRIB3DPROC glpfVertexAttrib3d;
GLAPI PFNGLVERTEXATTRIB3DVPROC glpfVertexAttrib3dv;
GLAPI PFNGLVERTEXATTRIB3FPROC glpfVertexAttrib3f;
GLAPI PFNGLVERTEXATTRIB3FVPROC glpfVertexAttrib3fv;
GLAPI PFNGLVERTEXATTRIB3SPROC glpfVertexAttrib3s;
GLAPI PFNGLVERTEXATTRIB3SVPROC glpfVertexAttrib3sv;
GLAPI PFNGLVERTEXATTRIB4NBVPROC glpfVertexAttrib4Nbv;
GLAPI PFNGLVERTEXATTRIB4NIVPROC glpfVertexAttrib4Niv;
GLAPI PFNGLVERTEXATTRIB4NSVPROC glpfVertexAttrib4Nsv;
GLAPI PFNGLVERTEXATTRIB4NUBPROC glpfVertexAttrib4Nub;
GLAPI PFNGLVERTEXATTRIB4NUBVPROC glpfVertexAttrib4Nubv;
GLAPI PFNGLVERTEXATTRIB4NUIVPROC glpfVertexAttrib4Nuiv;
GLAPI PFNGLVERTEXATTRIB4NUSVPROC glpfVertexAttrib4Nusv;
GLAPI PFNGLVERTEXATTRIB4BVPROC glpfVertexAttrib4bv;
GLAPI PFNGLVERTEXATTRIB4DPROC glpfVertexAttrib4d;
GLAPI PFNGLVERTEXATTRIB4DVPROC glpfVertexAttrib4dv;
GLAPI PFNGLVERTEXATTRIB4FPROC glpfVertexAttrib4f;
GLAPI PFNGLVERTEXATTRIB4FVPROC glpfVertexAttrib4fv;
GLAPI PFNGLVERTEXATTRIB4IVPROC glpfVertexAttrib4iv;
GLAPI PFNGLVERTEXATTRIB4SPROC glpfVertexAttrib4s;
GLAPI PFNGLVERTEXATTRIB4SVPROC glpfVertexAttrib4sv;
GLAPI PFNGLVERTEXATTRIB4UBVPROC glpfVertexAttrib4ubv;
GLAPI PFNGLVERTEXATTRIB4UIVPROC glpfVertexAttrib4uiv;
GLAPI PFNGLVERTEXATTRIB4USVPROC glpfVertexAttrib4usv;
GLAPI PFNGLVERTEXATTRIBPOINTERPROC glpfVertexAttribPointer;

#define glBlendEquationSeparate glpfBlendEquationSeparate
#define glDrawBuffers glpfDrawBuffers
#define glStencilOpSeparate glpfStencilOpSeparate
#define glStencilFuncSeparate glpfStencilFuncSeparate
#define glStencilMaskSeparate glpfStencilMaskSeparate
#define glAttachShader glpfAttachShader
#define glBindAttribLocation glpfBindAttribLocation
#define glCompileShader glpfCompileShader
#define glCreateProgram glpfCreateProgram
#define glCreateShader glpfCreateShader
#define glDeleteProgram glpfDeleteProgram
#define glDeleteShader glpfDeleteShader
#define glDetachShader glpfDetachShader
#define glDisableVertexAttribArray glpfDisableVertexAttribArray
#define glEnableVertexAttribArray glpfEnableVertexAttribArray
#define glGetActiveAttrib glpfGetActiveAttrib
#define glGetActiveUniform glpfGetActiveUniform
#define glGetAttachedShaders glpfGetAttachedShaders
#define glGetAttribLocation glpfGetAttribLocation
#define glGetProgramiv glpfGetProgramiv
#define glGetProgramInfoLog glpfGetProgramInfoLog
#define glGetShaderiv glpfGetShaderiv
#define glGetShaderInfoLog glpfGetShaderInfoLog
#define glGetShaderSource glpfGetShaderSource
#define glGetUniformLocation glpfGetUniformLocation
#define glGetUniformfv glpfGetUniformfv
#define glGetUniformiv glpfGetUniformiv
#define glGetVertexAttribdv glpfGetVertexAttribdv
#define glGetVertexAttribfv glpfGetVertexAttribfv
#define glGetVertexAttribiv glpfGetVertexAttribiv
#define glGetVertexAttribPointerv glpfGetVertexAttribPointerv
#define glIsProgram glpfIsProgram
#define glIsShader glpfIsShader
#define glLinkProgram glpfLinkProgram
#define glShaderSource glpfShaderSource
#define glUseProgram glpfUseProgram
#define glUniform1f glpfUniform1f
#define glUniform2f glpfUniform2f
#define glUniform3f glpfUniform3f
#define glUniform4f glpfUniform4f
#define glUniform1i glpfUniform1i
#define glUniform2i glpfUniform2i
#define glUniform3i glpfUniform3i
#define glUniform4i glpfUniform4i
#define glUniform1fv glpfUniform1fv
#define glUniform2fv glpfUniform2fv
#define glUniform3fv glpfUniform3fv
#define glUniform4fv glpfUniform4fv
#define glUniform1iv glpfUniform1iv
#define glUniform2iv glpfUniform2iv
#define glUniform3iv glpfUniform3iv
#define glUniform4iv glpfUniform4iv
#define glUniformMatrix2fv glpfUniformMatrix2fv
#define glUniformMatrix3fv glpfUniformMatrix3fv
#define glUniformMatrix4fv glpfUniformMatrix4fv
#define glValidateProgram glpfValidateProgram
#define glVertexAttrib1d glpfVertexAttrib1d
#define glVertexAttrib1dv glpfVertexAttrib1dv
#define glVertexAttrib1f glpfVertexAttrib1f
#define glVertexAttrib1fv glpfVertexAttrib1fv
#define glVertexAttrib1s glpfVertexAttrib1s
#define glVertexAttrib1sv glpfVertexAttrib1sv
#define glVertexAttrib2d glpfVertexAttrib2d
#define glVertexAttrib2dv glpfVertexAttrib2dv
#define glVertexAttrib2f glpfVertexAttrib2f
#define glVertexAttrib2fv glpfVertexAttrib2fv
#define glVertexAttrib2s glpfVertexAttrib2s
#define glVertexAttrib2sv glpfVertexAttrib2sv
#define glVertexAttrib3d glpfVertexAttrib3d
#define glVertexAttrib3dv glpfVertexAttrib3dv
#define glVertexAttrib3f glpfVertexAttrib3f
#define glVertexAttrib3fv glpfVertexAttrib3fv
#define glVertexAttrib3s glpfVertexAttrib3s
#define glVertexAttrib3sv glpfVertexAttrib3sv
#define glVertexAttrib4Nbv glpfVertexAttrib4Nbv
#define glVertexAttrib4Niv glpfVertexAttrib4Niv
#define glVertexAttrib4Nsv glpfVertexAttrib4Nsv
#define glVertexAttrib4Nub glpfVertexAttrib4Nub
#define glVertexAttrib4Nubv glpfVertexAttrib4Nubv
#define glVertexAttrib4Nuiv glpfVertexAttrib4Nuiv
#define glVertexAttrib4Nusv glpfVertexAttrib4Nusv
#define glVertexAttrib4bv glpfVertexAttrib4bv
#define glVertexAttrib4d glpfVertexAttrib4d
#define glVertexAttrib4dv glpfVertexAttrib4dv
#define glVertexAttrib4f glpfVertexAttrib4f
#define glVertexAttrib4fv glpfVertexAttrib4fv
#define glVertexAttrib4iv glpfVertexAttrib4iv
#define glVertexAttrib4s glpfVertexAttrib4s
#define glVertexAttrib4sv glpfVertexAttrib4sv
#define glVertexAttrib4ubv glpfVertexAttrib4ubv
#define glVertexAttrib4uiv glpfVertexAttrib4uiv
#define glVertexAttrib4usv glpfVertexAttrib4usv
#define glVertexAttribPointer glpfVertexAttribPointer

// VERSION_2_1

typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X3FVPROC)(const GLint location, const GLsizei count, const GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X2FVPROC)(const GLint location, const GLsizei count, const GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X4FVPROC)(const GLint location, const GLsizei count, const GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X2FVPROC)(const GLint location, const GLsizei count, const GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X4FVPROC)(const GLint location, const GLsizei count, const GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X3FVPROC)(const GLint location, const GLsizei count, const GLboolean transpose, const GLfloat* value);

GLAPI PFNGLUNIFORMMATRIX2X3FVPROC glpfUniformMatrix2x3fv;
GLAPI PFNGLUNIFORMMATRIX3X2FVPROC glpfUniformMatrix3x2fv;
GLAPI PFNGLUNIFORMMATRIX2X4FVPROC glpfUniformMatrix2x4fv;
GLAPI PFNGLUNIFORMMATRIX4X2FVPROC glpfUniformMatrix4x2fv;
GLAPI PFNGLUNIFORMMATRIX3X4FVPROC glpfUniformMatrix3x4fv;
GLAPI PFNGLUNIFORMMATRIX4X3FVPROC glpfUniformMatrix4x3fv;

#define glUniformMatrix2x3fv glpfUniformMatrix2x3fv
#define glUniformMatrix3x2fv glpfUniformMatrix3x2fv
#define glUniformMatrix2x4fv glpfUniformMatrix2x4fv
#define glUniformMatrix4x2fv glpfUniformMatrix4x2fv
#define glUniformMatrix3x4fv glpfUniformMatrix3x4fv
#define glUniformMatrix4x3fv glpfUniformMatrix4x3fv

// VERSION_3_0

typedef void (APIENTRYP PFNGLCOLORMASKIPROC)(const GLuint index, const GLboolean r, const GLboolean g, const GLboolean b, const GLboolean a);
typedef void (APIENTRYP PFNGLGETBOOLEANI_VPROC)(const GLenum target, const GLuint index, GLboolean* data);
typedef void (APIENTRYP PFNGLGETINTEGERI_VPROC)(const GLenum target, const GLuint index, GLint* data);
typedef void (APIENTRYP PFNGLENABLEIPROC)(const GLenum target, const GLuint index);
typedef void (APIENTRYP PFNGLDISABLEIPROC)(const GLenum target, const GLuint index);
typedef GLboolean (APIENTRYP PFNGLISENABLEDIPROC)(const GLenum target, const GLuint index);
typedef void (APIENTRYP PFNGLBEGINTRANSFORMFEEDBACKPROC)(const GLenum primitiveMode);
typedef void (APIENTRYP PFNGLENDTRANSFORMFEEDBACKPROC)();
typedef void (APIENTRYP PFNGLBINDBUFFERRANGEPROC)(const GLenum target, const GLuint index, const GLuint buffer, const GLintptr offset, const GLsizeiptr size);
typedef void (APIENTRYP PFNGLBINDBUFFERBASEPROC)(const GLenum target, const GLuint index, const GLuint buffer);
typedef void (APIENTRYP PFNGLTRANSFORMFEEDBACKVARYINGSPROC)(const GLuint program, const GLsizei count, const GLchar** varyings, const GLenum bufferMode);
typedef void (APIENTRYP PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)(const GLuint program, const GLuint index, const GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
typedef void (APIENTRYP PFNGLCLAMPCOLORPROC)(const GLenum target, const GLenum clamp);
typedef void (APIENTRYP PFNGLBEGINCONDITIONALRENDERPROC)(const GLuint id, const GLenum mode);
typedef void (APIENTRYP PFNGLENDCONDITIONALRENDERPROC)();
typedef void (APIENTRYP PFNGLVERTEXATTRIBIPOINTERPROC)(const GLuint index, const GLint size, const GLenum type, const GLsizei stride, const GLvoid* pointer);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIIVPROC)(const GLuint index, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIUIVPROC)(const GLuint index, const GLenum pname, GLuint* params);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1IPROC)(const GLuint index, const GLint x);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2IPROC)(const GLuint index, const GLint x, const GLint y);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3IPROC)(const GLuint index, const GLint x, const GLint y, const GLint z);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4IPROC)(const GLuint index, const GLint x, const GLint y, const GLint z, const GLint w);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1UIPROC)(const GLuint index, const GLuint x);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2UIPROC)(const GLuint index, const GLuint x, const GLuint y);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3UIPROC)(const GLuint index, const GLuint x, const GLuint y, const GLuint z);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UIPROC)(const GLuint index, const GLuint x, const GLuint y, const GLuint z, const GLuint w);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1IVPROC)(const GLuint index, const GLint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2IVPROC)(const GLuint index, const GLint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3IVPROC)(const GLuint index, const GLint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4IVPROC)(const GLuint index, const GLint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1UIVPROC)(const GLuint index, const GLuint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2UIVPROC)(const GLuint index, const GLuint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3UIVPROC)(const GLuint index, const GLuint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UIVPROC)(const GLuint index, const GLuint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4BVPROC)(const GLuint index, const GLbyte* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4SVPROC)(const GLuint index, const GLshort* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UBVPROC)(const GLuint index, const GLubyte* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4USVPROC)(const GLuint index, const GLushort* v);
typedef void (APIENTRYP PFNGLGETUNIFORMUIVPROC)(const GLuint program, const GLint location, GLuint* params);
typedef void (APIENTRYP PFNGLBINDFRAGDATALOCATIONPROC)(const GLuint program, const GLuint color, const GLchar* name);
typedef GLint (APIENTRYP PFNGLGETFRAGDATALOCATIONPROC)(const GLuint program, const GLchar* name);
typedef void (APIENTRYP PFNGLUNIFORM1UIPROC)(const GLint location, const GLuint v0);
typedef void (APIENTRYP PFNGLUNIFORM2UIPROC)(const GLint location, const GLuint v0, const GLuint v1);
typedef void (APIENTRYP PFNGLUNIFORM3UIPROC)(const GLint location, const GLuint v0, const GLuint v1, const GLuint v2);
typedef void (APIENTRYP PFNGLUNIFORM4UIPROC)(const GLint location, const GLuint v0, const GLuint v1, const GLuint v2, const GLuint v3);
typedef void (APIENTRYP PFNGLUNIFORM1UIVPROC)(const GLint location, const GLsizei count, const GLuint* value);
typedef void (APIENTRYP PFNGLUNIFORM2UIVPROC)(const GLint location, const GLsizei count, const GLuint* value);
typedef void (APIENTRYP PFNGLUNIFORM3UIVPROC)(const GLint location, const GLsizei count, const GLuint* value);
typedef void (APIENTRYP PFNGLUNIFORM4UIVPROC)(const GLint location, const GLsizei count, const GLuint* value);
typedef void (APIENTRYP PFNGLTEXPARAMETERIIVPROC)(const GLenum target, const GLenum pname, const GLint* params);
typedef void (APIENTRYP PFNGLTEXPARAMETERIUIVPROC)(const GLenum target, const GLenum pname, const GLuint* params);
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIIVPROC)(const GLenum target, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIUIVPROC)(const GLenum target, const GLenum pname, GLuint* params);
typedef void (APIENTRYP PFNGLCLEARBUFFERIVPROC)(const GLenum buffer, const GLint drawbuffer, const GLint* value);
typedef void (APIENTRYP PFNGLCLEARBUFFERUIVPROC)(const GLenum buffer, const GLint drawbuffer, const GLuint* value);
typedef void (APIENTRYP PFNGLCLEARBUFFERFVPROC)(const GLenum buffer, const GLint drawbuffer, const GLfloat* value);
typedef void (APIENTRYP PFNGLCLEARBUFFERFIPROC)(const GLenum buffer, const GLint drawbuffer, const GLfloat depth, const GLint stencil);
typedef const GLubyte * (APIENTRYP PFNGLGETSTRINGIPROC)(const GLenum name, const GLuint index);
typedef GLboolean (APIENTRYP PFNGLISRENDERBUFFERPROC)(const GLuint renderbuffer);
typedef void (APIENTRYP PFNGLBINDRENDERBUFFERPROC)(const GLenum target, const GLuint renderbuffer);
typedef void (APIENTRYP PFNGLDELETERENDERBUFFERSPROC)(const GLsizei n, const GLuint* renderbuffers);
typedef void (APIENTRYP PFNGLGENRENDERBUFFERSPROC)(const GLsizei n, GLuint* renderbuffers);
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC)(const GLenum target, const GLenum internalformat, const GLsizei width, const GLsizei height);
typedef void (APIENTRYP PFNGLGETRENDERBUFFERPARAMETERIVPROC)(const GLenum target, const GLenum pname, GLint* params);
typedef GLboolean (APIENTRYP PFNGLISFRAMEBUFFERPROC)(const GLuint framebuffer);
typedef void (APIENTRYP PFNGLBINDFRAMEBUFFERPROC)(const GLenum target, const GLuint framebuffer);
typedef void (APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC)(const GLsizei n, const GLuint* framebuffers);
typedef void (APIENTRYP PFNGLGENFRAMEBUFFERSPROC)(const GLsizei n, GLuint* framebuffers);
typedef GLenum (APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC)(const GLenum target);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE1DPROC)(const GLenum target, const GLenum attachment, const GLenum textarget, const GLuint texture, const GLint level);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC)(const GLenum target, const GLenum attachment, const GLenum textarget, const GLuint texture, const GLint level);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE3DPROC)(const GLenum target, const GLenum attachment, const GLenum textarget, const GLuint texture, const GLint level, const GLint zoffset);
typedef void (APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFERPROC)(const GLenum target, const GLenum attachment, const GLenum renderbuffertarget, const GLuint renderbuffer);
typedef void (APIENTRYP PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(const GLenum target, const GLenum attachment, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGENERATEMIPMAPPROC)(const GLenum target);
typedef void (APIENTRYP PFNGLBLITFRAMEBUFFERPROC)(const GLint srcX0, const GLint srcY0, const GLint srcX1, const GLint srcY1, const GLint dstX0, const GLint dstY0, const GLint dstX1, const GLint dstY1, const GLbitfield mask, const GLenum filter);
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)(const GLenum target, const GLsizei samples, const GLenum internalformat, const GLsizei width, const GLsizei height);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURELAYERPROC)(const GLenum target, const GLenum attachment, const GLuint texture, const GLint level, const GLint layer);
typedef GLvoid* (APIENTRYP PFNGLMAPBUFFERRANGEPROC)(const GLenum target, const GLintptr offset, const GLsizeiptr length, const GLbitfield access);
typedef void (APIENTRYP PFNGLFLUSHMAPPEDBUFFERRANGEPROC)(const GLenum target, const GLintptr offset, const GLsizeiptr length);
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC)(const GLuint array);
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC)(const GLsizei n, const GLuint* arrays);
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC)(const GLsizei n, GLuint* arrays);
typedef GLboolean (APIENTRYP PFNGLISVERTEXARRAYPROC)(const GLuint array);

GLAPI PFNGLCOLORMASKIPROC glpfColorMaski;
GLAPI PFNGLGETBOOLEANI_VPROC glpfGetBooleani_v;
GLAPI PFNGLGETINTEGERI_VPROC glpfGetIntegeri_v;
GLAPI PFNGLENABLEIPROC glpfEnablei;
GLAPI PFNGLDISABLEIPROC glpfDisablei;
GLAPI PFNGLISENABLEDIPROC glpfIsEnabledi;
GLAPI PFNGLBEGINTRANSFORMFEEDBACKPROC glpfBeginTransformFeedback;
GLAPI PFNGLENDTRANSFORMFEEDBACKPROC glpfEndTransformFeedback;
GLAPI PFNGLBINDBUFFERRANGEPROC glpfBindBufferRange;
GLAPI PFNGLBINDBUFFERBASEPROC glpfBindBufferBase;
GLAPI PFNGLTRANSFORMFEEDBACKVARYINGSPROC glpfTransformFeedbackVaryings;
GLAPI PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glpfGetTransformFeedbackVarying;
GLAPI PFNGLCLAMPCOLORPROC glpfClampColor;
GLAPI PFNGLBEGINCONDITIONALRENDERPROC glpfBeginConditionalRender;
GLAPI PFNGLENDCONDITIONALRENDERPROC glpfEndConditionalRender;
GLAPI PFNGLVERTEXATTRIBIPOINTERPROC glpfVertexAttribIPointer;
GLAPI PFNGLGETVERTEXATTRIBIIVPROC glpfGetVertexAttribIiv;
GLAPI PFNGLGETVERTEXATTRIBIUIVPROC glpfGetVertexAttribIuiv;
GLAPI PFNGLVERTEXATTRIBI1IPROC glpfVertexAttribI1i;
GLAPI PFNGLVERTEXATTRIBI2IPROC glpfVertexAttribI2i;
GLAPI PFNGLVERTEXATTRIBI3IPROC glpfVertexAttribI3i;
GLAPI PFNGLVERTEXATTRIBI4IPROC glpfVertexAttribI4i;
GLAPI PFNGLVERTEXATTRIBI1UIPROC glpfVertexAttribI1ui;
GLAPI PFNGLVERTEXATTRIBI2UIPROC glpfVertexAttribI2ui;
GLAPI PFNGLVERTEXATTRIBI3UIPROC glpfVertexAttribI3ui;
GLAPI PFNGLVERTEXATTRIBI4UIPROC glpfVertexAttribI4ui;
GLAPI PFNGLVERTEXATTRIBI1IVPROC glpfVertexAttribI1iv;
GLAPI PFNGLVERTEXATTRIBI2IVPROC glpfVertexAttribI2iv;
GLAPI PFNGLVERTEXATTRIBI3IVPROC glpfVertexAttribI3iv;
GLAPI PFNGLVERTEXATTRIBI4IVPROC glpfVertexAttribI4iv;
GLAPI PFNGLVERTEXATTRIBI1UIVPROC glpfVertexAttribI1uiv;
GLAPI PFNGLVERTEXATTRIBI2UIVPROC glpfVertexAttribI2uiv;
GLAPI PFNGLVERTEXATTRIBI3UIVPROC glpfVertexAttribI3uiv;
GLAPI PFNGLVERTEXATTRIBI4UIVPROC glpfVertexAttribI4uiv;
GLAPI PFNGLVERTEXATTRIBI4BVPROC glpfVertexAttribI4bv;
GLAPI PFNGLVERTEXATTRIBI4SVPROC glpfVertexAttribI4sv;
GLAPI PFNGLVERTEXATTRIBI4UBVPROC glpfVertexAttribI4ubv;
GLAPI PFNGLVERTEXATTRIBI4USVPROC glpfVertexAttribI4usv;
GLAPI PFNGLGETUNIFORMUIVPROC glpfGetUniformuiv;
GLAPI PFNGLBINDFRAGDATALOCATIONPROC glpfBindFragDataLocation;
GLAPI PFNGLGETFRAGDATALOCATIONPROC glpfGetFragDataLocation;
GLAPI PFNGLUNIFORM1UIPROC glpfUniform1ui;
GLAPI PFNGLUNIFORM2UIPROC glpfUniform2ui;
GLAPI PFNGLUNIFORM3UIPROC glpfUniform3ui;
GLAPI PFNGLUNIFORM4UIPROC glpfUniform4ui;
GLAPI PFNGLUNIFORM1UIVPROC glpfUniform1uiv;
GLAPI PFNGLUNIFORM2UIVPROC glpfUniform2uiv;
GLAPI PFNGLUNIFORM3UIVPROC glpfUniform3uiv;
GLAPI PFNGLUNIFORM4UIVPROC glpfUniform4uiv;
GLAPI PFNGLTEXPARAMETERIIVPROC glpfTexParameterIiv;
GLAPI PFNGLTEXPARAMETERIUIVPROC glpfTexParameterIuiv;
GLAPI PFNGLGETTEXPARAMETERIIVPROC glpfGetTexParameterIiv;
GLAPI PFNGLGETTEXPARAMETERIUIVPROC glpfGetTexParameterIuiv;
GLAPI PFNGLCLEARBUFFERIVPROC glpfClearBufferiv;
GLAPI PFNGLCLEARBUFFERUIVPROC glpfClearBufferuiv;
GLAPI PFNGLCLEARBUFFERFVPROC glpfClearBufferfv;
GLAPI PFNGLCLEARBUFFERFIPROC glpfClearBufferfi;
GLAPI PFNGLGETSTRINGIPROC glpfGetStringi;
GLAPI PFNGLISRENDERBUFFERPROC glpfIsRenderbuffer;
GLAPI PFNGLBINDRENDERBUFFERPROC glpfBindRenderbuffer;
GLAPI PFNGLDELETERENDERBUFFERSPROC glpfDeleteRenderbuffers;
GLAPI PFNGLGENRENDERBUFFERSPROC glpfGenRenderbuffers;
GLAPI PFNGLRENDERBUFFERSTORAGEPROC glpfRenderbufferStorage;
GLAPI PFNGLGETRENDERBUFFERPARAMETERIVPROC glpfGetRenderbufferParameteriv;
GLAPI PFNGLISFRAMEBUFFERPROC glpfIsFramebuffer;
GLAPI PFNGLBINDFRAMEBUFFERPROC glpfBindFramebuffer;
GLAPI PFNGLDELETEFRAMEBUFFERSPROC glpfDeleteFramebuffers;
GLAPI PFNGLGENFRAMEBUFFERSPROC glpfGenFramebuffers;
GLAPI PFNGLCHECKFRAMEBUFFERSTATUSPROC glpfCheckFramebufferStatus;
GLAPI PFNGLFRAMEBUFFERTEXTURE1DPROC glpfFramebufferTexture1D;
GLAPI PFNGLFRAMEBUFFERTEXTURE2DPROC glpfFramebufferTexture2D;
GLAPI PFNGLFRAMEBUFFERTEXTURE3DPROC glpfFramebufferTexture3D;
GLAPI PFNGLFRAMEBUFFERRENDERBUFFERPROC glpfFramebufferRenderbuffer;
GLAPI PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glpfGetFramebufferAttachmentParameteriv;
GLAPI PFNGLGENERATEMIPMAPPROC glpfGenerateMipmap;
GLAPI PFNGLBLITFRAMEBUFFERPROC glpfBlitFramebuffer;
GLAPI PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glpfRenderbufferStorageMultisample;
GLAPI PFNGLFRAMEBUFFERTEXTURELAYERPROC glpfFramebufferTextureLayer;
GLAPI PFNGLMAPBUFFERRANGEPROC glpfMapBufferRange;
GLAPI PFNGLFLUSHMAPPEDBUFFERRANGEPROC glpfFlushMappedBufferRange;
GLAPI PFNGLBINDVERTEXARRAYPROC glpfBindVertexArray;
GLAPI PFNGLDELETEVERTEXARRAYSPROC glpfDeleteVertexArrays;
GLAPI PFNGLGENVERTEXARRAYSPROC glpfGenVertexArrays;
GLAPI PFNGLISVERTEXARRAYPROC glpfIsVertexArray;

#define glColorMaski glpfColorMaski
#define glGetBooleani_v glpfGetBooleani_v
#define glGetIntegeri_v glpfGetIntegeri_v
#define glEnablei glpfEnablei
#define glDisablei glpfDisablei
#define glIsEnabledi glpfIsEnabledi
#define glBeginTransformFeedback glpfBeginTransformFeedback
#define glEndTransformFeedback glpfEndTransformFeedback
#define glBindBufferRange glpfBindBufferRange
#define glBindBufferBase glpfBindBufferBase
#define glTransformFeedbackVaryings glpfTransformFeedbackVaryings
#define glGetTransformFeedbackVarying glpfGetTransformFeedbackVarying
#define glClampColor glpfClampColor
#define glBeginConditionalRender glpfBeginConditionalRender
#define glEndConditionalRender glpfEndConditionalRender
#define glVertexAttribIPointer glpfVertexAttribIPointer
#define glGetVertexAttribIiv glpfGetVertexAttribIiv
#define glGetVertexAttribIuiv glpfGetVertexAttribIuiv
#define glVertexAttribI1i glpfVertexAttribI1i
#define glVertexAttribI2i glpfVertexAttribI2i
#define glVertexAttribI3i glpfVertexAttribI3i
#define glVertexAttribI4i glpfVertexAttribI4i
#define glVertexAttribI1ui glpfVertexAttribI1ui
#define glVertexAttribI2ui glpfVertexAttribI2ui
#define glVertexAttribI3ui glpfVertexAttribI3ui
#define glVertexAttribI4ui glpfVertexAttribI4ui
#define glVertexAttribI1iv glpfVertexAttribI1iv
#define glVertexAttribI2iv glpfVertexAttribI2iv
#define glVertexAttribI3iv glpfVertexAttribI3iv
#define glVertexAttribI4iv glpfVertexAttribI4iv
#define glVertexAttribI1uiv glpfVertexAttribI1uiv
#define glVertexAttribI2uiv glpfVertexAttribI2uiv
#define glVertexAttribI3uiv glpfVertexAttribI3uiv
#define glVertexAttribI4uiv glpfVertexAttribI4uiv
#define glVertexAttribI4bv glpfVertexAttribI4bv
#define glVertexAttribI4sv glpfVertexAttribI4sv
#define glVertexAttribI4ubv glpfVertexAttribI4ubv
#define glVertexAttribI4usv glpfVertexAttribI4usv
#define glGetUniformuiv glpfGetUniformuiv
#define glBindFragDataLocation glpfBindFragDataLocation
#define glGetFragDataLocation glpfGetFragDataLocation
#define glUniform1ui glpfUniform1ui
#define glUniform2ui glpfUniform2ui
#define glUniform3ui glpfUniform3ui
#define glUniform4ui glpfUniform4ui
#define glUniform1uiv glpfUniform1uiv
#define glUniform2uiv glpfUniform2uiv
#define glUniform3uiv glpfUniform3uiv
#define glUniform4uiv glpfUniform4uiv
#define glTexParameterIiv glpfTexParameterIiv
#define glTexParameterIuiv glpfTexParameterIuiv
#define glGetTexParameterIiv glpfGetTexParameterIiv
#define glGetTexParameterIuiv glpfGetTexParameterIuiv
#define glClearBufferiv glpfClearBufferiv
#define glClearBufferuiv glpfClearBufferuiv
#define glClearBufferfv glpfClearBufferfv
#define glClearBufferfi glpfClearBufferfi
#define glGetStringi glpfGetStringi
#define glIsRenderbuffer glpfIsRenderbuffer
#define glBindRenderbuffer glpfBindRenderbuffer
#define glDeleteRenderbuffers glpfDeleteRenderbuffers
#define glGenRenderbuffers glpfGenRenderbuffers
#define glRenderbufferStorage glpfRenderbufferStorage
#define glGetRenderbufferParameteriv glpfGetRenderbufferParameteriv
#define glIsFramebuffer glpfIsFramebuffer
#define glBindFramebuffer glpfBindFramebuffer
#define glDeleteFramebuffers glpfDeleteFramebuffers
#define glGenFramebuffers glpfGenFramebuffers
#define glCheckFramebufferStatus glpfCheckFramebufferStatus
#define glFramebufferTexture1D glpfFramebufferTexture1D
#define glFramebufferTexture2D glpfFramebufferTexture2D
#define glFramebufferTexture3D glpfFramebufferTexture3D
#define glFramebufferRenderbuffer glpfFramebufferRenderbuffer
#define glGetFramebufferAttachmentParameteriv glpfGetFramebufferAttachmentParameteriv
#define glGenerateMipmap glpfGenerateMipmap
#define glBlitFramebuffer glpfBlitFramebuffer
#define glRenderbufferStorageMultisample glpfRenderbufferStorageMultisample
#define glFramebufferTextureLayer glpfFramebufferTextureLayer
#define glMapBufferRange glpfMapBufferRange
#define glFlushMappedBufferRange glpfFlushMappedBufferRange
#define glBindVertexArray glpfBindVertexArray
#define glDeleteVertexArrays glpfDeleteVertexArrays
#define glGenVertexArrays glpfGenVertexArrays
#define glIsVertexArray glpfIsVertexArray

// VERSION_3_1

typedef void (APIENTRYP PFNGLDRAWARRAYSINSTANCEDPROC)(const GLenum mode, const GLint first, const GLsizei count, const GLsizei primcount);
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDPROC)(const GLenum mode, const GLsizei count, const GLenum type, const GLvoid* indices, const GLsizei primcount);
typedef void (APIENTRYP PFNGLTEXBUFFERPROC)(const GLenum target, const GLenum internalformat, const GLuint buffer);
typedef void (APIENTRYP PFNGLPRIMITIVERESTARTINDEXPROC)(const GLuint index);
typedef void (APIENTRYP PFNGLCOPYBUFFERSUBDATAPROC)(const GLenum readTarget, const GLenum writeTarget, const GLintptr readOffset, const GLintptr writeOffset, const GLsizeiptr size);
typedef void (APIENTRYP PFNGLGETUNIFORMINDICESPROC)(const GLuint program, const GLsizei uniformCount, const GLchar** uniformNames, GLuint* uniformIndices);
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMSIVPROC)(const GLuint program, const GLsizei uniformCount, const GLuint* uniformIndices, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMNAMEPROC)(const GLuint program, const GLuint uniformIndex, const GLsizei bufSize, GLsizei* length, GLchar* uniformName);
typedef GLuint (APIENTRYP PFNGLGETUNIFORMBLOCKINDEXPROC)(const GLuint program, const GLchar* uniformBlockName);
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMBLOCKIVPROC)(const GLuint program, const GLuint uniformBlockIndex, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)(const GLuint program, const GLuint uniformBlockIndex, const GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
typedef void (APIENTRYP PFNGLUNIFORMBLOCKBINDINGPROC)(const GLuint program, const GLuint uniformBlockIndex, const GLuint uniformBlockBinding);

GLAPI PFNGLDRAWARRAYSINSTANCEDPROC glpfDrawArraysInstanced;
GLAPI PFNGLDRAWELEMENTSINSTANCEDPROC glpfDrawElementsInstanced;
GLAPI PFNGLTEXBUFFERPROC glpfTexBuffer;
GLAPI PFNGLPRIMITIVERESTARTINDEXPROC glpfPrimitiveRestartIndex;
GLAPI PFNGLCOPYBUFFERSUBDATAPROC glpfCopyBufferSubData;
GLAPI PFNGLGETUNIFORMINDICESPROC glpfGetUniformIndices;
GLAPI PFNGLGETACTIVEUNIFORMSIVPROC glpfGetActiveUniformsiv;
GLAPI PFNGLGETACTIVEUNIFORMNAMEPROC glpfGetActiveUniformName;
GLAPI PFNGLGETUNIFORMBLOCKINDEXPROC glpfGetUniformBlockIndex;
GLAPI PFNGLGETACTIVEUNIFORMBLOCKIVPROC glpfGetActiveUniformBlockiv;
GLAPI PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glpfGetActiveUniformBlockName;
GLAPI PFNGLUNIFORMBLOCKBINDINGPROC glpfUniformBlockBinding;

#define glDrawArraysInstanced glpfDrawArraysInstanced
#define glDrawElementsInstanced glpfDrawElementsInstanced
#define glTexBuffer glpfTexBuffer
#define glPrimitiveRestartIndex glpfPrimitiveRestartIndex
#define glCopyBufferSubData glpfCopyBufferSubData
#define glGetUniformIndices glpfGetUniformIndices
#define glGetActiveUniformsiv glpfGetActiveUniformsiv
#define glGetActiveUniformName glpfGetActiveUniformName
#define glGetUniformBlockIndex glpfGetUniformBlockIndex
#define glGetActiveUniformBlockiv glpfGetActiveUniformBlockiv
#define glGetActiveUniformBlockName glpfGetActiveUniformBlockName
#define glUniformBlockBinding glpfUniformBlockBinding

// VERSION_3_2

typedef void (APIENTRYP PFNGLGETINTEGER64I_VPROC)(const GLenum target, const GLuint index, GLint64* data);
typedef void (APIENTRYP PFNGLGETBUFFERPARAMETERI64VPROC)(const GLenum target, const GLenum pname, GLint64* params);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTUREPROC)(const GLenum target, const GLenum attachment, const GLuint texture, const GLint level);
typedef void (APIENTRYP PFNGLDRAWELEMENTSBASEVERTEXPROC)(const GLenum mode, const GLsizei count, const GLenum type, const GLvoid* indices, const GLint basevertex);
typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)(const GLenum mode, const GLuint start, const GLuint end, const GLsizei count, const GLenum type, const GLvoid* indices, const GLint basevertex);
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)(const GLenum mode, const GLsizei count, const GLenum type, const GLvoid* indices, const GLsizei primcount, const GLint basevertex);
typedef void (APIENTRYP PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)(const GLenum mode, const GLsizei* count, const GLenum type, const GLvoid** indices, const GLsizei primcount, const GLint* basevertex);
typedef void (APIENTRYP PFNGLPROVOKINGVERTEXPROC)(const GLenum mode);
typedef GLsync (APIENTRYP PFNGLFENCESYNCPROC)(const GLenum condition, const GLbitfield flags);
typedef GLboolean (APIENTRYP PFNGLISSYNCPROC)(const GLsync sync);
typedef void (APIENTRYP PFNGLDELETESYNCPROC)(const GLsync sync);
typedef GLenum (APIENTRYP PFNGLCLIENTWAITSYNCPROC)(const GLsync sync, const GLbitfield flags, const GLuint64 timeout);
typedef void (APIENTRYP PFNGLWAITSYNCPROC)(const GLsync sync, const GLbitfield flags, const GLuint64 timeout);
typedef void (APIENTRYP PFNGLGETINTEGER64VPROC)(const GLenum pname, GLint64* params);
typedef void (APIENTRYP PFNGLGETSYNCIVPROC)(const GLsync sync, const GLenum pname, const GLsizei bufSize, GLsizei* length, GLint* values);
typedef void (APIENTRYP PFNGLTEXIMAGE2DMULTISAMPLEPROC)(const GLenum target, const GLsizei samples, const GLint internalformat, const GLsizei width, const GLsizei height, const GLboolean fixedsamplelocations);
typedef void (APIENTRYP PFNGLTEXIMAGE3DMULTISAMPLEPROC)(const GLenum target, const GLsizei samples, const GLint internalformat, const GLsizei width, const GLsizei height, const GLsizei depth, const GLboolean fixedsamplelocations);
typedef void (APIENTRYP PFNGLGETMULTISAMPLEFVPROC)(const GLenum pname, const GLuint index, GLfloat* val);
typedef void (APIENTRYP PFNGLSAMPLEMASKIPROC)(const GLuint index, const GLbitfield mask);

GLAPI PFNGLGETINTEGER64I_VPROC glpfGetInteger64i_v;
GLAPI PFNGLGETBUFFERPARAMETERI64VPROC glpfGetBufferParameteri64v;
GLAPI PFNGLFRAMEBUFFERTEXTUREPROC glpfFramebufferTexture;
GLAPI PFNGLDRAWELEMENTSBASEVERTEXPROC glpfDrawElementsBaseVertex;
GLAPI PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glpfDrawRangeElementsBaseVertex;
GLAPI PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glpfDrawElementsInstancedBaseVertex;
GLAPI PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glpfMultiDrawElementsBaseVertex;
GLAPI PFNGLPROVOKINGVERTEXPROC glpfProvokingVertex;
GLAPI PFNGLFENCESYNCPROC glpfFenceSync;
GLAPI PFNGLISSYNCPROC glpfIsSync;
GLAPI PFNGLDELETESYNCPROC glpfDeleteSync;
GLAPI PFNGLCLIENTWAITSYNCPROC glpfClientWaitSync;
GLAPI PFNGLWAITSYNCPROC glpfWaitSync;
GLAPI PFNGLGETINTEGER64VPROC glpfGetInteger64v;
GLAPI PFNGLGETSYNCIVPROC glpfGetSynciv;
GLAPI PFNGLTEXIMAGE2DMULTISAMPLEPROC glpfTexImage2DMultisample;
GLAPI PFNGLTEXIMAGE3DMULTISAMPLEPROC glpfTexImage3DMultisample;
GLAPI PFNGLGETMULTISAMPLEFVPROC glpfGetMultisamplefv;
GLAPI PFNGLSAMPLEMASKIPROC glpfSampleMaski;

#define glGetInteger64i_v glpfGetInteger64i_v
#define glGetBufferParameteri64v glpfGetBufferParameteri64v
#define glFramebufferTexture glpfFramebufferTexture
#define glDrawElementsBaseVertex glpfDrawElementsBaseVertex
#define glDrawRangeElementsBaseVertex glpfDrawRangeElementsBaseVertex
#define glDrawElementsInstancedBaseVertex glpfDrawElementsInstancedBaseVertex
#define glMultiDrawElementsBaseVertex glpfMultiDrawElementsBaseVertex
#define glProvokingVertex glpfProvokingVertex
#define glFenceSync glpfFenceSync
#define glIsSync glpfIsSync
#define glDeleteSync glpfDeleteSync
#define glClientWaitSync glpfClientWaitSync
#define glWaitSync glpfWaitSync
#define glGetInteger64v glpfGetInteger64v
#define glGetSynciv glpfGetSynciv
#define glTexImage2DMultisample glpfTexImage2DMultisample
#define glTexImage3DMultisample glpfTexImage3DMultisample
#define glGetMultisamplefv glpfGetMultisamplefv
#define glSampleMaski glpfSampleMaski

// VERSION_3_3

typedef void (APIENTRYP PFNGLVERTEXATTRIBDIVISORPROC)(const GLuint index, const GLuint divisor);
typedef void (APIENTRYP PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)(const GLuint program, const GLuint colorNumber, const GLuint index, const GLchar* name);
typedef GLint (APIENTRYP PFNGLGETFRAGDATAINDEXPROC)(const GLuint program, const GLchar* name);
typedef void (APIENTRYP PFNGLGENSAMPLERSPROC)(const GLsizei count, GLuint* samplers);
typedef void (APIENTRYP PFNGLDELETESAMPLERSPROC)(const GLsizei count, const GLuint* samplers);
typedef GLboolean (APIENTRYP PFNGLISSAMPLERPROC)(const GLuint sampler);
typedef void (APIENTRYP PFNGLBINDSAMPLERPROC)(const GLuint unit, const GLuint sampler);
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIPROC)(const GLuint sampler, const GLenum pname, const GLint param);
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIVPROC)(const GLuint sampler, const GLenum pname, const GLint* param);
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERFPROC)(const GLuint sampler, const GLenum pname, const GLfloat param);
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERFVPROC)(const GLuint sampler, const GLenum pname, const GLfloat* param);
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIIVPROC)(const GLuint sampler, const GLenum pname, const GLint* param);
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIUIVPROC)(const GLuint sampler, const GLenum pname, const GLuint* param);
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIVPROC)(const GLuint sampler, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIIVPROC)(const GLuint sampler, const GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERFVPROC)(const GLuint sampler, const GLenum pname, GLfloat* params);
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIUIVPROC)(const GLuint sampler, const GLenum pname, GLuint* params);
typedef void (APIENTRYP PFNGLQUERYCOUNTERPROC)(const GLuint id, const GLenum target);
typedef void (APIENTRYP PFNGLGETQUERYOBJECTI64VPROC)(const GLuint id, const GLenum pname, GLint64* params);
typedef void (APIENTRYP PFNGLGETQUERYOBJECTUI64VPROC)(const GLuint id, const GLenum pname, GLuint64* params);
typedef void (APIENTRYP PFNGLVERTEXP2UIPROC)(const GLenum type, const GLuint value);
typedef void (APIENTRYP PFNGLVERTEXP2UIVPROC)(const GLenum type, const GLuint* value);
typedef void (APIENTRYP PFNGLVERTEXP3UIPROC)(const GLenum type, const GLuint value);
typedef void (APIENTRYP PFNGLVERTEXP3UIVPROC)(const GLenum type, const GLuint* value);
typedef void (APIENTRYP PFNGLVERTEXP4UIPROC)(const GLenum type, const GLuint value);
typedef void (APIENTRYP PFNGLVERTEXP4UIVPROC)(const GLenum type, const GLuint* value);
typedef void (APIENTRYP PFNGLTEXCOORDP1UIPROC)(const GLenum type, const GLuint coords);
typedef void (APIENTRYP PFNGLTEXCOORDP1UIVPROC)(const GLenum type, const GLuint* coords);
typedef void (APIENTRYP PFNGLTEXCOORDP2UIPROC)(const GLenum type, const GLuint coords);
typedef void (APIENTRYP PFNGLTEXCOORDP2UIVPROC)(const GLenum type, const GLuint* coords);
typedef void (APIENTRYP PFNGLTEXCOORDP3UIPROC)(const GLenum type, const GLuint coords);
typedef void (APIENTRYP PFNGLTEXCOORDP3UIVPROC)(const GLenum type, const GLuint* coords);
typedef void (APIENTRYP PFNGLTEXCOORDP4UIPROC)(const GLenum type, const GLuint coords);
typedef void (APIENTRYP PFNGLTEXCOORDP4UIVPROC)(const GLenum type, const GLuint* coords);
typedef void (APIENTRYP PFNGLMULTITEXCOORDP1UIPROC)(const GLenum texture, const GLenum type, const GLuint coords);
typedef void (APIENTRYP PFNGLMULTITEXCOORDP1UIVPROC)(const GLenum texture, const GLenum type, const GLuint* coords);
typedef void (APIENTRYP PFNGLMULTITEXCOORDP2UIPROC)(const GLenum texture, const GLenum type, const GLuint coords);
typedef void (APIENTRYP PFNGLMULTITEXCOORDP2UIVPROC)(const GLenum texture, const GLenum type, const GLuint* coords);
typedef void (APIENTRYP PFNGLMULTITEXCOORDP3UIPROC)(const GLenum texture, const GLenum type, const GLuint coords);
typedef void (APIENTRYP PFNGLMULTITEXCOORDP3UIVPROC)(const GLenum texture, const GLenum type, const GLuint* coords);
typedef void (APIENTRYP PFNGLMULTITEXCOORDP4UIPROC)(const GLenum texture, const GLenum type, const GLuint coords);
typedef void (APIENTRYP PFNGLMULTITEXCOORDP4UIVPROC)(const GLenum texture, const GLenum type, const GLuint* coords);
typedef void (APIENTRYP PFNGLNORMALP3UIPROC)(const GLenum type, const GLuint coords);
typedef void (APIENTRYP PFNGLNORMALP3UIVPROC)(const GLenum type, const GLuint* coords);
typedef void (APIENTRYP PFNGLCOLORP3UIPROC)(const GLenum type, const GLuint color);
typedef void (APIENTRYP PFNGLCOLORP3UIVPROC)(const GLenum type, const GLuint* color);
typedef void (APIENTRYP PFNGLCOLORP4UIPROC)(const GLenum type, const GLuint color);
typedef void (APIENTRYP PFNGLCOLORP4UIVPROC)(const GLenum type, const GLuint* color);
typedef void (APIENTRYP PFNGLSECONDARYCOLORP3UIPROC)(const GLenum type, const GLuint color);
typedef void (APIENTRYP PFNGLSECONDARYCOLORP3UIVPROC)(const GLenum type, const GLuint* color);
typedef void (APIENTRYP PFNGLVERTEXATTRIBP1UIPROC)(const GLuint index, const GLenum type, const GLboolean normalized, const GLuint value);
typedef void (APIENTRYP PFNGLVERTEXATTRIBP1UIVPROC)(const GLuint index, const GLenum type, const GLboolean normalized, const GLuint* value);
typedef void (APIENTRYP PFNGLVERTEXATTRIBP2UIPROC)(const GLuint index, const GLenum type, const GLboolean normalized, const GLuint value);
typedef void (APIENTRYP PFNGLVERTEXATTRIBP2UIVPROC)(const GLuint index, const GLenum type, const GLboolean normalized, const GLuint* value);
typedef void (APIENTRYP PFNGLVERTEXATTRIBP3UIPROC)(const GLuint index, const GLenum type, const GLboolean normalized, const GLuint value);
typedef void (APIENTRYP PFNGLVERTEXATTRIBP3UIVPROC)(const GLuint index, const GLenum type, const GLboolean normalized, const GLuint* value);
typedef void (APIENTRYP PFNGLVERTEXATTRIBP4UIPROC)(const GLuint index, const GLenum type, const GLboolean normalized, const GLuint value);
typedef void (APIENTRYP PFNGLVERTEXATTRIBP4UIVPROC)(const GLuint index, const GLenum type, const GLboolean normalized, const GLuint* value);

GLAPI PFNGLVERTEXATTRIBDIVISORPROC glpfVertexAttribDivisor;
GLAPI PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glpfBindFragDataLocationIndexed;
GLAPI PFNGLGETFRAGDATAINDEXPROC glpfGetFragDataIndex;
GLAPI PFNGLGENSAMPLERSPROC glpfGenSamplers;
GLAPI PFNGLDELETESAMPLERSPROC glpfDeleteSamplers;
GLAPI PFNGLISSAMPLERPROC glpfIsSampler;
GLAPI PFNGLBINDSAMPLERPROC glpfBindSampler;
GLAPI PFNGLSAMPLERPARAMETERIPROC glpfSamplerParameteri;
GLAPI PFNGLSAMPLERPARAMETERIVPROC glpfSamplerParameteriv;
GLAPI PFNGLSAMPLERPARAMETERFPROC glpfSamplerParameterf;
GLAPI PFNGLSAMPLERPARAMETERFVPROC glpfSamplerParameterfv;
GLAPI PFNGLSAMPLERPARAMETERIIVPROC glpfSamplerParameterIiv;
GLAPI PFNGLSAMPLERPARAMETERIUIVPROC glpfSamplerParameterIuiv;
GLAPI PFNGLGETSAMPLERPARAMETERIVPROC glpfGetSamplerParameteriv;
GLAPI PFNGLGETSAMPLERPARAMETERIIVPROC glpfGetSamplerParameterIiv;
GLAPI PFNGLGETSAMPLERPARAMETERFVPROC glpfGetSamplerParameterfv;
GLAPI PFNGLGETSAMPLERPARAMETERIUIVPROC glpfGetSamplerParameterIuiv;
GLAPI PFNGLQUERYCOUNTERPROC glpfQueryCounter;
GLAPI PFNGLGETQUERYOBJECTI64VPROC glpfGetQueryObjecti64v;
GLAPI PFNGLGETQUERYOBJECTUI64VPROC glpfGetQueryObjectui64v;
GLAPI PFNGLVERTEXP2UIPROC glpfVertexP2ui;
GLAPI PFNGLVERTEXP2UIVPROC glpfVertexP2uiv;
GLAPI PFNGLVERTEXP3UIPROC glpfVertexP3ui;
GLAPI PFNGLVERTEXP3UIVPROC glpfVertexP3uiv;
GLAPI PFNGLVERTEXP4UIPROC glpfVertexP4ui;
GLAPI PFNGLVERTEXP4UIVPROC glpfVertexP4uiv;
GLAPI PFNGLTEXCOORDP1UIPROC glpfTexCoordP1ui;
GLAPI PFNGLTEXCOORDP1UIVPROC glpfTexCoordP1uiv;
GLAPI PFNGLTEXCOORDP2UIPROC glpfTexCoordP2ui;
GLAPI PFNGLTEXCOORDP2UIVPROC glpfTexCoordP2uiv;
GLAPI PFNGLTEXCOORDP3UIPROC glpfTexCoordP3ui;
GLAPI PFNGLTEXCOORDP3UIVPROC glpfTexCoordP3uiv;
GLAPI PFNGLTEXCOORDP4UIPROC glpfTexCoordP4ui;
GLAPI PFNGLTEXCOORDP4UIVPROC glpfTexCoordP4uiv;
GLAPI PFNGLMULTITEXCOORDP1UIPROC glpfMultiTexCoordP1ui;
GLAPI PFNGLMULTITEXCOORDP1UIVPROC glpfMultiTexCoordP1uiv;
GLAPI PFNGLMULTITEXCOORDP2UIPROC glpfMultiTexCoordP2ui;
GLAPI PFNGLMULTITEXCOORDP2UIVPROC glpfMultiTexCoordP2uiv;
GLAPI PFNGLMULTITEXCOORDP3UIPROC glpfMultiTexCoordP3ui;
GLAPI PFNGLMULTITEXCOORDP3UIVPROC glpfMultiTexCoordP3uiv;
GLAPI PFNGLMULTITEXCOORDP4UIPROC glpfMultiTexCoordP4ui;
GLAPI PFNGLMULTITEXCOORDP4UIVPROC glpfMultiTexCoordP4uiv;
GLAPI PFNGLNORMALP3UIPROC glpfNormalP3ui;
GLAPI PFNGLNORMALP3UIVPROC glpfNormalP3uiv;
GLAPI PFNGLCOLORP3UIPROC glpfColorP3ui;
GLAPI PFNGLCOLORP3UIVPROC glpfColorP3uiv;
GLAPI PFNGLCOLORP4UIPROC glpfColorP4ui;
GLAPI PFNGLCOLORP4UIVPROC glpfColorP4uiv;
GLAPI PFNGLSECONDARYCOLORP3UIPROC glpfSecondaryColorP3ui;
GLAPI PFNGLSECONDARYCOLORP3UIVPROC glpfSecondaryColorP3uiv;
GLAPI PFNGLVERTEXATTRIBP1UIPROC glpfVertexAttribP1ui;
GLAPI PFNGLVERTEXATTRIBP1UIVPROC glpfVertexAttribP1uiv;
GLAPI PFNGLVERTEXATTRIBP2UIPROC glpfVertexAttribP2ui;
GLAPI PFNGLVERTEXATTRIBP2UIVPROC glpfVertexAttribP2uiv;
GLAPI PFNGLVERTEXATTRIBP3UIPROC glpfVertexAttribP3ui;
GLAPI PFNGLVERTEXATTRIBP3UIVPROC glpfVertexAttribP3uiv;
GLAPI PFNGLVERTEXATTRIBP4UIPROC glpfVertexAttribP4ui;
GLAPI PFNGLVERTEXATTRIBP4UIVPROC glpfVertexAttribP4uiv;

#define glVertexAttribDivisor glpfVertexAttribDivisor
#define glBindFragDataLocationIndexed glpfBindFragDataLocationIndexed
#define glGetFragDataIndex glpfGetFragDataIndex
#define glGenSamplers glpfGenSamplers
#define glDeleteSamplers glpfDeleteSamplers
#define glIsSampler glpfIsSampler
#define glBindSampler glpfBindSampler
#define glSamplerParameteri glpfSamplerParameteri
#define glSamplerParameteriv glpfSamplerParameteriv
#define glSamplerParameterf glpfSamplerParameterf
#define glSamplerParameterfv glpfSamplerParameterfv
#define glSamplerParameterIiv glpfSamplerParameterIiv
#define glSamplerParameterIuiv glpfSamplerParameterIuiv
#define glGetSamplerParameteriv glpfGetSamplerParameteriv
#define glGetSamplerParameterIiv glpfGetSamplerParameterIiv
#define glGetSamplerParameterfv glpfGetSamplerParameterfv
#define glGetSamplerParameterIuiv glpfGetSamplerParameterIuiv
#define glQueryCounter glpfQueryCounter
#define glGetQueryObjecti64v glpfGetQueryObjecti64v
#define glGetQueryObjectui64v glpfGetQueryObjectui64v
#define glVertexP2ui glpfVertexP2ui
#define glVertexP2uiv glpfVertexP2uiv
#define glVertexP3ui glpfVertexP3ui
#define glVertexP3uiv glpfVertexP3uiv
#define glVertexP4ui glpfVertexP4ui
#define glVertexP4uiv glpfVertexP4uiv
#define glTexCoordP1ui glpfTexCoordP1ui
#define glTexCoordP1uiv glpfTexCoordP1uiv
#define glTexCoordP2ui glpfTexCoordP2ui
#define glTexCoordP2uiv glpfTexCoordP2uiv
#define glTexCoordP3ui glpfTexCoordP3ui
#define glTexCoordP3uiv glpfTexCoordP3uiv
#define glTexCoordP4ui glpfTexCoordP4ui
#define glTexCoordP4uiv glpfTexCoordP4uiv
#define glMultiTexCoordP1ui glpfMultiTexCoordP1ui
#define glMultiTexCoordP1uiv glpfMultiTexCoordP1uiv
#define glMultiTexCoordP2ui glpfMultiTexCoordP2ui
#define glMultiTexCoordP2uiv glpfMultiTexCoordP2uiv
#define glMultiTexCoordP3ui glpfMultiTexCoordP3ui
#define glMultiTexCoordP3uiv glpfMultiTexCoordP3uiv
#define glMultiTexCoordP4ui glpfMultiTexCoordP4ui
#define glMultiTexCoordP4uiv glpfMultiTexCoordP4uiv
#define glNormalP3ui glpfNormalP3ui
#define glNormalP3uiv glpfNormalP3uiv
#define glColorP3ui glpfColorP3ui
#define glColorP3uiv glpfColorP3uiv
#define glColorP4ui glpfColorP4ui
#define glColorP4uiv glpfColorP4uiv
#define glSecondaryColorP3ui glpfSecondaryColorP3ui
#define glSecondaryColorP3uiv glpfSecondaryColorP3uiv
#define glVertexAttribP1ui glpfVertexAttribP1ui
#define glVertexAttribP1uiv glpfVertexAttribP1uiv
#define glVertexAttribP2ui glpfVertexAttribP2ui
#define glVertexAttribP2uiv glpfVertexAttribP2uiv
#define glVertexAttribP3ui glpfVertexAttribP3ui
#define glVertexAttribP3uiv glpfVertexAttribP3uiv
#define glVertexAttribP4ui glpfVertexAttribP4ui
#define glVertexAttribP4uiv glpfVertexAttribP4uiv

#define GL_VERSION_1_0 1
#define GL_VERSION_1_0_DEPRECATED 1
#define GL_VERSION_1_1 1
#define GL_VERSION_1_1_DEPRECATED 1
#define GL_VERSION_1_2 1
#define GL_VERSION_1_2_DEPRECATED 1
#define GL_VERSION_1_3 1
#define GL_VERSION_1_3_DEPRECATED 1
#define GL_VERSION_1_4 1
#define GL_VERSION_1_4_DEPRECATED 1
#define GL_VERSION_1_5 1
#define GL_VERSION_1_5_DEPRECATED 1
#define GL_VERSION_1_6 1
#define GL_VERSION_1_6_DEPRECATED 1
#define GL_VERSION_1_7 1
#define GL_VERSION_1_7_DEPRECATED 1
#define GL_VERSION_1_8 1
#define GL_VERSION_1_8_DEPRECATED 1
#define GL_VERSION_1_9 1
#define GL_VERSION_1_9_DEPRECATED 1
#define GL_VERSION_2_0 1
#define GL_VERSION_2_0_DEPRECATED 1
#define GL_VERSION_2_1 1
#define GL_VERSION_2_1_DEPRECATED 1
#define GL_VERSION_2_2 1
#define GL_VERSION_2_2_DEPRECATED 1
#define GL_VERSION_2_3 1
#define GL_VERSION_2_3_DEPRECATED 1
#define GL_VERSION_2_4 1
#define GL_VERSION_2_4_DEPRECATED 1
#define GL_VERSION_2_5 1
#define GL_VERSION_2_5_DEPRECATED 1
#define GL_VERSION_2_6 1
#define GL_VERSION_2_6_DEPRECATED 1
#define GL_VERSION_2_7 1
#define GL_VERSION_2_7_DEPRECATED 1
#define GL_VERSION_2_8 1
#define GL_VERSION_2_8_DEPRECATED 1
#define GL_VERSION_2_9 1
#define GL_VERSION_2_9_DEPRECATED 1
#define GL_VERSION_3_0 1
#define GL_VERSION_3_0_DEPRECATED 1
#define GL_VERSION_3_1 1
#define GL_VERSION_3_1_DEPRECATED 1
#define GL_VERSION_3_2 1
#define GL_VERSION_3_2_DEPRECATED 1
#define GL_VERSION_3_3 1
#define GL_VERSION_3_3_DEPRECATED 1


// ------------------ Flags for optional extensions ---------------- 


int importOpenGL(void);

#define OPENGL_MAJOR_VERSION 3
#define OPENGL_MINOR_VERSION 3
#define OPENGL_CORE_PROFILE 0

#ifdef __cplusplus
}
#endif



#endif /* __gl_h_ */
