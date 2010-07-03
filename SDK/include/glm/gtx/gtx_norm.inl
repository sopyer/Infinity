//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_norm.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_norm_inl__
#define __gtx_norm_inl__

#include "gtx_norm.h"
#if(GLM_INCLUDED(GLM_GTX_norm, GLM_EXTENSION_GTX, GLM_GTX_norm_required))

namespace glm
{
	template <typename T>
    inline T length2GTX(const T x)
    {
        return x * x;
    }

    template <typename T>
    inline T length2GTX(const _xvec2<T>& x)
    {
        return glm::dot(x, x);
    }

    template <typename T>
    inline T length2GTX(const _xvec3<T>& x)
    {
        return glm::dot(x, x);
    }

    template <typename T>
    inline T length2GTX(const _xvec4<T>& x)
    {
        return glm::dot(x, x);
    }

    template <typename T> 
    inline T length2GTX(const _xquat<T>& q)
    {
        return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    }

    template <typename T> 
    inline T l1NormGTX(const _xvec3<T>& a, const _xvec3<T>& b)
    {
        return glm::abs(b.x - a.x) + glm::abs(b.y - a.y) + glm::abs(b.z - a.z);
    }
}

#endif//included

#endif//__gtx_norm_inl__
