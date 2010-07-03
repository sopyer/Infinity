//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-11-13
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
//////////////////////////////////////////////////////////////////////////////////

#ifndef __glmsetup_h__
#define __glmsetup_h__

//////////////////////////////////////////////////////////////////////////////////
// Version

#define GLM_VERSION 50

//////////////////////////////////////////////////////////////////////////////////
// Conformance

#define GLM_CONFORMANCE_STRICT		0x00000001
#define GLM_CONFORMANCE_FRIENDLY	0x00000002

//! By default:
// #define GLM_CONFORMANCE GLM_CONFORMANCE_STRICT

//////////////////////////////////////////////////////////////////////////////////
// Swizzle operators

#define GLM_SWIZZLE_NONE            0x00000000
#define GLM_SWIZZLE_XYZW            0x00000001
#define GLM_SWIZZLE_RGBA            0x00000002
#define GLM_SWIZZLE_STQP            0x00000004
#define GLM_SWIZZLE_FULL            (GLM_SWIZZLE_XYZW | GLM_SWIZZLE_RGBA | GLM_SWIZZLE_STQP)

//! By default:
// #define GLM_SWIZZLE GLM_SWIZZLE_NONE

//////////////////////////////////////////////////////////////////////////////////
// Compiler

#define GLM_COMPILER_UNDEFINED		0x00000000

#define GLM_COMPILER_VC				0x01000000
#define GLM_COMPILER_VC40			0x01000100 //!< Visual C++ 4 isn't supported
#define GLM_COMPILER_VC50			0x01000200 //!< Visual C++ 5 isn't supported
#define GLM_COMPILER_VC60			0x01000400 //!< Visual C++ 6 isn't supported
#define GLM_COMPILER_VC70			0x01000800
#define GLM_COMPILER_VC71			0x01001000
#define GLM_COMPILER_VC80			0x01002000

#define GLM_COMPILER_GCC			0x02000000
#define GLM_COMPILER_GCC29			0x02000400 //!< GCC 2.9x isn't supported
#define GLM_COMPILER_GCC30			0x02000800 //!< GCC 3.0 isn't supported
#define GLM_COMPILER_GCC31			0x02001000 //!< GCC 3.1 isn't supported
#define GLM_COMPILER_GCC32			0x02002000
#define GLM_COMPILER_GCC33			0x02004000
#define GLM_COMPILER_GCC34			0x02008000
#define GLM_COMPILER_GCC40		    0x02010000
#define GLM_COMPILER_GCC41			0x02020000

//! By default:
// #define GLM_COMPILER GLM_COMPILER_UNDEFINED

//////////////////////////////////////////////////////////////////////////////////
// Extension

#define GLM_EXTENSION_REQUIRE		0x00000000
#define GLM_EXTENSION_ENABLE		0x00000001
#define GLM_EXTENSION_WARN			0x00000002
#define GLM_EXTENSION_DISABLE		0x00000003

//! By default:
// #define GLM_EXTENSION_ALL	GLM_ENABLE

// We could overload GLM_EXTENSION_ALL behaviour with the following #define
// #define GLM_EXTENSION_GTC					"behaviour"
// #define GLM_EXTENSION_GTX					"behaviour"

// And with each extention (all of them aren't available)
// #define GLM_GTX_array_range					"behaviour"
// #define GLM_GTX_closest_point				"behaviour"
// #define GLM_GTX_color_space					"behaviour"
// #define GLM_GTX_complex						"behaviour"
// #define GLM_GTX_determinant					"behaviour"
// #define GLM_GTX_double						"behaviour"
// #define GLM_GTX_epsilon						"behaviour"
// #define GLM_GTX_eular_angles					"behaviour"
// #define GLM_GTX_extend						"behaviour"
// #define GLM_GTX_fast_exponential				"behaviour"
// #define GLM_GTX_fast_square_root				"behaviour"
// #define GLM_GTX_fast_trigonometry			"behaviour"
// #define GLM_GTX_half							"behaviour"
// #define GLM_GTX_handed_coordinate_space		"behaviour"
// #define GLM_GTX_inertia						"behaviour"
// #define GLM_GTX_integer						"behaviour"
// #define GLM_GTX_inverse						"behaviour"
// #define GLM_GTX_inverse_transpose			"behaviour"
// #define GLM_GTX_mat4x3						"behaviour"
// #define GLM_GTX_matrix_access				"behaviour"
// #define GLM_GTX_matrix_cross_product			"behaviour"
// #define GLM_GTX_matrix_major_storage			"behaviour"
// #define GLM_GTX_matrix_projection			"behaviour"
// #define GLM_GTX_matrix_selection				"behaviour"
// #define GLM_GTX_matx							"behaviour"
// #define GLM_GTX_mul							"behaviour"
// #define GLM_GTX_norm							"behaviour"
// #define GLM_GTX_normal						"behaviour"
// #define GLM_GTX_optimum_pow					"behaviour"
// #define GLM_GTX_orthonormalize				"behaviour"
// #define GLM_GTX_outer_product				"behaviour"
// #define GLM_GTX_perpendicular				"behaviour"
// #define GLM_GTX_projection					"behaviour"
// #define GLM_GTX_quaternion					"behaviour"
// #define GLM_GTX_random						"behaviour"
// #define GLM_GTX_round						"behaviour"
// #define GLM_GTX_transform					"behaviour"
// #define GLM_GTX_transform2					"behaviour"
// #define GLM_GTX_transpose					"behaviour"
// #define GLM_GTX_vector_access				"behaviour"
// #define GLM_GTX_vector_angle					"behaviour"
// #define GLM_GTX_vector_comp_mult				"behaviour"
// #define GLM_GTX_gpu_shader4					"behaviour"

//////////////////////////////////////////////////////////////////////////////////
// Error

#define GLM_ERROR_REPORT			0x00000000
#define GLM_ERROR_STOP				0x00000001

//! By default:
// #define GLM_ERROR			    GLM_ERROR_REPORT

//////////////////////////////////////////////////////////////////////////////////
// Macro for internal purpose
#define GLM_INCLUDED(EXTENSION_NAME, EXTENSION_GROUP, EXTENSION_REQUIRED) \
	EXTENSION_NAME == GLM_EXTENSION_REQUIRE || EXTENSION_NAME == GLM_EXTENSION_ENABLE || EXTENSION_REQUIRED == 1 ||	                                        \
	 (!defined(GLM_EXTENSION_ALL) && !defined(EXTENSION_GROUP) && !defined(EXTENSION_NAME)) ||	                                        \
	 ( defined(GLM_EXTENSION_ALL) && GLM_EXTENSION_ALL == GLM_EXTENSION_REQUIRE && !defined(EXTENSION_NAME) && !defined(EXTENSION_GROUP)) ||		\
	 ( defined(GLM_EXTENSION_ALL) && GLM_EXTENSION_ALL == GLM_EXTENSION_ENABLE  && !defined(EXTENSION_NAME) && !defined(EXTENSION_GROUP)) ||		\
	 ( defined(EXTENSION_GROUP) && EXTENSION_GROUP == GLM_EXTENSION_REQUIRE && !defined(EXTENSION_NAME)) ||										\
	 ( defined(EXTENSION_GROUP) && EXTENSION_GROUP == GLM_EXTENSION_ENABLE  && !defined(EXTENSION_NAME))

#endif//__glmsetup_h__
