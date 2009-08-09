//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_matrix_projection.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_matrix_projection_inl__
#define __gtx_matrix_projection_inl__

#include "gtx_matrix_projection.h"
#if(GLM_INCLUDED(GLM_GTX_matrix_projection, GLM_EXTENSION_GTX, GLM_GTX_matrix_projection_required))

namespace glm
{
    template <typename T> 
    inline _xmat4<T> perspectiveGTX(T fovy, T aspect, T zNear, T zFar)
    {
        T range = glm::tan(glm::radians(fovy / 2)) * zNear;	
        T left = -range * aspect;
        T right = range * aspect;
        T bottom = -range;
        T top = range;

        glm::_xmat4<T> Result(T(0));
        Result[0][0] = (2 * zNear) / (right - left);
        Result[1][1] = (2 * zNear) / (top - bottom);
        Result[2][2] = - (zFar + zNear) / (zFar - zNear);
        Result[2][3] = -1;
        Result[3][2] = - (2 * zFar * zNear) / (zFar - zNear);
        return Result;
    }

    template <typename T> 
    inline _xmat4<T> infinitePerspectiveGTX(T fovy, T aspect, T zNear)
    {
        T range = glm::tan(glm::radians(fovy / 2)) * zNear;	
        T left = -range * aspect;
        T right = range * aspect;
        T bottom = -range;
        T top = range;

        glm::_xmat4<T> Result(T(0));
        Result[0][0] = (2 * zNear) / (right - left);
        Result[1][1] = (2 * zNear) / (top - bottom);
        Result[2][2] = -1;
        Result[2][3] = -1;
        Result[3][2] = - 2 * zNear;
        return Result;
    }

    template <typename T> 
    inline _xmat4<T> tweakedInfinitePerspectiveGTX(T fovy, T aspect, T zNear)
    {
        T range = glm::tan(glm::radians(fovy / 2)) * zNear;	
        T left = -range * aspect;
        T right = range * aspect;
        T bottom = -range;
        T top = range;

        glm::_xmat4<T> Result(T(0));
        Result[0][0] = (2 * zNear) / (right - left);
        Result[1][1] = (2 * zNear) / (top - bottom);
        Result[2][2] = T(0.0001f) -1;
        Result[2][3] = -1;
        Result[3][2] = - (0.0001f - 2) * zNear;
        return Result;
    }

    template <typename T> 
    inline _xmat4<T> ortho2DGTX(T left, T right, T bottom, T top)
    {
        glm::_xmat4<T> Result(1);
        Result[0][0] = 2 / (right - left);
        Result[1][1] = 2 / (top - bottom);
        Result[3][0] = - (right + left) / (right - left);
        Result[3][1] = - (top + bottom) / (top - bottom);
        return Result;
    }

    template <typename T> 
    inline _xmat4<T> ortho3DGTX(T left, T right, T bottom, T top, T zNear, T zFar)
    {
        glm::_xmat4<T> Result(1);
        Result[0][0] = 2 / (right - left);
        Result[1][1] = 2 / (top - bottom);
        Result[2][2] = 2 / (zFar - zNear);
        Result[3][0] = - (right + left) / (right - left);
        Result[3][1] = - (top + bottom) / (top - bottom);
        Result[3][2] = - (zFar + zNear) / (zFar - zNear);
        return Result;
    }

    template <typename T, typename U> 
    _xvec3<T> projectGTX(const _xvec3<T>& obj, const _xmat4<T>& model, const _xmat4<T>& proj, const _xvec4<U>& view)
    {
        glm::_xvec4<T> v = proj * model * glm::_xvec4<T>(obj, 1);
        return glm::_xvec3<T>(T(view.x) + T(view.z) * (v.x + 1) / 2, T(view.y) + T(view.w) * (v.y + 1) / 2, (v.z + 1) / 2);
    }

    template <typename T, typename U> 
    _xvec3<T> unProjectGTX(const _xvec3<T>& win, const _xmat4<T>& model, const _xmat4<T>& proj, const _xvec4<U>& view)
    {
        glm::_xvec4<T> v(2 * (win.x - T(view.x)) / view.z - 1, 2 * (win.y - T(view.y)) / view.w - 1, 2 * win.z - 1, 1);
        return glm::_xvec3<T>(glm::inverseGTX(proj * model) * v);
    }
}

#endif//included

#endif//__gtx_matrix_projection_inl__
