//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_vector_comp_mult.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_vector_comp_mult_inl__
#define __gtx_vector_comp_mult_inl__

#include "gtx_vector_comp_mult.h"
#if(GLM_INCLUDED(GLM_GTX_vector_comp_mult, GLM_EXTENSION_GTX, GLM_GTX_vector_comp_mult_required))

namespace glm
{
    template <typename T> 
    inline _xvec2<T> vectorCompMultGTX(const _xmat2<T>& m, const _xvec2<T>& v)
    {
        return _xvec2<T>(
            m[0][0] * v.x + m[0][1] * v.y,
            m[1][0] * v.x + m[1][1] * v.y);
    }

    template <typename T> 
    inline _xvec3<T> vectorCompMultGTX(const _xmat3<T>& m, const _xvec3<T>& v)
    {
        return _xvec3<T>(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
    }

    template <typename T> 
    inline _xvec4<T> vectorCompMultGTX(const _xmat4<T>& m, const _xvec4<T>& v)
    {
        return _xvec4<T>(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
            m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w);
    }
}

#endif//included

#endif//__gtx_vector_comp_mult_inl__
