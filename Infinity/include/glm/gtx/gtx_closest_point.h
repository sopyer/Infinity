//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-30
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_closest_point.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_closest_point_h__
#define __gtx_closest_point_h__

#if (GLM_GTX_closest_point == GLM_EXTENSION_DISABLE && defined(GLM_GTX_closest_point_required))
#error GLM_GTX_closest_point is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_closest_point, GLM_EXTENSION_GTX, GLM_GTX_closest_point_required))

#define GLM_GTX_closest_point_supported 

namespace glm
{
    template <typename T> inline _xvec3<T> closestPointOnLine(const _xvec3<T>& point, const _xvec3<T>& a, const _xvec3<T>& b); //!< Finds the position on straight line (A, B) witch is the closest of point. (from GLM_GTX_closest_point extension) 
}

#endif//included

#endif//__gtx_closest_point_h__
