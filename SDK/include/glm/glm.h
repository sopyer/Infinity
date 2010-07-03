//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-01-14
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
//////////////////////////////////////////////////////////////////////////////////

#ifndef __glm_h__
#define __glm_h__
/*
#if defined(_WINDOWS_) || defined(_WINDOWS_H)
#error windows.h included before glm.h
#endif
*/
#undef max
#undef min

#include "./glmsetup.h"

#include "./core/_xvec2.inl"
#include "./core/_xvec3.inl"
#include "./core/_xvec4.inl"
#include "./core/_bvec2.inl"
#include "./core/_bvec3.inl"
#include "./core/_bvec4.inl"
#include "./core/_xmat2.inl"
#include "./core/_xmat3.inl"
#include "./core/_xmat4.inl"
#include "./core/_xmat2x3.inl"
#include "./core/_xmat2x4.inl"
#include "./core/_xmat3x2.inl"
#include "./core/_xmat3x4.inl"
#include "./core/_xmat4x2.inl"
#include "./core/_xmat4x3.inl"
#include "./core/_func.inl"
#include "./core/_swizzle.h"
#include "./core/_xref2.inl"
#include "./core/_xref3.inl"
#include "./core/_xref4.inl"
#include "./core/_bref2.inl"
#include "./core/_bref3.inl"
#include "./core/_bref4.inl"

//! \brief OpenGL Mathematics namespace
namespace glm
{
    typedef _xvec2<float>   vec2;		//!< \brief Vector of 2 floating-point numbers. (From GLSL 1.10.59 specification)
    typedef _xvec2<int>     ivec2;		//!< \brief Vector of 2 integers. (From GLSL 1.10.59 specification)
    typedef _bvec2          bvec2;		//!< \brief Vector of 2 booleans. (From GLSL 1.10.59 specification)
    typedef _xvec3<float>   vec3;		//!< \brief Vector of 3 floating-point numbers. (From GLSL 1.10.59 specification)
    typedef _xvec3<int>     ivec3;		//!< \brief Vector of 3 integers. (From GLSL 1.10.59 specification)
    typedef _bvec3          bvec3;		//!< \brief Vector of 3 booleans. (From GLSL 1.10.59 specification)
    typedef _xvec4<float>   vec4;		//!< \brief Vector of 4 floating-point numbers. (From GLSL 1.10.59 specification)
    typedef _xvec4<int>     ivec4;		//!< \brief Vector of 4 integers. (From GLSL 1.10.59 specification)
    typedef _bvec4          bvec4;		//!< \brief Vector of 4 booleans. (From GLSL 1.10.59 specification)
    typedef _xmat2<float>   mat2;		//!< \brief 2 * 2 matrix of floating-point numbers. (From GLSL 1.10.59 specification)
    typedef _xmat3<float>   mat3;		//!< \brief 3 * 3 matrix of floating-point numbers. (From GLSL 1.10.59 specification)
    typedef _xmat4<float>   mat4;		//!< \brief 4 * 4 matrix of floating-point numbers. (From GLSL 1.10.59 specification)
    typedef _xmat2x3<float> mat2x3;		//!< \brief 2 * 3 matrix of floating-point numbers. (From GLSL 1.20.6 specification)
    typedef _xmat3x2<float> mat3x2;		//!< \brief 3 * 2 matrix of floating-point numbers. (From GLSL 1.20.6 specification)
    typedef _xmat2x4<float> mat2x4;		//!< \brief 2 * 4 matrix of floating-point numbers. (From GLSL 1.20.6 specification)
	typedef _xmat4x2<float> mat4x2;		//!< \brief 4 * 2 matrix of floating-point numbers. (From GLSL 1.20.6 specification)
	typedef _xmat3x4<float> mat3x4;		//!< \brief 3 * 4 matrix of floating-point numbers. (From GLSL 1.20.6 specification)
	typedef _xmat4x3<float> mat4x3;		//!< \brief 4 * 3 matrix of floating-point numbers. (From GLSL 1.20.6 specification)

} //namespace glm

#endif //__glm_h__
