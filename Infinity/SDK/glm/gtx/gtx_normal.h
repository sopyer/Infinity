//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_normal.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_normal_h__
#define __gtx_normal_h__

#if (GLM_GTX_normal == GLM_EXTENSION_DISABLE && defined(GLM_GTX_normal_required))
#error GLM_GTX_normal is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_normal, GLM_EXTENSION_GTX, GLM_GTX_normal_required))

#define GLM_GTX_normal_supported 

namespace glm
{
    template <typename T> _xvec3<T> triangleNormalGTX(const _xvec3<T>& p1, const _xvec3<T>& p2, const _xvec3<T>& p3); //!< /brief Computes triangle normal from triangle points. (from GLM_GTX_normal extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xvec3<T> triangleNormal(const _xvec3<T>& p1, const _xvec3<T>& p2, const _xvec3<T>& p3){return triangleNormalGTX(p1, p2, p3);}

#endif
}

#endif//included

#endif//__gtx_normal_h__
