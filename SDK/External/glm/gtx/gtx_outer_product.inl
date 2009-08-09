//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-03-30
// Updated : 2006-03-30
// Licence : This source is under GNU LGPL licence
// File    : gtx_outer_product.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_outer_product_inl__
#define __gtx_outer_product_inl__

#include "gtx_outer_product.h"
#if(GLM_INCLUDED(GLM_GTX_outer_product, GLM_EXTENSION_GTX, GLM_GTX_outer_product_required))

namespace glm
{
    template <typename T> 
    inline _xmat2<T> outerProductGTX(const _xvec2<T>& a, const _xvec2<T>& b)
    {
		_xmat2<T> m;
		m[0][0] = a.x * b.x;
		m[0][1] = a.x * b.y;
		m[1][0] = a.y * b.x;
		m[1][1] = a.y * b.y;
        return m;
    }

    template <typename T> 
    inline _xmat3<T> outerProductGTX(const _xvec3<T>& a, const _xvec3<T>& b)
    {
		_xmat3<T> m;
		m[0][0] = a.x * b.x;
		m[0][1] = a.x * b.y;
		m[0][2] = a.x * b.z;
		m[1][0] = a.y * b.x;
		m[1][1] = a.y * b.y;
		m[1][2] = a.y * b.z;
		m[2][0] = a.z * b.x;
		m[2][1] = a.z * b.y;
		m[2][2] = a.z * b.z;
        return m;
    }

    template <typename T> 
    inline _xmat4<T> outerProductGTX(const _xvec4<T>& a, const _xvec4<T>& b)
    {
		_xmat4<T> m;
		m[0][0] = a.x * b.x;
		m[0][1] = a.x * b.y;
		m[0][2] = a.x * b.z;
		m[0][3] = T(0);
		m[1][0] = a.y * b.x;
		m[1][1] = a.y * b.y;
		m[1][2] = a.y * b.z;
		m[1][3] = T(0);
		m[2][0] = a.z * b.x;
		m[2][1] = a.z * b.y;
		m[2][2] = a.z * b.z;
		m[2][3] = T(0);
		m[3] = _xvec4<T>(T(0), T(0), T(0), T(1));
        return m;
    }
}

#endif//included

#endif//__gtx_outer_product_inl__
