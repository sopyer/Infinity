//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-27
// Updated : 2006-12-03
// Licence : This source is under GNU LGPL licence
// File    : _xvec4.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __xvec4_inl__
#define __xvec4_inl__

#include "./_xvec2.h"
#include "./_bvec2.h"
#include "./_xvec3.h"
#include "./_bvec3.h"
#include "./_xvec4.h"
#include "./_bvec4.h"
#include "./_swizzle.inl"

namespace glm
{
    // Common constructors
    template <typename T> 
    inline _xvec4<T>::_xvec4() :
        x(0),
        y(0),
        z(0),
        w(0)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec4<T>& v) :
        x(v.x),
        y(v.y),
        z(v.z),
        w(v.w)
    {}

    // Swizzle constructors
    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xref4<T>& r) :
        x(r.x),
        y(r.y),
        z(r.z),
        w(r.w)
    {}

    // T constructors
    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s) :
        x(s),
        y(s),
        z(s),
        w(s)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T x, const T y, const T z, const T w) :
        x(x),
        y(y),
        z(z),
        w(w)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s1, const T s2, const T s3, const _xvec2<T>& v) :
        x(s1),
        y(s2),
        z(s3),
        w(v.x)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s1, const T s2, const T s3, const _xvec3<T>& v) :
        x(s1),
        y(s2),
        z(s3),
        w(v.x)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s1, const T s2, const T s3, const _xvec4<T>& v) :
        x(s1),
        y(s2),
        z(s3),
        w(v.x)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s1, const T s2, const _xvec2<T>& v) :
        x(s1),
        y(s2),
        z(v.x),
        w(v.y)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s1, const T s2, const _xvec3<T>& v) :
        x(s1),
        y(s2),
        z(v.x),
        w(v.y)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s1, const T s2, const _xvec4<T>& v) :
        x(s1),
        y(s2),
        z(v.x),
        w(v.y)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s1, const _xvec2<T>& v, const T s2) :
        x(s1),
        y(v.x),
        z(v.y),
        w(s1)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s, const _xvec2<T>& v1, const _xvec2<T>& v2) :
        x(s),
        y(v1.x),
        z(v1.y),
        w(v2.x)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s, const _xvec2<T>& v1, const _xvec3<T>& v2) :
        x(s),
        y(v1.x),
        z(v1.y),
        w(v2.x)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s, const _xvec2<T>& v1, const _xvec4<T>& v2) :
        x(s),
        y(v1.x),
        z(v1.y),
        w(v2.x)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s, const _xvec3<T>& v) :
        x(s),
        y(v.x),
        z(v.y),
        w(v.z)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const T s, const _xvec4<T>& v) :
        x(s),
        y(v.x),
        z(v.y),
        w(v.z)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec2<T>& v, const T s1, const T s2) :
        x(v.x),
        y(v.y),
        z(s1),
        w(s2)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec2<T>& v1, const T s, const _xvec2<T>& v2) :
        x(v1.x),
        y(v1.y),
        z(s),
        w(v2.x)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec2<T>& v1, const T s, const _xvec3<T>& v2) :
        x(v1.x),
        y(v1.y),
        z(s),
        w(v2.x)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec2<T>& v1, const T s, const _xvec4<T>& v2) :
        x(v1.x),
        y(v1.y),
        z(s),
        w(v2.x)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec2<T>& v1, const _xvec2<T>& v2) :
        x(v1.x),
        y(v1.y),
        z(v2.x),
        w(v2.y)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec2<T>& v1, const _xvec3<T>& v2) :
        x(v1.x),
        y(v1.y),
        z(v2.x),
        w(v2.y)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec2<T>& v1, const _xvec4<T>& v2) :
        x(v1.x),
        y(v1.y),
        z(v2.x),
        w(v2.y)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec3<T>& v, const T s) :
        x(v.x),
        y(v.y),
        z(v.z),
        w(s)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec3<T>& v1, const _xvec2<T>& v2) :
        x(v1.x),
        y(v1.y),
        z(v1.z),
        w(v2.x)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec3<T>& v1, const _xvec3<T>& v2) :
        x(v1.x),
        y(v1.y),
        z(v1.z),
        w(v2.x)
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _xvec3<T>& v1, const _xvec4<T>& v2) :
        x(v1.x),
        y(v1.y),
        z(v1.z),
        w(v2.x)
    {}

    // U constructors
    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s) :
        x(T(s)),
        y(T(s)),
        z(T(s)),
        w(T(s))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s1, const U s2, const U s3, const U s4) :
        x(T(s1)),
        y(T(s2)),
        z(T(s3)),
        w(T(s4))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s1, const U s2, const U s3, const _xvec2<U>& v) :
        x(T(s1)),
        y(T(s2)),
        z(T(s3)),
        w(T(v.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s1, const U s2, const U s3, const _xvec3<U>& v) :
        x(T(s1)),
        y(T(s2)),
        z(T(s3)),
        w(T(v.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s1, const U s2, const U s3, const _xvec4<U>& v) :
        x(T(s1)),
        y(T(s2)),
        z(T(s3)),
        w(T(v.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s1, const U s2, const _xvec2<U>& v) :
        x(T(s1)),
        y(T(s2)),
        z(T(v.x)),
        w(T(v.y))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s1, const U s2, const _xvec3<U>& v) :
        x(T(s1)),
        y(T(s2)),
        z(T(v.x)),
        w(T(v.y))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s1, const U s2, const _xvec4<U>& v) :
        x(T(s1)),
        y(T(s2)),
        z(T(v.x)),
        w(T(v.y))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s1, const _xvec2<U>& v, const U s2) :
        x(T(s1)),
        y(T(v.x)),
        z(T(v.y)),
        w(T(s2))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s, const _xvec2<U>& v1, const _xvec2<U>& v2) :
        x(T(s)),
        y(T(v1.x)),
        z(T(v1.y)),
        w(T(v2.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s, const _xvec2<U>& v1, const _xvec3<U>& v2) :
        x(T(s)),
        y(T(v1.x)),
        z(T(v1.y)),
        w(T(v2.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s, const _xvec2<U>& v1, const _xvec4<U>& v2) :
        x(T(s)),
        y(T(v1.x)),
        z(T(v1.y)),
        w(T(v2.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s, const _xvec3<U>& v) :
        x(T(s)),
        y(T(v.x)),
        z(T(v.y)),
        w(T(v.z))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const U s, const _xvec4<U>& v) :
        x(T(s)),
        y(T(v.x)),
        z(T(v.y)),
        w(T(v.z))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec2<U>& v, const U s1, const U s2) :
        x(T(v.x)),
        y(T(v.y)),
        z(T(s1)),
        w(T(s2))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec2<U>& v1, const U s, const _xvec2<U>& v2) :
        x(T(v1.x)),
        y(T(v1.y)),
        z(T(s)),
        w(T(v2.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec2<U>& v1, const U s, const _xvec3<U>& v2) :
        x(T(v1.x)),
        y(T(v1.y)),
        z(T(s)),
        w(T(v2.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec2<U>& v1, const U s, const _xvec4<U>& v2) :
        x(T(v1.x)),
        y(T(v1.y)),
        z(T(s)),
        w(T(v2.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec2<U>& v1, const _xvec2<U>& v2) :
        x(T(v1.x)),
        y(T(v1.y)),
        z(T(v2.x)),
        w(T(v2.y))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec2<U>& v1, const _xvec3<U>& v2) :
        x(T(v1.x)),
        y(T(v1.y)),
        z(T(v2.x)),
        w(T(v2.y))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec2<U>& v1, const _xvec4<U>& v2) :
        x(T(v1.x)),
        y(T(v1.y)),
        z(T(v2.x)),
        w(T(v2.y))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec3<U>& v, const U s) :
        x(T(v.x)),
        y(T(v.y)),
        z(T(v.z)),
        w(T(s))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec3<U>& v1, const _xvec2<U>& v2) :
        x(T(v1.x)),
        y(T(v1.y)),
        z(T(v1.z)),
        w(T(v2.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec3<U>& v1, const _xvec3<U>& v2) :
        x(T(v1.x)),
        y(T(v1.y)),
        z(T(v1.z)),
        w(T(v2.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec3<U>& v1, const _xvec4<U>& v2) :
        x(T(v1.x)),
        y(T(v1.y)),
        z(T(v1.z)),
        w(T(v2.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec4<T>::_xvec4(const _xvec4<U>& v) :
        x(T(v.x)),
        y(T(v.y)),
        z(T(v.z)),
        w(T(v.w))
    {}

    // Bool constructors
    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool x, const bool y, const bool z, const bool w) :
        x(bool(x)),
        y(bool(y)),
        z(bool(z)),
        w(bool(w))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s1, const bool s2, const bool s3, const _bvec2& v) :
        x(bool(s1)),
        y(bool(s2)),
        z(bool(s3)),
        w(bool(v.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s1, const bool s2, const bool s3, const _bvec3& v) :
        x(bool(s1)),
        y(bool(s2)),
        z(bool(s3)),
        w(bool(v.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s1, const bool s2, const bool s3, const _bvec4& v) :
        x(bool(s1)),
        y(bool(s2)),
        z(bool(s3)),
        w(bool(v.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s1, const bool s2, const _bvec2& v) :
        x(bool(s1)),
        y(bool(s2)),
        z(bool(v.x)),
        w(bool(v.y))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s1, const bool s2, const _bvec3& v) :
        x(bool(s1)),
        y(bool(s2)),
        z(bool(v.x)),
        w(bool(v.y))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s1, const bool s2, const _bvec4& v) :
        x(bool(s1)),
        y(bool(s2)),
        z(bool(v.x)),
        w(bool(v.y))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s1, const _bvec2& v, const bool s2) :
        x(bool(s1)),
        y(bool(v.x)),
        z(bool(v.y)),
        w(bool(s2))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s, const _bvec2& v1, const _bvec2& v2) :
        x(bool(s)),
        y(bool(v1.x)),
        z(bool(v1.y)),
        w(bool(v2.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s, const _bvec2& v1, const _bvec3& v2) :
        x(bool(s)),
        y(bool(v1.x)),
        z(bool(v1.y)),
        w(bool(v2.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s, const _bvec2& v1, const _bvec4& v2) :
        x(bool(s)),
        y(bool(v1.x)),
        z(bool(v1.y)),
        w(bool(v2.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s, const _bvec3& v) :
        x(bool(s)),
        y(bool(v.x)),
        z(bool(v.y)),
        w(bool(v.z))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const bool s, const _bvec4& v) :
        x(bool(s)),
        y(bool(v.x)),
        z(bool(v.y)),
        w(bool(v.z))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec2& v, const bool s1, const bool s2) :
        x(bool(v.x)),
        y(bool(v.y)),
        z(bool(s1)),
        w(bool(s2))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec2& v1, const bool s, const _bvec2& v2) :
        x(bool(v1.x)),
        y(bool(v1.y)),
        z(bool(s)),
        w(bool(v2.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec2& v1, const bool s, const _bvec3& v2) :
        x(bool(v1.x)),
        y(bool(v1.y)),
        z(bool(s)),
        w(bool(v2.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec2& v1, const bool s, const _bvec4& v2) :
        x(bool(v1.x)),
        y(bool(v1.y)),
        z(bool(s)),
        w(bool(v2.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec2& v1, const _bvec2& v2) :
        x(bool(v1.x)),
        y(bool(v1.y)),
        z(bool(v2.x)),
        w(bool(v2.y))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec2& v1, const _bvec3& v2) :
        x(bool(v1.x)),
        y(bool(v1.y)),
        z(bool(v2.x)),
        w(bool(v2.y))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec2& v1, const _bvec4& v2) :
        x(bool(v1.x)),
        y(bool(v1.y)),
        z(bool(v2.x)),
        w(bool(v2.y))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec3& v, const bool s) :
        x(bool(v.x)),
        y(bool(v.y)),
        z(bool(v.z)),
        w(bool(x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec3& v1, const _bvec2& v2) :
        x(bool(v1.x)),
        y(bool(v1.y)),
        z(bool(v1.z)),
        w(bool(v2.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec3& v1, const _bvec3& v2) :
        x(bool(v1.x)),
        y(bool(v1.y)),
        z(bool(v1.z)),
        w(bool(v2.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec3& v1, const _bvec4& v2) :
        x(bool(v1.x)),
        y(bool(v1.y)),
        z(bool(v1.z)),
        w(bool(v2.x))
    {}

    template <typename T> 
    inline _xvec4<T>::_xvec4(const _bvec4& v) :
        x(bool(v.x)),
        y(bool(v.y)),
        z(bool(v.z)),
        w(bool(v.w))
    {}

    // vec4 and ivec4 accesses
    template <typename T> 
    inline T& _xvec4<T>::operator[](int i)
    {
        return (&x)[i];
    }

    template <typename T> 
    inline T _xvec4<T>::operator[](int i) const
    {
        return (&x)[i];
    }

    template <typename T> 
    inline _xvec4<T>::operator T*()
    {
        return &x;
    }

    template <typename T> 
    inline _xvec4<T>::operator const T*() const 
    {
        return &x;
    }

    // vec4 and ivec4 operators definitions

    // This function shouldn't required but it seems that VC7.1 have an optimisation bug if this operator wasn't declared
    template <typename T> 
    inline _xvec4<T>& _xvec4<T>::operator=(const _xvec4<T>& x)
    {
        this->x = x.x;
        this->y = x.y;
        this->z = x.z;
        this->w = x.w;
        return *this;
    }

    template <typename T> 
    inline _xvec4<T>& _xvec4<T>::operator+= (const T s)
    {
	    this->x += s;
	    this->y += s;
        this->z += s;
        this->w += s;
	    return *this;
    }

    template <typename T> 
    inline _xvec4<T>& _xvec4<T>::operator+=(const _xvec4<T> & v)
    {
	    this->x += v.x;
	    this->y += v.y;
        this->z += v.z;
        this->w += v.w;
	    return *this;
    }

    template <typename T> 
    inline _xvec4<T>& _xvec4<T>::operator-= (const T s)
    {
	    this->x -= s;
	    this->y -= s;
        this->z -= s;
        this->w -= s;
	    return *this;
    }

    template <typename T> 
    inline _xvec4<T>& _xvec4<T>::operator-=(const _xvec4<T> & v)
    {
	    this->x -= v.x;
	    this->y -= v.y;
        this->z -= v.z;
        this->w -= v.w;
	    return *this;
    }

    template <typename T> 
    inline _xvec4<T>& _xvec4<T>::operator*=(const T s)
    {
	    this->x *= s;
	    this->y *= s;
        this->z *= s;
        this->w *= s;
	    return *this;
    }

    template <typename T> 
    inline _xvec4<T>& _xvec4<T>::operator*= (const _xvec4<T> & v)
    {
	    this->x *= v.x;
	    this->y *= v.y;
        this->z *= v.z;
        this->w *= v.w;
	    return *this;
    }

    template <typename T> 
    inline _xvec4<T>& _xvec4<T>::operator/=(const T s)
    {
	    this->x /= s;
	    this->y /= s;
        this->z /= s;
        this->w /= s;
	    return *this;
    }

    template <typename T> 
    inline _xvec4<T>& _xvec4<T>::operator/= (const _xvec4<T> & v)
    {
	    this->x /= v.x;
	    this->y /= v.y;
        this->z /= v.z;
        this->w /= v.w;
	    return *this;
    }

    // Unary constant operators
    template <typename T> 
    inline const _xvec4<T> operator- (const _xvec4<T>& v)
    {
        return _xvec4<T>(
            -v.x, 
            -v.y,
            -v.z,
            -v.w);
    }

    template <typename T> 
    inline const _xvec4<T> operator++ (const _xvec4<T>& v, int)
    {
        return _xvec4<T>(
            v.x + T(1), 
            v.y + T(1),
            v.z + T(1),
            v.w + T(1));
    }

    template <typename T> 
    inline const _xvec4<T> operator-- (const _xvec4<T>& v, int)
    {
        return _xvec4<T>(
            v.x - T(1), 
            v.y - T(1),
            v.z - T(1),
            v.w - T(1));
    }

    // Binary operators
    template <typename T>
	inline _xvec4<T> operator+ (const _xvec4<T>& v, const T s)
    {
	    return _xvec4<T>(
		    v.x + s,
		    v.y + s,
            v.z + s,
            v.w + s);
    }

    template <typename T>
    inline _xvec4<T> operator+ (const T s, const _xvec4<T>& v)
    {
	    return _xvec4<T>(
		    v.x + s,
		    v.y + s,
            v.z + s,
            v.w + s);
    }

    template <typename T>
    inline _xvec4<T> operator+ (const _xvec4<T>& v1, const _xvec4<T>& v2)
    {
	    return _xvec4<T>(
		    v1.x + v2.x,
		    v1.y + v2.y,
            v1.z + v2.z,
            v1.w + v2.w);
    }
    
    template <typename T>
	inline _xvec4<T> operator- (const _xvec4<T>& v, const T s)
    {
	    return _xvec4<T>(
		    v.x - s,
		    v.y - s,
            v.z - s,
            v.w - s);
    }

    template <typename T>
    inline _xvec4<T> operator- (const T s, const _xvec4<T>& v)
    {
	    return _xvec4<T>(
		    s - v.x,
		    s - v.y,
            s - v.z,
            s - v.w);
    }

    template <typename T>
    inline _xvec4<T> operator- (const _xvec4<T>& v1, const _xvec4<T>& v2)
    {
	    return _xvec4<T>(
		    v1.x - v2.x,
		    v1.y - v2.y,
            v1.z - v2.z,
            v1.w - v2.w);
    }

    template <typename T>
    inline _xvec4<T> operator* (const _xvec4<T>& v, const T s)
    {
	    return _xvec4<T>(
		    v.x * s,
		    v.y * s,
            v.z * s,
            v.w * s);
    }

    template <typename T>
    inline _xvec4<T> operator* (const T s, const _xvec4<T>& v)
    {
	    return _xvec4<T>(
		    v.x * s,
		    v.y * s,
            v.z * s,
            v.w * s);
    }

    template <typename T>
    inline _xvec4<T> operator* (const _xvec4<T>& v1, const _xvec4<T>& v2)
    {
	    return _xvec4<T>(
		    v1.x * v2.x,
		    v1.y * v2.y,
            v1.z * v2.z,
            v1.w * v2.w);
    }

    template <typename T>
    inline _xvec4<T> operator/ (const _xvec4<T>& v, const T s)
    {
	    return _xvec4<T>(
		    v.x / s,
		    v.y / s,
            v.z / s,
            v.w / s);
    }

    template <typename T>
    inline _xvec4<T> operator/ (const T s, const _xvec4<T>& v)
    {
	    return _xvec4<T>(
		    s / v.x,
		    s / v.y,
            s / v.z,
            s / v.w);
    }

    template <typename T>
    inline _xvec4<T> operator/ (const _xvec4<T>& v1, const _xvec4<T>& v2)
    {
	    return _xvec4<T>(
		    v1.x / v2.x,
		    v1.y / v2.y,
            v1.z / v2.z,
            v1.w / v2.w);
    }

#if defined(GLM_SWIZZLE)
    // 2 components swizzles operators

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_xx() const
    {
        return _xvec2<T>(this->x, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_yx() const
    {
        return _xvec2<T>(this->y, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_zx() const
    {
        return _xvec2<T>(this->z, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_wx() const
    {
        return _xvec2<T>(this->w, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_xy() const
    {
        return _xvec2<T>(this->x, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_yy() const
    {
        return _xvec2<T>(this->y, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_zy() const
    {
        return _xvec2<T>(this->z, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_wy() const
    {
        return _xvec2<T>(this->w, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_xz() const
    {
        return _xvec2<T>(this->x, this->z);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_yz() const
    {
        return _xvec2<T>(this->y, this->z);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_zz() const
    {
        return _xvec2<T>(this->z, this->z);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_wz() const
    {
        return _xvec2<T>(this->w, this->z);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_xw() const
    {
        return _xvec2<T>(this->x, this->w);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_yw() const
    {
        return _xvec2<T>(this->y, this->w);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_zw() const
    {
        return _xvec2<T>(this->z, this->w);
    }

    template <typename T>
    inline _xvec2<T> _xvec4<T>::_ww() const
    {
        return _xvec2<T>(this->w, this->w);
    }

    //////////////////////////////////////////////////////////////
    // 3 components swizzles operators

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xxx() const
    {
        return _xvec3<T>(this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yxx() const
    {
        return _xvec3<T>(this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zxx() const
    {
        return _xvec3<T>(this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wxx() const
    {
        return _xvec3<T>(this->w, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xyx() const
    {
        return _xvec3<T>(this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yyx() const
    {
        return _xvec3<T>(this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zyx() const
    {
        return _xvec3<T>(this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wyx() const
    {
        return _xvec3<T>(this->w, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xzx() const
    {
        return _xvec3<T>(this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yzx() const
    {
        return _xvec3<T>(this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zzx() const
    {
        return _xvec3<T>(this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wzx() const
    {
        return _xvec3<T>(this->w, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xwx() const
    {
        return _xvec3<T>(this->x, this->w, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_ywx() const
    {
        return _xvec3<T>(this->y, this->w, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zwx() const
    {
        return _xvec3<T>(this->z, this->w, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wwx() const
    {
        return _xvec3<T>(this->w, this->w, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xxy() const
    {
        return _xvec3<T>(this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yxy() const
    {
        return _xvec3<T>(this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zxy() const
    {
        return _xvec3<T>(this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wxy() const
    {
        return _xvec3<T>(this->w, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xyy() const
    {
        return _xvec3<T>(this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yyy() const
    {
        return _xvec3<T>(this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zyy() const
    {
        return _xvec3<T>(this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wyy() const
    {
        return _xvec3<T>(this->w, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xzy() const
    {
        return _xvec3<T>(this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yzy() const
    {
        return _xvec3<T>(this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zzy() const
    {
        return _xvec3<T>(this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wzy() const
    {
        return _xvec3<T>(this->w, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xwy() const
    {
        return _xvec3<T>(this->x, this->w, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_ywy() const
    {
        return _xvec3<T>(this->y, this->w, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zwy() const
    {
        return _xvec3<T>(this->z, this->w, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wwy() const
    {
        return _xvec3<T>(this->w, this->w, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xxz() const
    {
        return _xvec3<T>(this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yxz() const
    {
        return _xvec3<T>(this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zxz() const
    {
        return _xvec3<T>(this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wxz() const
    {
        return _xvec3<T>(this->w, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xyz() const
    {
        return _xvec3<T>(this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yyz() const
    {
        return _xvec3<T>(this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zyz() const
    {
        return _xvec3<T>(this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wyz() const
    {
        return _xvec3<T>(this->w, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xzz() const
    {
        return _xvec3<T>(this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yzz() const
    {
        return _xvec3<T>(this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zzz() const
    {
        return _xvec3<T>(this->z, this->z, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wzz() const
    {
        return _xvec3<T>(this->w, this->z, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xwz() const
    {
        return _xvec3<T>(this->x, this->w, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_ywz() const
    {
        return _xvec3<T>(this->y, this->w, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zwz() const
    {
        return _xvec3<T>(this->z, this->w, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wwz() const
    {
        return _xvec3<T>(this->w, this->w, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xxw() const
    {
        return _xvec3<T>(this->x, this->x, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yxw() const
    {
        return _xvec3<T>(this->y, this->x, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zxw() const
    {
        return _xvec3<T>(this->z, this->x, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wxw() const
    {
        return _xvec3<T>(this->w, this->x, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xyw() const
    {
        return _xvec3<T>(this->x, this->y, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yyw() const
    {
        return _xvec3<T>(this->y, this->y, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zyw() const
    {
        return _xvec3<T>(this->z, this->y, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wyw() const
    {
        return _xvec3<T>(this->w, this->y, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xzw() const
    {
        return _xvec3<T>(this->x, this->z, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yzw() const
    {
        return _xvec3<T>(this->y, this->z, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zzw() const
    {
        return _xvec3<T>(this->z, this->z, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_wzw() const
    {
        return _xvec3<T>(this->w, this->z, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_xww() const
    {
        return _xvec3<T>(this->x, this->w, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_yww() const
    {
        return _xvec3<T>(this->y, this->w, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_zww() const
    {
        return _xvec3<T>(this->z, this->w, this->w);
    }

    template <typename T>
    inline _xvec3<T> _xvec4<T>::_www() const
    {
        return _xvec3<T>(this->w, this->w, this->w);
    }

    // 4 components swizzles operators
    template <typename T>
    inline _xref4<T> _xvec4<T>::_wzyx()
    {
        return _xref4<T>(this->w, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_zwyx()
    {
        return _xref4<T>(this->z, this->w, this->y, this->x);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_wyzx()
    {
        return _xref4<T>(this->w, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_ywzx()
    {
        return _xref4<T>(this->y, this->w, this->z, this->x);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_zywx()
    {
        return _xref4<T>(this->z, this->y, this->w, this->x);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_yzwx()
    {
        return _xref4<T>(this->y, this->z, this->w, this->x);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_wzxy()
    {
        return _xref4<T>(this->w, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_zwxy()
    {
        return _xref4<T>(this->z, this->w, this->x, this->y);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_wxzy()
    {
        return _xref4<T>(this->w, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_xwzy()
    {
        return _xref4<T>(this->x, this->w, this->z, this->y);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_zxwy()
    {
        return _xref4<T>(this->z, this->x, this->w, this->y);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_xzwy()
    {
        return _xref4<T>(this->x, this->z, this->w, this->y);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_wyxz()
    {
        return _xref4<T>(this->w, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_ywxz()
    {
        return _xref4<T>(this->y, this->w, this->x, this->z);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_wxyz()
    {
        return _xref4<T>(this->w, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_xwyz()
    {
        return _xref4<T>(this->x, this->w, this->y, this->z);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_yxwz()
    {
        return _xref4<T>(this->y, this->x, this->w, this->z);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_xywz()
    {
        return _xref4<T>(this->x, this->y, this->w, this->z);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_zyxw()
    {
        return _xref4<T>(this->z, this->y, this->x, this->w);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_yzxw()
    {
        return _xref4<T>(this->y, this->z, this->x, this->w);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_zxyw()
    {
        return _xref4<T>(this->z, this->x, this->y, this->w);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_xzyw()
    {
        return _xref4<T>(this->x, this->z, this->y, this->w);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_yxzw()
    {
        return _xref4<T>(this->y, this->x, this->z, this->w);
    }

    template <typename T>
    inline _xref4<T> _xvec4<T>::_xyzw()
    {
        return _xref4<T>(this->x, this->y, this->z, this->w);
    }

    // 4 components swizzles operators
    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxxx() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxxx() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxxx() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxxx() const
    {
        return _xvec4<T>(this->w, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyxx() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyxx() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyxx() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyxx() const
    {
        return _xvec4<T>(this->w, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzxx() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzxx() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzxx() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzxx() const
    {
        return _xvec4<T>(this->w, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwxx() const
    {
        return _xvec4<T>(this->x, this->w, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywxx() const
    {
        return _xvec4<T>(this->y, this->w, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwxx() const
    {
        return _xvec4<T>(this->z, this->w, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwxx() const
    {
        return _xvec4<T>(this->w, this->w, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxyx() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxyx() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxyx() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxyx() const
    {
        return _xvec4<T>(this->w, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyyx() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyyx() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyyx() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyyx() const
    {
        return _xvec4<T>(this->w, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzyx() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzyx() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzyx() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzyx() const
    {
        return _xvec4<T>(this->w, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwyx() const
    {
        return _xvec4<T>(this->x, this->w, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywyx() const
    {
        return _xvec4<T>(this->y, this->w, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwyx() const
    {
        return _xvec4<T>(this->z, this->w, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwyx() const
    {
        return _xvec4<T>(this->w, this->w, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxzx() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxzx() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxzx() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxzx() const
    {
        return _xvec4<T>(this->w, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyzx() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyzx() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyzx() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyzx() const
    {
        return _xvec4<T>(this->w, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzzx() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzzx() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzzx() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzzx() const
    {
        return _xvec4<T>(this->w, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwzx() const
    {
        return _xvec4<T>(this->x, this->w, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywzx() const
    {
        return _xvec4<T>(this->y, this->w, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwzx() const
    {
        return _xvec4<T>(this->z, this->w, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwzx() const
    {
        return _xvec4<T>(this->w, this->w, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxwx() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxwx() const
    {
        return _xvec4<T>(this->y, this->x, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxwx() const
    {
        return _xvec4<T>(this->z, this->x, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxwx() const
    {
        return _xvec4<T>(this->w, this->x, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xywx() const
    {
        return _xvec4<T>(this->x, this->y, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yywx() const
    {
        return _xvec4<T>(this->y, this->y, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zywx() const
    {
        return _xvec4<T>(this->z, this->y, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wywx() const
    {
        return _xvec4<T>(this->w, this->y, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzwx() const
    {
        return _xvec4<T>(this->x, this->z, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzwx() const
    {
        return _xvec4<T>(this->y, this->z, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzwx() const
    {
        return _xvec4<T>(this->z, this->z, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzwx() const
    {
        return _xvec4<T>(this->w, this->z, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwwx() const
    {
        return _xvec4<T>(this->x, this->w, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywwx() const
    {
        return _xvec4<T>(this->y, this->w, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwwx() const
    {
        return _xvec4<T>(this->z, this->w, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwwx() const
    {
        return _xvec4<T>(this->w, this->w, this->w, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxxy() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxxy() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxxy() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxxy() const
    {
        return _xvec4<T>(this->w, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyxy() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyxy() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyxy() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyxy() const
    {
        return _xvec4<T>(this->w, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzxy() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzxy() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzxy() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzxy() const
    {
        return _xvec4<T>(this->w, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwxy() const
    {
        return _xvec4<T>(this->x, this->w, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywxy() const
    {
        return _xvec4<T>(this->y, this->w, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwxy() const
    {
        return _xvec4<T>(this->z, this->w, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwxy() const
    {
        return _xvec4<T>(this->w, this->w, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxyy() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxyy() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxyy() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxyy() const
    {
        return _xvec4<T>(this->w, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyyy() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyyy() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyyy() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyyy() const
    {
        return _xvec4<T>(this->w, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzyy() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzyy() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzyy() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzyy() const
    {
        return _xvec4<T>(this->w, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwyy() const
    {
        return _xvec4<T>(this->x, this->w, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywyy() const
    {
        return _xvec4<T>(this->y, this->w, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwyy() const
    {
        return _xvec4<T>(this->z, this->w, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwyy() const
    {
        return _xvec4<T>(this->w, this->w, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxzy() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxzy() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxzy() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxzy() const
    {
        return _xvec4<T>(this->w, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyzy() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyzy() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyzy() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyzy() const
    {
        return _xvec4<T>(this->w, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzzy() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzzy() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzzy() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzzy() const
    {
        return _xvec4<T>(this->w, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwzy() const
    {
        return _xvec4<T>(this->x, this->w, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywzy() const
    {
        return _xvec4<T>(this->y, this->w, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwzy() const
    {
        return _xvec4<T>(this->z, this->w, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwzy() const
    {
        return _xvec4<T>(this->w, this->w, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxwy() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxwy() const
    {
        return _xvec4<T>(this->y, this->x, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxwy() const
    {
        return _xvec4<T>(this->z, this->x, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxwy() const
    {
        return _xvec4<T>(this->w, this->x, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xywy() const
    {
        return _xvec4<T>(this->x, this->y, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yywy() const
    {
        return _xvec4<T>(this->y, this->y, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zywy() const
    {
        return _xvec4<T>(this->z, this->y, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wywy() const
    {
        return _xvec4<T>(this->w, this->y, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzwy() const
    {
        return _xvec4<T>(this->x, this->z, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzwy() const
    {
        return _xvec4<T>(this->y, this->z, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzwy() const
    {
        return _xvec4<T>(this->z, this->z, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzwy() const
    {
        return _xvec4<T>(this->w, this->z, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwwy() const
    {
        return _xvec4<T>(this->x, this->w, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywwy() const
    {
        return _xvec4<T>(this->y, this->w, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwwy() const
    {
        return _xvec4<T>(this->z, this->w, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwwy() const
    {
        return _xvec4<T>(this->w, this->w, this->w, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxxz() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxxz() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxxz() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxxz() const
    {
        return _xvec4<T>(this->w, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyxz() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyxz() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyxz() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyxz() const
    {
        return _xvec4<T>(this->w, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzxz() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzxz() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzxz() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzxz() const
    {
        return _xvec4<T>(this->w, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwxz() const
    {
        return _xvec4<T>(this->x, this->w, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywxz() const
    {
        return _xvec4<T>(this->y, this->w, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwxz() const
    {
        return _xvec4<T>(this->z, this->w, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwxz() const
    {
        return _xvec4<T>(this->w, this->w, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxyz() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxyz() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxyz() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxyz() const
    {
        return _xvec4<T>(this->w, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyyz() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyyz() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyyz() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyyz() const
    {
        return _xvec4<T>(this->w, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzyz() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzyz() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzyz() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzyz() const
    {
        return _xvec4<T>(this->w, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwyz() const
    {
        return _xvec4<T>(this->x, this->w, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywyz() const
    {
        return _xvec4<T>(this->y, this->w, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwyz() const
    {
        return _xvec4<T>(this->z, this->w, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwyz() const
    {
        return _xvec4<T>(this->w, this->w, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxzz() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxzz() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxzz() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxzz() const
    {
        return _xvec4<T>(this->w, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyzz() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyzz() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyzz() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyzz() const
    {
        return _xvec4<T>(this->w, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzzz() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzzz() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzzz() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzzz() const
    {
        return _xvec4<T>(this->w, this->z, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwzz() const
    {
        return _xvec4<T>(this->x, this->w, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywzz() const
    {
        return _xvec4<T>(this->y, this->w, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwzz() const
    {
        return _xvec4<T>(this->z, this->w, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwzz() const
    {
        return _xvec4<T>(this->w, this->w, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxwz() const
    {
        return _xvec4<T>(this->x, this->x, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxwz() const
    {
        return _xvec4<T>(this->y, this->x, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxwz() const
    {
        return _xvec4<T>(this->z, this->x, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxwz() const
    {
        return _xvec4<T>(this->w, this->x, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xywz() const
    {
        return _xvec4<T>(this->x, this->y, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yywz() const
    {
        return _xvec4<T>(this->y, this->y, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zywz() const
    {
        return _xvec4<T>(this->z, this->y, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wywz() const
    {
        return _xvec4<T>(this->w, this->y, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzwz() const
    {
        return _xvec4<T>(this->x, this->z, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzwz() const
    {
        return _xvec4<T>(this->y, this->z, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzwz() const
    {
        return _xvec4<T>(this->z, this->z, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzwz() const
    {
        return _xvec4<T>(this->w, this->z, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwwz() const
    {
        return _xvec4<T>(this->x, this->w, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywwz() const
    {
        return _xvec4<T>(this->y, this->w, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwwz() const
    {
        return _xvec4<T>(this->z, this->w, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwwz() const
    {
        return _xvec4<T>(this->w, this->w, this->w, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxxw() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxxw() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxxw() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxxw() const
    {
        return _xvec4<T>(this->w, this->x, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyxw() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyxw() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyxw() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyxw() const
    {
        return _xvec4<T>(this->w, this->y, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzxw() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzxw() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzxw() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzxw() const
    {
        return _xvec4<T>(this->w, this->z, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwxw() const
    {
        return _xvec4<T>(this->x, this->w, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywxw() const
    {
        return _xvec4<T>(this->y, this->w, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwxw() const
    {
        return _xvec4<T>(this->z, this->w, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwxw() const
    {
        return _xvec4<T>(this->w, this->w, this->x, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxyw() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxyw() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxyw() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxyw() const
    {
        return _xvec4<T>(this->w, this->x, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyyw() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyyw() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyyw() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyyw() const
    {
        return _xvec4<T>(this->w, this->y, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzyw() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzyw() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzyw() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzyw() const
    {
        return _xvec4<T>(this->w, this->z, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwyw() const
    {
        return _xvec4<T>(this->x, this->w, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywyw() const
    {
        return _xvec4<T>(this->y, this->w, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwyw() const
    {
        return _xvec4<T>(this->z, this->w, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwyw() const
    {
        return _xvec4<T>(this->w, this->w, this->y, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxzw() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxzw() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxzw() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxzw() const
    {
        return _xvec4<T>(this->w, this->x, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyzw() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyzw() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyzw() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyzw() const
    {
        return _xvec4<T>(this->w, this->y, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzzw() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzzw() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzzw() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzzw() const
    {
        return _xvec4<T>(this->w, this->z, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwzw() const
    {
        return _xvec4<T>(this->x, this->w, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywzw() const
    {
        return _xvec4<T>(this->y, this->w, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwzw() const
    {
        return _xvec4<T>(this->z, this->w, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwzw() const
    {
        return _xvec4<T>(this->w, this->w, this->z, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xxww() const
    {
        return _xvec4<T>(this->x, this->x, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yxww() const
    {
        return _xvec4<T>(this->y, this->x, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zxww() const
    {
        return _xvec4<T>(this->z, this->x, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wxww() const
    {
        return _xvec4<T>(this->w, this->x, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xyww() const
    {
        return _xvec4<T>(this->x, this->y, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yyww() const
    {
        return _xvec4<T>(this->y, this->y, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zyww() const
    {
        return _xvec4<T>(this->z, this->y, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wyww() const
    {
        return _xvec4<T>(this->w, this->y, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xzww() const
    {
        return _xvec4<T>(this->x, this->z, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_yzww() const
    {
        return _xvec4<T>(this->y, this->z, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zzww() const
    {
        return _xvec4<T>(this->z, this->z, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wzww() const
    {
        return _xvec4<T>(this->w, this->z, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_xwww() const
    {
        return _xvec4<T>(this->x, this->w, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_ywww() const
    {
        return _xvec4<T>(this->y, this->w, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_zwww() const
    {
        return _xvec4<T>(this->z, this->w, this->w, this->w);
    }

    template <typename T>
    inline _xvec4<T> _xvec4<T>::_wwww() const
    {
        return _xvec4<T>(this->w, this->w, this->w, this->w);
    }
#endif //defined(GLM_SWIZZLE)
} //namespace glm

#endif//__xvec4_inl__
