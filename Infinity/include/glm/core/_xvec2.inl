//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-27
// Updated : 2006-05-28
// Licence : This source is under GNU LGPL licence
// File    : _xvec2.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __xvec2_inl__
#define __xvec2_inl__

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
    inline _xvec2<T>::_xvec2() : 
        x(T(0)),
        y(T(0))
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const _xvec2<T>& v) :
        x(v.x),
        y(v.y)
    {}

    // Swizzle constructor
    template <typename T> 
    inline _xvec2<T>::_xvec2(const _xref2<T>& r) :
        x(r.x),
        y(r.y)
    {}

    // T constructors
    template <typename T> 
    inline _xvec2<T>::_xvec2(const T x) :
        x(x),
        y(x)
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const T a, const T b) :
        x(a),
        y(b)
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const T a, const _xvec2<T>& b) :
        x(a),
        y(b.x)
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const T a, const _xvec3<T>& b) :
        x(a),
        y(b.x)
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const T a, const _xvec4<T>& b) :
        x(a),
        y(b.x)
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const _xvec3<T>& a) :
        x(a.x),
        y(a.y)
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const _xvec4<T>& a) :
        x(a.x),
        y(a.y)
    {}

    // U constructors
    template <typename T> 
    template <typename U> 
    inline _xvec2<T>::_xvec2(const U x) :
        x(T(x)),
        y(T(x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec2<T>::_xvec2(const U a, const U b) :
        x(T(a)),
        y(T(b))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec2<T>::_xvec2(const U a, const _xvec2<U>& b) :
        x(T(a)),
        y(T(b.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec2<T>::_xvec2(const U a, const _xvec3<U>& b) :
        x(T(a)),
        y(T(b.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec2<T>::_xvec2(const U a, const _xvec4<U>& b) :
        x(T(a)),
        y(T(b.x))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec2<T>::_xvec2(const _xvec2<U>& a) :
        x(T(a.x)),
        y(T(a.y))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec2<T>::_xvec2(const _xvec3<U>& a) :
        x(T(a.x)),
        y(T(a.y))
    {}

    template <typename T> 
    template <typename U> 
    inline _xvec2<T>::_xvec2(const _xvec4<U>& a) :
        x(T(a.x)),
        y(T(a.y))
    {}

    // Bool constructors
    template <typename T> 
    inline _xvec2<T>::_xvec2(const bool x) :
        x(T(x)),
        y(T(x))
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const bool a, const bool b) :
        x(T(a)),
        y(T(b))
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const bool a, const _bvec2& b) :
        x(T(a)),
        y(T(b.x))
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const bool a, const _bvec3& b) :
        x(T(a)),
        y(T(b.x))
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const bool a, const _bvec4& b) :
        x(T(a)),
        y(T(b.x))
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const _bvec3& a) :
        x(T(a.x)),
        y(T(a.y))
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const _bvec4& a) :
        x(T(a.x)),
        y(T(a.y))
    {}

    template <typename T> 
    inline _xvec2<T>::_xvec2(const _bvec2& a) :
        x(T(a.x)),
        y(T(a.y))
    {}

    //////////////////////////////////////////////////////////////
    // vec2 and ivec2 accesses

    template <typename T> 
    inline T& _xvec2<T>::operator [] (int i)
    {
        return (&x)[i];
    }

    template <typename T> 
    inline T _xvec2<T>::operator [] (int i) const
    {
        return (&x)[i];
    }

    template <typename T> 
    inline _xvec2<T>::operator T* ()
    {
        return &x;
    }

    template <typename T> 
    inline _xvec2<T>::operator const T* () const 
    {
        return &x;
    }

    //////////////////////////////////////////////////////////////
    // vec2 and ivec2 operators

    template <typename T> 
    inline _xvec2<T>& _xvec2<T>::operator=(const _xvec2<T>& v)
    {
        this->x = v.x;
        this->y = v.y;
        return *this;
    }

    template <typename T> 
    inline _xvec2<T>& _xvec2<T>::operator+= (const T s)
    {
	    this->x += s;
	    this->y += s;
	    return *this;
    }

    template <typename T> 
    inline _xvec2<T>& _xvec2<T>::operator+= (const _xvec2<T>& v)
    {
	    this->x += v.x;
	    this->y += v.y;
	    return *this;
    }

    template <typename T> 
    inline _xvec2<T>& _xvec2<T>::operator-= (const T s)
    {
	    this->x -= s;
	    this->y -= s;
	    return *this;
    }

    template <typename T> 
    inline _xvec2<T>& _xvec2<T>::operator-= (const _xvec2<T>& v)
    {
	    this->x -= v.x;
	    this->y -= v.y;
	    return *this;
    }

    template <typename T> 
    inline _xvec2<T>& _xvec2<T>::operator*= (const T s)
    {
	    this->x *= s;
	    this->y *= s;
	    return *this;
    }

    template <typename T> 
    inline _xvec2<T>& _xvec2<T>::operator*= (const _xvec2<T>& v)
    {
	    this->x *= v.x;
	    this->y *= v.y;
	    return *this;
    }

    template <typename T> 
    inline _xvec2<T>& _xvec2<T>::operator/= (const T s)
    {
	    this->x /= s;
	    this->y /= s;
	    return *this;
    }

    template <typename T> 
    inline _xvec2<T>& _xvec2<T>::operator/= (const _xvec2<T>& v)
    {
	    this->x /= v.x;
	    this->y /= v.y;
	    return *this;
    }

    template <typename T> 
    inline _xvec2<T>& _xvec2<T>::operator++ ()
    {
	    ++this->x;
	    ++this->y;
	    return *this;
    }

    template <typename T> 
    inline _xvec2<T>& _xvec2<T>::operator-- ()
    {
	    --this->x;
	    --this->y;
	    return *this;
    }

    // Unary constant operators
    template <typename T> 
    inline const _xvec2<T> operator- (const _xvec2<T>& v)
    {
        return _xvec2<T>(
            -v.x, 
            -v.y);
    }

    template <typename T> 
    inline const _xvec2<T> operator++ (const _xvec2<T>& v, int)
    {
        return _xvec2<T>(
            v.x + T(1), 
            v.y + T(1));
    }

    template <typename T> 
    inline const _xvec2<T> operator-- (const _xvec2<T>& v, int)
    {
        return _xvec2<T>(
            v.x - T(1), 
            v.y - T(1));
    }

    // Binary operators
    template <typename T> 
    inline _xvec2<T> operator+ (const _xvec2<T>& v, const T s)
    {
	    return _xvec2<T>(
		    v.x + s,
		    v.y + s);
    }

    template <typename T> 
    inline _xvec2<T> operator+ (const T s, const _xvec2<T>& v)
    {
	    return _xvec2<T>(
		    v.x + s,
		    v.y + s);
    }

    template <typename T> 
    inline _xvec2<T> operator+ (const _xvec2<T>& v1, const _xvec2<T>& v2)
    {
	    return _xvec2<T>(
		    v1.x + v2.x,
		    v1.y + v2.y);
    }

    template <typename T> 
    inline _xvec2<T> operator- (const _xvec2<T>& v, const T s)
    {
	    return _xvec2<T>(
		    v.x - s,
		    v.y - s);
    }

    template <typename T> 
    inline _xvec2<T> operator- (const T s, const _xvec2<T>& v)
    {
	    return _xvec2<T>(
		    v.x - s,
		    v.y - s);
    }

    template <typename T> 
    inline _xvec2<T> operator- (const _xvec2<T>& v1, const _xvec2<T>& v2)
    {
	    return _xvec2<T>(
		    v1.x - v2.x,
		    v1.y - v2.y);
    }

    template <typename T> 
    inline _xvec2<T> operator* (const _xvec2<T>& v, const T s)
    {
	    return _xvec2<T>(
		    v.x * s,
		    v.y * s);
    }

    template <typename T> 
    inline _xvec2<T> operator* (const T s, const _xvec2<T>& v)
    {
	    return _xvec2<T>(
		    s * v.x,
		    s * v.y);
    }

    template <typename T> 
    inline _xvec2<T> operator* (const _xvec2<T>& v1, const _xvec2<T> & v2)
    {
	    return _xvec2<T>(
		    v1.x * v2.x,
		    v1.y * v2.y);
    }

    template <typename T> 
    inline _xvec2<T> operator/ (const _xvec2<T>& v, const T s)
    {
	    return _xvec2<T>(
		    v.x / s,
		    v.y / s);
    }

    template <typename T> 
    inline _xvec2<T> operator/ (const T s, const _xvec2<T>& v)
    {
	    return _xvec2<T>(
		    s / v.x,
		    s / v.y);
    }

    template <typename T> 
    inline _xvec2<T> operator/ (const _xvec2<T>& v1, const _xvec2<T>& v2)
    {
	    return _xvec2<T>(
		    v1.x / v2.x,
		    v1.y / v2.y);
    }

    //////////////////////////////////////////////////////////////
    // Left hand side 2 components common swizzle operator

    template <typename T>
    inline _xref2<T> _xvec2<T>::_xy()
    {
        return _xref2<T>(this->x, this->y);
    }

    template <typename T>
    inline _xref2<T> _xvec2<T>::_yx()
    {
        return _xref2<T>(this->y, this->x);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 2 components common swizzle operators

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_xx() const
    {
        return _xvec2<T>(this->x, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_yx() const
    {
        return _xvec2<T>(this->y, this->x);
    }

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_xy() const
    {
        return _xvec2<T>(this->x, this->y);
    }

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_yy() const
    {
        return _xvec2<T>(this->y, this->y);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components common swizzle operators

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_xxx() const
    {
        return _xvec3<T>(this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_yxx() const
    {
        return _xvec3<T>(this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_xyx() const
    {
        return _xvec3<T>(this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_yyx() const
    {
        return _xvec3<T>(this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_xxy() const
    {
        return _xvec3<T>(this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_yxy() const
    {
        return _xvec3<T>(this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_xyy() const
    {
        return _xvec3<T>(this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_yyy() const
    {
        return _xvec3<T>(this->y, this->y, this->y);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 4 components common swizzle operators

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_xxxx() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_yxxx() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_xyxx() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_yyxx() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_xxyx() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_yxyx() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_xyyx() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_yyyx() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->x);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_xxxy() const
    {
        return _xvec4<T>(this->x, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_yxxy() const
    {
        return _xvec4<T>(this->y, this->x, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_xyxy() const
    {
        return _xvec4<T>(this->x, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_yyxy() const
    {
        return _xvec4<T>(this->y, this->y, this->x, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_xxyy() const
    {
        return _xvec4<T>(this->x, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_yxyy() const
    {
        return _xvec4<T>(this->y, this->x, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_xyyy() const
    {
        return _xvec4<T>(this->x, this->y, this->y, this->y);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_yyyy() const
    {
        return _xvec4<T>(this->y, this->y, this->y, this->y);
    }
/*
    //////////////////////////////////////////////////////////////
    // Left hand side 2 components color swizzle operator

    template <typename T>
    inline _xref2<T> _xvec2<T>::_rg()
    {
        return _xref2<T>(this->r, this->g);
    }

    template <typename T>
    inline _xref2<T> _xvec2<T>::_gr()
    {
        return _xref2<T>(this->g, this->r);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 2 components color swizzle operators

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_rr() const
    {
        return _xvec2<T>(this->r, this->r);
    }

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_gr() const
    {
        return _xvec2<T>(this->g, this->r);
    }

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_rg() const
    {
        return _xvec2<T>(this->r, this->g);
    }

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_gg() const
    {
        return _xvec2<T>(this->g, this->g);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components color swizzle operators

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_rrr() const
    {
        return _xvec3<T>(this->r, this->r, this->r);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_grr() const
    {
        return _xvec3<T>(this->g, this->r, this->r);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_rgr() const
    {
        return _xvec3<T>(this->r, this->g, this->r);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_ggr() const
    {
        return _xvec3<T>(this->g, this->g, this->r);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_rrg() const
    {
        return _xvec3<T>(this->r, this->r, this->g);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_grg() const
    {
        return _xvec3<T>(this->g, this->r, this->g);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_rgg() const
    {
        return _xvec3<T>(this->r, this->g, this->g);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_ggg() const
    {
        return _xvec3<T>(this->g, this->g, this->g);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 4 components color swizzle operators

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_rrrr() const
    {
        return _xvec4<T>(this->r, this->r, this->r, this->r);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_grrr() const
    {
        return _xvec4<T>(this->g, this->r, this->r, this->r);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_rgrr() const
    {
        return _xvec4<T>(this->r, this->g, this->r, this->r);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_ggrr() const
    {
        return _xvec4<T>(this->g, this->g, this->r, this->r);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_rrgr() const
    {
        return _xvec4<T>(this->r, this->r, this->g, this->r);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_grgr() const
    {
        return _xvec4<T>(this->g, this->r, this->g, this->r);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_rggr() const
    {
        return _xvec4<T>(this->r, this->g, this->g, this->r);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_gggr() const
    {
        return _xvec4<T>(this->g, this->g, this->g, this->r);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_rrrg() const
    {
        return _xvec4<T>(this->r, this->r, this->r, this->g);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_grrg() const
    {
        return _xvec4<T>(this->g, this->r, this->r, this->g);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_rgrg() const
    {
        return _xvec4<T>(this->r, this->g, this->r, this->g);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_ggrg() const
    {
        return _xvec4<T>(this->g, this->g, this->r, this->g);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_rrgg() const
    {
        return _xvec4<T>(this->r, this->r, this->g, this->g);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_grgg() const
    {
        return _xvec4<T>(this->g, this->r, this->g, this->g);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_rggg() const
    {
        return _xvec4<T>(this->r, this->g, this->g, this->g);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_gggg() const
    {
        return _xvec4<T>(this->g, this->g, this->g, this->g);
    }

    //////////////////////////////////////////////////////////////
    // Left hand side 2 components texcoord swizzle operator

    template <typename T>
    inline _xref2<T> _xvec2<T>::_st()
    {
        return _xref2<T>(this->s, this->t);
    }

    template <typename T>
    inline _xref2<T> _xvec2<T>::_ts()
    {
        return _xref2<T>(this->t, this->s);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 2 components texcoord swizzle operators

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_ss() const
    {
        return _xvec2<T>(this->s, this->s);
    }

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_ts() const
    {
        return _xvec2<T>(this->t, this->s);
    }

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_st() const
    {
        return _xvec2<T>(this->s, this->t);
    }

    template <typename T>
    inline _xvec2<T> _xvec2<T>::_tt() const
    {
        return _xvec2<T>(this->t, this->t);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components texcoord swizzle operators

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_sss() const
    {
        return _xvec3<T>(this->s, this->s, this->s);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_tss() const
    {
        return _xvec3<T>(this->t, this->s, this->s);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_sts() const
    {
        return _xvec3<T>(this->s, this->t, this->s);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_tts() const
    {
        return _xvec3<T>(this->t, this->t, this->s);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_sst() const
    {
        return _xvec3<T>(this->s, this->s, this->t);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_tst() const
    {
        return _xvec3<T>(this->t, this->s, this->t);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_stt() const
    {
        return _xvec3<T>(this->s, this->t, this->t);
    }

    template <typename T>
    inline _xvec3<T> _xvec2<T>::_ttt() const
    {
        return _xvec3<T>(this->t, this->t, this->t);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 4 components texcoord swizzle operators

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_ssss() const
    {
        return _xvec4<T>(this->s, this->s, this->s, this->s);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_tsss() const
    {
        return _xvec4<T>(this->t, this->s, this->s, this->s);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_stss() const
    {
        return _xvec4<T>(this->s, this->t, this->s, this->s);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_ttss() const
    {
        return _xvec4<T>(this->t, this->t, this->s, this->s);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_ssts() const
    {
        return _xvec4<T>(this->s, this->s, this->t, this->s);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_tsts() const
    {
        return _xvec4<T>(this->t, this->s, this->t, this->s);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_stts() const
    {
        return _xvec4<T>(this->s, this->t, this->t, this->s);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_ttts() const
    {
        return _xvec4<T>(this->t, this->t, this->t, this->s);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_ssst() const
    {
        return _xvec4<T>(this->s, this->s, this->s, this->t);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_tsst() const
    {
        return _xvec4<T>(this->t, this->s, this->s, this->t);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_stst() const
    {
        return _xvec4<T>(this->s, this->t, this->s, this->t);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_ttst() const
    {
        return _xvec4<T>(this->t, this->t, this->s, this->t);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_sstt() const
    {
        return _xvec4<T>(this->s, this->s, this->t, this->t);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_tstt() const
    {
        return _xvec4<T>(this->t, this->s, this->t, this->t);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_sttt() const
    {
        return _xvec4<T>(this->s, this->t, this->t, this->t);
    }

    template <typename T>
    inline _xvec4<T> _xvec2<T>::_tttt() const
    {
        return _xvec4<T>(this->t, this->t, this->t, this->t);
    }
*/
} //namespace glm

#endif //__xvec2_inl__
