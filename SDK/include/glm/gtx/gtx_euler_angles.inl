//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_euler_angles.inl
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GL_GTX_double
//////////////////////////////////////////////////////////////////////////////////
// ToDo:
// - mat2 mat2GTX(const vec2& angles) undefined
// - mat3 mat3GTX(const vec2& angles) undefined
// - Add functions to double numbers
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_euler_angles_inl__
#define __gtx_euler_angles_inl__

#include "gtx_euler_angles.h"
#if(GLM_INCLUDED(GLM_GTX_euler_angles, GLM_EXTENSION_GTX, GLM_GTX_euler_angles_required))

namespace glm
{
    template <typename T> 
	inline _xmat2<T> mat2GTX(const T angle)
	{
        T c = glm::cos(angle);
        T s = glm::sin(angle);

		_xmat3<T> Result;
        Result[0][0] = c;
        Result[0][1] = s;
        Result[1][0] = -s;
        Result[1][1] = c;
		return Result;
	}

    template <typename T> 
	inline _xmat3<T> mat3GTX(const T angle)
	{
        T c = glm::cos(angle);
        T s = glm::sin(angle);

		_xmat3<T> Result;
        Result[0][0] = c;
        Result[0][1] = s;
		Result[0][2] = 0.0f;
        Result[1][0] = -s;
        Result[1][1] = c;
		Result[1][2] = 0.0f;
		Result[2][0] = 0.0f;
		Result[2][1] = 0.0f;
		Result[2][2] = 1.0f;
		return Result;
	}

	template <typename T>
    inline _xmat3<T> mat3GTX(const _xvec3<T>& angles)
    {
        T tmp_ch = glm::cos(angles.x);
        T tmp_sh = glm::sin(angles.x);
        T tmp_cp = glm::cos(angles.y);
        T tmp_sp = glm::sin(angles.y);
        T tmp_cb = glm::cos(angles.z);
        T tmp_sb = glm::sin(angles.z);

        _xmat3<T> Result;
        Result[0][0] = tmp_ch * tmp_cb + tmp_sh * tmp_sp * tmp_sb;
        Result[0][1] = tmp_sb * tmp_cp;
        Result[0][2] = -tmp_sh * tmp_cb + tmp_ch * tmp_sp * tmp_sb;
        Result[1][0] = -tmp_ch * tmp_sb + tmp_sh * tmp_sp * tmp_cb;
        Result[1][1] = tmp_cb * tmp_cp;
        Result[1][2] = tmp_sb * tmp_sh + tmp_ch * tmp_sp * tmp_cb;
        Result[2][0] = tmp_sh * tmp_cp;
        Result[2][1] = -tmp_sp;
        Result[2][2] = tmp_ch * tmp_cp;
        return Result;
    }

	template <typename T>
    inline _xmat4<T> mat4GTX(const _xvec3<T>& angles)
    {
        T tmp_ch = glm::cos(angles.x);
        T tmp_sh = glm::sin(angles.x);
        T tmp_cp = glm::cos(angles.y);
        T tmp_sp = glm::sin(angles.y);
        T tmp_cb = glm::cos(angles.z);
        T tmp_sb = glm::sin(angles.z);

        _xmat4<T> Result;
        Result[0][0] = tmp_ch * tmp_cb + tmp_sh * tmp_sp * tmp_sb;
        Result[0][1] = tmp_sb * tmp_cp;
        Result[0][2] = -tmp_sh * tmp_cb + tmp_ch * tmp_sp * tmp_sb;
        Result[0][3] = T(0);
        Result[1][0] = -tmp_ch * tmp_sb + tmp_sh * tmp_sp * tmp_cb;
        Result[1][1] = tmp_cb * tmp_cp;
        Result[1][2] = tmp_sb * tmp_sh + tmp_ch * tmp_sp * tmp_cb;
        Result[1][3] = T(0);
        Result[2][0] = tmp_sh * tmp_cp;
        Result[2][1] = -tmp_sp;
        Result[2][2] = tmp_ch * tmp_cp;
        Result[2][3] = T(0);
        Result[3][0] = T(0);
        Result[3][1] = T(0);
        Result[3][2] = T(0);
        Result[3][3] = T(1);
        return Result;
    }
}

#endif//included

#endif//__gtx_euler_angles_inl__
