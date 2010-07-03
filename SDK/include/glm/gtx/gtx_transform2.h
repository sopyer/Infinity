//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_transform2.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_transform2_h__
#define __gtx_transform2_h__

#if (GLM_GTX_transform2 == GLM_EXTENSION_DISABLE && defined(GLM_GTX_transform2_required))
#error GLM_GTX_transform2 is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_transform2, GLM_EXTENSION_GTX, GLM_GTX_transform2_required))

//#define GLM_GTX_transform2_supported 

namespace glm
{
    template <typename T> _xmat3<T> shearX2DGTX(const _xmat3<T> & m, T y);
    template <typename T> _xmat3<T> shearY2DGTX(const _xmat3<T> & m, T x);
    template <typename T> _xmat4<T> shearX3DGTX(const _xmat4<T> & m, T y, T z);
    template <typename T> _xmat4<T> shearY3DGTX(const _xmat4<T> & m, T x, T z);
    template <typename T> _xmat4<T> shearZ3DGTX(const _xmat4<T> & m, T x, T y);
    //template <typename T> _xmat4<T> shearGTX(const _xmat4<T> & m, shearPlane, planePoint, angle)
    // Identity + tan(angle) * cross(Normal, OnPlaneVector)     0
    // - dot(PointOnPlane, normal) * OnPlaneVector              1

    // Reflect functions seem to don't work
    template <typename T> _xmat3<T> reflect2DGTX(const _xmat3<T> & m, const _xvec3<T>& normal);
    template <typename T> _xmat4<T> reflect3DGTX(const _xmat4<T> & m, const _xvec3<T>& normal);
    template <typename T> _xmat3<T> proj2DGTX(const _xmat3<T> & m, const _xvec3<T>& normal);
    template <typename T> _xmat4<T> proj3DGTX(const _xmat4<T> & m, const _xvec3<T>& normal);
    template <typename T> _xmat4<T> lookAtGTX(const _xvec3<T>& eye, const _xvec3<T>& center, const _xvec3<T>& up);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat3<T> shearX2D(const _xmat3<T> & m, T y){return shearX2DGTX(m, y);}
    template <typename T> _xmat3<T> shearY2D(const _xmat3<T> & m, T x){return shearY2DGTX(m, x);}
    template <typename T> _xmat4<T> shearX3D(const _xmat4<T> & m, T y, T z){return shearX3DGTX(m, y, z);}
    template <typename T> _xmat4<T> shearY3D(const _xmat4<T> & m, T x, T z){return shearY3DGTX(m, x, z);}
    template <typename T> _xmat4<T> shearZ3D(const _xmat4<T> & m, T x, T y){return shearZ3DGTX(m, x, y);}
    //template <typename T> _xmat4<T> shear(const _xmat4<T> & m, shearPlane, planePoint, angle)
    // Identity + tan(angle) * cross(Normal, OnPlaneVector)     0
    // - dot(PointOnPlane, normal) * OnPlaneVector              1

    // Reflect functions seem to don't work
    template <typename T> _xmat3<T> reflect2D(const _xmat3<T> & m, const _xvec3<T>& normal){return reflect2DGTX(m, normal);}
    template <typename T> _xmat4<T> reflect3D(const _xmat4<T> & m, const _xvec3<T>& normal){return reflect3DGTX(m, normal);}
    template <typename T> _xmat3<T> proj2D(const _xmat3<T> & m, const _xvec3<T>& normal){return proj2DGTX(m, normal);}
    template <typename T> _xmat4<T> proj3D(const _xmat4<T> & m, const _xvec3<T>& normal){return proj3DGTX(m, normal);}
    template <typename T> _xmat4<T> lookAt(const _xvec3<T>& eye, const _xvec3<T>& center, const _xvec3<T>& up){return lookAtGTX(eye, center, up);}

#endif
}

#endif//included

#endif//__gtx_transform2_h__
