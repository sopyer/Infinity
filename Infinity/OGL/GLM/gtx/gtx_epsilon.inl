//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-01-16
// Licence : This source is under GNU LGPL licence
// File    : gtx_epsilon.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_epsilon_inl__
#define __gtx_epsilon_inl__

#include "gtx_epsilon.h"
#if(GLM_INCLUDED(GLM_GTX_epsilon, GLM_EXTENSION_GTX, GLM_GTX_epsilon_required))

namespace glm
{
    template <typename T> 
    T equalEpsilonGTX(T x, T y, T epsilon)
    {
        return glm::abs(x - y) < epsilon;
    }

    template <typename T> 
    T notEqualEpsilonGTX(T x, T y, T epsilon)
    {
        return glm::abs(x - y) >= epsilon;
    }

    template <typename T> 
    inline _bvec2 equalEpsilonGTX(const _xvec2<T> & x, const _xvec2<T> & y, const T epsilon)
    {
        return _bvec2(
            glm::abs(x.x - y.x) < epsilon,
            glm::abs(y.y - y.y) < epsilon);
    }

    template <typename T> 
    inline _bvec3 equalEpsilonGTX(const _xvec3<T> & x, const _xvec3<T> & y, const T epsilon)
    {
        return _bvec3(
            glm::abs(x.x - y.x) < epsilon,
            glm::abs(y.y - y.y) < epsilon,
            glm::abs(y.z - y.z) < epsilon);
    }

    template <typename T> 
    inline _bvec4 equalEpsilonGTX(const _xvec4<T> & x, const _xvec4<T> & y, const T epsilon)
    {
        return _bvec4(
            glm::abs(x.x - y.x) < epsilon,
            glm::abs(y.y - y.y) < epsilon,
            glm::abs(y.z - y.z) < epsilon,
            glm::abs(y.w - y.w) < epsilon);
    }

    template <typename T> 
    inline _bvec2 notEqualEpsilonGTX(const _xvec2<T> & x, const _xvec2<T> & y, const T epsilon)
    {
        return _bvec2(
            glm::abs(x.x - y.x) >= epsilon,
            glm::abs(y.y - y.y) >= epsilon);
    }

    template <typename T> 
    inline _bvec3 notEqualEpsilonGTX(const _xvec3<T> & x, const _xvec3<T> & y, const T epsilon)
    {
        return _bvec3(
            glm::abs(x.x - y.x) >= epsilon,
            glm::abs(y.y - y.y) >= epsilon,
            glm::abs(y.z - y.z) >= epsilon);
    }

    template <typename T> 
    inline _bvec4 notEqualEpsilonGTX(const _xvec4<T> & x, const _xvec4<T> & y, const T epsilon)
    {
        return _bvec4(
            glm::abs(x.x - y.x) >= epsilon,
            glm::abs(y.y - y.y) >= epsilon,
            glm::abs(y.z - y.z) >= epsilon,
            glm::abs(y.w - y.w) >= epsilon);
    }

    template <typename T> 
    inline _bvec2 equalEpsilonGTX(const _xvec2<T>& x, const _xvec2<T>& y, const _xvec2<T>& epsilon)
    {
        return _bvec2(
            glm::abs(x.x - y.x) < epsilon.x,
            glm::abs(y.y - y.y) < epsilon.y);
    }

    template <typename T>
    inline _bvec3 equalEpsilonGTX(const _xvec3<T>& x, const _xvec3<T>& y, const _xvec3<T>& epsilon)
    {
        return _bvec3(
            glm::abs(x.x - y.x) < epsilon.x,
            glm::abs(y.y - y.y) < epsilon.y,
            glm::abs(y.z - y.z) < epsilon.z);
    }

    template <typename T> 
    inline _bvec4 equalEpsilonGTX(const _xvec4<T>& x, const _xvec4<T>& y, const _xvec4<T>& epsilon)
    {
        return _bvec4(
            glm::abs(x.x - y.x) < epsilon.x,
            glm::abs(y.y - y.y) < epsilon.y,
            glm::abs(y.z - y.z) < epsilon.z,
            glm::abs(y.w - y.w) < epsilon.w);
    }

    template <typename T> 
    inline _bvec2 notEqualEpsilonGTX(const _xvec2<T>& x, const _xvec2<T>& y, const _xvec2<T>& epsilon)
    {
        return _bvec2(
            glm::abs(x.x - y.x) >= epsilon.x,
            glm::abs(y.y - y.y) >= epsilon.y);
    }

    template <typename T> 
    inline _bvec3 notEqualEpsilonGTX(const _xvec3<T>& x, const _xvec3<T>& y, const _xvec3<T>& epsilon)
    {
        return _bvec3(
            glm::abs(x.x - y.x) >= epsilon.x,
            glm::abs(y.y - y.y) >= epsilon.y,
            glm::abs(y.z - y.z) >= epsilon.z);
    }

    template <typename T> 
    inline _bvec4 notEqualEpsilonGTX(const _xvec4<T>& x, const _xvec4<T>& y, const _xvec4<T>& epsilon)
    {
        return _bvec4(
            glm::abs(x.x - y.x) >= epsilon.x,
            glm::abs(y.y - y.y) >= epsilon.y,
            glm::abs(y.z - y.z) >= epsilon.z,
            glm::abs(y.w - y.w) >= epsilon.w);
    }
}

#endif//included

#endif//__gtx_epsilon_inl__
