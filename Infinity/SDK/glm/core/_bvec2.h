//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-27
// Updated : 2006-04-27
// Licence : This source is under GNU LGPL licence
// File    : _bvec2.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __bvec2_h__
#define __bvec2_h__

namespace glm
{
    template <typename T> class _xvec2;
    template <typename T> class _xvec3;
    template <typename T> class _xvec4;
	class _bvec3;
    class _bvec4;
    struct _bref2;

    //!< \brief Class for Vector of 2 booleans. (From GLSL 1.10.59 specification)
    class _bvec2
    {
    public:
        int _size () const;

    public:
        // Data
        union{bool x, r, s;};
        union{bool y, g, t;};

        // Common constructors
	    _bvec2();
        _bvec2(const _bvec2& v);

        // Swizzle constructors
        _bvec2(const _bref2& r);

        // Bool constructors
        explicit _bvec2(const bool a);
        explicit _bvec2(const bool a, const bool b);
        explicit _bvec2(const bool a, const _bvec2& b);
        explicit _bvec2(const bool a, const _bvec3& b);
        explicit _bvec2(const bool a, const _bvec4& b);
        explicit _bvec2(const _bvec3& a);
        explicit _bvec2(const _bvec4& a);

        // U constructors
        template <typename U> explicit _bvec2(const U x);
        template <typename U> explicit _bvec2(const U a, const U b);
        template <typename U> explicit _bvec2(const U a, const _xvec2<U>& b);
        template <typename U> explicit _bvec2(const U a, const _xvec3<U>& b);
        template <typename U> explicit _bvec2(const U a, const _xvec4<U>& b);
        template <typename U> explicit _bvec2(const _xvec3<U>& a);
        template <typename U> explicit _bvec2(const _xvec4<U>& a);

        // Accesses
        bool& operator[](int i);
        bool operator[](int i) const;
        operator bool*();
	    operator const bool*() const;

        // Operators
        _bvec2& operator=(const _bvec2& v);
/*
	    _bvec2 operator-- ();
	    const _bvec2 operator-- (int n) const;
        _bvec2 operator++ ();
	    const _bvec2 operator++ (int n) const;
*/
	    _bvec2 operator! () const;

        // Left hand side 2 components common swizzle operators
        _bref2 _xy();
        _bref2 _yx();

        // Right hand side 2 components common swizzle operators
        _bvec2 _xx() const;
        _bvec2 _yx() const;
        _bvec2 _xy() const;
        _bvec2 _yy() const;

        // Right hand side 3 components common swizzle operators
        _bvec3 _xxx() const;
        _bvec3 _yxx() const;
        _bvec3 _xyx() const;
        _bvec3 _yyx() const;
        _bvec3 _xxy() const;
        _bvec3 _yxy() const;
        _bvec3 _xyy() const;
        _bvec3 _yyy() const;

        // Right hand side 4 components common swizzle operators
        _bvec4 _xxxx() const;
        _bvec4 _yxxx() const;
        _bvec4 _xyxx() const;
        _bvec4 _yyxx() const;
        _bvec4 _xxyx() const;
        _bvec4 _yxyx() const;
        _bvec4 _xyyx() const;
        _bvec4 _yyyx() const;
        _bvec4 _xxxy() const;
        _bvec4 _yxxy() const;
        _bvec4 _xyxy() const;
        _bvec4 _yyxy() const;
        _bvec4 _xxyy() const;
        _bvec4 _yxyy() const;
        _bvec4 _xyyy() const;
        _bvec4 _yyyy() const;
/*
        // Left hand side 2 components color swizzle operators
        _bref2 _rg();
        _bref2 _gr();

        // Right hand side 2 components color swizzle operators
        _bvec2 _rr() const;
        _bvec2 _gr() const;
        _bvec2 _rg() const;
        _bvec2 _gg() const;

        // Right hand side 3 components color swizzle operators
        _bvec3 _rrr() const;
        _bvec3 _grr() const;
        _bvec3 _rgr() const;
        _bvec3 _ggr() const;
        _bvec3 _rrg() const;
        _bvec3 _grg() const;
        _bvec3 _rgg() const;
        _bvec3 _ggg() const;

        // Right hand side 4 components color swizzle operators
        _bvec4 _rrrr() const;
        _bvec4 _grrr() const;
        _bvec4 _rgrr() const;
        _bvec4 _ggrr() const;
        _bvec4 _rrgr() const;
        _bvec4 _grgr() const;
        _bvec4 _rggr() const;
        _bvec4 _gggr() const;
        _bvec4 _rrrg() const;
        _bvec4 _grrg() const;
        _bvec4 _rgrg() const;
        _bvec4 _ggrg() const;
        _bvec4 _rrgg() const;
        _bvec4 _grgg() const;
        _bvec4 _rggg() const;
        _bvec4 _gggg() const;

        // Left hand side 2 components texcoord swizzle operators
        _bref2 _st();
        _bref2 _ts();

        // Right hand side 2 components texcoord swizzle operators
        _bvec2 _ss() const;
        _bvec2 _ts() const;
        _bvec2 _st() const;
        _bvec2 _tt() const;

        // Right hand side 3 components texcoord swizzle operators
        _bvec3 _sss() const;
        _bvec3 _tss() const;
        _bvec3 _sts() const;
        _bvec3 _tts() const;
        _bvec3 _sst() const;
        _bvec3 _tst() const;
        _bvec3 _stt() const;
        _bvec3 _ttt() const;

        // Right hand side 4 components texcoord swizzle operators
        _bvec4 _ssss() const;
        _bvec4 _tsss() const;
        _bvec4 _stss() const;
        _bvec4 _ttss() const;
        _bvec4 _ssts() const;
        _bvec4 _tsts() const;
        _bvec4 _stts() const;
        _bvec4 _ttts() const;
        _bvec4 _ssst() const;
        _bvec4 _tsst() const;
        _bvec4 _stst() const;
        _bvec4 _ttst() const;
        _bvec4 _sstt() const;
        _bvec4 _tstt() const;
        _bvec4 _sttt() const;
        _bvec4 _tttt() const;
*/
    };
} //namespace glm

#endif //__bvec2_h__
