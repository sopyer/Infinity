//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-11-02
// Updated : 2006-11-02
// Licence : This source is under GNU LGPL licence
// File    : gtx_rotate_vector.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_rotate_vector_inl__
#define __gtx_rotate_vector_inl__

#include "gtx_rotate_vector.h"
#include "gtx_transform.h"
#if(GLM_INCLUDED(GLM_GTX_rotate_vector, GLM_EXTENSION_GTX, GLM_GTX_rotate_vector_required))

namespace glm
{
	template <typename T>
	inline _xvec2<T> rotateGTX(const _xvec2<T>& v, T angle)
	{
		_xvec2<T> Result;
		const T Cos = glm::cos(glm::radians(angle));
        const T Sin = glm::sin(glm::radians(angle));
		Result.x = v.x * Cos - v.y * Sin;
		Result.y = v.x * Sin + v.y * Cos;
		return Result;
	};

	template <typename T> 
	inline _xvec3<T> rotateGTX(const _xvec3<T>& v, T angle, const _xvec3<T>& normal)
	{
		return rotateGTX(angle, normal) * v;
	}
/*
    template <typename T> 
    inline _xvec3<T> rotateGTX(const _xvec3<T>& x, T angle, const _xvec3<T>& normal)
    {
        const T Cos = cos(radians(angle));
        const T Sin = sin(radians(angle));
        return x * Cos + ((x * normal) * (T(1) - Cos)) * normal + glm::cross(x, normal) * Sin;
    }
*/
	template <typename T> 
	inline _xvec4<T> rotateGTX(const _xvec4<T>& v, T angle, const _xvec3<T>& normal)
	{
		return rotateGTX(angle, normal) * v;
	}

	template <typename T>
	inline _xvec3<T> rotateXGTX(const _xvec3<T>& v, T angle)
	{
		_xvec3<T> Result = v;
		const T Cos = glm::cos(glm::radians(angle));
        const T Sin = glm::sin(glm::radians(angle));
		Result.y = v.y * Cos - v.z * Sin;
		Result.z = v.y * Sin + v.z * Cos;
		return Result;
	}

	template <typename T>
	inline _xvec3<T> rotateYGTX(const _xvec3<T>& v, T angle)
	{
		_xvec3<T> Result = v;
		const T Cos = glm::cos(glm::radians(angle));
        const T Sin = glm::sin(glm::radians(angle));
		Result.x =  v.x * Cos + v.z * Sin;
		Result.z = -v.x * Sin + v.z * Cos;
		return Result;
	}

	template <typename T>
	inline _xvec3<T> rotateZGTX(const _xvec3<T>& v, T angle)
	{
		_xvec3<T> Result = v;
		const T Cos = glm::cos(glm::radians(angle));
        const T Sin = glm::sin(glm::radians(angle));
		Result.x = v.x * Cos - v.y * Sin;
		Result.y = v.x * Sin + v.y * Cos;
		return Result;
	};

	template <typename T>
	inline _xvec4<T> rotateXGTX(const _xvec4<T>& v, T angle)
	{
		_xvec4<T> Result = v;
		const T Cos = glm::cos(glm::radians(angle));
        const T Sin = glm::sin(glm::radians(angle));
		Result.y = v.y * Cos - v.z * Sin;
		Result.z = v.y * Sin + v.z * Cos;
		return Result;
	}

	template <typename T>
	inline _xvec4<T> rotateYGTX(const _xvec4<T>& v, T angle)
	{
		_xvec4<T> Result = v;
		const T Cos = glm::cos(glm::radians(angle));
        const T Sin = glm::sin(glm::radians(angle));
		Result.x =  v.x * Cos + v.z * Sin;
		Result.z = -v.x * Sin + v.z * Cos;
		return Result;
	}

	template <typename T>
	inline _xvec4<T> rotateZGTX(const _xvec4<T>& v, T angle)
	{
		_xvec4<T> Result = v;
		const T Cos = glm::cos(glm::radians(angle));
        const T Sin = glm::sin(glm::radians(angle));
		Result.x = v.x * Cos - v.y * Sin;
		Result.y = v.x * Sin + v.y * Cos;
		return Result;
	};

	template <typename T>
	inline _xmat4<T> orientationGTX(const _xvec3<T>& Normal, const _xvec3<T>& Up)
	{
		if(glm::all(glm::equal(Normal, Up)))
			return _xmat4<T>(T(1));

		_xvec3<T> RotationAxis = glm::cross(Up, Normal);
		T Angle = glm::degrees(acos(glm::dot(Normal, Up)));
		return glm::rotateGTX(Angle, RotationAxis);
	}
}

#endif//included

#endif//__gtx_rotate_vector_inl__
