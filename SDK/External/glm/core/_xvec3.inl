//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-27
// Updated : 2006-04-27
// Licence : This source is under GNU LGPL licence
// File    : _xvec3.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __xvec3_inl__
#define __xvec3_inl__

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
    inline _xvec3<T>::_xvec3() :
        x(T(0)),
        y(T(0)),
        z(T(0))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _xvec3<T>& v) :
        x(v.x),
        y(v.y),
        z(v.z)
    {}

    // Swizzle constructors
    template <typename T> 
    inline _xvec3<T>::_xvec3(const _xref3<T>& r) :
        x(r.x),
        y(r.y),
        z(r.z)
    {}

    // T constructors
    template <typename T> 
    inline _xvec3<T>::_xvec3(const T x) :
        x(x),
        y(x),
        z(x)
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const T a, const T b, const T c) :
        x(a),
        y(b),
        z(c)
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const T a, const T b, const _xvec2<T>& c) :
        x(a),
        y(b),
        z(c.x)
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const T a, const T b, const _xvec3<T>& c) :
        x(a),
        y(b),
        z(c.x)
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const T a, const T b, const _xvec4<T>& c) :
        x(a),
        y(b),
        z(c.x)
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const T a, const _xvec2<T>& b) :
        x(a),
        y(b.x),
        z(b.y)
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const T a, const _xvec3<T>& b) :
        x(a),
        y(b.x),
        z(b.y)
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const T a, const _xvec4<T>& b) :
        x(a),
        y(b.x),
        z(b.y)
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _xvec2<T>& a, T b) :
        x(a.x),
        y(a.y),
        z(b)
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _xvec2<T>& a, const _xvec2<T>& b) :
        x(),
        y(),
        z()
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _xvec2<T>& a, const _xvec3<T>& b) :
        x(a.x),
        y(a.y),
        z(b.x)
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _xvec2<T>& a, const _xvec4<T>& b) :
        x(a.x),
        y(a.y),
        z(b.x)
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _xvec4<T>& a) :
        x(a.x),
        y(a.y),
        z(a.z)
    {}

    // U constructors
    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const U x) :
        x(T(x)),
        y(T(x)),
        z(T(x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const U a, const U b, const U c) :
        x(T(a)),
        y(T(b)),
        z(T(c))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const U a, const U b, const _xvec2<U>& c) :
        x(T(a)),
        y(T(b)),
        z(T(c.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const U a, const U b, const _xvec3<U>& c) :
        x(T(a)),
        y(T(b)),
        z(T(c.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const U a, const U b, const _xvec4<U>& c) :
        x(T(a)),
        y(T(b)),
        z(T(c.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const U a, const _xvec2<U>& b) :
        x(T(a)),
        y(T(b.x)),
        z(T(b.y))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const U a, const _xvec3<U>& b) :
        x(T(a)),
        y(T(b.x)),
        z(T(b.y))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const U a, const _xvec4<U>& b) :
        x(T(a)),
        y(T(b.x)),
        z(T(b.y))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const _xvec2<U>& a, U b) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(b))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const _xvec2<U>& a, const _xvec2<U>& b) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(b.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const _xvec2<U>& a, const _xvec3<U>& b) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(b.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const _xvec2<U>& a, const _xvec4<U>& b) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(b.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const _xvec3<U>& a) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(a.z))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec3<T>::_xvec3(const _xvec4<U>& a) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(a.z))
    {}

    // Bool constructors
    template <typename T> 
    inline _xvec3<T>::_xvec3(const bool x) :
        x(T(x)),
        y(T(x)),
        z(T(x))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const bool a, const bool b, const bool c) :
        x(T(a)),
        y(T(b)),
        z(T(c))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const bool a, const bool b, const _bvec2& c) :
        x(T(a)),
        y(T(b)),
        z(T(c.x))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const bool a, const bool b, const _bvec3& c) :
        x(T(a)),
        y(T(b)),
        z(T(c.x))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const bool a, const bool b, const _bvec4& c) :
        x(T(a)),
        y(T(b)),
        z(T(c.x))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const bool a, const _bvec2& b) :
        x(T(a)),
        y(T(b.x)),
        z(T(b.y))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const bool a, const _bvec3& b) :
        x(T(a)),
        y(T(b.x)),
        z(T(b.y))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const bool a, const _bvec4& b) :
        x(T(a)),
        y(T(b.x)),
        z(T(b.y))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _bvec2& a, bool b) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(b))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _bvec2& a, const _bvec2& b) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(b.x))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _bvec2& a, const _bvec3& b) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(b.x))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _bvec2& a, const _bvec4& b) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(b.x))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _bvec3& a) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(a.z))
    {}

    template <typename T> 
    inline _xvec3<T>::_xvec3(const _bvec4& a) :
        x(T(a.x)),
        y(T(a.y)),
        z(T(a.z))
    {}

    //////////////////////////////////////////////////////////////
    // vec3 and ivec3 accesses

    template <typename T> 
    inline T& _xvec3<T>::operator [] (int i)
    {
        return (&x)[i];
    }

    template <typename T> 
    inline T _xvec3<T>::operator [] (int i) const
    {
        return (&x)[i];
    }

    template <typename T> 
    inline _xvec3<T>::operator T* ()
    {
        return &x;
    }

    template <typename T> 
    inline _xvec3<T>::operator const T* () const 
    {
        return &x;
    }

    //////////////////////////////////////////////////////////////
    // vec3 and ivec3 operators

    // This function shouldn't required but it seems that VC7.1 have an optimisation bug if this operator wasn't declared
    template <typename T> 
    inline _xvec3<T>& _xvec3<T>::operator=(const _xvec3<T>& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }

    template <typename T> 
    inline _xvec3<T>& _xvec3<T>::operator+= (const T s)
    {
	    this->x += s;
	    this->y += s;
        this->z += s;
	    return *this;
    }

    template <typename T> 
    inline _xvec3<T>& _xvec3<T>::operator+= (const _xvec3<T>& v)
    {
	    this->x += v.x;
	    this->y += v.y;
        this->z += v.z;
	    return *this;
    }

    template <typename T> 
    inline _xvec3<T>& _xvec3<T>::operator-= (const T s)
    {
	    this->x -= s;
	    this->y -= s;
        this->z -= s;
	    return *this;
    }

    template <typename T> 
    inline _xvec3<T>& _xvec3<T>::operator-= (const _xvec3<T>& v)
    {
	    this->x -= v.x;
	    this->y -= v.y;
        this->z -= v.z;
	    return *this;
    }

    template <typename T> 
    inline _xvec3<T>& _xvec3<T>::operator*= (const T s)
    {
	    this->x *= s;
	    this->y *= s;
        this->z *= s;
	    return *this;
    }

    template <typename T> 
    inline _xvec3<T>& _xvec3<T>::operator*= (const _xvec3<T>& v)
    {
	    this->x *= v.x;
	    this->y *= v.y;
        this->z *= v.z;
	    return *this;
    }

    template <typename T> 
    inline _xvec3<T>& _xvec3<T>::operator/= (const T s)
    {
	    this->x /= s;
	    this->y /= s;
        this->z /= s;
	    return *this;
    }

    template <typename T> 
    inline _xvec3<T>& _xvec3<T>::operator/= (const _xvec3<T>& v)
    {
	    this->x /= v.x;
	    this->y /= v.y;
        this->z /= v.z;
	    return *this;
    }

    template <typename T> 
    inline _xvec3<T>& _xvec3<T>::operator++ ()
    {
	    this->x++;
	    this->y++;
        this->z++;
	    return *this;
    }

    template <typename T> 
    inline _xvec3<T>& _xvec3<T>::operator-- ()
    {
	    this->x--;
	    this->y--;
        this->z--;
	    return *this;
    }

    // Unary constant operators
    template <typename T> 
    inline const _xvec3<T> operator- (const _xvec3<T>& v)
    {
        return _xvec3<T>(
            -v.x, 
            -v.y,
            -v.z);
    }

    template <typename T> 
    inline const _xvec3<T> operator++ (const _xvec3<T>& v, int)
    {
        return _xvec3<T>(
            v.x + T(1), 
            v.y + T(1),
            v.z + T(1));
    }

    template <typename T> 
    inline const _xvec3<T> operator-- (const _xvec3<T>& v, int)
    {
        return _xvec3<T>(
            v.x - T(1), 
            v.y - T(1),
            v.z - T(1));
    }

    // Binary operators
    template <typename T> 
    inline _xvec3<T> operator+ (const _xvec3<T>& v, const T s)
    {
	    return _xvec3<T>(
		    v.x + s,
		    v.y + s,
            v.z + s);
    }

    template <typename T> 
    inline _xvec3<T> operator+ (const T s, const _xvec3<T>& v)
    {
	    return _xvec3<T>(
		    s + v.x,
		    s + v.y,
            s + v.z);
    }

    template <typename T> 
    inline _xvec3<T> operator+ (const _xvec3<T>& v1, const _xvec3<T>& v2)
    {
	    return _xvec3<T>(
		    v1.x + v2.x,
		    v1.y + v2.y,
            v1.z + v2.z);
    }

    template <typename T> 
    inline _xvec3<T> operator- (const _xvec3<T>& v, const T s)
    {
	    return _xvec3<T>(
		    v.x - s,
		    v.y - s,
            v.z - s);
    }

    template <typename T> 
    inline _xvec3<T> operator- (const T s, const _xvec3<T>& v)
    {
	    return _xvec3<T>(
		    s - v.x,
		    s - v.y,
            s - v.z);
    }

    template <typename T> 
    inline _xvec3<T> operator- (const _xvec3<T>& v1, const _xvec3<T>& v2)
    {
	    return _xvec3<T>(
		    v1.x - v2.x,
		    v1.y - v2.y,
            v1.z - v2.z);
    }

    template <typename T> 
    inline _xvec3<T> operator* (const _xvec3<T>& v, const T s)
    {
	    return _xvec3<T>(
		    v.x * s,
		    v.y * s,
            v.z * s);
    }

    template <typename T> 
    inline _xvec3<T> operator* (const T s, const _xvec3<T>& v)
    {
	    return _xvec3<T>(
		    s * v.x,
		    s * v.y,
            s * v.z);
    }

    template <typename T> 
    inline _xvec3<T> operator* (const _xvec3<T>& v1, const _xvec3<T> & v2)
    {
	    return _xvec3<T>(
		    v1.x * v2.x,
		    v1.y * v2.y,
            v1.z * v2.z);
    }

    template <typename T> 
    inline _xvec3<T> operator/ (const _xvec3<T>& v, const T s)
    {
	    return _xvec3<T>(
		    v.x / s,
		    v.y / s,
            v.z / s);
    }

    template <typename T> 
    inline _xvec3<T> operator/ (const T s, const _xvec3<T>& v)
    {
	    return _xvec3<T>(
		    s / v.x,
		    s / v.y,
            s / v.z);
    }

    template <typename T> 
    inline _xvec3<T> operator/ (const _xvec3<T>& v1, const _xvec3<T>& v2)
    {
	    return _xvec3<T>(
		    v1.x / v2.x,
		    v1.y / v2.y,
            v1.z / v2.z);
    }

    //////////////////////////////////////////////////////////////
    // 2 components swizzles operators

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_xx() const
    {
        return _xvec2<T>(this->x, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_yx() const
    {
        return _xvec2<T>(this->y, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_zx() const
    {
        return _xvec2<T>(this->z, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_xy() const
    {
        return _xvec2<T>(this->x, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_yy() const
    {
        return _xvec2<T>(this->y, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_zy() const
    {
        return _xvec2<T>(this->z, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_xz() const
    {
        return _xvec2<T>(this->x, this->z);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_yz() const
    {
        return _xvec2<T>(this->y, this->z);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_zz() const
    {
        return _xvec2<T>(this->z, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Left hand side 3 components common swizzle operator

    template <typename T>
    inline _xref3<T> _xvec3<T>::_zyx()
    {
        return _xref3<T>(this->z, this->y, this->x);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_yzx()
    {
        return _xref3<T>(this->y, this->z, this->x);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_zxy()
    {
        return _xref3<T>(this->z, this->x, this->y);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_xzy()
    {
        return _xref3<T>(this->x, this->z, this->y);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_yxz()
    {
        return _xref3<T>(this->y, this->x, this->z);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_xyz()
    {
        return _xref3<T>(this->x, this->y, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components common swizzle operators

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_xxx() const
    {
        return _xvec3<T>(this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_yxx() const
    {
        return _xvec3<T>(this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_zxx() const
    {
        return _xvec3<T>(this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_xyx() const
    {
        return _xvec3<T>(this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_yyx() const
    {
        return _xvec3<T>(this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_zyx() const
    {
        return _xvec3<T>(this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_xzx() const
    {
        return _xvec3<T>(this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_yzx() const
    {
        return _xvec3<T>(this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_zzx() const
    {
        return _xvec3<T>(this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_xxy() const
    {
        return _xvec3<T>(this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_yxy() const
    {
        return _xvec3<T>(this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_zxy() const
    {
        return _xvec3<T>(this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_xyy() const
    {
        return _xvec3<T>(this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_yyy() const
    {
        return _xvec3<T>(this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_zyy() const
    {
        return _xvec3<T>(this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_xzy() const
    {
        return _xvec3<T>(this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_yzy() const
    {
        return _xvec3<T>(this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_zzy() const
    {
        return _xvec3<T>(this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_xxz() const
    {
        return _xvec3<T>(this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_yxz() const
    {
        return _xvec3<T>(this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_zxz() const
    {
        return _xvec3<T>(this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_xyz() const
    {
        return _xvec3<T>(this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_yyz() const
    {
        return _xvec3<T>(this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_zyz() const
    {
        return _xvec3<T>(this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_xzz() const
    {
        return _xvec3<T>(this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_yzz() const
    {
        return _xvec3<T>(this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_zzz() const
    {
        return _xvec3<T>(this->z, this->z, this->z);
    }

    // 4 components swizzles operators
    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xxxx() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yxxx() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zxxx() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xyxx() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yyxx() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zyxx() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xzxx() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yzxx() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zzxx() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xxyx() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yxyx() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zxyx() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xyyx() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yyyx() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zyyx() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xzyx() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yzyx() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zzyx() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xxzx() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yxzx() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zxzx() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xyzx() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yyzx() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zyzx() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xzzx() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yzzx() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zzzx() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xxxy() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yxxy() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zxxy() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xyxy() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yyxy() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zyxy() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xzxy() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yzxy() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zzxy() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xxyy() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yxyy() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zxyy() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xyyy() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yyyy() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zyyy() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xzyy() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yzyy() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zzyy() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xxzy() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yxzy() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zxzy() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xyzy() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yyzy() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zyzy() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xzzy() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yzzy() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zzzy() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xxxz() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yxxz() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zxxz() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xyxz() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yyxz() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zyxz() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xzxz() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yzxz() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zzxz() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xxyz() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yxyz() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zxyz() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xyyz() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yyyz() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zyyz() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xzyz() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yzyz() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zzyz() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xxzz() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yxzz() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zxzz() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xyzz() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yyzz() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zyzz() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_xzzz() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_yzzz() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_zzzz() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->z);
    }
/*
    //////////////////////////////////////////////////////////////
    // 2 components swizzles operators

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_rr() const
    {
        return inline _xvec2<T>(this->x, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_gr() const
    {
        return inline _xvec2<T>(this->y, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_br() const
    {
        return inline _xvec2<T>(this->z, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_rg() const
    {
        return inline _xvec2<T>(this->x, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_gg() const
    {
        return inline _xvec2<T>(this->y, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_bg() const
    {
        return inline _xvec2<T>(this->z, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_rb() const
    {
        return inline _xvec2<T>(this->x, this->z);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_gb() const
    {
        return inline _xvec2<T>(this->y, this->z);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_bb() const
    {
        return inline _xvec2<T>(this->z, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Left hand side 3 components common swizzle operator

    template <typename T>
    inline _xref3<T> _xvec3<T>::_bgr()
    {
        return _xref3<T>(this->z, this->y, this->x);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_gbr()
    {
        return _xref3<T>(this->y, this->z, this->x);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_brg()
    {
        return _xref3<T>(this->z, this->x, this->y);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_rbg()
    {
        return _xref3<T>(this->x, this->z, this->y);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_grb()
    {
        return _xref3<T>(this->y, this->x, this->z);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_rgb()
    {
        return _xref3<T>(this->x, this->y, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components common swizzle operators

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_rrr() const
    {
        return _xvec3<T>(this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_grr() const
    {
        return _xvec3<T>(this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_brr() const
    {
        return _xvec3<T>(this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_rgr() const
    {
        return _xvec3<T>(this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_ggr() const
    {
        return _xvec3<T>(this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_bgr() const
    {
        return _xvec3<T>(this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_rbr() const
    {
        return _xvec3<T>(this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_gbr() const
    {
        return _xvec3<T>(this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_bbr() const
    {
        return _xvec3<T>(this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_rrg() const
    {
        return _xvec3<T>(this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_grg() const
    {
        return _xvec3<T>(this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_brg() const
    {
        return _xvec3<T>(this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_rgg() const
    {
        return _xvec3<T>(this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_ggg() const
    {
        return _xvec3<T>(this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_bgg() const
    {
        return _xvec3<T>(this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_rbg() const
    {
        return _xvec3<T>(this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_gbg() const
    {
        return _xvec3<T>(this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_bbg() const
    {
        return _xvec3<T>(this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_rrb() const
    {
        return _xvec3<T>(this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_grb() const
    {
        return _xvec3<T>(this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_brb() const
    {
        return _xvec3<T>(this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_rgb() const
    {
        return _xvec3<T>(this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_ggb() const
    {
        return _xvec3<T>(this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_bgb() const
    {
        return _xvec3<T>(this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_rbb() const
    {
        return _xvec3<T>(this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_gbb() const
    {
        return _xvec3<T>(this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_bbb() const
    {
        return _xvec3<T>(this->z, this->z, this->z);
    }

    // 4 components swizzles operators
    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rrrr() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_grrr() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_brrr() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rgrr() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ggrr() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bgrr() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rbrr() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gbrr() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bbrr() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rrgr() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_grgr() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_brgr() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rggr() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gggr() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bggr() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rbgr() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gbgr() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bbgr() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rrbr() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_grbr() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_brbr() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rgbr() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ggbr() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bgbr() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rbbr() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gbbr() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bbbr() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rrrg() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_grrg() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_brrg() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rgrg() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ggrg() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bgrg() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rbrg() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gbrg() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bbrg() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rrgg() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_grgg() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_brgg() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rggg() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gggg() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bggg() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rbgg() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gbgg() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bbgg() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rrbg() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_grbg() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_brbg() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rgbg() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ggbg() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bgbg() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rbbg() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gbbg() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bbbg() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rrrb() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_grrb() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_brrb() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rgrb() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ggrb() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bgrb() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rbrb() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gbrb() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bbrb() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rrgb() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_grgb() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_brgb() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rggb() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gggb() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bggb() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rbgb() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gbgb() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bbgb() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rrbb() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_grbb() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_brbb() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rgbb() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ggbb() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bgbb() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_rbbb() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_gbbb() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_bbbb() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->z);
    }

    //////////////////////////////////////////////////////////////
    // 2 components swizzles operators

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_ss() const
    {
        return inline _xvec2<T>(this->x, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_ts() const
    {
        return inline _xvec2<T>(this->y, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_ps() const
    {
        return inline _xvec2<T>(this->z, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_st() const
    {
        return inline _xvec2<T>(this->x, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_tt() const
    {
        return inline _xvec2<T>(this->y, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_pt() const
    {
        return inline _xvec2<T>(this->z, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_sp() const
    {
        return inline _xvec2<T>(this->x, this->z);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_tp() const
    {
        return inline _xvec2<T>(this->y, this->z);
    }

    template <typename T>
    inline _xvec2<T> _xvec3<T>::_pp() const
    {
        return inline _xvec2<T>(this->z, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Left hand side 3 components common swizzle operator

    template <typename T>
    inline _xref3<T> _xvec3<T>::_pts()
    {
        return _xref3<T>(this->z, this->y, this->x);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_tps()
    {
        return _xref3<T>(this->y, this->z, this->x);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_pst()
    {
        return _xref3<T>(this->z, this->x, this->y);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_spt()
    {
        return _xref3<T>(this->x, this->z, this->y);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_tsp()
    {
        return _xref3<T>(this->y, this->x, this->z);
    }

    template <typename T>
    inline _xref3<T> _xvec3<T>::_stp()
    {
        return _xref3<T>(this->x, this->y, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components common swizzle operators

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_sss() const
    {
        return _xvec3<T>(this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_tss() const
    {
        return _xvec3<T>(this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_pss() const
    {
        return _xvec3<T>(this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_sts() const
    {
        return _xvec3<T>(this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_tts() const
    {
        return _xvec3<T>(this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_pts() const
    {
        return _xvec3<T>(this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_sps() const
    {
        return _xvec3<T>(this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_tps() const
    {
        return _xvec3<T>(this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_pps() const
    {
        return _xvec3<T>(this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_sst() const
    {
        return _xvec3<T>(this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_tst() const
    {
        return _xvec3<T>(this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_pst() const
    {
        return _xvec3<T>(this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_stt() const
    {
        return _xvec3<T>(this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_ttt() const
    {
        return _xvec3<T>(this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_ptt() const
    {
        return _xvec3<T>(this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_spt() const
    {
        return _xvec3<T>(this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_tpt() const
    {
        return _xvec3<T>(this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_ppt() const
    {
        return _xvec3<T>(this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_ssp() const
    {
        return _xvec3<T>(this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_tsp() const
    {
        return _xvec3<T>(this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_psp() const
    {
        return _xvec3<T>(this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_stp() const
    {
        return _xvec3<T>(this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_ttp() const
    {
        return _xvec3<T>(this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_ptp() const
    {
        return _xvec3<T>(this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_spp() const
    {
        return _xvec3<T>(this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_tpp() const
    {
        return _xvec3<T>(this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec3<T> _xvec3<T>::_ppp() const
    {
        return _xvec3<T>(this->z, this->z, this->z);
    }

    // 4 components swizzles operators
    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ssss() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tsss() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_psss() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_stss() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ttss() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ptss() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_spss() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tpss() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ppss() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ssts() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tsts() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_psts() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_stts() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ttts() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ptts() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_spts() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tpts() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ppts() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ssps() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tsps() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_psps() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_stps() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ttps() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ptps() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_spps() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tpps() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ppps() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ssst() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tsst() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_psst() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_stst() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ttst() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ptst() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_spst() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tpst() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ppst() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_sstt() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tstt() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_pstt() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_sttt() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tttt() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_pttt() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_sptt() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tptt() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_pptt() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_sspt() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tspt() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_pspt() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_stpt() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ttpt() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ptpt() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_sppt() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tppt() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_pppt() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_sssp() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tssp() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_pssp() const
    {
        return _xvec4<T>(this->z, this->x, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_stsp() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ttsp() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ptsp() const
    {
        return _xvec4<T>(this->z, this->y, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_spsp() const
    {
        return _xvec4<T>(this->x, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tpsp() const
    {
        return _xvec4<T>(this->y, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ppsp() const
    {
        return _xvec4<T>(this->z, this->z, this->x, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_sstp() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tstp() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_pstp() const
    {
        return _xvec4<T>(this->z, this->x, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_sttp() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tttp() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_pttp() const
    {
        return _xvec4<T>(this->z, this->y, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_sptp() const
    {
        return _xvec4<T>(this->x, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tptp() const
    {
        return _xvec4<T>(this->y, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_pptp() const
    {
        return _xvec4<T>(this->z, this->z, this->y, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_sspp() const
    {
        return _xvec4<T>(this->x, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tspp() const
    {
        return _xvec4<T>(this->y, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_pspp() const
    {
        return _xvec4<T>(this->z, this->x, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_stpp() const
    {
        return _xvec4<T>(this->x, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ttpp() const
    {
        return _xvec4<T>(this->y, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_ptpp() const
    {
        return _xvec4<T>(this->z, this->y, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_sppp() const
    {
        return _xvec4<T>(this->x, this->z, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_tppp() const
    {
        return _xvec4<T>(this->y, this->z, this->z, this->z);
    }

    template <typename T>
    inline _xvec4<T> _xvec3<T>::_pppp() const
    {
        return _xvec4<T>(this->z, this->z, this->z, this->z);
    }
*/
} //namespace glm

#endif//__xvec3_inl__
