//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-27
// Updated : 2006-05-28
// Licence : This source is under GNU LGPL licence
// File    : _xvec2.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __xvec2_h__
#define __xvec2_h__

namespace glm
{
    template <typename T> class _xvec3;
    template <typename T> class _xvec4;
    class _bvec2;
    class _bvec3;
    class _bvec4;
    template <typename T> struct _xref2;

	//!< \brief Template for Vector of 2 floating-point numbers or 2 integers . (From GLSL 1.10.59 specification)
    template <typename T> 
    class _xvec2
    {
    public:
        // Data
        /* ISO C++ version unavailable with VC7.1 ...
            union{T x, r, s;};
            union{T y, g, t;};
        */
        // Solution away from ISO C++ but available with VC7.1 and GCC without -pedantic
        union 
        {
            struct{T x, y;};
            struct{T r, g;};
            struct{T s, t;};
        };

        // Common constructors
	    _xvec2();
        _xvec2(const _xvec2<T>& v);

        // Swizzle constructors
        _xvec2(const _xref2<T>& r);

        // T constructors
        explicit _xvec2(const T x);
        explicit _xvec2(const T a, const T b);
        explicit _xvec2(const T a, const _xvec2<T>& b);
        explicit _xvec2(const T a, const _xvec3<T>& b);
        explicit _xvec2(const T a, const _xvec4<T>& b);
        explicit _xvec2(const _xvec3<T>& a);
        explicit _xvec2(const _xvec4<T>& a);

        // U constructors
        template <typename U> explicit _xvec2(const U x);
        template <typename U> explicit _xvec2(const U a, const U b);
        template <typename U> explicit _xvec2(const U a, const _xvec2<U>& b);
        template <typename U> explicit _xvec2(const U a, const _xvec3<U>& b);
        template <typename U> explicit _xvec2(const U a, const _xvec4<U>& b);
        template <typename U> explicit _xvec2(const _xvec2<U>& a);
        template <typename U> explicit _xvec2(const _xvec3<U>& a);
        template <typename U> explicit _xvec2(const _xvec4<U>& a);

        // Bool constructors
        explicit _xvec2(const bool x);
        explicit _xvec2(const bool a, const bool b);
        explicit _xvec2(const bool a, const _bvec2& b);
        explicit _xvec2(const bool a, const _bvec3& b);
        explicit _xvec2(const bool a, const _bvec4& b);
        explicit _xvec2(const _bvec2& a);
        explicit _xvec2(const _bvec3& a);
        explicit _xvec2(const _bvec4& a);

        // Accesses
        T& operator[](int i);
        T operator[](int i) const;
        operator T*();
	    operator const T*() const;

        // Unary updatable operators
        _xvec2<T>& operator= (const _xvec2<T>& v);
	    _xvec2<T>& operator+=(const T s);
	    _xvec2<T>& operator+=(const _xvec2<T>& v);
	    _xvec2<T>& operator-=(const T s);
	    _xvec2<T>& operator-=(const _xvec2<T>& v);
	    _xvec2<T>& operator*=(const T s);
	    _xvec2<T>& operator*=(const _xvec2<T>& v);
	    _xvec2<T>& operator/=(const T s);
	    _xvec2<T>& operator/=(const _xvec2<T>& v);
        _xvec2<T>& operator++();
	    _xvec2<T>& operator--();

        // Left hand side 2 components common swizzle operators
        _xref2<T> _xy();
        _xref2<T> _yx();

        // Right hand side 2 components common swizzle operators
        _xvec2<T> _xx() const;
        _xvec2<T> _yx() const;
        _xvec2<T> _xy() const;
        _xvec2<T> _yy() const;

        // Right hand side 3 components common swizzle operators
        _xvec3<T> _xxx() const;
        _xvec3<T> _yxx() const;
        _xvec3<T> _xyx() const;
        _xvec3<T> _yyx() const;
        _xvec3<T> _xxy() const;
        _xvec3<T> _yxy() const;
        _xvec3<T> _xyy() const;
        _xvec3<T> _yyy() const;

        // Right hand side 4 components common swizzle operators
        _xvec4<T> _xxxx() const;
        _xvec4<T> _yxxx() const;
        _xvec4<T> _xyxx() const;
        _xvec4<T> _yyxx() const;
        _xvec4<T> _xxyx() const;
        _xvec4<T> _yxyx() const;
        _xvec4<T> _xyyx() const;
        _xvec4<T> _yyyx() const;
        _xvec4<T> _xxxy() const;
        _xvec4<T> _yxxy() const;
        _xvec4<T> _xyxy() const;
        _xvec4<T> _yyxy() const;
        _xvec4<T> _xxyy() const;
        _xvec4<T> _yxyy() const;
        _xvec4<T> _xyyy() const;
        _xvec4<T> _yyyy() const;
/*
        // Left hand side 2 components color swizzle operators
        _xref2<T> _rg();
        _xref2<T> _gr();

        // Right hand side 2 components color swizzle operators
        _xvec2<T> _rr() const;
        _xvec2<T> _gr() const;
        _xvec2<T> _rg() const;
        _xvec2<T> _gg() const;

        // Right hand side 3 components color swizzle operators
        _xvec3<T> _rrr() const;
        _xvec3<T> _grr() const;
        _xvec3<T> _rgr() const;
        _xvec3<T> _ggr() const;
        _xvec3<T> _rrg() const;
        _xvec3<T> _grg() const;
        _xvec3<T> _rgg() const;
        _xvec3<T> _ggg() const;

        // Right hand side 4 components color swizzle operators
        _xvec4<T> _rrrr() const;
        _xvec4<T> _grrr() const;
        _xvec4<T> _rgrr() const;
        _xvec4<T> _ggrr() const;
        _xvec4<T> _rrgr() const;
        _xvec4<T> _grgr() const;
        _xvec4<T> _rggr() const;
        _xvec4<T> _gggr() const;
        _xvec4<T> _rrrg() const;
        _xvec4<T> _grrg() const;
        _xvec4<T> _rgrg() const;
        _xvec4<T> _ggrg() const;
        _xvec4<T> _rrgg() const;
        _xvec4<T> _grgg() const;
        _xvec4<T> _rggg() const;
        _xvec4<T> _gggg() const;

        // Left hand side 2 components texcoord swizzle operators
        _xref2<T> _st();
        _xref2<T> _ts();

        // Right hand side 2 components texcoord swizzle operators
        _xvec2<T> _ss() const;
        _xvec2<T> _ts() const;
        _xvec2<T> _st() const;
        _xvec2<T> _tt() const;

        // Right hand side 3 components texcoord swizzle operators
        _xvec3<T> _sss() const;
        _xvec3<T> _tss() const;
        _xvec3<T> _sts() const;
        _xvec3<T> _tts() const;
        _xvec3<T> _sst() const;
        _xvec3<T> _tst() const;
        _xvec3<T> _stt() const;
        _xvec3<T> _ttt() const;

        // Right hand side 4 components texcoord swizzle operators
        _xvec4<T> _ssss() const;
        _xvec4<T> _tsss() const;
        _xvec4<T> _stss() const;
        _xvec4<T> _ttss() const;
        _xvec4<T> _ssts() const;
        _xvec4<T> _tsts() const;
        _xvec4<T> _stts() const;
        _xvec4<T> _ttts() const;
        _xvec4<T> _ssst() const;
        _xvec4<T> _tsst() const;
        _xvec4<T> _stst() const;
        _xvec4<T> _ttst() const;
        _xvec4<T> _sstt() const;
        _xvec4<T> _tstt() const;
        _xvec4<T> _sttt() const;
        _xvec4<T> _tttt() const;
*/
    };

    // Binary operators
    template <typename T>
	_xvec2<T> operator+ (const _xvec2<T>& v, const T s);

    template <typename T>
    _xvec2<T> operator+ (const T s, const _xvec2<T>& v);

    template <typename T>
    _xvec2<T> operator+ (const _xvec2<T>& v1, const _xvec2<T>& v2);
    
    template <typename T>
	_xvec2<T> operator- (const _xvec2<T>& v, const T s);

    template <typename T>
    _xvec2<T> operator- (const T s, const _xvec2<T>& v);

    template <typename T>
    _xvec2<T> operator- (const _xvec2<T>& v1, const _xvec2<T>& v2);

    template <typename T>
    _xvec2<T> operator* (const _xvec2<T>& v, const T s);

    template <typename T>
    _xvec2<T> operator* (const T s, const _xvec2<T>& v);

    template <typename T>
    _xvec2<T> operator* (const _xvec2<T>& v1, const _xvec2<T>& v2);

    template <typename T>
    _xvec2<T> operator/ (const _xvec2<T>& v, const T s);

    template <typename T>
    _xvec2<T> operator/ (const T s, const _xvec2<T>& v);

    template <typename T>
    _xvec2<T> operator/ (const _xvec2<T>& v1, const _xvec2<T>& v2);

    // Unary constant operators
    template <typename T> 
    inline const _xvec2<T> operator- (const _xvec2<T>& v);

    template <typename T> 
    inline const _xvec2<T> operator--(const _xvec2<T>& v, int);

    template <typename T> 
    inline const _xvec2<T> operator++(const _xvec2<T>& v, int);
} //namespace glm

#endif //__xvec2_h__
