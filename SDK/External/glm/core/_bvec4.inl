//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-27
// Updated : 2006-04-27
// Licence : This source is under GNU LGPL licence
// File    : _bvec4.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __bvec4_inl__
#define __bvec4_inl__

#include "./_bvec2.h"
#include "./_bvec3.h"
#include "./_bvec4.h"
#include "./_bref2.h"
#include "./_bref3.h"
#include "./_bref4.h"
#include "./_swizzle.inl"

namespace glm
{
    //////////////////////////////////////////////////////////////
    // bvec4 constructors

    inline _bvec4::_bvec4() :
        x(false),
        y(false),
        z(false),
        w(false)
    {}

    inline _bvec4::_bvec4(const _bvec4& v) :
        x(v.x),
        y(v.y),
        z(v.z),
        w(v.w)
    {}

    //////////////////////////////////////////////////////////////
    // Swizzle constructors
    inline _bvec4::_bvec4(const _bref4& r) :
        x(r.x),
        y(r.y),
        z(r.z),
        w(r.w)
    {}

    // Bool constructors
    inline _bvec4::_bvec4(const bool a) :
        x(a),
        y(a),
        z(a),
        w(a)
    {}

    inline _bvec4::_bvec4(const bool a, const bool b, const bool c, const bool d) :
        x(a),
        y(b),
        z(c),
        w(b)
    {}

    inline _bvec4::_bvec4(const bool a, const bool b, const bool c, const _bvec2& d) :
        x(a),
        y(b),
        z(c),
        w(d.x)
    {}

    inline _bvec4::_bvec4(const bool a, const bool b, const bool c, const _bvec3& d) :
        x(a),
        y(b),
        z(c),
        w(d.x)
    {}

    inline _bvec4::_bvec4(const bool a, const bool b, const bool c, const _bvec4& d) :
        x(a),
        y(b),
        z(c),
        w(d.x)
    {}

    inline _bvec4::_bvec4(const bool a, const bool b, const _bvec2& c) :
        x(a),
        y(b),
        z(c.x),
        w(c.y)
    {}

    inline _bvec4::_bvec4(const bool a, const bool b, const _bvec3& c) :
        x(a),
        y(b),
        z(c.x),
        w(c.y)
    {}

    inline _bvec4::_bvec4(const bool a, const bool b, const _bvec4& c) :
        x(a),
        y(b),
        z(c.x),
        w(c.y)
    {}

    inline _bvec4::_bvec4(const bool a, const _bvec2& b, const bool c) :
        x(a),
        y(b.x),
        z(b.y),
        w(c)
    {}

    inline _bvec4::_bvec4(const bool a, const _bvec2& b, const _bvec2& c) :
        x(a),
        y(b.x),
        z(b.y),
        w(c.x)
    {}

    inline _bvec4::_bvec4(const bool a, const _bvec2& b, const _bvec3& c) :
        x(a),
        y(b.x),
        z(b.y),
        w(c.x)
    {}

    inline _bvec4::_bvec4(const bool a, const _bvec2& b, const _bvec4& c) :
        x(a),
        y(b.x),
        z(b.y),
        w(c.x)
    {}

    inline _bvec4::_bvec4(const bool a, const _bvec3& b) :
        x(a),
        y(b.x),
        z(b.y),
        w(b.z)
    {}

    inline _bvec4::_bvec4(const bool a, const _bvec4& b) :
        x(a),
        y(b.x),
        z(b.y),
        w(b.z)
    {}

    inline _bvec4::_bvec4(const _bvec2& a, const bool b, const bool c) :
        x(a.x),
        y(a.y),
        z(b),
        w(c)
    {}

    inline _bvec4::_bvec4(const _bvec2& a, const bool b, const _bvec2& c) :
        x(a.x),
        y(a.y),
        z(b),
        w(c.x)
    {}

    inline _bvec4::_bvec4(const _bvec2& a, const bool b, const _bvec3& c) :
        x(a.x),
        y(a.y),
        z(b),
        w(c.x)
    {}

    inline _bvec4::_bvec4(const _bvec2& a, const bool b, const _bvec4& c) :
        x(a.x),
        y(a.y),
        z(b),
        w(c.x)
    {}

    inline _bvec4::_bvec4(const _bvec2& a, const _bvec2& b) :
        x(a.x),
        y(a.y),
        z(b.x),
        w(b.y)
    {}

    inline _bvec4::_bvec4(const _bvec2& a, const _bvec3& b) :
        x(a.x),
        y(a.y),
        z(b.x),
        w(b.y)
    {}

    inline _bvec4::_bvec4(const _bvec2& a, const _bvec4& b) :
        x(a.x),
        y(a.y),
        z(b.x),
        w(b.y)
    {}

    inline _bvec4::_bvec4(const _bvec3& a, const bool b) :
        x(a.x),
        y(a.y),
        z(a.z),
        w(b)
    {}

    inline _bvec4::_bvec4(const _bvec3& a, const _bvec2& b) :
        x(a.x),
        y(a.y),
        z(a.z),
        w(b.x)
    {}

    inline _bvec4::_bvec4(const _bvec3& a, const _bvec3& b) :
        x(a.x),
        y(a.y),
        z(a.z),
        w(b.x)
    {}

    inline _bvec4::_bvec4(const _bvec3& a, const _bvec4& b) :
        x(a.x),
        y(a.y),
        z(a.z),
        w(b.x)
    {}

    //////////////////////////////////////////////////////////////
    // U constructors
    template <typename U> 
    inline _bvec4::_bvec4(const U a) :
        x(bool(a)),
        y(bool(a)),
        z(bool(a)),
        w(bool(a))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const U b, const U c, const U d) :
        x(bool(a)),
        y(bool(b)),
        z(bool(c)),
        w(bool(d))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const U b, const U c, const _xvec2<U>& d) :
        x(bool(a)),
        y(bool(b)),
        z(bool(c)),
        w(bool(d.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const U b, const U c, const _xvec3<U>& d) :
        x(bool(a)),
        y(bool(b)),
        z(bool(c)),
        w(bool(d.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const U b, const U c, const _xvec4<U>& d) :
        x(bool(a)),
        y(bool(b)),
        z(bool(c)),
        w(bool(d.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const U b, const _xvec2<U>& c) :
        x(bool(a)),
        y(bool(b)),
        z(bool(c.x)),
        w(bool(c.y))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const U b, const _xvec3<U>& c) :
        x(bool(a)),
        y(bool(b)),
        z(bool(c.x)),
        w(bool(c.y))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const U b, const _xvec4<U>& c) :
        x(bool(a)),
        y(bool(b)),
        z(bool(c.x)),
        w(bool(c.y))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const _xvec2<U>& b, const U c) :
        x(bool(a)),
        y(bool(b.x)),
        z(bool(b.y)),
        w(bool(c))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const _xvec2<U>& b, const _xvec2<U>& c) :
        x(bool(a)),
        y(bool(b.x)),
        z(bool(b.y)),
        w(bool(c.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const _xvec2<U>& b, const _xvec3<U>& c) :
        x(bool(a)),
        y(bool(b.x)),
        z(bool(b.y)),
        w(bool(c.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const _xvec2<U>& b, const _xvec4<U>& c) :
        x(bool(a)),
        y(bool(b.x)),
        z(bool(b.y)),
        w(bool(c.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const _xvec3<U>& b) :
        x(bool(a)),
        y(bool(b.x)),
        z(bool(b.y)),
        w(bool(b.z))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const U a, const _xvec4<U>& b) :
        x(bool(a)),
        y(bool(b.x)),
        z(bool(b.y)),
        w(bool(b.z))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec2<U>& a, const U b, const U c) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(b)),
        w(bool(c))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec2<U>& a, const U b, const _xvec2<U>& c) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(b)),
        w(bool(c.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec2<U>& a, const U b, const _xvec3<U>& c) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(b)),
        w(bool(c.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec2<U>& a, const U b, const _xvec4<U>& c) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(b)),
        w(bool(c.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec2<U>& a, const _xvec2<U>& b) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(b.x)),
        w(bool(b.y))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec2<U>& a, const _xvec3<U>& b) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(b.x)),
        w(bool(b.y))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec2<U>& a, const _xvec4<U>& b) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(b.x)),
        w(bool(b.y))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec3<U>& a, const U b) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(a.z)),
        w(bool(b))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec3<U>& a, const _xvec2<U>& b) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(a.z)),
        w(bool(b.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec3<U>& a, const _xvec3<U>& b) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(a.z)),
        w(bool(b.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec3<U>& a, const _xvec4<U>& b) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(a.z)),
        w(bool(b.x))
    {}

    template <typename U> 
    inline _bvec4::_bvec4(const _xvec4<U>& a) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(a.z)),
        w(bool(a.w))
    {}

    //////////////////////////////////////////////////////////////
    // bvec4 accesses

    inline bool& _bvec4::operator[](int i)
    {
        return (&x)[i];
    }

    inline bool _bvec4::operator[](int i) const
    {
        return (&x)[i];
    }

    inline _bvec4::operator bool*()
    {
        return &x;
    }

    inline _bvec4::operator const bool*() const 
    {
        return &x;
    }

    //////////////////////////////////////////////////////////////
    // bvec4 operators

    // This function shouldn't required but it seems that VC7.1 have an optimisation bug if this operator wasn't declared
    inline _bvec4& _bvec4::operator=(const _bvec4& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = v.w;
        return *this;
    }
/*
    inline _bvec4 _bvec4::operator--()
    {
	    this->x = !x;
	    this->y = !y;
        this->z = !z;
        this->w = !w;
	    return *this;
    }

    inline _bvec4 _bvec4::operator++()
    {
	    this->x = !x;
	    this->y = !y;
        this->z = !z;
        this->w = !w;
	    return *this;
    }

    inline const _bvec4 _bvec4::operator--(int n) const 
    {
		return _bvec4(
			!this->x, 
			!this->y, 
			!this->z, 
			!this->w);
    }

    inline const _bvec4 _bvec4::operator++(int n) const
    {
		return _bvec4(
			!this->x, 
			!this->y, 
			!this->z, 
			!this->w);
    }
*/
    inline _bvec4 _bvec4::operator!() const 
    {
		return _bvec4(
			!this->x, 
			!this->y, 
			!this->z, 
			!this->w);
    }

    // 2 components swizzles operators

    
    inline _bvec2 _bvec4::_xx() const
    {
        return _bvec2(this->x, this->x);
    }

    
    inline _bvec2 _bvec4::_yx() const
    {
        return _bvec2(this->y, this->x);
    }

    
    inline _bvec2 _bvec4::_zx() const
    {
        return _bvec2(this->z, this->x);
    }

    
    inline _bvec2 _bvec4::_wx() const
    {
        return _bvec2(this->w, this->x);
    }

    
    inline _bvec2 _bvec4::_xy() const
    {
        return _bvec2(this->x, this->y);
    }

    
    inline _bvec2 _bvec4::_yy() const
    {
        return _bvec2(this->y, this->y);
    }

    
    inline _bvec2 _bvec4::_zy() const
    {
        return _bvec2(this->z, this->y);
    }

    
    inline _bvec2 _bvec4::_wy() const
    {
        return _bvec2(this->w, this->y);
    }

    
    inline _bvec2 _bvec4::_xz() const
    {
        return _bvec2(this->x, this->z);
    }

    
    inline _bvec2 _bvec4::_yz() const
    {
        return _bvec2(this->y, this->z);
    }

    
    inline _bvec2 _bvec4::_zz() const
    {
        return _bvec2(this->z, this->z);
    }

    
    inline _bvec2 _bvec4::_wz() const
    {
        return _bvec2(this->w, this->z);
    }

    
    inline _bvec2 _bvec4::_xw() const
    {
        return _bvec2(this->x, this->w);
    }

    
    inline _bvec2 _bvec4::_yw() const
    {
        return _bvec2(this->y, this->w);
    }

    
    inline _bvec2 _bvec4::_zw() const
    {
        return _bvec2(this->z, this->w);
    }

    
    inline _bvec2 _bvec4::_ww() const
    {
        return _bvec2(this->w, this->w);
    }

    //////////////////////////////////////////////////////////////
    // 3 components swizzles operators

    
    inline _bvec3 _bvec4::_xxx() const
    {
        return _bvec3(this->x, this->x, this->x);
    }

    
    inline _bvec3 _bvec4::_yxx() const
    {
        return _bvec3(this->y, this->x, this->x);
    }

    
    inline _bvec3 _bvec4::_zxx() const
    {
        return _bvec3(this->z, this->x, this->x);
    }

    
    inline _bvec3 _bvec4::_wxx() const
    {
        return _bvec3(this->w, this->x, this->x);
    }

    
    inline _bvec3 _bvec4::_xyx() const
    {
        return _bvec3(this->x, this->y, this->x);
    }

    
    inline _bvec3 _bvec4::_yyx() const
    {
        return _bvec3(this->y, this->y, this->x);
    }

    
    inline _bvec3 _bvec4::_zyx() const
    {
        return _bvec3(this->z, this->y, this->x);
    }

    
    inline _bvec3 _bvec4::_wyx() const
    {
        return _bvec3(this->w, this->y, this->x);
    }

    
    inline _bvec3 _bvec4::_xzx() const
    {
        return _bvec3(this->x, this->z, this->x);
    }

    
    inline _bvec3 _bvec4::_yzx() const
    {
        return _bvec3(this->y, this->z, this->x);
    }

    
    inline _bvec3 _bvec4::_zzx() const
    {
        return _bvec3(this->z, this->z, this->x);
    }

    
    inline _bvec3 _bvec4::_wzx() const
    {
        return _bvec3(this->w, this->z, this->x);
    }

    
    inline _bvec3 _bvec4::_xwx() const
    {
        return _bvec3(this->x, this->w, this->x);
    }

    
    inline _bvec3 _bvec4::_ywx() const
    {
        return _bvec3(this->y, this->w, this->x);
    }

    
    inline _bvec3 _bvec4::_zwx() const
    {
        return _bvec3(this->z, this->w, this->x);
    }

    
    inline _bvec3 _bvec4::_wwx() const
    {
        return _bvec3(this->w, this->w, this->x);
    }

    
    inline _bvec3 _bvec4::_xxy() const
    {
        return _bvec3(this->x, this->x, this->y);
    }

    
    inline _bvec3 _bvec4::_yxy() const
    {
        return _bvec3(this->y, this->x, this->y);
    }

    
    inline _bvec3 _bvec4::_zxy() const
    {
        return _bvec3(this->z, this->x, this->y);
    }

    
    inline _bvec3 _bvec4::_wxy() const
    {
        return _bvec3(this->w, this->x, this->y);
    }

    
    inline _bvec3 _bvec4::_xyy() const
    {
        return _bvec3(this->x, this->y, this->y);
    }

    
    inline _bvec3 _bvec4::_yyy() const
    {
        return _bvec3(this->y, this->y, this->y);
    }

    
    inline _bvec3 _bvec4::_zyy() const
    {
        return _bvec3(this->z, this->y, this->y);
    }

    
    inline _bvec3 _bvec4::_wyy() const
    {
        return _bvec3(this->w, this->y, this->y);
    }

    
    inline _bvec3 _bvec4::_xzy() const
    {
        return _bvec3(this->x, this->z, this->y);
    }

    
    inline _bvec3 _bvec4::_yzy() const
    {
        return _bvec3(this->y, this->z, this->y);
    }

    
    inline _bvec3 _bvec4::_zzy() const
    {
        return _bvec3(this->z, this->z, this->y);
    }

    
    inline _bvec3 _bvec4::_wzy() const
    {
        return _bvec3(this->w, this->z, this->y);
    }

    
    inline _bvec3 _bvec4::_xwy() const
    {
        return _bvec3(this->x, this->w, this->y);
    }

    
    inline _bvec3 _bvec4::_ywy() const
    {
        return _bvec3(this->y, this->w, this->y);
    }

    
    inline _bvec3 _bvec4::_zwy() const
    {
        return _bvec3(this->z, this->w, this->y);
    }

    
    inline _bvec3 _bvec4::_wwy() const
    {
        return _bvec3(this->w, this->w, this->y);
    }

    
    inline _bvec3 _bvec4::_xxz() const
    {
        return _bvec3(this->x, this->x, this->z);
    }

    
    inline _bvec3 _bvec4::_yxz() const
    {
        return _bvec3(this->y, this->x, this->z);
    }

    
    inline _bvec3 _bvec4::_zxz() const
    {
        return _bvec3(this->z, this->x, this->z);
    }

    
    inline _bvec3 _bvec4::_wxz() const
    {
        return _bvec3(this->w, this->x, this->z);
    }

    
    inline _bvec3 _bvec4::_xyz() const
    {
        return _bvec3(this->x, this->y, this->z);
    }

    
    inline _bvec3 _bvec4::_yyz() const
    {
        return _bvec3(this->y, this->y, this->z);
    }

    
    inline _bvec3 _bvec4::_zyz() const
    {
        return _bvec3(this->z, this->y, this->z);
    }

    
    inline _bvec3 _bvec4::_wyz() const
    {
        return _bvec3(this->w, this->y, this->z);
    }

    
    inline _bvec3 _bvec4::_xzz() const
    {
        return _bvec3(this->x, this->z, this->z);
    }

    
    inline _bvec3 _bvec4::_yzz() const
    {
        return _bvec3(this->y, this->z, this->z);
    }

    
    inline _bvec3 _bvec4::_zzz() const
    {
        return _bvec3(this->z, this->z, this->z);
    }

    
    inline _bvec3 _bvec4::_wzz() const
    {
        return _bvec3(this->w, this->z, this->z);
    }

    
    inline _bvec3 _bvec4::_xwz() const
    {
        return _bvec3(this->x, this->w, this->z);
    }

    
    inline _bvec3 _bvec4::_ywz() const
    {
        return _bvec3(this->y, this->w, this->z);
    }

    
    inline _bvec3 _bvec4::_zwz() const
    {
        return _bvec3(this->z, this->w, this->z);
    }

    
    inline _bvec3 _bvec4::_wwz() const
    {
        return _bvec3(this->w, this->w, this->z);
    }

    
    inline _bvec3 _bvec4::_xxw() const
    {
        return _bvec3(this->x, this->x, this->w);
    }

    
    inline _bvec3 _bvec4::_yxw() const
    {
        return _bvec3(this->y, this->x, this->w);
    }

    
    inline _bvec3 _bvec4::_zxw() const
    {
        return _bvec3(this->z, this->x, this->w);
    }

    
    inline _bvec3 _bvec4::_wxw() const
    {
        return _bvec3(this->w, this->x, this->w);
    }

    
    inline _bvec3 _bvec4::_xyw() const
    {
        return _bvec3(this->x, this->y, this->w);
    }

    
    inline _bvec3 _bvec4::_yyw() const
    {
        return _bvec3(this->y, this->y, this->w);
    }

    
    inline _bvec3 _bvec4::_zyw() const
    {
        return _bvec3(this->z, this->y, this->w);
    }

    
    inline _bvec3 _bvec4::_wyw() const
    {
        return _bvec3(this->w, this->y, this->w);
    }

    
    inline _bvec3 _bvec4::_xzw() const
    {
        return _bvec3(this->x, this->z, this->w);
    }

    
    inline _bvec3 _bvec4::_yzw() const
    {
        return _bvec3(this->y, this->z, this->w);
    }

    
    inline _bvec3 _bvec4::_zzw() const
    {
        return _bvec3(this->z, this->z, this->w);
    }

    
    inline _bvec3 _bvec4::_wzw() const
    {
        return _bvec3(this->w, this->z, this->w);
    }

    
    inline _bvec3 _bvec4::_xww() const
    {
        return _bvec3(this->x, this->w, this->w);
    }

    
    inline _bvec3 _bvec4::_yww() const
    {
        return _bvec3(this->y, this->w, this->w);
    }

    
    inline _bvec3 _bvec4::_zww() const
    {
        return _bvec3(this->z, this->w, this->w);
    }

    
    inline _bvec3 _bvec4::_www() const
    {
        return _bvec3(this->w, this->w, this->w);
    }

    // 4 components swizzles operators
    
    inline _bref4 _bvec4::_wzyx()
    {
        return _bref4(this->w, this->z, this->y, this->x);
    }

    
    inline _bref4 _bvec4::_zwyx()
    {
        return _bref4(this->z, this->w, this->y, this->x);
    }

    
    inline _bref4 _bvec4::_wyzx()
    {
        return _bref4(this->w, this->y, this->z, this->x);
    }

    
    inline _bref4 _bvec4::_ywzx()
    {
        return _bref4(this->y, this->w, this->z, this->x);
    }

    
    inline _bref4 _bvec4::_zywx()
    {
        return _bref4(this->z, this->y, this->w, this->x);
    }

    
    inline _bref4 _bvec4::_yzwx()
    {
        return _bref4(this->y, this->z, this->w, this->x);
    }

    
    inline _bref4 _bvec4::_wzxy()
    {
        return _bref4(this->w, this->z, this->x, this->y);
    }

    
    inline _bref4 _bvec4::_zwxy()
    {
        return _bref4(this->z, this->w, this->x, this->y);
    }

    
    inline _bref4 _bvec4::_wxzy()
    {
        return _bref4(this->w, this->x, this->z, this->y);
    }

    
    inline _bref4 _bvec4::_xwzy()
    {
        return _bref4(this->x, this->w, this->z, this->y);
    }

    
    inline _bref4 _bvec4::_zxwy()
    {
        return _bref4(this->z, this->x, this->w, this->y);
    }

    
    inline _bref4 _bvec4::_xzwy()
    {
        return _bref4(this->x, this->z, this->w, this->y);
    }

    
    inline _bref4 _bvec4::_wyxz()
    {
        return _bref4(this->w, this->y, this->x, this->z);
    }

    
    inline _bref4 _bvec4::_ywxz()
    {
        return _bref4(this->y, this->w, this->x, this->z);
    }

    
    inline _bref4 _bvec4::_wxyz()
    {
        return _bref4(this->w, this->x, this->y, this->z);
    }

    
    inline _bref4 _bvec4::_xwyz()
    {
        return _bref4(this->x, this->w, this->y, this->z);
    }

    
    inline _bref4 _bvec4::_yxwz()
    {
        return _bref4(this->y, this->x, this->w, this->z);
    }

    
    inline _bref4 _bvec4::_xywz()
    {
        return _bref4(this->x, this->y, this->w, this->z);
    }

    
    inline _bref4 _bvec4::_zyxw()
    {
        return _bref4(this->z, this->y, this->x, this->w);
    }

    
    inline _bref4 _bvec4::_yzxw()
    {
        return _bref4(this->y, this->z, this->x, this->w);
    }

    
    inline _bref4 _bvec4::_zxyw()
    {
        return _bref4(this->z, this->x, this->y, this->w);
    }

    
    inline _bref4 _bvec4::_xzyw()
    {
        return _bref4(this->x, this->z, this->y, this->w);
    }

    
    inline _bref4 _bvec4::_yxzw()
    {
        return _bref4(this->y, this->x, this->z, this->w);
    }

    
    inline _bref4 _bvec4::_xyzw()
    {
        return _bref4(this->x, this->y, this->z, this->w);
    }

    // 4 components swizzles operators
    
    inline _bvec4 _bvec4::_xxxx() const
    {
        return _bvec4(this->x, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_yxxx() const
    {
        return _bvec4(this->y, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_zxxx() const
    {
        return _bvec4(this->z, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_wxxx() const
    {
        return _bvec4(this->w, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_xyxx() const
    {
        return _bvec4(this->x, this->y, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_yyxx() const
    {
        return _bvec4(this->y, this->y, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_zyxx() const
    {
        return _bvec4(this->z, this->y, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_wyxx() const
    {
        return _bvec4(this->w, this->y, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_xzxx() const
    {
        return _bvec4(this->x, this->z, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_yzxx() const
    {
        return _bvec4(this->y, this->z, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_zzxx() const
    {
        return _bvec4(this->z, this->z, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_wzxx() const
    {
        return _bvec4(this->w, this->z, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_xwxx() const
    {
        return _bvec4(this->x, this->w, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_ywxx() const
    {
        return _bvec4(this->y, this->w, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_zwxx() const
    {
        return _bvec4(this->z, this->w, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_wwxx() const
    {
        return _bvec4(this->w, this->w, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_xxyx() const
    {
        return _bvec4(this->x, this->x, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_yxyx() const
    {
        return _bvec4(this->y, this->x, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_zxyx() const
    {
        return _bvec4(this->z, this->x, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_wxyx() const
    {
        return _bvec4(this->w, this->x, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_xyyx() const
    {
        return _bvec4(this->x, this->y, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_yyyx() const
    {
        return _bvec4(this->y, this->y, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_zyyx() const
    {
        return _bvec4(this->z, this->y, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_wyyx() const
    {
        return _bvec4(this->w, this->y, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_xzyx() const
    {
        return _bvec4(this->x, this->z, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_yzyx() const
    {
        return _bvec4(this->y, this->z, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_zzyx() const
    {
        return _bvec4(this->z, this->z, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_wzyx() const
    {
        return _bvec4(this->w, this->z, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_xwyx() const
    {
        return _bvec4(this->x, this->w, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_ywyx() const
    {
        return _bvec4(this->y, this->w, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_zwyx() const
    {
        return _bvec4(this->z, this->w, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_wwyx() const
    {
        return _bvec4(this->w, this->w, this->y, this->x);
    }

    
    inline _bvec4 _bvec4::_xxzx() const
    {
        return _bvec4(this->x, this->x, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_yxzx() const
    {
        return _bvec4(this->y, this->x, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_zxzx() const
    {
        return _bvec4(this->z, this->x, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_wxzx() const
    {
        return _bvec4(this->w, this->x, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_xyzx() const
    {
        return _bvec4(this->x, this->y, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_yyzx() const
    {
        return _bvec4(this->y, this->y, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_zyzx() const
    {
        return _bvec4(this->z, this->y, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_wyzx() const
    {
        return _bvec4(this->w, this->y, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_xzzx() const
    {
        return _bvec4(this->x, this->z, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_yzzx() const
    {
        return _bvec4(this->y, this->z, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_zzzx() const
    {
        return _bvec4(this->z, this->z, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_wzzx() const
    {
        return _bvec4(this->w, this->z, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_xwzx() const
    {
        return _bvec4(this->x, this->w, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_ywzx() const
    {
        return _bvec4(this->y, this->w, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_zwzx() const
    {
        return _bvec4(this->z, this->w, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_wwzx() const
    {
        return _bvec4(this->w, this->w, this->z, this->x);
    }

    
    inline _bvec4 _bvec4::_xxwx() const
    {
        return _bvec4(this->x, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_yxwx() const
    {
        return _bvec4(this->y, this->x, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_zxwx() const
    {
        return _bvec4(this->z, this->x, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_wxwx() const
    {
        return _bvec4(this->w, this->x, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_xywx() const
    {
        return _bvec4(this->x, this->y, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_yywx() const
    {
        return _bvec4(this->y, this->y, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_zywx() const
    {
        return _bvec4(this->z, this->y, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_wywx() const
    {
        return _bvec4(this->w, this->y, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_xzwx() const
    {
        return _bvec4(this->x, this->z, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_yzwx() const
    {
        return _bvec4(this->y, this->z, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_zzwx() const
    {
        return _bvec4(this->z, this->z, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_wzwx() const
    {
        return _bvec4(this->w, this->z, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_xwwx() const
    {
        return _bvec4(this->x, this->w, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_ywwx() const
    {
        return _bvec4(this->y, this->w, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_zwwx() const
    {
        return _bvec4(this->z, this->w, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_wwwx() const
    {
        return _bvec4(this->w, this->w, this->w, this->x);
    }

    
    inline _bvec4 _bvec4::_xxxy() const
    {
        return _bvec4(this->x, this->x, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_yxxy() const
    {
        return _bvec4(this->y, this->x, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_zxxy() const
    {
        return _bvec4(this->z, this->x, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_wxxy() const
    {
        return _bvec4(this->w, this->x, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_xyxy() const
    {
        return _bvec4(this->x, this->y, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_yyxy() const
    {
        return _bvec4(this->y, this->y, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_zyxy() const
    {
        return _bvec4(this->z, this->y, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_wyxy() const
    {
        return _bvec4(this->w, this->y, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_xzxy() const
    {
        return _bvec4(this->x, this->z, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_yzxy() const
    {
        return _bvec4(this->y, this->z, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_zzxy() const
    {
        return _bvec4(this->z, this->z, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_wzxy() const
    {
        return _bvec4(this->w, this->z, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_xwxy() const
    {
        return _bvec4(this->x, this->w, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_ywxy() const
    {
        return _bvec4(this->y, this->w, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_zwxy() const
    {
        return _bvec4(this->z, this->w, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_wwxy() const
    {
        return _bvec4(this->w, this->w, this->x, this->y);
    }

    
    inline _bvec4 _bvec4::_xxyy() const
    {
        return _bvec4(this->x, this->x, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_yxyy() const
    {
        return _bvec4(this->y, this->x, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_zxyy() const
    {
        return _bvec4(this->z, this->x, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_wxyy() const
    {
        return _bvec4(this->w, this->x, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_xyyy() const
    {
        return _bvec4(this->x, this->y, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_yyyy() const
    {
        return _bvec4(this->y, this->y, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_zyyy() const
    {
        return _bvec4(this->z, this->y, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_wyyy() const
    {
        return _bvec4(this->w, this->y, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_xzyy() const
    {
        return _bvec4(this->x, this->z, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_yzyy() const
    {
        return _bvec4(this->y, this->z, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_zzyy() const
    {
        return _bvec4(this->z, this->z, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_wzyy() const
    {
        return _bvec4(this->w, this->z, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_xwyy() const
    {
        return _bvec4(this->x, this->w, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_ywyy() const
    {
        return _bvec4(this->y, this->w, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_zwyy() const
    {
        return _bvec4(this->z, this->w, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_wwyy() const
    {
        return _bvec4(this->w, this->w, this->y, this->y);
    }

    
    inline _bvec4 _bvec4::_xxzy() const
    {
        return _bvec4(this->x, this->x, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_yxzy() const
    {
        return _bvec4(this->y, this->x, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_zxzy() const
    {
        return _bvec4(this->z, this->x, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_wxzy() const
    {
        return _bvec4(this->w, this->x, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_xyzy() const
    {
        return _bvec4(this->x, this->y, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_yyzy() const
    {
        return _bvec4(this->y, this->y, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_zyzy() const
    {
        return _bvec4(this->z, this->y, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_wyzy() const
    {
        return _bvec4(this->w, this->y, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_xzzy() const
    {
        return _bvec4(this->x, this->z, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_yzzy() const
    {
        return _bvec4(this->y, this->z, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_zzzy() const
    {
        return _bvec4(this->z, this->z, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_wzzy() const
    {
        return _bvec4(this->w, this->z, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_xwzy() const
    {
        return _bvec4(this->x, this->w, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_ywzy() const
    {
        return _bvec4(this->y, this->w, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_zwzy() const
    {
        return _bvec4(this->z, this->w, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_wwzy() const
    {
        return _bvec4(this->w, this->w, this->z, this->y);
    }

    
    inline _bvec4 _bvec4::_xxwy() const
    {
        return _bvec4(this->x, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec4::_yxwy() const
    {
        return _bvec4(this->y, this->x, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_zxwy() const
    {
        return _bvec4(this->z, this->x, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_wxwy() const
    {
        return _bvec4(this->w, this->x, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_xywy() const
    {
        return _bvec4(this->x, this->y, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_yywy() const
    {
        return _bvec4(this->y, this->y, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_zywy() const
    {
        return _bvec4(this->z, this->y, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_wywy() const
    {
        return _bvec4(this->w, this->y, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_xzwy() const
    {
        return _bvec4(this->x, this->z, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_yzwy() const
    {
        return _bvec4(this->y, this->z, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_zzwy() const
    {
        return _bvec4(this->z, this->z, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_wzwy() const
    {
        return _bvec4(this->w, this->z, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_xwwy() const
    {
        return _bvec4(this->x, this->w, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_ywwy() const
    {
        return _bvec4(this->y, this->w, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_zwwy() const
    {
        return _bvec4(this->z, this->w, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_wwwy() const
    {
        return _bvec4(this->w, this->w, this->w, this->y);
    }

    
    inline _bvec4 _bvec4::_xxxz() const
    {
        return _bvec4(this->x, this->x, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_yxxz() const
    {
        return _bvec4(this->y, this->x, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_zxxz() const
    {
        return _bvec4(this->z, this->x, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_wxxz() const
    {
        return _bvec4(this->w, this->x, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_xyxz() const
    {
        return _bvec4(this->x, this->y, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_yyxz() const
    {
        return _bvec4(this->y, this->y, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_zyxz() const
    {
        return _bvec4(this->z, this->y, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_wyxz() const
    {
        return _bvec4(this->w, this->y, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_xzxz() const
    {
        return _bvec4(this->x, this->z, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_yzxz() const
    {
        return _bvec4(this->y, this->z, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_zzxz() const
    {
        return _bvec4(this->z, this->z, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_wzxz() const
    {
        return _bvec4(this->w, this->z, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_xwxz() const
    {
        return _bvec4(this->x, this->w, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_ywxz() const
    {
        return _bvec4(this->y, this->w, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_zwxz() const
    {
        return _bvec4(this->z, this->w, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_wwxz() const
    {
        return _bvec4(this->w, this->w, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_xxyz() const
    {
        return _bvec4(this->x, this->x, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_yxyz() const
    {
        return _bvec4(this->y, this->x, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_zxyz() const
    {
        return _bvec4(this->z, this->x, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_wxyz() const
    {
        return _bvec4(this->w, this->x, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_xyyz() const
    {
        return _bvec4(this->x, this->y, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_yyyz() const
    {
        return _bvec4(this->y, this->y, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_zyyz() const
    {
        return _bvec4(this->z, this->y, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_wyyz() const
    {
        return _bvec4(this->w, this->y, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_xzyz() const
    {
        return _bvec4(this->x, this->z, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_yzyz() const
    {
        return _bvec4(this->y, this->z, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_zzyz() const
    {
        return _bvec4(this->z, this->z, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_wzyz() const
    {
        return _bvec4(this->w, this->z, this->x, this->z);
    }

    
    inline _bvec4 _bvec4::_xwyz() const
    {
        return _bvec4(this->x, this->w, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_ywyz() const
    {
        return _bvec4(this->y, this->w, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_zwyz() const
    {
        return _bvec4(this->z, this->w, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_wwyz() const
    {
        return _bvec4(this->w, this->w, this->y, this->z);
    }

    
    inline _bvec4 _bvec4::_xxzz() const
    {
        return _bvec4(this->x, this->x, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_yxzz() const
    {
        return _bvec4(this->y, this->x, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_zxzz() const
    {
        return _bvec4(this->z, this->x, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_wxzz() const
    {
        return _bvec4(this->w, this->x, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_xyzz() const
    {
        return _bvec4(this->x, this->y, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_yyzz() const
    {
        return _bvec4(this->y, this->y, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_zyzz() const
    {
        return _bvec4(this->z, this->y, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_wyzz() const
    {
        return _bvec4(this->w, this->y, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_xzzz() const
    {
        return _bvec4(this->x, this->z, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_yzzz() const
    {
        return _bvec4(this->y, this->z, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_zzzz() const
    {
        return _bvec4(this->z, this->z, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_wzzz() const
    {
        return _bvec4(this->w, this->z, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_xwzz() const
    {
        return _bvec4(this->x, this->w, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_ywzz() const
    {
        return _bvec4(this->y, this->w, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_zwzz() const
    {
        return _bvec4(this->z, this->w, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_wwzz() const
    {
        return _bvec4(this->w, this->w, this->z, this->z);
    }

    
    inline _bvec4 _bvec4::_xxwz() const
    {
        return _bvec4(this->x, this->x, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_yxwz() const
    {
        return _bvec4(this->y, this->x, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_zxwz() const
    {
        return _bvec4(this->z, this->x, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_wxwz() const
    {
        return _bvec4(this->w, this->x, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_xywz() const
    {
        return _bvec4(this->x, this->y, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_yywz() const
    {
        return _bvec4(this->y, this->y, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_zywz() const
    {
        return _bvec4(this->z, this->y, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_wywz() const
    {
        return _bvec4(this->w, this->y, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_xzwz() const
    {
        return _bvec4(this->x, this->z, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_yzwz() const
    {
        return _bvec4(this->y, this->z, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_zzwz() const
    {
        return _bvec4(this->z, this->z, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_wzwz() const
    {
        return _bvec4(this->w, this->z, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_xwwz() const
    {
        return _bvec4(this->x, this->w, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_ywwz() const
    {
        return _bvec4(this->y, this->w, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_zwwz() const
    {
        return _bvec4(this->z, this->w, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_wwwz() const
    {
        return _bvec4(this->w, this->w, this->w, this->z);
    }

    
    inline _bvec4 _bvec4::_xxxw() const
    {
        return _bvec4(this->x, this->x, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_yxxw() const
    {
        return _bvec4(this->y, this->x, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_zxxw() const
    {
        return _bvec4(this->z, this->x, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_wxxw() const
    {
        return _bvec4(this->w, this->x, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_xyxw() const
    {
        return _bvec4(this->x, this->y, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_yyxw() const
    {
        return _bvec4(this->y, this->y, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_zyxw() const
    {
        return _bvec4(this->z, this->y, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_wyxw() const
    {
        return _bvec4(this->w, this->y, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_xzxw() const
    {
        return _bvec4(this->x, this->z, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_yzxw() const
    {
        return _bvec4(this->y, this->z, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_zzxw() const
    {
        return _bvec4(this->z, this->z, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_wzxw() const
    {
        return _bvec4(this->w, this->z, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_xwxw() const
    {
        return _bvec4(this->x, this->w, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_ywxw() const
    {
        return _bvec4(this->y, this->w, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_zwxw() const
    {
        return _bvec4(this->z, this->w, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_wwxw() const
    {
        return _bvec4(this->w, this->w, this->x, this->w);
    }

    
    inline _bvec4 _bvec4::_xxyw() const
    {
        return _bvec4(this->x, this->x, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_yxyw() const
    {
        return _bvec4(this->y, this->x, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_zxyw() const
    {
        return _bvec4(this->z, this->x, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_wxyw() const
    {
        return _bvec4(this->w, this->x, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_xyyw() const
    {
        return _bvec4(this->x, this->y, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_yyyw() const
    {
        return _bvec4(this->y, this->y, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_zyyw() const
    {
        return _bvec4(this->z, this->y, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_wyyw() const
    {
        return _bvec4(this->w, this->y, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_xzyw() const
    {
        return _bvec4(this->x, this->z, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_yzyw() const
    {
        return _bvec4(this->y, this->z, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_zzyw() const
    {
        return _bvec4(this->z, this->z, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_wzyw() const
    {
        return _bvec4(this->w, this->z, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_xwyw() const
    {
        return _bvec4(this->x, this->w, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_ywyw() const
    {
        return _bvec4(this->y, this->w, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_zwyw() const
    {
        return _bvec4(this->z, this->w, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_wwyw() const
    {
        return _bvec4(this->w, this->w, this->y, this->w);
    }

    
    inline _bvec4 _bvec4::_xxzw() const
    {
        return _bvec4(this->x, this->x, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_yxzw() const
    {
        return _bvec4(this->y, this->x, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_zxzw() const
    {
        return _bvec4(this->z, this->x, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_wxzw() const
    {
        return _bvec4(this->w, this->x, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_xyzw() const
    {
        return _bvec4(this->x, this->y, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_yyzw() const
    {
        return _bvec4(this->y, this->y, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_zyzw() const
    {
        return _bvec4(this->z, this->y, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_wyzw() const
    {
        return _bvec4(this->w, this->y, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_xzzw() const
    {
        return _bvec4(this->x, this->z, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_yzzw() const
    {
        return _bvec4(this->y, this->z, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_zzzw() const
    {
        return _bvec4(this->z, this->z, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_wzzw() const
    {
        return _bvec4(this->w, this->z, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_xwzw() const
    {
        return _bvec4(this->x, this->w, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_ywzw() const
    {
        return _bvec4(this->y, this->w, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_zwzw() const
    {
        return _bvec4(this->z, this->w, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_wwzw() const
    {
        return _bvec4(this->w, this->w, this->z, this->w);
    }

    
    inline _bvec4 _bvec4::_xxww() const
    {
        return _bvec4(this->x, this->x, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_yxww() const
    {
        return _bvec4(this->y, this->x, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_zxww() const
    {
        return _bvec4(this->z, this->x, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_wxww() const
    {
        return _bvec4(this->w, this->x, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_xyww() const
    {
        return _bvec4(this->x, this->y, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_yyww() const
    {
        return _bvec4(this->y, this->y, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_zyww() const
    {
        return _bvec4(this->z, this->y, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_wyww() const
    {
        return _bvec4(this->w, this->y, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_xzww() const
    {
        return _bvec4(this->x, this->z, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_yzww() const
    {
        return _bvec4(this->y, this->z, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_zzww() const
    {
        return _bvec4(this->z, this->z, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_wzww() const
    {
        return _bvec4(this->w, this->z, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_xwww() const
    {
        return _bvec4(this->x, this->w, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_ywww() const
    {
        return _bvec4(this->y, this->w, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_zwww() const
    {
        return _bvec4(this->z, this->w, this->w, this->w);
    }

    
    inline _bvec4 _bvec4::_wwww() const
    {
        return _bvec4(this->w, this->w, this->w, this->w);
    }
} //namespace glm

#endif//__bvec4_inl__
