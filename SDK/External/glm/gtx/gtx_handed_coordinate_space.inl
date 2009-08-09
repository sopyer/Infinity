//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_handed_coordinate_space.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_handed_coordinate_space_inl__
#define __gtx_handed_coordinate_space_inl__

#include "gtx_handed_coordinate_space.h"
#if(GLM_INCLUDED(GLM_GTX_handed_coordinate_space, GLM_EXTENSION_GTX, GLM_GTX_handed_coordinate_space_required))

namespace glm
{
    template <typename T> 
    inline bool rightHandedGTX(const _xvec3<T>& tangent, const _xvec3<T>& binormal, const _xvec3<T>& normal)
    {
        return glm::dot(glm::cross(normal, tangent), binormal) > T(0);
    }

    template <typename T> 
    inline bool leftHandedGTX(const _xvec3<T>& tangent, const _xvec3<T>& binormal, const _xvec3<T>& normal)
    {
        return glm::dot(glm::cross(normal, tangent), binormal) < T(0);
    }
}

#endif//included

#endif//__gtx_handed_coordinate_space_inl__
