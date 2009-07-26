//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-27
// Updated : 2006-04-27
// Licence : This source is under GNU LGPL licence
// File    : _bvec3.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __bvec3_inl__
#define __bvec3_inl__

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
    // Common constructors

    inline _bvec3::_bvec3() :
        x(false),
        y(false),
        z(false)
    {}

    inline _bvec3::_bvec3(const _bvec3& v) : 
        x(v.x),
        y(v.y),
        z(v.z)
    {}

    //////////////////////////////////////////////////////////////
    // Swizzle constructors
    inline _bvec3::_bvec3(const _bref3& r) :
        x(r.x),
        y(r.y),
        z(r.z)
    {}

    //////////////////////////////////////////////////////////////
    // Bool constructors

    inline _bvec3::_bvec3(const bool a) :
        x(a),
        y(a),
        z(a)
    {}

    inline _bvec3::_bvec3(const bool a, const bool b, const bool c) :
        x(a),
        y(b),
        z(c)
    {}

    inline _bvec3::_bvec3(const bool a, const bool b, const _bvec2& c) :
        x(a),
        y(b),
        z(c.x)
    {}

    inline _bvec3::_bvec3(const bool a, const bool b, const _bvec3& c) :
        x(a),
        y(b),
        z(c.x)
    {}

    inline _bvec3::_bvec3(const bool a, const bool b, const _bvec4& c) :
        x(a),
        y(b),
        z(c.x)
    {}

    inline _bvec3::_bvec3(const bool a, const _bvec2& b) :
        x(a),
        y(b.x),
        z(b.y)
    {}

    inline _bvec3::_bvec3(const bool a, const _bvec3& b) :
        x(a),
        y(b.x),
        z(b.y)
    {}

    inline _bvec3::_bvec3(const bool a, const _bvec4& b) :
        x(a),
        y(b.x),
        z(b.y)
    {}

    inline _bvec3::_bvec3(const _bvec2& a, bool b) :
        x(a.x),
        y(a.y),
        z(b)
    {}

    inline _bvec3::_bvec3(const _bvec2& a, const _bvec2& b) :
        x(a.x),
        y(a.y),
        z(b.x)
    {}

    inline _bvec3::_bvec3(const _bvec2& a, const _bvec3& b) :
        x(a.x),
        y(a.y),
        z(b.x)
    {}

    inline _bvec3::_bvec3(const _bvec2& a, const _bvec4& b) :
        x(a.x),
        y(a.y),
        z(b.x)
    {}

    inline _bvec3::_bvec3(const _bvec4& a) :
        x(a.x),
        y(a.y),
        z(a.z)
    {}

    //////////////////////////////////////////////////////////////
    // U constructors

    template <typename U> 
    inline _bvec3::_bvec3(const U a) :
        x(bool(a)),
        y(bool(a)),
        z(bool(a))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const U a, const U b, const U c) :
        x(bool(a)),
        y(bool(b)),
        z(bool(c))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const U a, const U b, const _xvec2<U>& c) :
        x(bool(a)),
        y(bool(b)),
        z(bool(c.x))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const U a, const U b, const _xvec3<U>& c) :
        x(bool(a)),
        y(bool(b)),
        z(bool(c.x))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const U a, const U b, const _xvec4<U>& c) :
        x(bool(a)),
        y(bool(b)),
        z(bool(c.x))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const U a, const _xvec2<U>& b) :
        x(bool(a)),
        y(bool(b.x)),
        z(bool(b.y))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const U a, const _xvec3<U>& b) :
        x(bool(a)),
        y(bool(b.x)),
        z(bool(b.y))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const U a, const _xvec4<U>& b) :
        x(bool(a)),
        y(bool(b.x)),
        z(bool(b.y))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const _xvec2<U>& a, U b) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(b))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const _xvec2<U>& a, const _xvec2<U>& b) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(b.x))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const _xvec2<U>& a, const _xvec3<U>& b) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(b.x))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const _xvec2<U>& a, const _xvec4<U>& b) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(b.x))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const _xvec3<U>& a) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(a.z))
    {}

    template <typename U> 
    inline _bvec3::_bvec3(const _xvec4<U>& a) :
        x(bool(a.x)),
        y(bool(a.y)),
        z(bool(a.z))
    {}

    //////////////////////////////////////////////////////////////
    // bvec3 accesses

    inline bool& _bvec3::operator [] (int i)
    {
        return (&x)[i];
    }

    inline bool _bvec3::operator [] (int i) const
    {
        return (&x)[i];
    }

    inline _bvec3::operator bool* ()
    {
        return &x;
    }

    inline _bvec3::operator const bool* () const 
    {
        return &x;
    }

    //////////////////////////////////////////////////////////////
    // bvec3 operators

    // This function shouldn't required but it seems that VC7.1 have an optimisation bug if this operator wasn't declared
    inline _bvec3& _bvec3::operator=(const _bvec3& x)
    {
        this->x = x.x;
        this->y = x.y;
        this->z = x.z;
        return *this;
    }
/*
    inline _bvec3 _bvec3::operator--()
    {
	    this->x = !x;
	    this->y = !y;
        this->z = !z;
	    return *this;
    }

    inline _bvec3 _bvec3::operator++()
    {
	    this->x = !x;
	    this->y = !y;
        this->z = !z;
	    return *this;
    }

    inline const _bvec3 _bvec3::operator--(int n) const 
    {
		return _bvec3(
			!this->x, 
			!this->y, 
			!this->z);
    }

    inline const _bvec3 _bvec3::operator++(int n) const
    {
		return _bvec3(
			!this->x, 
			!this->y, 
			!this->z);
    }
*/
    inline _bvec3 _bvec3::operator!() const 
    {
		return _bvec3(
			!this->x, 
			!this->y, 
			!this->z);
    }

    //////////////////////////////////////////////////////////////
    // 2 components swizzles operators

    inline _bvec2 _bvec3::_xx() const
    {
        return _bvec2(this->x, this->x);
    }

    inline _bvec2 _bvec3::_yx() const
    {
        return _bvec2(this->y, this->x);
    }

    inline _bvec2 _bvec3::_zx() const
    {
        return _bvec2(this->z, this->x);
    }

    inline _bvec2 _bvec3::_xy() const
    {
        return _bvec2(this->x, this->y);
    }

    inline _bvec2 _bvec3::_yy() const
    {
        return _bvec2(this->y, this->y);
    }

    inline _bvec2 _bvec3::_zy() const
    {
        return _bvec2(this->z, this->y);
    }

    inline _bvec2 _bvec3::_xz() const
    {
        return _bvec2(this->x, this->z);
    }

    inline _bvec2 _bvec3::_yz() const
    {
        return _bvec2(this->y, this->z);
    }

    inline _bvec2 _bvec3::_zz() const
    {
        return _bvec2(this->z, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Left hand side 3 components common swizzle operator

    
    inline _bref3 _bvec3::_zyx()
    {
        return _bref3(this->z, this->y, this->x);
    }

    
    inline _bref3 _bvec3::_yzx()
    {
        return _bref3(this->y, this->z, this->x);
    }

    
    inline _bref3 _bvec3::_zxy()
    {
        return _bref3(this->z, this->x, this->y);
    }

    
    inline _bref3 _bvec3::_xzy()
    {
        return _bref3(this->x, this->z, this->y);
    }

    
    inline _bref3 _bvec3::_yxz()
    {
        return _bref3(this->y, this->x, this->z);
    }

    
    inline _bref3 _bvec3::_xyz()
    {
        return _bref3(this->x, this->y, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components common swizzle operators

    
    inline _bvec3 _bvec3::_xxx() const
    {
        return _bvec3(this->x, this->x, this->x);
    }

    
    inline _bvec3 _bvec3::_yxx() const
    {
        return _bvec3(this->y, this->x, this->x);
    }

    
    inline _bvec3 _bvec3::_zxx() const
    {
        return _bvec3(this->z, this->x, this->x);
    }

    
    inline _bvec3 _bvec3::_xyx() const
    {
        return _bvec3(this->x, this->y, this->x);
    }

    
    inline _bvec3 _bvec3::_yyx() const
    {
        return _bvec3(this->y, this->y, this->x);
    }

    
    inline _bvec3 _bvec3::_zyx() const
    {
        return _bvec3(this->z, this->y, this->x);
    }

    
    inline _bvec3 _bvec3::_xzx() const
    {
        return _bvec3(this->x, this->z, this->x);
    }

    
    inline _bvec3 _bvec3::_yzx() const
    {
        return _bvec3(this->y, this->z, this->x);
    }

    
    inline _bvec3 _bvec3::_zzx() const
    {
        return _bvec3(this->z, this->z, this->x);
    }

    
    inline _bvec3 _bvec3::_xxy() const
    {
        return _bvec3(this->x, this->x, this->y);
    }

    
    inline _bvec3 _bvec3::_yxy() const
    {
        return _bvec3(this->y, this->x, this->y);
    }

    
    inline _bvec3 _bvec3::_zxy() const
    {
        return _bvec3(this->z, this->x, this->y);
    }

    
    inline _bvec3 _bvec3::_xyy() const
    {
        return _bvec3(this->x, this->y, this->y);
    }

    
    inline _bvec3 _bvec3::_yyy() const
    {
        return _bvec3(this->y, this->y, this->y);
    }

    
    inline _bvec3 _bvec3::_zyy() const
    {
        return _bvec3(this->z, this->y, this->y);
    }

    
    inline _bvec3 _bvec3::_xzy() const
    {
        return _bvec3(this->x, this->z, this->y);
    }

    
    inline _bvec3 _bvec3::_yzy() const
    {
        return _bvec3(this->y, this->z, this->y);
    }

    
    inline _bvec3 _bvec3::_zzy() const
    {
        return _bvec3(this->z, this->z, this->y);
    }

    
    inline _bvec3 _bvec3::_xxz() const
    {
        return _bvec3(this->x, this->x, this->z);
    }

    
    inline _bvec3 _bvec3::_yxz() const
    {
        return _bvec3(this->y, this->x, this->z);
    }

    
    inline _bvec3 _bvec3::_zxz() const
    {
        return _bvec3(this->z, this->x, this->z);
    }

    
    inline _bvec3 _bvec3::_xyz() const
    {
        return _bvec3(this->x, this->y, this->z);
    }

    
    inline _bvec3 _bvec3::_yyz() const
    {
        return _bvec3(this->y, this->y, this->z);
    }

    
    inline _bvec3 _bvec3::_zyz() const
    {
        return _bvec3(this->z, this->y, this->z);
    }

    
    inline _bvec3 _bvec3::_xzz() const
    {
        return _bvec3(this->x, this->z, this->z);
    }

    
    inline _bvec3 _bvec3::_yzz() const
    {
        return _bvec3(this->y, this->z, this->z);
    }

    
    inline _bvec3 _bvec3::_zzz() const
    {
        return _bvec3(this->z, this->z, this->z);
    }

    // 4 components swizzles operators

    inline _bvec4 _bvec3::_xxxx() const
    {
        return _bvec4(this->x, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_yxxx() const
    {
        return _bvec4(this->y, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_zxxx() const
    {
        return _bvec4(this->z, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_xyxx() const
    {
        return _bvec4(this->x, this->y, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_yyxx() const
    {
        return _bvec4(this->y, this->y, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_zyxx() const
    {
        return _bvec4(this->z, this->y, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_xzxx() const
    {
        return _bvec4(this->x, this->z, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_yzxx() const
    {
        return _bvec4(this->y, this->z, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_zzxx() const
    {
        return _bvec4(this->z, this->z, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_xxyx() const
    {
        return _bvec4(this->x, this->x, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_yxyx() const
    {
        return _bvec4(this->y, this->x, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_zxyx() const
    {
        return _bvec4(this->z, this->x, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_xyyx() const
    {
        return _bvec4(this->x, this->y, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_yyyx() const
    {
        return _bvec4(this->y, this->y, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_zyyx() const
    {
        return _bvec4(this->z, this->y, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_xzyx() const
    {
        return _bvec4(this->x, this->z, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_yzyx() const
    {
        return _bvec4(this->y, this->z, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_zzyx() const
    {
        return _bvec4(this->z, this->z, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_xxzx() const
    {
        return _bvec4(this->x, this->x, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_yxzx() const
    {
        return _bvec4(this->y, this->x, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_zxzx() const
    {
        return _bvec4(this->z, this->x, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_xyzx() const
    {
        return _bvec4(this->x, this->y, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_yyzx() const
    {
        return _bvec4(this->y, this->y, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_zyzx() const
    {
        return _bvec4(this->z, this->y, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_xzzx() const
    {
        return _bvec4(this->x, this->z, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_yzzx() const
    {
        return _bvec4(this->y, this->z, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_zzzx() const
    {
        return _bvec4(this->z, this->z, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_xxxy() const
    {
        return _bvec4(this->x, this->x, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_yxxy() const
    {
        return _bvec4(this->y, this->x, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_zxxy() const
    {
        return _bvec4(this->z, this->x, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_xyxy() const
    {
        return _bvec4(this->x, this->y, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_yyxy() const
    {
        return _bvec4(this->y, this->y, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_zyxy() const
    {
        return _bvec4(this->z, this->y, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_xzxy() const
    {
        return _bvec4(this->x, this->z, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_yzxy() const
    {
        return _bvec4(this->y, this->z, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_zzxy() const
    {
        return _bvec4(this->z, this->z, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_xxyy() const
    {
        return _bvec4(this->x, this->x, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_yxyy() const
    {
        return _bvec4(this->y, this->x, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_zxyy() const
    {
        return _bvec4(this->z, this->x, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_xyyy() const
    {
        return _bvec4(this->x, this->y, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_yyyy() const
    {
        return _bvec4(this->y, this->y, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_zyyy() const
    {
        return _bvec4(this->z, this->y, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_xzyy() const
    {
        return _bvec4(this->x, this->z, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_yzyy() const
    {
        return _bvec4(this->y, this->z, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_zzyy() const
    {
        return _bvec4(this->z, this->z, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_xxzy() const
    {
        return _bvec4(this->x, this->x, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_yxzy() const
    {
        return _bvec4(this->y, this->x, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_zxzy() const
    {
        return _bvec4(this->z, this->x, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_xyzy() const
    {
        return _bvec4(this->x, this->y, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_yyzy() const
    {
        return _bvec4(this->y, this->y, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_zyzy() const
    {
        return _bvec4(this->z, this->y, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_xzzy() const
    {
        return _bvec4(this->x, this->z, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_yzzy() const
    {
        return _bvec4(this->y, this->z, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_zzzy() const
    {
        return _bvec4(this->z, this->z, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_xxxz() const
    {
        return _bvec4(this->x, this->x, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_yxxz() const
    {
        return _bvec4(this->y, this->x, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_zxxz() const
    {
        return _bvec4(this->z, this->x, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_xyxz() const
    {
        return _bvec4(this->x, this->y, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_yyxz() const
    {
        return _bvec4(this->y, this->y, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_zyxz() const
    {
        return _bvec4(this->z, this->y, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_xzxz() const
    {
        return _bvec4(this->x, this->z, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_yzxz() const
    {
        return _bvec4(this->y, this->z, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_zzxz() const
    {
        return _bvec4(this->z, this->z, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_xxyz() const
    {
        return _bvec4(this->x, this->x, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_yxyz() const
    {
        return _bvec4(this->y, this->x, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_zxyz() const
    {
        return _bvec4(this->z, this->x, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_xyyz() const
    {
        return _bvec4(this->x, this->y, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_yyyz() const
    {
        return _bvec4(this->y, this->y, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_zyyz() const
    {
        return _bvec4(this->z, this->y, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_xzyz() const
    {
        return _bvec4(this->x, this->z, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_yzyz() const
    {
        return _bvec4(this->y, this->z, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_zzyz() const
    {
        return _bvec4(this->z, this->z, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_xxzz() const
    {
        return _bvec4(this->x, this->x, this->z, this->z);
    }

    
    inline _bvec4 _bvec3::_yxzz() const
    {
        return _bvec4(this->y, this->x, this->z, this->z);
    }

    
    inline _bvec4 _bvec3::_zxzz() const
    {
        return _bvec4(this->z, this->x, this->z, this->z);
    }

    
    inline _bvec4 _bvec3::_xyzz() const
    {
        return _bvec4(this->x, this->y, this->z, this->z);
    }

    
    inline _bvec4 _bvec3::_yyzz() const
    {
        return _bvec4(this->y, this->y, this->z, this->z);
    }

    
    inline _bvec4 _bvec3::_zyzz() const
    {
        return _bvec4(this->z, this->y, this->z, this->z);
    }

    
    inline _bvec4 _bvec3::_xzzz() const
    {
        return _bvec4(this->x, this->z, this->z, this->z);
    }

    
    inline _bvec4 _bvec3::_yzzz() const
    {
        return _bvec4(this->y, this->z, this->z, this->z);
    }

    
    inline _bvec4 _bvec3::_zzzz() const
    {
        return _bvec4(this->z, this->z, this->z, this->z);
    }
/*
    //////////////////////////////////////////////////////////////
    // 2 components swizzles operators
    
    inline _bvec2 _bvec3::_rr() const
    {
        return _bvec2(this->x, this->x);
    }

    
    inline _bvec2 _bvec3::_gr() const
    {
        return _bvec2(this->y, this->x);
    }

    
    inline _bvec2 _bvec3::_br() const
    {
        return _bvec2(this->z, this->x);
    }

    
    inline _bvec2 _bvec3::_rg() const
    {
        return _bvec2(this->x, this->y);
    }

    
    inline _bvec2 _bvec3::_gg() const
    {
        return _bvec2(this->y, this->y);
    }

    
    inline _bvec2 _bvec3::_bg() const
    {
        return _bvec2(this->z, this->y);
    }

    
    inline _bvec2 _bvec3::_rb() const
    {
        return _bvec2(this->x, this->z);
    }

    
    inline _bvec2 _bvec3::_gb() const
    {
        return _bvec2(this->y, this->z);
    }

    
    inline _bvec2 _bvec3::_bb() const
    {
        return _bvec2(this->z, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Left hand side 3 components common swizzle operator

    
    inline _bref3 _bvec3::_bgr()
    {
        return _bref3(this->z, this->y, this->x);
    }

    
    inline _bref3 _bvec3::_gbr()
    {
        return _bref3(this->y, this->z, this->x);
    }

    
    inline _bref3 _bvec3::_brg()
    {
        return _bref3(this->z, this->x, this->y);
    }

    
    inline _bref3 _bvec3::_rbg()
    {
        return _bref3(this->x, this->z, this->y);
    }

    
    inline _bref3 _bvec3::_grb()
    {
        return _bref3(this->y, this->x, this->z);
    }

    
    inline _bref3 _bvec3::_rgb()
    {
        return _bref3(this->x, this->y, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components common swizzle operators

    
    inline _bvec3 _bvec3::_rrr() const
    {
        return _bvec3(this->x, this->x, this->x);
    }

    
    inline _bvec3 _bvec3::_grr() const
    {
        return _bvec3(this->y, this->x, this->x);
    }

    
    inline _bvec3 _bvec3::_brr() const
    {
        return _bvec3(this->z, this->x, this->x);
    }

    
    inline _bvec3 _bvec3::_rgr() const
    {
        return _bvec3(this->x, this->y, this->x);
    }

    
    inline _bvec3 _bvec3::_ggr() const
    {
        return _bvec3(this->y, this->y, this->x);
    }

    
    inline _bvec3 _bvec3::_bgr() const
    {
        return _bvec3(this->z, this->y, this->x);
    }

    
    inline _bvec3 _bvec3::_rbr() const
    {
        return _bvec3(this->x, this->z, this->x);
    }

    
    inline _bvec3 _bvec3::_gbr() const
    {
        return _bvec3(this->y, this->z, this->x);
    }

    
    inline _bvec3 _bvec3::_bbr() const
    {
        return _bvec3(this->z, this->z, this->x);
    }

    
    inline _bvec3 _bvec3::_rrg() const
    {
        return _bvec3(this->x, this->x, this->y);
    }

    
    inline _bvec3 _bvec3::_grg() const
    {
        return _bvec3(this->y, this->x, this->y);
    }

    
    inline _bvec3 _bvec3::_brg() const
    {
        return _bvec3(this->z, this->x, this->y);
    }

    
    inline _bvec3 _bvec3::_rgg() const
    {
        return _bvec3(this->x, this->y, this->y);
    }

    
    inline _bvec3 _bvec3::_ggg() const
    {
        return _bvec3(this->y, this->y, this->y);
    }

    
    inline _bvec3 _bvec3::_bgg() const
    {
        return _bvec3(this->z, this->y, this->y);
    }

    
    inline _bvec3 _bvec3::_rbg() const
    {
        return _bvec3(this->x, this->z, this->y);
    }

    
    inline _bvec3 _bvec3::_gbg() const
    {
        return _bvec3(this->y, this->z, this->y);
    }

    
    inline _bvec3 _bvec3::_bbg() const
    {
        return _bvec3(this->z, this->z, this->y);
    }

    
    inline _bvec3 _bvec3::_rrb() const
    {
        return _bvec3(this->x, this->x, this->z);
    }

    
    inline _bvec3 _bvec3::_grb() const
    {
        return _bvec3(this->y, this->x, this->z);
    }

    
    inline _bvec3 _bvec3::_brb() const
    {
        return _bvec3(this->z, this->x, this->z);
    }

    
    inline _bvec3 _bvec3::_rgb() const
    {
        return _bvec3(this->x, this->y, this->z);
    }

    
    inline _bvec3 _bvec3::_ggb() const
    {
        return _bvec3(this->y, this->y, this->z);
    }

    
    inline _bvec3 _bvec3::_bgb() const
    {
        return _bvec3(this->z, this->y, this->z);
    }

    
    inline _bvec3 _bvec3::_rbb() const
    {
        return _bvec3(this->x, this->z, this->z);
    }

    
    inline _bvec3 _bvec3::_gbb() const
    {
        return _bvec3(this->y, this->z, this->z);
    }

    
    inline _bvec3 _bvec3::_bbb() const
    {
        return _bvec3(this->z, this->z, this->z);
    }

    // 4 components swizzles operators
    
    inline _bvec4 _bvec3::_rrrr() const
    {
        return _bvec4(this->x, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_grrr() const
    {
        return _bvec4(this->y, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_brrr() const
    {
        return _bvec4(this->z, this->x, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_rgrr() const
    {
        return _bvec4(this->x, this->y, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_ggrr() const
    {
        return _bvec4(this->y, this->y, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_bgrr() const
    {
        return _bvec4(this->z, this->y, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_rbrr() const
    {
        return _bvec4(this->x, this->z, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_gbrr() const
    {
        return _bvec4(this->y, this->z, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_bbrr() const
    {
        return _bvec4(this->z, this->z, this->x, this->x);
    }

    
    inline _bvec4 _bvec3::_rrgr() const
    {
        return _bvec4(this->x, this->x, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_grgr() const
    {
        return _bvec4(this->y, this->x, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_brgr() const
    {
        return _bvec4(this->z, this->x, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_rggr() const
    {
        return _bvec4(this->x, this->y, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_gggr() const
    {
        return _bvec4(this->y, this->y, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_bggr() const
    {
        return _bvec4(this->z, this->y, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_rbgr() const
    {
        return _bvec4(this->x, this->z, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_gbgr() const
    {
        return _bvec4(this->y, this->z, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_bbgr() const
    {
        return _bvec4(this->z, this->z, this->y, this->x);
    }

    
    inline _bvec4 _bvec3::_rrbr() const
    {
        return _bvec4(this->x, this->x, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_grbr() const
    {
        return _bvec4(this->y, this->x, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_brbr() const
    {
        return _bvec4(this->z, this->x, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_rgbr() const
    {
        return _bvec4(this->x, this->y, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_ggbr() const
    {
        return _bvec4(this->y, this->y, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_bgbr() const
    {
        return _bvec4(this->z, this->y, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_rbbr() const
    {
        return _bvec4(this->x, this->z, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_gbbr() const
    {
        return _bvec4(this->y, this->z, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_bbbr() const
    {
        return _bvec4(this->z, this->z, this->z, this->x);
    }

    
    inline _bvec4 _bvec3::_rrrg() const
    {
        return _bvec4(this->x, this->x, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_grrg() const
    {
        return _bvec4(this->y, this->x, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_brrg() const
    {
        return _bvec4(this->z, this->x, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_rgrg() const
    {
        return _bvec4(this->x, this->y, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_ggrg() const
    {
        return _bvec4(this->y, this->y, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_bgrg() const
    {
        return _bvec4(this->z, this->y, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_rbrg() const
    {
        return _bvec4(this->x, this->z, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_gbrg() const
    {
        return _bvec4(this->y, this->z, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_bbrg() const
    {
        return _bvec4(this->z, this->z, this->x, this->y);
    }

    
    inline _bvec4 _bvec3::_rrgg() const
    {
        return _bvec4(this->x, this->x, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_grgg() const
    {
        return _bvec4(this->y, this->x, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_brgg() const
    {
        return _bvec4(this->z, this->x, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_rggg() const
    {
        return _bvec4(this->x, this->y, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_gggg() const
    {
        return _bvec4(this->y, this->y, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_bggg() const
    {
        return _bvec4(this->z, this->y, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_rbgg() const
    {
        return _bvec4(this->x, this->z, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_gbgg() const
    {
        return _bvec4(this->y, this->z, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_bbgg() const
    {
        return _bvec4(this->z, this->z, this->y, this->y);
    }

    
    inline _bvec4 _bvec3::_rrbg() const
    {
        return _bvec4(this->x, this->x, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_grbg() const
    {
        return _bvec4(this->y, this->x, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_brbg() const
    {
        return _bvec4(this->z, this->x, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_rgbg() const
    {
        return _bvec4(this->x, this->y, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_ggbg() const
    {
        return _bvec4(this->y, this->y, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_bgbg() const
    {
        return _bvec4(this->z, this->y, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_rbbg() const
    {
        return _bvec4(this->x, this->z, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_gbbg() const
    {
        return _bvec4(this->y, this->z, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_bbbg() const
    {
        return _bvec4(this->z, this->z, this->z, this->y);
    }

    
    inline _bvec4 _bvec3::_rrrb() const
    {
        return _bvec4(this->x, this->x, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_grrb() const
    {
        return _bvec4(this->y, this->x, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_brrb() const
    {
        return _bvec4(this->z, this->x, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_rgrb() const
    {
        return _bvec4(this->x, this->y, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_ggrb() const
    {
        return _bvec4(this->y, this->y, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_bgrb() const
    {
        return _bvec4(this->z, this->y, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_rbrb() const
    {
        return _bvec4(this->x, this->z, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_gbrb() const
    {
        return _bvec4(this->y, this->z, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_bbrb() const
    {
        return _bvec4(this->z, this->z, this->x, this->z);
    }

    
    inline _bvec4 _bvec3::_rrgb() const
    {
        return _bvec4(this->x, this->x, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_grgb() const
    {
        return _bvec4(this->y, this->x, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_brgb() const
    {
        return _bvec4(this->z, this->x, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_rggb() const
    {
        return _bvec4(this->x, this->y, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_gggb() const
    {
        return _bvec4(this->y, this->y, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_bggb() const
    {
        return _bvec4(this->z, this->y, this->y, this->z);
    }

    
    inline _bvec4 _bvec3::_rbgb() const
    {
        return _bvec4(this->x, this->z, this->y, this->z);
    }

    inline _bvec4 _bvec3::_gbgb() const
    {
        return _bvec4(this->y, this->z, this->y, this->z);
    }

    inline _bvec4 _bvec3::_bbgb() const
    {
        return _bvec4(this->z, this->z, this->y, this->z);
    }

    inline _bvec4 _bvec3::_rrbb() const
    {
        return _bvec4(this->x, this->x, this->z, this->z);
    }

    inline _bvec4 _bvec3::_grbb() const
    {
        return _bvec4(this->y, this->x, this->z, this->z);
    }

    inline _bvec4 _bvec3::_brbb() const
    {
        return _bvec4(this->z, this->x, this->z, this->z);
    }

    inline _bvec4 _bvec3::_rgbb() const
    {
        return _bvec4(this->x, this->y, this->z, this->z);
    }

    inline _bvec4 _bvec3::_ggbb() const
    {
        return _bvec4(this->y, this->y, this->z, this->z);
    }

    inline _bvec4 _bvec3::_bgbb() const
    {
        return _bvec4(this->z, this->y, this->z, this->z);
    }

    inline _bvec4 _bvec3::_rbbb() const
    {
        return _bvec4(this->x, this->z, this->z, this->z);
    }

    inline _bvec4 _bvec3::_gbbb() const
    {
        return _bvec4(this->y, this->z, this->z, this->z);
    }

    inline _bvec4 _bvec3::_bbbb() const
    {
        return _bvec4(this->z, this->z, this->z, this->z);
    }

    //////////////////////////////////////////////////////////////
    // 2 components swizzles operators

    inline _bvec2 _bvec3::_ss() const
    {
        return _bvec2(this->x, this->x);
    }

    inline _bvec2 _bvec3::_ts() const
    {
        return _bvec2(this->y, this->x);
    }

    inline _bvec2 _bvec3::_ps() const
    {
        return _bvec2(this->z, this->x);
    }

    inline _bvec2 _bvec3::_st() const
    {
        return _bvec2(this->x, this->y);
    }

    inline _bvec2 _bvec3::_tt() const
    {
        return _bvec2(this->y, this->y);
    }

    inline _bvec2 _bvec3::_pt() const
    {
        return _bvec2(this->z, this->y);
    }

    inline _bvec2 _bvec3::_sp() const
    {
        return _bvec2(this->x, this->z);
    }

    inline _bvec2 _bvec3::_tp() const
    {
        return _bvec2(this->y, this->z);
    }

    inline _bvec2 _bvec3::_pp() const
    {
        return _bvec2(this->z, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Left hand side 3 components common swizzle operator

    inline _bref3 _bvec3::_pts()
    {
        return _bref3(this->z, this->y, this->x);
    }

    inline _bref3 _bvec3::_tps()
    {
        return _bref3(this->y, this->z, this->x);
    }

    inline _bref3 _bvec3::_pst()
    {
        return _bref3(this->z, this->x, this->y);
    }

    inline _bref3 _bvec3::_spt()
    {
        return _bref3(this->x, this->z, this->y);
    }

    inline _bref3 _bvec3::_tsp()
    {
        return _bref3(this->y, this->x, this->z);
    }

    inline _bref3 _bvec3::_stp()
    {
        return _bref3(this->x, this->y, this->z);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components common swizzle operators

    inline _bvec3 _bvec3::_sss() const
    {
        return _bvec3(this->x, this->x, this->x);
    }

    inline _bvec3 _bvec3::_tss() const
    {
        return _bvec3(this->y, this->x, this->x);
    }

    inline _bvec3 _bvec3::_pss() const
    {
        return _bvec3(this->z, this->x, this->x);
    }

    inline _bvec3 _bvec3::_sts() const
    {
        return _bvec3(this->x, this->y, this->x);
    }

    inline _bvec3 _bvec3::_tts() const
    {
        return _bvec3(this->y, this->y, this->x);
    }

    inline _bvec3 _bvec3::_pts() const
    {
        return _bvec3(this->z, this->y, this->x);
    }

    inline _bvec3 _bvec3::_sps() const
    {
        return _bvec3(this->x, this->z, this->x);
    }

    inline _bvec3 _bvec3::_tps() const
    {
        return _bvec3(this->y, this->z, this->x);
    }

    inline _bvec3 _bvec3::_pps() const
    {
        return _bvec3(this->z, this->z, this->x);
    }

    inline _bvec3 _bvec3::_sst() const
    {
        return _bvec3(this->x, this->x, this->y);
    }

    inline _bvec3 _bvec3::_tst() const
    {
        return _bvec3(this->y, this->x, this->y);
    }

    inline _bvec3 _bvec3::_pst() const
    {
        return _bvec3(this->z, this->x, this->y);
    }

    inline _bvec3 _bvec3::_stt() const
    {
        return _bvec3(this->x, this->y, this->y);
    }

    inline _bvec3 _bvec3::_ttt() const
    {
        return _bvec3(this->y, this->y, this->y);
    }

    inline _bvec3 _bvec3::_ptt() const
    {
        return _bvec3(this->z, this->y, this->y);
    }

    inline _bvec3 _bvec3::_spt() const
    {
        return _bvec3(this->x, this->z, this->y);
    }

    inline _bvec3 _bvec3::_tpt() const
    {
        return _bvec3(this->y, this->z, this->y);
    }

    inline _bvec3 _bvec3::_ppt() const
    {
        return _bvec3(this->z, this->z, this->y);
    }

    inline _bvec3 _bvec3::_ssp() const
    {
        return _bvec3(this->x, this->x, this->z);
    }

    inline _bvec3 _bvec3::_tsp() const
    {
        return _bvec3(this->y, this->x, this->z);
    }

    inline _bvec3 _bvec3::_psp() const
    {
        return _bvec3(this->z, this->x, this->z);
    }

    inline _bvec3 _bvec3::_stp() const
    {
        return _bvec3(this->x, this->y, this->z);
    }

    inline _bvec3 _bvec3::_ttp() const
    {
        return _bvec3(this->y, this->y, this->z);
    }

    inline _bvec3 _bvec3::_ptp() const
    {
        return _bvec3(this->z, this->y, this->z);
    }

    inline _bvec3 _bvec3::_spp() const
    {
        return _bvec3(this->x, this->z, this->z);
    }

    inline _bvec3 _bvec3::_tpp() const
    {
        return _bvec3(this->y, this->z, this->z);
    }

    inline _bvec3 _bvec3::_ppp() const
    {
        return _bvec3(this->z, this->z, this->z);
    }

    // 4 components swizzles operators
    inline _bvec4 _bvec3::_ssss() const
    {
        return _bvec4(this->x, this->x, this->x, this->x);
    }

    inline _bvec4 _bvec3::_tsss() const
    {
        return _bvec4(this->y, this->x, this->x, this->x);
    }

    inline _bvec4 _bvec3::_psss() const
    {
        return _bvec4(this->z, this->x, this->x, this->x);
    }

    inline _bvec4 _bvec3::_stss() const
    {
        return _bvec4(this->x, this->y, this->x, this->x);
    }

    inline _bvec4 _bvec3::_ttss() const
    {
        return _bvec4(this->y, this->y, this->x, this->x);
    }

    inline _bvec4 _bvec3::_ptss() const
    {
        return _bvec4(this->z, this->y, this->x, this->x);
    }

    inline _bvec4 _bvec3::_spss() const
    {
        return _bvec4(this->x, this->z, this->x, this->x);
    }

    inline _bvec4 _bvec3::_tpss() const
    {
        return _bvec4(this->y, this->z, this->x, this->x);
    }

    inline _bvec4 _bvec3::_ppss() const
    {
        return _bvec4(this->z, this->z, this->x, this->x);
    }

    inline _bvec4 _bvec3::_ssts() const
    {
        return _bvec4(this->x, this->x, this->y, this->x);
    }

    inline _bvec4 _bvec3::_tsts() const
    {
        return _bvec4(this->y, this->x, this->y, this->x);
    }

    inline _bvec4 _bvec3::_psts() const
    {
        return _bvec4(this->z, this->x, this->y, this->x);
    }

    inline _bvec4 _bvec3::_stts() const
    {
        return _bvec4(this->x, this->y, this->y, this->x);
    }

    inline _bvec4 _bvec3::_ttts() const
    {
        return _bvec4(this->y, this->y, this->y, this->x);
    }

    inline _bvec4 _bvec3::_ptts() const
    {
        return _bvec4(this->z, this->y, this->y, this->x);
    }

    inline _bvec4 _bvec3::_spts() const
    {
        return _bvec4(this->x, this->z, this->y, this->x);
    }

    inline _bvec4 _bvec3::_tpts() const
    {
        return _bvec4(this->y, this->z, this->y, this->x);
    }

    inline _bvec4 _bvec3::_ppts() const
    {
        return _bvec4(this->z, this->z, this->y, this->x);
    }

    inline _bvec4 _bvec3::_ssps() const
    {
        return _bvec4(this->x, this->x, this->z, this->x);
    }

    inline _bvec4 _bvec3::_tsps() const
    {
        return _bvec4(this->y, this->x, this->z, this->x);
    }

    inline _bvec4 _bvec3::_psps() const
    {
        return _bvec4(this->z, this->x, this->z, this->x);
    }

    inline _bvec4 _bvec3::_stps() const
    {
        return _bvec4(this->x, this->y, this->z, this->x);
    }

    inline _bvec4 _bvec3::_ttps() const
    {
        return _bvec4(this->y, this->y, this->z, this->x);
    }

    inline _bvec4 _bvec3::_ptps() const
    {
        return _bvec4(this->z, this->y, this->z, this->x);
    }

    inline _bvec4 _bvec3::_spps() const
    {
        return _bvec4(this->x, this->z, this->z, this->x);
    }

    inline _bvec4 _bvec3::_tpps() const
    {
        return _bvec4(this->y, this->z, this->z, this->x);
    }

    inline _bvec4 _bvec3::_ppps() const
    {
        return _bvec4(this->z, this->z, this->z, this->x);
    }

    inline _bvec4 _bvec3::_ssst() const
    {
        return _bvec4(this->x, this->x, this->x, this->y);
    }

    inline _bvec4 _bvec3::_tsst() const
    {
        return _bvec4(this->y, this->x, this->x, this->y);
    }

    inline _bvec4 _bvec3::_psst() const
    {
        return _bvec4(this->z, this->x, this->x, this->y);
    }

    inline _bvec4 _bvec3::_stst() const
    {
        return _bvec4(this->x, this->y, this->x, this->y);
    }

    inline _bvec4 _bvec3::_ttst() const
    {
        return _bvec4(this->y, this->y, this->x, this->y);
    }

    inline _bvec4 _bvec3::_ptst() const
    {
        return _bvec4(this->z, this->y, this->x, this->y);
    }

    inline _bvec4 _bvec3::_spst() const
    {
        return _bvec4(this->x, this->z, this->x, this->y);
    }

    inline _bvec4 _bvec3::_tpst() const
    {
        return _bvec4(this->y, this->z, this->x, this->y);
    }

    inline _bvec4 _bvec3::_ppst() const
    {
        return _bvec4(this->z, this->z, this->x, this->y);
    }

    inline _bvec4 _bvec3::_sstt() const
    {
        return _bvec4(this->x, this->x, this->y, this->y);
    }

    inline _bvec4 _bvec3::_tstt() const
    {
        return _bvec4(this->y, this->x, this->y, this->y);
    }

    inline _bvec4 _bvec3::_pstt() const
    {
        return _bvec4(this->z, this->x, this->y, this->y);
    }

    inline _bvec4 _bvec3::_sttt() const
    {
        return _bvec4(this->x, this->y, this->y, this->y);
    }

    inline _bvec4 _bvec3::_tttt() const
    {
        return _bvec4(this->y, this->y, this->y, this->y);
    }

    inline _bvec4 _bvec3::_pttt() const
    {
        return _bvec4(this->z, this->y, this->y, this->y);
    }

    inline _bvec4 _bvec3::_sptt() const
    {
        return _bvec4(this->x, this->z, this->y, this->y);
    }

    inline _bvec4 _bvec3::_tptt() const
    {
        return _bvec4(this->y, this->z, this->y, this->y);
    }

    inline _bvec4 _bvec3::_pptt() const
    {
        return _bvec4(this->z, this->z, this->y, this->y);
    }

    inline _bvec4 _bvec3::_sspt() const
    {
        return _bvec4(this->x, this->x, this->z, this->y);
    }

    inline _bvec4 _bvec3::_tspt() const
    {
        return _bvec4(this->y, this->x, this->z, this->y);
    }

    inline _bvec4 _bvec3::_pspt() const
    {
        return _bvec4(this->z, this->x, this->z, this->y);
    }

    inline _bvec4 _bvec3::_stpt() const
    {
        return _bvec4(this->x, this->y, this->z, this->y);
    }

    inline _bvec4 _bvec3::_ttpt() const
    {
        return _bvec4(this->y, this->y, this->z, this->y);
    }

    inline _bvec4 _bvec3::_ptpt() const
    {
        return _bvec4(this->z, this->y, this->z, this->y);
    }

    inline _bvec4 _bvec3::_sppt() const
    {
        return _bvec4(this->x, this->z, this->z, this->y);
    }

    inline _bvec4 _bvec3::_tppt() const
    {
        return _bvec4(this->y, this->z, this->z, this->y);
    }

    inline _bvec4 _bvec3::_pppt() const
    {
        return _bvec4(this->z, this->z, this->z, this->y);
    }

    inline _bvec4 _bvec3::_sssp() const
    {
        return _bvec4(this->x, this->x, this->x, this->z);
    }

    inline _bvec4 _bvec3::_tssp() const
    {
        return _bvec4(this->y, this->x, this->x, this->z);
    }

    inline _bvec4 _bvec3::_pssp() const
    {
        return _bvec4(this->z, this->x, this->x, this->z);
    }

    inline _bvec4 _bvec3::_stsp() const
    {
        return _bvec4(this->x, this->y, this->x, this->z);
    }

    inline _bvec4 _bvec3::_ttsp() const
    {
        return _bvec4(this->y, this->y, this->x, this->z);
    }

    inline _bvec4 _bvec3::_ptsp() const
    {
        return _bvec4(this->z, this->y, this->x, this->z);
    }

    inline _bvec4 _bvec3::_spsp() const
    {
        return _bvec4(this->x, this->z, this->x, this->z);
    }

    inline _bvec4 _bvec3::_tpsp() const
    {
        return _bvec4(this->y, this->z, this->x, this->z);
    }

    inline _bvec4 _bvec3::_ppsp() const
    {
        return _bvec4(this->z, this->z, this->x, this->z);
    }

    inline _bvec4 _bvec3::_sstp() const
    {
        return _bvec4(this->x, this->x, this->y, this->z);
    }

    inline _bvec4 _bvec3::_tstp() const
    {
        return _bvec4(this->y, this->x, this->y, this->z);
    }

    inline _bvec4 _bvec3::_pstp() const
    {
        return _bvec4(this->z, this->x, this->y, this->z);
    }

    inline _bvec4 _bvec3::_sttp() const
    {
        return _bvec4(this->x, this->y, this->y, this->z);
    }

    inline _bvec4 _bvec3::_tttp() const
    {
        return _bvec4(this->y, this->y, this->y, this->z);
    }

    inline _bvec4 _bvec3::_pttp() const
    {
        return _bvec4(this->z, this->y, this->y, this->z);
    }

    inline _bvec4 _bvec3::_sptp() const
    {
        return _bvec4(this->x, this->z, this->y, this->z);
    }

    inline _bvec4 _bvec3::_tptp() const
    {
        return _bvec4(this->y, this->z, this->y, this->z);
    }

    inline _bvec4 _bvec3::_pptp() const
    {
        return _bvec4(this->z, this->z, this->y, this->z);
    }

    inline _bvec4 _bvec3::_sspp() const
    {
        return _bvec4(this->x, this->x, this->z, this->z);
    }

    inline _bvec4 _bvec3::_tspp() const
    {
        return _bvec4(this->y, this->x, this->z, this->z);
    }

    inline _bvec4 _bvec3::_pspp() const
    {
        return _bvec4(this->z, this->x, this->z, this->z);
    }

    inline _bvec4 _bvec3::_stpp() const
    {
        return _bvec4(this->x, this->y, this->z, this->z);
    }

    inline _bvec4 _bvec3::_ttpp() const
    {
        return _bvec4(this->y, this->y, this->z, this->z);
    }

    inline _bvec4 _bvec3::_ptpp() const
    {
        return _bvec4(this->z, this->y, this->z, this->z);
    }

    inline _bvec4 _bvec3::_sppp() const
    {
        return _bvec4(this->x, this->z, this->z, this->z);
    }

    inline _bvec4 _bvec3::_tppp() const
    {
        return _bvec4(this->y, this->z, this->z, this->z);
    }

    inline _bvec4 _bvec3::_pppp() const
    {
        return _bvec4(this->z, this->z, this->z, this->z);
    }
*/
} //namespace glm

#endif//__bvec3_inl__
