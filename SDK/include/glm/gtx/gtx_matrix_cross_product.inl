//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_matrix_cross_product.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_matrix_cross_product_inl__
#define __gtx_matrix_cross_product_inl__

#include "gtx_matrix_cross_product.h"
#if(GLM_INCLUDED(GLM_GTX_matrix_cross_product, GLM_EXTENSION_GTX, GLM_GTX_matrix_cross_product_required))

namespace glm
{
    template <typename T> 
    inline _xmat3<T> matrixCross3GTX(const _xvec3<T>& x)
    {
        _xmat3<T> Result(T(0));
        Result[0][1] = x.z;
        Result[1][0] = -x.z;
        Result[0][2] = -x.y;
        Result[2][0] = x.y;
        Result[1][2] = x.x;
        Result[2][1] = -x.x;
        return Result;
    }

    template <typename T> 
    inline _xmat4<T> matrixCross4GTX(const _xvec3<T>& x)
    {
        _xmat4<T> Result(T(0));
        Result[0][1] = x.z;
        Result[1][0] = -x.z;
        Result[0][2] = -x.y;
        Result[2][0] = x.y;
        Result[1][2] = x.x;
        Result[2][1] = -x.x;
        return Result;
    }
}

#endif//included

#endif//__gtx_matrix_cross_product_inl__
