//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-12-06
// Licence : This source is under GNU LGPL licence
// File    : gtx_perpendicular.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_perpendicular_h__
#define __gtx_perpendicular_h__

#if (GLM_GTX_perpendicular == GLM_EXTENSION_DISABLE && defined(GLM_GTX_perpendicular_required))
#error GLM_GTX_perpendicular is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_perpendicular, GLM_EXTENSION_GTX, GLM_GTX_perpendicular_required))

#define GLM_GTX_perpendicular_supported 

namespace glm
{
    template <typename T> _xvec2<T> perpGTX(const _xvec2<T>& x, const _xvec2<T>& Normal); //!< \brief Projects x a perpendicular axis of Normal (from GLM_GTX_perpendicular extension)
    template <typename T> _xvec3<T> perpGTX(const _xvec3<T>& x, const _xvec3<T>& Normal); //!< \brief Projects x a perpendicular axis of Normal (from GLM_GTX_perpendicular extension)
    template <typename T> _xvec4<T> perpGTX(const _xvec4<T>& x, const _xvec4<T>& Normal); //!< \brief Projects x a perpendicular axis of Normal (from GLM_GTX_perpendicular extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xvec2<T> perp(const _xvec2<T>& x, const _xvec2<T>& Normal){return perpGTX(x, Normal);}
    template <typename T> _xvec3<T> perp(const _xvec3<T>& x, const _xvec3<T>& Normal){return perpGTX(x, Normal);}
    template <typename T> _xvec4<T> perp(const _xvec4<T>& x, const _xvec4<T>& Normal){return perpGTX(x, Normal);}

#endif
}

#endif//included

#endif//__gtx_perpendicular_h__
