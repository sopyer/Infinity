//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_transpose.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_transpose_inl__
#define __gtx_transpose_inl__

#include "gtx_transpose.h"
#if(GLM_INCLUDED(GLM_GTX_transpose, GLM_EXTENSION_GTX, GLM_GTX_transpose_required))

namespace glm
{
    template <typename T>
    inline _xmat2<T> transposeGTX(const _xmat2<T>& m)
    {
        _xmat2<T> Transpose;
        Transpose[0][0] = m[0][0];
        Transpose[0][1] = m[1][0];
        Transpose[1][0] = m[0][1];
        Transpose[1][1] = m[1][1];
        return Transpose;
    }

    template <typename T>
    inline _xmat3<T> transposeGTX(const _xmat3<T> & m)
    {
        _xmat3<T> Transpose;
        Transpose[0][0] = m[0][0];
        Transpose[0][1] = m[1][0];
        Transpose[0][2] = m[2][0];

        Transpose[1][0] = m[0][1];
        Transpose[1][1] = m[1][1];
        Transpose[1][2] = m[2][1];

        Transpose[2][0] = m[0][2];
        Transpose[2][1] = m[1][2];
        Transpose[2][2] = m[2][2];
        return Transpose;
    }

    template <typename T>
    inline _xmat4<T> transposeGTX(const _xmat4<T> & m)
    {
        _xmat4<T> Transpose;
        Transpose[0][0] = m[0][0];
        Transpose[0][1] = m[1][0];
        Transpose[0][2] = m[2][0];
        Transpose[0][3] = m[3][0];

        Transpose[1][0] = m[0][1];
        Transpose[1][1] = m[1][1];
        Transpose[1][2] = m[2][1];
        Transpose[1][3] = m[3][1];

        Transpose[2][0] = m[0][2];
        Transpose[2][1] = m[1][2];
        Transpose[2][2] = m[2][2];
        Transpose[2][3] = m[3][2];

        Transpose[3][0] = m[0][3];
        Transpose[3][1] = m[1][3];
        Transpose[3][2] = m[2][3];
        Transpose[3][3] = m[3][3];
        return Transpose;
    }
}

#endif//included

#endif//__gtx_transpose_inl__
