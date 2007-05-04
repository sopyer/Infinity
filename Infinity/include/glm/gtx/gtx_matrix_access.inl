//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-27
// Updated : 2005-12-27
// Licence : This source is under GNU LGPL licence
// File    : gtx_matrix_access.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_matrix_access_inl__
#define __gtx_matrix_access_inl__

#include "gtx_matrix_access.h"
#if(GLM_INCLUDED(GLM_GTX_matrix_access, GLM_EXTENSION_GTX, GLM_GTX_matrix_access_required))

namespace glm
{
    template <typename T>
    inline _xmat2<T> rowGTX(const _xmat2<T>& m, int index, const _xvec2<T>& x)
    {
        _xmat2<T> Result = m;
        Result[0][index] = x[0];
        Result[1][index] = x[1];
        return Result;
    }

    template <typename T>
    inline _xmat3<T> rowGTX(const _xmat3<T>& m, int index, const _xvec3<T>& x)
    {
        _xmat3<T> Result = m;
        Result[0][index] = x[0];
        Result[1][index] = x[1];
        Result[2][index] = x[2];
        return Result;
    }

    template <typename T>
    inline _xmat4<T> rowGTX(const _xmat4<T>& m, int index, const _xvec4<T>& x)
    {
        _xmat4<T> Result = m;
        Result[0][index] = x[0];
        Result[1][index] = x[1];
        Result[2][index] = x[2];
        Result[3][index] = x[3];
        return Result;
    }

    template <typename T>
    inline _xmat2<T> columnGTX(const _xmat2<T> & m, int index, const _xvec2<T>& x)
    {
        _xmat2<T> Result = m;
        Result[index] = x;
        return Result;
    }

    template <typename T>
    inline _xmat3<T> columnGTX(const _xmat3<T> & m, int index, const _xvec3<T>& x)
    {
        _xmat3<T> Result = m;
        Result[index] = x;
        return Result;
    }

    template <typename T>
    inline _xmat4<T> columnGTX(const _xmat4<T> & m, int index, const _xvec4<T>& x)
    {
        _xmat4<T> Result = m;
        Result[index] = x;
        return Result;
    }
}

#endif//included

#endif//__gtx_matrix_access_inl__
