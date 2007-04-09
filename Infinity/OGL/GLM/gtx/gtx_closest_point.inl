//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-30
// Updated : 2006-01-04
// Licence : This source is under GNU LGPL licence
// File    : gtx_closest_point.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_closest_point_inl__
#define __gtx_closest_point_inl__

#include "gtx_closest_point.h"
#if(GLM_INCLUDED(GLM_GTX_closest_point, GLM_EXTENSION_GTX, GLM_GTX_closest_point_required))

namespace glm
{
    template <typename T> 
    inline _xvec3<T> closestPointOnLine(const _xvec3<T>& point, const _xvec3<T>& a, const _xvec3<T>& b)
    {
        T LineLength = glm::distance(a, b);
        glm::_xvec3<T> Vector = point - a;
        glm::_xvec3<T> LineDirection = (b - a) / LineLength;

        // Project Vector to LineDirection to get the distance of point from a
        T Distance = glm::dot(Vector, LineDirection);

        if(Distance <= 0) return a;
        if(Distance >= LineLength) return b;
        return a + LineDirection * Distance;
    }
}

#endif//included

#endif//__gtx_closest_point_inl__
