//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-04-03
// Updated : 2006-12-15
// Licence : This source is under GNU LGPL licence
//////////////////////////////////////////////////////////////////////////////////
// Note:
// GTX extensions are experimental extensions
//////////////////////////////////////////////////////////////////////////////////

#ifndef __glx_h__
#define __glx_h__

#include "../glm.h"
#include "./gtx_array_range.inl"
#include "./gtx_closest_point.inl"
#include "./gtx_color_space.inl"
#include "./gtx_complex.inl"
#include "./gtx_determinant.inl"
#include "./gtx_double.inl"
#include "./gtx_epsilon.inl"
#include "./gtx_euler_angles.inl"
#include "./gtx_extend.inl"
#include "./gtx_fast_exponential.inl"
#include "./gtx_fast_square_root.inl"
#include "./gtx_fast_trigonometry.inl"
#include "./gtx_gpu_shader4.inl"
#include "./gtx_half.inl"
#include "./gtx_handed_coordinate_space.inl"
#include "./gtx_inertia.inl"
#include "./gtx_integer.inl"
#include "./gtx_inverse.inl"
#include "./gtx_inverse_transpose.inl"
#include "./gtx_mat4x3.inl"
#include "./gtx_matrix_access.inl"
#include "./gtx_matrix_cross_product.inl"
#include "./gtx_matrix_major_storage.inl"
#include "./gtx_matrix_projection.inl"
#include "./gtx_matrix_selection.inl"
#include "./gtx_matx.inl"
#include "./gtx_mul.inl"
#include "./gtx_norm.inl"
#include "./gtx_normal.inl"
#include "./gtx_optimum_pow.inl"
#include "./gtx_orthonormalize.inl"
#include "./gtx_outer_product.inl"
#include "./gtx_perpendicular.inl"
#include "./gtx_projection.inl"
#include "./gtx_quaternion.inl"
#include "./gtx_random.inl"
#include "./gtx_rotate_vector.inl"
#include "./gtx_round.inl"
#include "./gtx_transform.inl"
#include "./gtx_transform2.inl"
#include "./gtx_transpose.inl"
#include "./gtx_vector_access.inl"
#include "./gtx_vector_angle.inl"
#include "./gtx_vector_comp_mult.inl"

#if (!defined(GLM_ERROR) ||(GLM_ERROR == GLM_REPORT))

#if (defined(GLM_GTX_array_range) && (GLM_GTX_array_range == GLM_REQUIRED) && !defined(GLM_GTX_array_range_supported))
	#error GLM_GTX_array_range unsupported
#elif ((GLM_GTX_array_range == GLM_ENABLE) && !defined(GLM_GTX_array_range_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_array_range unsupported
	#endif
#endif

#if (defined(GLM_GTX_closest_point) && (GLM_GTX_closest_point == GLM_REQUIRED) && !defined( GLM_GTX_closest_point_supported))
	#error GLM_GTX_closest_point unsupported
#elif ((GLM_GTX_closest_point == GLM_ENABLE) && !defined(GLM_GTX_closest_point_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_closest_point unsupported
	#endif
#endif

#if (defined(GLM_GTX_color_space) && (GLM_GTX_color_space == GLM_REQUIRED) && !defined(GLM_GTX_color_space_supported))
	#error GLM_GTX_color_space unsupported
#elif ((GLM_GTX_color_space == GLM_ENABLE) && !defined(GLM_GTX_color_space_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_color_space unsupported
	#endif
#endif

#if (defined(GLM_GTX_determinant) && (GLM_GTX_determinant == GLM_REQUIRED) && !defined(GLM_GTX_determinant_supported))
	#error GLM_GTX_determinant unsupported
#elif ((GLM_GTX_determinant == GLM_ENABLE) && !defined(GLM_GTX_determinant_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_determinant unsupported
	#endif
#endif

#if (defined(GLM_GTX_double) && (GLM_GTX_double == GLM_REQUIRED) && !defined(GLM_GTX_double_supported))
	#error GLM_GTX_double unsupported
#elif ((GLM_GTX_double == GLM_ENABLE) && !defined(GLM_GTX_double_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_double unsupported
	#endif
#endif

#if (defined(GLM_GTX_epsilon) && (GLM_GTX_epsilon == GLM_REQUIRED) && !defined(GLM_GTX_epsilon_supported))
	#error GLM_GTX_epsilon unsupported
#elif ((GLM_GTX_epsilon == GLM_ENABLE) && !defined(GLM_GTX_epsilon_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_epsilon unsupported
	#endif
#endif

#if (defined(GLM_GTX_eular_angles) && (GLM_GTX_eular_angles == GLM_REQUIRED) && !defined(GLM_GTX_eular_angles_supported))
	#error GLM_GTX_eular_angles unsupported
#elif ((GLM_GTX_eular_angles == GLM_ENABLE) && !defined(GLM_GTX_eular_angles_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_eular_angles unsupported
	#endif
#endif

#if (defined(GLM_GTX_extend) && (GLM_GTX_extend == GLM_REQUIRED) && !defined(GLM_GTX_extend_supported))
	#error GLM_GTX_extend unsupported
#elif ((GLM_GTX_extend == GLM_ENABLE) && !defined(GLM_GTX_extend_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_extend unsupported
	#endif
#endif

#if (defined(GLM_GTX_fast_exponential) && (GLM_GTX_fast_exponential == GLM_REQUIRED) && !defined(GLM_GTX_fast_exponential_supported))
	#error GLM_GTX_fast_exponential unsupported
#elif ((GLM_GTX_fast_exponential == GLM_ENABLE) && !defined(GLM_GTX_fast_exponential_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_fast_exponential unsupported
	#endif
#endif

#if (defined(GLM_GTX_fast_square_root) && (GLM_GTX_fast_square_root == GLM_REQUIRED) && !defined(GLM_GTX_fast_square_root_supported))
	#error GLM_GTX_fast_square_root unsupported
#elif ((GLM_GTX_fast_square_root == GLM_ENABLE) && !defined(GLM_GTX_fast_square_root_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_fast_square_root unsupported
	#endif
#endif

#if (defined(GLM_GTX_fast_trigonometry) && (GLM_GTX_fast_trigonometry == GLM_REQUIRED) && !defined(GLM_GTX_fast_trigonometry_supported))
	#error GLM_GTX_fast_trigonometry unsupported
#elif ((GLM_GTX_fast_trigonometry == GLM_ENABLE) && !defined(GLM_GTX_fast_trigonometry_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_fast_trigonometry unsupported
	#endif
#endif

#if (defined(GLM_GTX_half) && (GLM_GTX_half == GLM_REQUIRED) && !defined(GLM_GTX_half_supported))
	#error GLM_GTX_half unsupported
#elif ((GLM_GTX_array_range == GLM_ENABLE) && !defined(GLM_GTX_array_range_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_array_range unsupported
	#endif
#endif

#if (defined(GLM_GTX_handed_coordinate_space) && (GLM_GTX_handed_coordinate_space == GLM_REQUIRED) && !defined(GLM_GTX_handed_coordinate_space_supported))
	#error GLM_GTX_handed_coordinate_space unsupported
#elif ((GLM_GTX_array_range == GLM_ENABLE) && !defined(GLM_GTX_array_range_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_array_range unsupported
	#endif
#endif

#if (defined(GLM_GTX_inertia) && (GLM_GTX_inertia == GLM_REQUIRED) && !defined(GLM_GTX_inertia_supported))
	#error GLM_GTX_inertia unsupported
#elif ((GLM_GTX_array_range == GLM_ENABLE) && !defined(GLM_GTX_array_range_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_array_range unsupported
	#endif
#endif

#if (defined(GLM_GTX_integer) && (GLM_GTX_integer == GLM_REQUIRED) && !defined(GLM_GTX_integer_supported))
	#error GLM_GTX_integer unsupported
#elif ((GLM_GTX_array_range == GLM_ENABLE) && !defined(GLM_GTX_array_range_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_array_range unsupported
	#endif
#endif

#if (defined(GLM_GTX_inverse) && (GLM_GTX_inverse == GLM_REQUIRED) && !defined(GLM_GTX_inverse_supported))
	#error GLM_GTX_inverse unsupported
#elif ((GLM_GTX_array_range == GLM_ENABLE) && !defined(GLM_GTX_array_range_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_array_range unsupported
	#endif
#endif

#if (defined(GLM_GTX_inverse_transpose) && (GLM_GTX_inverse_transpose == GLM_REQUIRED) && !defined(GLM_GTX_inverse_transpose_supported))
	#error GLM_GTX_inverse_transpose unsupported
#elif ((GLM_GTX_inverse_transpose == GLM_ENABLE) && !defined(GLM_GTX_inverse_transpose_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_inverse_transpose unsupported
	#endif
#endif

#if (defined(GLM_GTX_mat4x3) && (GLM_GTX_mat4x3 == GLM_REQUIRED) && !defined(GLM_GTX_mat4x3_supported))
	#error GLM_GTX_mat4x3 unsupported
#elif ((GLM_GTX_mat4x3 == GLM_ENABLE) && !defined(GLM_GTX_mat4x3_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_mat4x3 unsupported
	#endif
#endif

#if (defined(GLM_GTX_matrix_access) && (GLM_GTX_matrix_access == GLM_REQUIRED) && !defined(GLM_GTX_matrix_access_supported))
	#error GLM_GTX_matrix_access unsupported
#elif ((GLM_GTX_matrix_access == GLM_ENABLE) && !defined(GLM_GTX_matrix_access_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_matrix_access unsupported
	#endif
#endif

#if (defined(GLM_GTX_matrix_cross_product) && (GLM_GTX_matrix_cross_product == GLM_REQUIRED) && !defined(GLM_GTX_matrix_cross_product_supported))
	#error GLM_GTX_matrix_cross_product unsupported
#elif ((GLM_GTX_matrix_cross_product == GLM_ENABLE) && !defined(GLM_GTX_matrix_cross_product_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_matrix_cross_product unsupported
	#endif
#endif

#if (defined(GLM_GTX_matrix_major_storage) && (GLM_GTX_matrix_major_storage == GLM_REQUIRED) && !defined(GLM_GTX_matrix_major_storage_supported))
	#error GLM_GTX_matrix_major_storage unsupported
#elif ((GLM_GTX_matrix_major_storage == GLM_ENABLE) && !defined(GLM_GTX_matrix_major_storage_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_matrix_major_storage unsupported
	#endif
#endif

#if (defined(GLM_GTX_matrix_projection) && (GLM_GTX_matrix_projection == GLM_REQUIRED) && !defined(GLM_GTX_matrix_projection_supported))
	#error GLM_GTX_matrix_projection unsupported
#elif ((GLM_GTX_matrix_projection == GLM_ENABLE) && !defined(GLM_GTX_matrix_projection_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_matrix_projection unsupported
	#endif
#endif

#if (defined(GLM_GTX_matrix_selection) && (GLM_GTX_matrix_selection == GLM_REQUIRED) && !defined(GLM_GTX_matrix_selection_supported))
	#error GLM_GTX_matrix_selection unsupported
#elif ((GLM_GTX_matrix_selection == GLM_ENABLE) && !defined(GLM_GTX_matrix_selection_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_matrix_selection unsupported
	#endif
#endif

#if (defined(GLM_GTX_matx) && (GLM_GTX_matx == GLM_REQUIRED) && !defined(GLM_GTX_matx_supported))
	#error GLM_GTX_matx unsupported
#elif ((GLM_GTX_matx == GLM_ENABLE) && !defined(GLM_GTX_matx_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_matx unsupported
	#endif
#endif

#if (defined(GLM_GTX_mul) && (GLM_GTX_mul == GLM_REQUIRED) && !defined(GLM_GTX_mul_supported))
	#error GLM_GTX_mul unsupported
#elif ((GLM_GTX_mul == GLM_ENABLE) && !defined(GLM_GTX_mul_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_mul unsupported
	#endif
#endif

#if (defined(GLM_GTX_norm) && (GLM_GTX_norm == GLM_REQUIRED) && !defined(GLM_GTX_norm_supported))
	#error GLM_GTX_norm unsupported
#elif ((GLM_GTX_norm == GLM_ENABLE) && !defined(GLM_GTX_norm_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_norm unsupported
	#endif
#endif

#if (defined(GLM_GTX_normal) && (GLM_GTX_normal == GLM_REQUIRED) && !defined(GLM_GTX_normal_supported))
	#error GLM_GTX_normal unsupported
#elif ((GLM_GTX_normal == GLM_ENABLE) && !defined(GLM_GTX_normal_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_normal unsupported
	#endif
#endif

#if (defined(GLM_GTX_optimum_pow) && (GLM_GTX_optimum_pow == GLM_REQUIRED) && !defined(GLM_GTX_optimum_pow_supported))
	#error GLM_GTX_optimum_pow unsupported
#elif ((GLM_GTX_optimum_pow == GLM_ENABLE) && !defined(GLM_GTX_optimum_pow_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_optimum_pow unsupported
	#endif
#endif

#if (defined(GLM_GTX_orthonormalize) && (GLM_GTX_orthonormalize == GLM_REQUIRED) && !defined(GLM_GTX_orthonormalize_supported))
	#error GLM_GTX_orthonormalize unsupported
#elif ((GLM_GTX_orthonormalize == GLM_ENABLE) && !defined(GLM_GTX_orthonormalize_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_orthonormalize unsupported
	#endif
#endif

#if (defined(GLM_GTX_outer_product) && (GLM_GTX_outer_product == GLM_REQUIRED) && !defined(GLM_GTX_outer_product))
	#error GLM_GTX_outer_product unsupported
#elif ((GLM_GTX_outer_product == GLM_ENABLE) && !defined(GLM_GTX_outer_product_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_outer_product unsupported
	#endif
#endif

#if (defined(GLM_GTX_perpendicular) && (GLM_GTX_perpendicular == GLM_REQUIRED) && !defined(GLM_GTX_perpendicular_supported))
	#error GLM_GTX_perpendicular unsupported
#elif ((GLM_GTX_perpendicular == GLM_ENABLE) && !defined(GLM_GTX_perpendicular_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_perpendicular unsupported
	#endif
#endif

#if (defined(GLM_GTX_projection) && (GLM_GTX_projection == GLM_REQUIRED) && !defined(GLM_GTX_projection_supported))
	#error GLM_GTX_projection unsupported
#elif ((GLM_GTX_projection == GLM_ENABLE) && !defined(GLM_GTX_projection_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_projection unsupported
	#endif
#endif

#if (defined(GLM_GTX_quaternion) && (GLM_GTX_quaternion == GLM_REQUIRED) && !defined(GLM_GTX_quaternion_supported))
	#error GLM_GTX_quaternion unsupported
#elif ((GLM_GTX_quaternion == GLM_ENABLE) && !defined(GLM_GTX_quaternion_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_quaternion unsupported
	#endif
#endif

#if (defined(GLM_GTX_random) && (GLM_GTX_random == GLM_REQUIRED) && !defined(GLM_GTX_random_supported))
	#error GLM_GTX_random unsupported
#elif ((GLM_GTX_random == GLM_ENABLE) && !defined(GLM_GTX_random_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_random unsupported
	#endif
#endif

#if (defined(GLM_GTX_round) && (GLM_GTX_round == GLM_REQUIRED) && !defined(GLM_GTX_round_supported))
	#error GLM_GTX_round unsupported
#elif ((GLM_GTX_round == GLM_ENABLE) && !defined(GLM_GTX_round_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_round unsupported
	#endif
#endif

#if (defined(GLM_GTX_transform) && (GLM_GTX_transform == GLM_REQUIRED) && !defined(GLM_GTX_transform_supported))
	#error GLM_GTX_transform unsupported
#elif ((GLM_GTX_transform == GLM_ENABLE) && !defined(GLM_GTX_transform_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_transform unsupported
	#endif
#endif

#if (defined(GLM_GTX_transform2) && (GLM_GTX_transform2 == GLM_REQUIRED) && !defined(GLM_GTX_transform2_supported))
	#error GLM_GTX_transform2 unsupported
#elif ((GLM_GTX_transform2 == GLM_ENABLE) && !defined(GLM_GTX_transform2_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_transform2 unsupported
	#endif
#endif

#if (defined(GLM_GTX_transpose) && (GLM_GTX_transpose == GLM_REQUIRED) && !defined(GLM_GTX_transpose_supported))
	#error GLM_GTX_transpose unsupported
#elif ((GLM_GTX_transpose == GLM_ENABLE) && !defined(GLM_GTX_transpose_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_transpose unsupported
	#endif
#endif

#if (defined(GLM_GTX_vector_access) && (GLM_GTX_vector_access == GLM_REQUIRED) && !defined(GLM_GTX_vector_access_supported))
	#error GLM_GTX_vector_access unsupported
#elif ((GLM_GTX_vector_access == GLM_ENABLE) && !defined(GLM_GTX_vector_access_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_vector_access unsupported
	#endif
#endif

#if (defined(GLM_GTX_vector_angle) && (GLM_GTX_vector_angle == GLM_REQUIRED) && !defined(GLM_GTX_vector_angle_supported))
	#error GLM_GTX_vector_angle unsupported
#elif ((GLM_GTX_vector_angle == GLM_ENABLE) && !defined(GLM_GTX_vector_angle_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_vector_angle unsupported
	#endif
#endif

#if (defined(GLM_GTX_vector_comp_mult) && (GLM_GTX_vector_comp_mult == GLM_REQUIRED) && !defined(GLM_GTX_vector_comp_mult_supported))
	#error GLM_GTX_vector_comp_mult unsupported
#elif ((GLM_GTX_vector_comp_mult == GLM_ENABLE) && !defined(GLM_GTX_vector_comp_mult_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_vector_comp_mult unsupported
	#endif
#endif

#if (defined(GLM_GTX_gpu_shader4) && (GLM_GTX_gpu_shader4 == GLM_REQUIRED) && !defined(GLM_GTX_gpu_shader4_supported))
	#error GLM_GTX_gpu_shader4 unsupported
#elif ((GLM_GTX_array_range == GLM_ENABLE) && !defined(GLM_GTX_gpu_shader4_supported))
	#if (GLM_COMPILER & GLM_GCC)
	#warning GLM_GTX_gpu_shader4 unsupported
	#endif
#endif

#endif//GLM_ERROR

#endif //__glx_h__
