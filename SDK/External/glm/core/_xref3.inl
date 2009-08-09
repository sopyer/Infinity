//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-05-29
// Updated : 2006-05-29
// Licence : This source is under GNU LGPL licence
// File    : _xref3.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __xref3_inl__
#define __xref3_inl__

#include "./_xref3.h"

namespace glm
{
    template <typename T>
    inline _xref3<T>::_xref3(T& x, T& y, T& z) :
        x(x),
        y(y),
        z(z)
    {}

    template <typename T>
    inline _xref3<T>& _xref3<T>::operator=(const _xref3<T>& r)
    {
        x = r.x;
        y = r.y;
        z = r.z;
        return *this;
    }

    template <typename T>
    inline _xref3<T>& _xref3<T>::operator+=(const _xref3<T>& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;
        return *this;
    }

    template <typename T>
	inline _xref3<T>& _xref3<T>::operator-=(const _xref3<T>& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;
        return *this;
    }

    template <typename T>
	inline _xref3<T>& _xref3<T>::operator*=(const _xref3<T>& r)
    {
        x *= r.x;
        y *= r.y;
        z *= r.z;
        return *this;
    }

    template <typename T>
	inline _xref3<T>& _xref3<T>::operator/=(const _xref3<T>& r)
    {
        x /= r.x;
        y /= r.y;
        z /= r.z;
        return *this;
    }

    template <typename T>
    inline _xref3<T>& _xref3<T>::operator=(const _xvec3<T>& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    template <typename T>
    inline _xref3<T>& _xref3<T>::operator+=(const _xvec3<T>& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    template <typename T>
	inline _xref3<T>& _xref3<T>::operator-=(const _xvec3<T>& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template <typename T>
	inline _xref3<T>& _xref3<T>::operator*=(const _xvec3<T>& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    template <typename T>
	inline _xref3<T>& _xref3<T>::operator/=(const _xvec3<T>& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
}

#endif//__xref3_inl__
