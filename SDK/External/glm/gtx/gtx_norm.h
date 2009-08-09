//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_norm.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
// - GLM_GTX_quaternion
// - GLM_GTX_double
//////////////////////////////////////////////////////////////////////////////////
// ToDo:
// - Study the validity of the notion of length2 to quaternion
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_norm_h__
#define __gtx_norm_h__

#if (GLM_GTX_norm == GLM_EXTENSION_DISABLE && defined(GLM_GTX_norm_required))
#error GLM_GTX_norm is disable but required
#endif

// Dependency:
#include "../glm.h"
#define GLM_GTX_double_required 1
#include "../gtx/gtx_double.h"
#define GLM_GTX_quaternion_required 1
#include "../gtx/gtx_quaternion.h"

#if(GLM_INCLUDED(GLM_GTX_norm, GLM_EXTENSION_GTX, GLM_GTX_norm_required))

//#define GLM_GTX_norm_supported 

namespace glm
{
    template <typename T> T length2GTX(const T x); //!< \brief Returns the squared length of x (from GLM_GTX_norm extension)
    template <typename T> T length2GTX(const _xvec2<T>& x); //!< \brief Returns the squared length of x (from GLM_GTX_norm extension)
    template <typename T> T length2GTX(const _xvec3<T>& x); //!< \brief Returns the squared length of x (from GLM_GTX_norm extension)
    template <typename T> T length2GTX(const _xvec4<T>& x); //!< \brief Returns the squared length of x (from GLM_GTX_norm extension)
    template <typename T> T length2GTX(const _xquat<T>& q); //!< \brief Returns the squared length of x (from GLM_GTX_norm extension)
    template <typename T> T l1NormGTX(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Returns the L1 norm between x and y (from GLM_GTX_norm extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> T length2(const T x){return length2GTX(x);}
    template <typename T> T length2(const _xvec2<T>& x){return length2GTX(x);}
    template <typename T> T length2(const _xvec3<T>& x){return length2GTX(x);}
    template <typename T> T length2(const _xvec4<T>& x){return length2GTX(x);}
    template <typename T> T length2(const _xquat<T>& q){return length2GTX(q);}
    template <typename T> T l1Norm(const _xvec3<T>& x, const _xvec3<T>& y){return l1NormGTX(x, y);}

#endif
}

#endif//included

#endif//__gtx_norm_h__
