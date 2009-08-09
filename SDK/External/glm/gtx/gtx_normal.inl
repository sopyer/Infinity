//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_normal.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_normal_inl__
#define __gtx_normal_inl__

#include "gtx_normal.h"
#if(GLM_INCLUDED(GLM_GTX_normal, GLM_EXTENSION_GTX, GLM_GTX_normal_required))

namespace glm
{
    template <typename T> 
    inline _xvec3<T> triangleNormalGTX(const _xvec3<T>& p1, const _xvec3<T>& p2, const _xvec3<T>& p3)
    {
        return glm::normalize(glm::cross(p1 - p2, p1 - p3));
    }
}

#endif//included

#endif//__gtx_normal_inl__
