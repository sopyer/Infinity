//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-12-06
// Licence : This source is under GNU LGPL licence
// File    : gtx_handed_coordinate_space.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_handed_coordinate_space_h__
#define __gtx_handed_coordinate_space_h__

#if (GLM_GTX_handed_coordinate_space == GLM_EXTENSION_DISABLE && defined(GLM_GTX_handed_coordinate_space_required))
#error GLM_GTX_handed_coordinate_space is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_handed_coordinate_space, GLM_EXTENSION_GTX, GLM_GTX_handed_coordinate_space_required))

#define GLM_GTX_handed_coordinate_space_supported 

namespace glm
{
    template <typename T> bool rightHandedGTX(const _xvec3<T>& tangent, const _xvec3<T>& binormal, const _xvec3<T>& normal);
    template <typename T> bool leftHandedGTX(const _xvec3<T>& tangent, const _xvec3<T>& binormal, const _xvec3<T>& normal);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> bool rightHanded(const _xvec3<T>& tangent, const _xvec3<T>& binormal, const _xvec3<T>& normal){return rightHandedGTX(tangent, binormal, normal);}
    template <typename T> bool leftHanded(const _xvec3<T>& tangent, const _xvec3<T>& binormal, const _xvec3<T>& normal){return leftHandedGTX(tangent, binormal, normal);}

#endif
}

#endif//included

#endif//__gtx_handed_coordinate_space_h__
