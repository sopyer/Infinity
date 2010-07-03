//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-19
// Updated : 2006-04-19
// Licence : This source is under GNU LGPL licence
// File    : gtx_matrix_major_storage.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_matrix_major_storage_inl__
#define __gtx_matrix_major_storage_inl__

#include "gtx_matrix_major_storage.h"
#if(GLM_INCLUDED(GLM_GTX_matrix_major_storage, GLM_EXTENSION_GTX, GLM_GTX_matrix_major_storage_required))

namespace glm
{
    template <typename T> 
    inline _xmat2<T> rowMajor2GTX(const _xvec2<T>& v1, const _xvec2<T>& v2)
    {
        _xmat2<T> Result;
        Result[0][0] = v1.x;
        Result[1][0] = v1.y;
        Result[0][1] = v2.x;
        Result[1][1] = v2.y;
        return Result;
    }

    template <typename T> 
    inline _xmat2<T> rowMajor2GTX(const _xmat2<T>& m)
    {
        _xmat2<T> Result;
        Result[0][0] = m[0][0];
        Result[0][1] = m[1][0];
        Result[1][0] = m[0][1];
        Result[1][1] = m[1][1];
        return Result;
    }

    template <typename T> 
    inline _xmat3<T> rowMajor3GTX(const _xvec3<T>& v1, const _xvec3<T>& v2, const _xvec3<T>& v3)
    {
        _xmat3<T> Result;
        Result[0][0] = v1.x;
        Result[1][0] = v1.y;
        Result[2][0] = v1.z;
        Result[0][1] = v2.x;
        Result[1][1] = v2.y;
        Result[2][1] = v2.z;
        Result[0][2] = v3.x;
        Result[1][2] = v3.y;
        Result[2][2] = v3.z;
        return Result;
    }

    template <typename T> 
    inline _xmat3<T> rowMajor3GTX(const _xmat3<T>& m)
    {
        _xmat3<T> Result;
        Result[0][0] = m[0][0];
        Result[0][1] = m[1][0];
        Result[0][2] = m[2][0];
        Result[1][0] = m[0][1];
        Result[1][1] = m[1][1];
        Result[1][2] = m[2][1];
        Result[2][0] = m[0][2];
        Result[2][1] = m[1][2];
        Result[2][2] = m[2][2];
        return Result;
    }

    template <typename T> 
    inline _xmat4<T> rowMajor4GTX(const _xvec4<T>& v1, const _xvec4<T>& v2, const _xvec4<T>& v3, const _xvec4<T>& v4)
    {
        _xmat4<T> Result;
        Result[0][0] = v1.x;
        Result[1][0] = v1.y;
        Result[2][0] = v1.z;
        Result[3][0] = v1.w;
        Result[0][1] = v2.x;
        Result[1][1] = v2.y;
        Result[2][1] = v2.z;
        Result[3][1] = v2.w;
        Result[0][2] = v3.x;
        Result[1][2] = v3.y;
        Result[2][2] = v3.z;
        Result[3][2] = v3.w;
        Result[0][3] = v4.x;
        Result[1][3] = v4.y;
        Result[2][3] = v4.z;
        Result[3][3] = v4.w;
        return Result;
    }

    template <typename T> 
    inline _xmat2<T> rowMajor4GTX(const _xmat4<T>& m)
    {
        _xmat2<T> Result;
        Result[0][0] = m[0][0];
        Result[0][1] = m[1][0];
        Result[0][2] = m[2][0];
        Result[0][3] = m[3][0];
        Result[1][0] = m[0][1];
        Result[1][1] = m[1][1];
        Result[1][2] = m[2][1];
        Result[1][3] = m[3][1];
        Result[2][0] = m[0][2];
        Result[2][1] = m[1][2];
        Result[2][2] = m[2][2];
        Result[2][3] = m[3][2];
        Result[3][0] = m[0][3];
        Result[3][1] = m[1][3];
        Result[3][2] = m[2][3];
        Result[3][3] = m[3][3];
        return Result;
    }

    template <typename T> 
    inline _xmat2<T> colMajor2GTX(const _xvec2<T>& v1, const _xvec2<T>& v2)
    {
        return _xmat2<T>(v1, v2);
    }

    template <typename T> 
    inline _xmat2<T> colMajor2GTX(const _xmat2<T>& m)
    {
        return _xmat2<T>(m);
    }

    template <typename T> 
    inline _xmat3<T> colMajor3GTX(const _xvec3<T>& v1, const _xvec3<T>& v2, const _xvec3<T>& v3)
    {
        return _xmat2<T>(v1, v2, v3);
    }

    template <typename T> 
    inline _xmat3<T> colMajor3GTX(const _xmat3<T>& m)
    {
        return _xmat3<T>(m);
    }

    template <typename T> 
    inline _xmat4<T> colMajor4GTX(const _xvec4<T>& v1, const _xvec4<T>& v2, const _xvec4<T>& v3, const _xvec4<T>& v4)
    {
        return _xmat2<T>(v1, v2, v3, v4);
    }

    template <typename T> 
    inline _xmat4<T> colMajor4GTX(const _xmat4<T>& m)
    {
        return _xmat4<T>(m);
    }
}

#endif//included

#endif//__gtx_matrix_major_storage_inl__
