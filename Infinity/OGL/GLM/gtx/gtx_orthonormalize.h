//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_orthonormalize.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_orthonormalize_h__
#define __gtx_orthonormalize_h__

#if (GLM_GTX_orthonormalize == GLM_EXTENSION_DISABLE && defined(GLM_GTX_orthonormalize_required))
#error GLM_GTX_orthonormalize is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_orthonormalize, GLM_EXTENSION_GTX, GLM_GTX_orthonormalize_required))

#define GLM_GTX_orthonormalize_supported 

namespace glm
{
    template <typename T> _xmat3<T> orthonormalizeGTX(const _xmat3<T>& m); //!< \brief Returns the orthonormalized matrix of m (from GLM_GTX_orthonormalize extension)
    template <typename T> _xvec3<T> orthonormalizeGTX(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Orthonormalizes x according y (from GLM_GTX_orthonormalize extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat3<T> orthonormalize(const _xmat3<T>& m){return orthonormalizeGTX(m);}
    template <typename T> _xvec3<T> orthonormalize(const _xvec3<T>& x, const _xvec3<T>& y){return orthonormalizeGTX(x, y);}

#endif
}

#endif//included

#endif//__gtx_orthonormalize_h__
