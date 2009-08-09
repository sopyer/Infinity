//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-27
// Updated : 2006-04-27
// Licence : This source is under GNU LGPL licence
// File    : _bvec2.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __bvec2_inl__
#define __bvec2_inl__

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

    inline _bvec2::_bvec2() :
        x(false),
        y(false)
    {}

    inline _bvec2::_bvec2(const _bvec2& v) :
        x(v.x),
        y(v.y)
    {}

    //////////////////////////////////////////////////////////////
    // Swizzle constructors
    inline _bvec2::_bvec2(const _bref2& r) :
        x(r.x),
        y(r.y)
    {}

    //////////////////////////////////////////////////////////////
    // Bool constructors

    inline _bvec2::_bvec2(const bool a) :
        x(a),
        y(a)
    {}

    inline _bvec2::_bvec2(const bool a, const bool b) :
        x(a),
        y(b)
    {}

    inline _bvec2::_bvec2(const bool a, const _bvec2& b) :
        x(a),
        y(b.x)
    {}

    inline _bvec2::_bvec2(const bool a, const _bvec3& b) :
        x(a),
        y(b.x)
    {}

    inline _bvec2::_bvec2(const bool a, const _bvec4& b) :
        x(a),
        y(b.x)
    {}

    inline _bvec2::_bvec2(const _bvec3& a) :
        x(a.x),
        y(a.y)
    {}

    inline _bvec2::_bvec2(const _bvec4& a) :
        x(a.x),
        y(a.y)
    {}

    //////////////////////////////////////////////////////////////
    // U constructors

    template <typename U> 
    inline _bvec2::_bvec2(const U a) :
        x(bool(a)),
        y(bool(a))
    {}

    template <typename U> 
    inline _bvec2::_bvec2(const U a, const U b) :
        x(bool(a)),
        y(bool(b))
    {}

    template <typename U> 
    inline _bvec2::_bvec2(const U a, const _xvec2<U>& b) :
        x(bool(a)),
        y(bool(b.x))
    {}

    template <typename U> 
    inline _bvec2::_bvec2(const U a, const _xvec3<U>& b) :
        x(bool(a)),
        y(bool(b.x))
    {}

    template <typename U> 
    inline _bvec2::_bvec2(const U a, const _xvec4<U>& b) :
        x(bool(a)),
        y(bool(b.x))
    {}

    template <typename U> 
    inline _bvec2::_bvec2(const _xvec3<U>& a) :
        x(bool(a.x)),
        y(bool(a.y))
    {}

    template <typename U> 
    inline _bvec2::_bvec2(const _xvec4<U>& a) :
        x(bool(a.x)),
        y(bool(a.y))
    {}

    //////////////////////////////////////////////////////////////
    // bvec2 accesses

    inline bool& _bvec2::operator[](int i)
    {
        return (&x)[i];
    }

    inline bool _bvec2::operator[](int i) const
    {
        return (&x)[i];
    }

    inline _bvec2::operator bool*()
    {
        return &x;
    }

    inline _bvec2::operator const bool*() const 
    {
        return &x;
    }

    //////////////////////////////////////////////////////////////
    // bvec2 operators

    // This function shouldn't required but it seems that VC7.1 have an optimisation bug if this operator wasn't declared
    inline _bvec2& _bvec2::operator=(const _bvec2& x)
    {
        this->x = x.x;
        this->y = x.y;
        return *this;
    }
/*
    inline _bvec2 _bvec2::operator--()
    {
	    x = !x;
	    y = !y;
	    return *this;
    }

    inline _bvec2 _bvec2::operator++()
    {
	    x = !x;
	    y = !y;
	    return *this;
    }

    inline const _bvec2 _bvec2::operator--(int n) const 
    {
        return _bvec2(
			!this->x, 
			!this->y);
    }

    inline const _bvec2 _bvec2::operator++(int n) const
    {
	    return _bvec2(
			!this->x, 
			!this->y);
    }
*/
    inline _bvec2 _bvec2::operator!() const 
    {
	    return _bvec2(
			!this->x, 
			!this->y);
    }
    //////////////////////////////////////////////////////////////
    // Left hand side 2 components common swizzle operator

    inline _bref2 _bvec2::_xy()
    {
        return _bref2(this->x, this->y);
    }

    inline _bref2 _bvec2::_yx()
    {
        return _bref2(this->y, this->x);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 2 components common swizzle operators

    inline _bvec2 _bvec2::_xx() const
    {
        return _bvec2(this->x, this->x);
    }

    inline _bvec2 _bvec2::_yx() const
    {
        return _bvec2(this->y, this->x);
    }

    inline _bvec2 _bvec2::_xy() const
    {
        return _bvec2(this->x, this->y);
    }

    inline _bvec2 _bvec2::_yy() const
    {
        return _bvec2(this->y, this->y);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components common swizzle operators

    inline _bvec3 _bvec2::_xxx() const
    {
        return _bvec3(this->x, this->x, this->x);
    }

    inline _bvec3 _bvec2::_yxx() const
    {
        return _bvec3(this->y, this->x, this->x);
    }

    inline _bvec3 _bvec2::_xyx() const
    {
        return _bvec3(this->x, this->y, this->x);
    }

    inline _bvec3 _bvec2::_yyx() const
    {
        return _bvec3(this->y, this->y, this->x);
    }

    inline _bvec3 _bvec2::_xxy() const
    {
        return _bvec3(this->x, this->x, this->y);
    }

    inline _bvec3 _bvec2::_yxy() const
    {
        return _bvec3(this->y, this->x, this->y);
    }

    inline _bvec3 _bvec2::_xyy() const
    {
        return _bvec3(this->x, this->y, this->y);
    }

    inline _bvec3 _bvec2::_yyy() const
    {
        return _bvec3(this->y, this->y, this->y);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 4 components common swizzle operators

    inline _bvec4 _bvec2::_xxxx() const
    {
        return _bvec4(this->x, this->x, this->x, this->x);
    }

    inline _bvec4 _bvec2::_yxxx() const
    {
        return _bvec4(this->y, this->x, this->x, this->x);
    }

    inline _bvec4 _bvec2::_xyxx() const
    {
        return _bvec4(this->x, this->y, this->x, this->x);
    }

    inline _bvec4 _bvec2::_yyxx() const
    {
        return _bvec4(this->y, this->y, this->x, this->x);
    }

    inline _bvec4 _bvec2::_xxyx() const
    {
        return _bvec4(this->x, this->x, this->y, this->x);
    }

    inline _bvec4 _bvec2::_yxyx() const
    {
        return _bvec4(this->y, this->x, this->y, this->x);
    }

    inline _bvec4 _bvec2::_xyyx() const
    {
        return _bvec4(this->x, this->y, this->y, this->x);
    }

    inline _bvec4 _bvec2::_yyyx() const
    {
        return _bvec4(this->y, this->y, this->y, this->x);
    }

    inline _bvec4 _bvec2::_xxxy() const
    {
        return _bvec4(this->x, this->x, this->x, this->y);
    }

    inline _bvec4 _bvec2::_yxxy() const
    {
        return _bvec4(this->y, this->x, this->x, this->y);
    }

    inline _bvec4 _bvec2::_xyxy() const
    {
        return _bvec4(this->x, this->y, this->x, this->y);
    }

    inline _bvec4 _bvec2::_yyxy() const
    {
        return _bvec4(this->y, this->y, this->x, this->y);
    }

    inline _bvec4 _bvec2::_xxyy() const
    {
        return _bvec4(this->x, this->x, this->y, this->y);
    }

    inline _bvec4 _bvec2::_yxyy() const
    {
        return _bvec4(this->y, this->x, this->y, this->y);
    }

    inline _bvec4 _bvec2::_xyyy() const
    {
        return _bvec4(this->x, this->y, this->y, this->y);
    }

    inline _bvec4 _bvec2::_yyyy() const
    {
        return _bvec4(this->y, this->y, this->y, this->y);
    }
/*
    //////////////////////////////////////////////////////////////
    // Left hand side 2 components color swizzle operator

    inline _bref2 _bvec2::_rg()
    {
        return _bref2(this->r, this->g);
    }

    inline _bref2 _bvec2::_gr()
    {
        return _bref2(this->g, this->r);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 2 components color swizzle operators

    inline _bvec2 _bvec2::_rr() const
    {
        return _bvec2(this->r, this->r);
    }

    inline _bvec2 _bvec2::_gr() const
    {
        return _bvec2(this->g, this->r);
    }

    inline _bvec2 _bvec2::_rg() const
    {
        return _bvec2(this->r, this->g);
    }

    inline _bvec2 _bvec2::_gg() const
    {
        return _bvec2(this->g, this->g);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components color swizzle operators

    inline _bvec3 _bvec2::_rrr() const
    {
        return _bvec3(this->r, this->r, this->r);
    }

    inline _bvec3 _bvec2::_grr() const
    {
        return _bvec3(this->g, this->r, this->r);
    }

    inline _bvec3 _bvec2::_rgr() const
    {
        return _bvec3(this->r, this->g, this->r);
    }

    inline _bvec3 _bvec2::_ggr() const
    {
        return _bvec3(this->g, this->g, this->r);
    }

    inline _bvec3 _bvec2::_rrg() const
    {
        return _bvec3(this->r, this->r, this->g);
    }

    inline _bvec3 _bvec2::_grg() const
    {
        return _bvec3(this->g, this->r, this->g);
    }

    inline _bvec3 _bvec2::_rgg() const
    {
        return _bvec3(this->r, this->g, this->g);
    }

    inline _bvec3 _bvec2::_ggg() const
    {
        return _bvec3(this->g, this->g, this->g);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 4 components color swizzle operators

    inline _bvec4 _bvec2::_rrrr() const
    {
        return _bvec4(this->r, this->r, this->r, this->r);
    }

    inline _bvec4 _bvec2::_grrr() const
    {
        return _bvec4(this->g, this->r, this->r, this->r);
    }

    inline _bvec4 _bvec2::_rgrr() const
    {
        return _bvec4(this->r, this->g, this->r, this->r);
    }

    inline _bvec4 _bvec2::_ggrr() const
    {
        return _bvec4(this->g, this->g, this->r, this->r);
    }

    inline _bvec4 _bvec2::_rrgr() const
    {
        return _bvec4(this->r, this->r, this->g, this->r);
    }

    inline _bvec4 _bvec2::_grgr() const
    {
        return _bvec4(this->g, this->r, this->g, this->r);
    }

    inline _bvec4 _bvec2::_rggr() const
    {
        return _bvec4(this->r, this->g, this->g, this->r);
    }

    inline _bvec4 _bvec2::_gggr() const
    {
        return _bvec4(this->g, this->g, this->g, this->r);
    }

    inline _bvec4 _bvec2::_rrrg() const
    {
        return _bvec4(this->r, this->r, this->r, this->g);
    }

    inline _bvec4 _bvec2::_grrg() const
    {
        return _bvec4(this->g, this->r, this->r, this->g);
    }

    inline _bvec4 _bvec2::_rgrg() const
    {
        return _bvec4(this->r, this->g, this->r, this->g);
    }

    inline _bvec4 _bvec2::_ggrg() const
    {
        return _bvec4(this->g, this->g, this->r, this->g);
    }

    inline _bvec4 _bvec2::_rrgg() const
    {
        return _bvec4(this->r, this->r, this->g, this->g);
    }

    inline _bvec4 _bvec2::_grgg() const
    {
        return _bvec4(this->g, this->r, this->g, this->g);
    }

    inline _bvec4 _bvec2::_rggg() const
    {
        return _bvec4(this->r, this->g, this->g, this->g);
    }

    inline _bvec4 _bvec2::_gggg() const
    {
        return _bvec4(this->g, this->g, this->g, this->g);
    }

    //////////////////////////////////////////////////////////////
    // Left hand side 2 components texcoord swizzle operator

    inline _bref2 _bvec2::_st()
    {
        return _bref2(this->s, this->t);
    }

    inline _bref2 _bvec2::_ts()
    {
        return _bref2(this->t, this->s);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 2 components texcoord swizzle operators

    inline _bvec2 _bvec2::_ss() const
    {
        return _bvec2(this->s, this->s);
    }

    inline _bvec2 _bvec2::_ts() const
    {
        return _bvec2(this->t, this->s);
    }

    inline _bvec2 _bvec2::_st() const
    {
        return _bvec2(this->s, this->t);
    }

    inline _bvec2 _bvec2::_tt() const
    {
        return _bvec2(this->t, this->t);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 3 components texcoord swizzle operators

    inline _bvec3 _bvec2::_sss() const
    {
        return _bvec3(this->s, this->s, this->s);
    }

    inline _bvec3 _bvec2::_tss() const
    {
        return _bvec3(this->t, this->s, this->s);
    }

    inline _bvec3 _bvec2::_sts() const
    {
        return _bvec3(this->s, this->t, this->s);
    }

    inline _bvec3 _bvec2::_tts() const
    {
        return _bvec3(this->t, this->t, this->s);
    }

    inline _bvec3 _bvec2::_sst() const
    {
        return _bvec3(this->s, this->s, this->t);
    }

    inline _bvec3 _bvec2::_tst() const
    {
        return _bvec3(this->t, this->s, this->t);
    }

    inline _bvec3 _bvec2::_stt() const
    {
        return _bvec3(this->s, this->t, this->t);
    }

    inline _bvec3 _bvec2::_ttt() const
    {
        return _bvec3(this->t, this->t, this->t);
    }

    //////////////////////////////////////////////////////////////
    // Right hand side 4 components texcoord swizzle operators

    inline _bvec4 _bvec2::_ssss() const
    {
        return _bvec4(this->s, this->s, this->s, this->s);
    }

    inline _bvec4 _bvec2::_tsss() const
    {
        return _bvec4(this->t, this->s, this->s, this->s);
    }

    inline _bvec4 _bvec2::_stss() const
    {
        return _bvec4(this->s, this->t, this->s, this->s);
    }

    inline _bvec4 _bvec2::_ttss() const
    {
        return _bvec4(this->t, this->t, this->s, this->s);
    }

    inline _bvec4 _bvec2::_ssts() const
    {
        return _bvec4(this->s, this->s, this->t, this->s);
    }

    inline _bvec4 _bvec2::_tsts() const
    {
        return _bvec4(this->t, this->s, this->t, this->s);
    }

    inline _bvec4 _bvec2::_stts() const
    {
        return _bvec4(this->s, this->t, this->t, this->s);
    }

    inline _bvec4 _bvec2::_ttts() const
    {
        return _bvec4(this->t, this->t, this->t, this->s);
    }

    inline _bvec4 _bvec2::_ssst() const
    {
        return _bvec4(this->s, this->s, this->s, this->t);
    }

    inline _bvec4 _bvec2::_tsst() const
    {
        return _bvec4(this->t, this->s, this->s, this->t);
    }

    inline _bvec4 _bvec2::_stst() const
    {
        return _bvec4(this->s, this->t, this->s, this->t);
    }

    inline _bvec4 _bvec2::_ttst() const
    {
        return _bvec4(this->t, this->t, this->s, this->t);
    }

    inline _bvec4 _bvec2::_sstt() const
    {
        return _bvec4(this->s, this->s, this->t, this->t);
    }

    inline _bvec4 _bvec2::_tstt() const
    {
        return _bvec4(this->t, this->s, this->t, this->t);
    }

    inline _bvec4 _bvec2::_sttt() const
    {
        return _bvec4(this->s, this->t, this->t, this->t);
    }

    inline _bvec4 _bvec2::_tttt() const
    {
        return _bvec4(this->t, this->t, this->t, this->t);
    }
*/
} //namespace glm

#endif //__bvec2_inl__
