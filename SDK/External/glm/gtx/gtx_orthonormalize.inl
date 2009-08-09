//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_orthonormalize.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_orthonormalize_inl__
#define __gtx_orthonormalize_inl__

#include "gtx_orthonormalize.h"
#if(GLM_INCLUDED(GLM_GTX_orthonormalize, GLM_EXTENSION_GTX, GLM_GTX_orthonormalize_required))

namespace glm
{
    template <typename T>
    inline _xmat3<T> orthonormalizeGTX(const _xmat3<T>& m)
    {
        glm::_xmat3<T> r = m;

        r[0] = glm::normalize(r[0]);

	    float d0 = glm::dot(r[0], r[1]);
	    r[1] -= r[0] * d0;
	    r[1] = glm::normalize(r[1]);

	    float d1 = glm::dot(r[1], r[2]);
	    d0 = glm::dot(r[0], r[2]);
	    r[2] -= r[0] * d0 + r[1] * d1;
	    r[2] = glm::normalize(r[2]);

        return r;
    }

    template <typename T> 
    inline _xvec3<T> orthonormalizeGTX(const _xvec3<T>& x, const _xvec3<T>& y)
    {
        return glm::normalize(x - y * glm::dot(y, x));
    }
}

#endif//included

#endif//__gtx_orthonormalize_inl__
