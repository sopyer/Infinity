//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_inertia.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_inertia_h__
#define __gtx_inertia_h__

#if (GLM_GTX_inertia == GLM_EXTENSION_DISABLE && defined(GLM_GTX_inertia_required))
#error GLM_GTX_inertia is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_inertia, GLM_EXTENSION_GTX, GLM_GTX_inertia_required))

#define GLM_GTX_inertia_supported 

namespace glm
{
    template <typename T> _xmat3<T> boxInertia3GTX(const T Mass, const _xvec3<T>& Scale);
    template <typename T> _xmat4<T> boxInertia4GTX(const T Mass, const _xvec3<T>& Scale);
    template <typename T> _xmat3<T> diskInertia3GTX(const T Mass, const T Radius);
    template <typename T> _xmat4<T> diskInertia4GTX(const T Mass, const T Radius);
    template <typename T> _xmat3<T> ballInertia3GTX(const T Mass, const T Radius);
    template <typename T> _xmat4<T> ballInertia4GTX(const T Mass, const T Radius);
    template <typename T> _xmat3<T> sphereInertia3GTX(const T Mass, const T Radius);
    template <typename T> _xmat4<T> sphereInertia4GTX(const T Mass, const T Radius);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat3<T> boxInertia3(const T Mass, const _xvec3<T>& Scale){return boxInertia3GTX(Mass, Scale);}
    template <typename T> _xmat4<T> boxInertia4(const T Mass, const _xvec3<T>& Scale){return boxInertia$GTX(Mass, Scale);}
    template <typename T> _xmat3<T> diskInertia3(const T Mass, const T Radius){return diskInertia3GTX(Mass, Radius);}
    template <typename T> _xmat4<T> diskInertia4(const T Mass, const T Radius){return diskInertia4GTX(Mass, Radius);}
    template <typename T> _xmat3<T> ballInertia3(const T Mass, const T Radius){return ballInertia3GTX(Mass, Radius);}
    template <typename T> _xmat4<T> ballInertia4(const T Mass, const T Radius){return ballInertia4GTX(Mass, Radius);}
    template <typename T> _xmat3<T> sphereInertia3(const T Mass, const T Radius){return sphereInertia3GTX(Mass, Radius);}
    template <typename T> _xmat4<T> sphereInertia4(const T Mass, const T Radius){return sphereInertia4GTX(Mass, Radius);}

#endif
}

#endif//included

#endif//__gtx_inertia_h__
