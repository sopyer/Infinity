//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-20
// Updated : 2006-04-20
// Licence : This source is under GNU LGPL licence
// File    : gtx_mul.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_mul_inl__
#define __gtx_mul_inl__

#include "gtx_mul.h"
#if(GLM_INCLUDED(GLM_GTX_mul, GLM_EXTENSION_GTX, GLM_GTX_mul_required))

namespace glm
{
    template <typename T>
    inline _xmat2<T> mulGTX(const _xmat2<T>& a, const _xmat2<T>& b)
    {
        return a * b;
    }

    template <typename T>
    inline _xmat3<T> mulGTX(const _xmat3<T>& a, const _xmat3<T>& b)
    {
        return a * b;
    }

    template <typename T>
    inline _xmat4<T> mulGTX(const _xmat4<T>& a, const _xmat4<T>& b)
    {
        return a * b;
    }

    template <typename T>
    inline _xvec2<T> mulGTX(const _xmat2<T>& m, const _xvec2<T>& v)
    {
        return m * v;
    }

    template <typename T>
    inline _xvec3<T> mulGTX(const _xmat3<T>& m, const _xvec3<T>& v)
    {
        return m * v;
    }

    template <typename T>
    inline _xvec4<T> mulGTX(const _xmat4<T>& m, const _xvec4<T>& v)
    {
        return m * v;
    }

    template <typename T>
    inline _xvec2<T> mulGTX(const _xvec2<T>& v, const _xmat2<T>& m)
    {
        return v * m;
    }

    template <typename T>
    inline _xvec3<T> mulGTX(const _xvec3<T>& v, const _xmat3<T>& m)
    {
        return v * m;
    }

    template <typename T>
    inline _xvec4<T> mulGTX(const _xvec4<T>& v, const _xmat4<T>& m)
    {
        return v * m;
    }
}

#endif//included

#endif//__gtx_mul_inl__
