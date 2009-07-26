//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-27
// Updated : 2006-04-27
// Licence : This source is under GNU LGPL licence
// File    : _xvec3.h
//////////////////////////////////////////////////////////////////////////////////
// Optimisation note: Don't use vec3 class instead of vec4 because you think it 
// whould be faster. It whouldn't be the case especially if you use SSE 
// intructions set in your compiler option and/or mat4 transformations.
// An advide: test your code speed with vec3 and vec4 to select the faster.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __xvec3_h__
#define __xvec3_h__

namespace glm
{
    template <typename T> class _xvec2;
    template <typename T> class _xvec4;
    class _bvec2;
    class _bvec3;
    class _bvec4;
    template <typename T> struct _xref3;

    //!< \brief Template for Vector of 3 floating-point numbers or 3 integers . (From GLSL 1.10.59 specification)
    template <typename T> 
    class _xvec3
    {
    public:
        // Data
        /* ISO C++ version unavailable with VC7.1 ...
            union{T x, r, s;};
            union{T y, g, t;};
            union{T z, b, p;};
        */
        // Solution away from ISO C++ but available with VC7.1 and GCC without -pedantic
        union 
        {
            struct{T x, y, z;};
            struct{T r, g, b;};
            struct{T s, t, p;};
        };

        // Common constructors
	    _xvec3();
        _xvec3(const _xvec3<T>& v);

        // Swizzle constructors
        _xvec3(const _xref3<T>& r);

        // T constructors
        explicit _xvec3(const T x);
        explicit _xvec3(const T a, const T b, const T c);
        explicit _xvec3(const T a, const T b, const _xvec2<T>& c);
        explicit _xvec3(const T a, const T b, const _xvec3<T>& c);
        explicit _xvec3(const T a, const T b, const _xvec4<T>& c);
        explicit _xvec3(const T a, const _xvec2<T>& b);
        explicit _xvec3(const T a, const _xvec3<T>& b);
        explicit _xvec3(const T a, const _xvec4<T>& b);
        explicit _xvec3(const _xvec2<T>& a, T b);
        explicit _xvec3(const _xvec2<T>& a, const _xvec2<T>& b);
        explicit _xvec3(const _xvec2<T>& a, const _xvec3<T>& b);
        explicit _xvec3(const _xvec2<T>& a, const _xvec4<T>& b);
        explicit _xvec3(const _xvec4<T>& a);

        // U constructors
        template <typename U> explicit _xvec3(const U x);
        template <typename U> explicit _xvec3(const U a, const U b, const U c);
        template <typename U> explicit _xvec3(const U a, const U b, const _xvec2<U>& c);
        template <typename U> explicit _xvec3(const U a, const U b, const _xvec3<U>& c);
        template <typename U> explicit _xvec3(const U a, const U b, const _xvec4<U>& c);
        template <typename U> explicit _xvec3(const U a, const _xvec2<U>& b);
        template <typename U> explicit _xvec3(const U a, const _xvec3<U>& b);
        template <typename U> explicit _xvec3(const U a, const _xvec4<U>& b);
        template <typename U> explicit _xvec3(const _xvec2<U>& a, U b);
        template <typename U> explicit _xvec3(const _xvec2<U>& a, const _xvec2<U>& b);
        template <typename U> explicit _xvec3(const _xvec2<U>& a, const _xvec3<U>& b);
        template <typename U> explicit _xvec3(const _xvec2<U>& a, const _xvec4<U>& b);
        template <typename U> explicit _xvec3(const _xvec3<U>& a);
        template <typename U> explicit _xvec3(const _xvec4<U>& a);

        // Bool constructors
        explicit _xvec3(const bool x);
        explicit _xvec3(const bool a, const bool b, const bool c);
        explicit _xvec3(const bool a, const bool b, const _bvec2& c);
        explicit _xvec3(const bool a, const bool b, const _bvec3& c);
        explicit _xvec3(const bool a, const bool b, const _bvec4& c);
        explicit _xvec3(const bool a, const _bvec2& b);
        explicit _xvec3(const bool a, const _bvec3& b);
        explicit _xvec3(const bool a, const _bvec4& b);
        explicit _xvec3(const _bvec2& a, bool b);
        explicit _xvec3(const _bvec2& a, const _bvec2& b);
        explicit _xvec3(const _bvec2& a, const _bvec3& b);
        explicit _xvec3(const _bvec2& a, const _bvec4& b);
        explicit _xvec3(const _bvec3& a);
        explicit _xvec3(const _bvec4& a);

        // Accesses
        T& operator [] (int i);
        T operator [] (int i) const;
        operator T* ();
	    operator const T* () const;

        // Unary updatable operators
        _xvec3<T>& operator= (const _xvec3<T>& v);
	    _xvec3<T>& operator+=(const T s);
	    _xvec3<T>& operator+=(const _xvec3<T>& v);
	    _xvec3<T>& operator-=(const T s);
	    _xvec3<T>& operator-=(const _xvec3<T>& v);
	    _xvec3<T>& operator*=(const T s);
	    _xvec3<T>& operator*=(const _xvec3<T>& v);
	    _xvec3<T>& operator/=(const T s);
	    _xvec3<T>& operator/=(const _xvec3<T>& v);
        _xvec3<T>& operator++();
	    _xvec3<T>& operator--();

        // Right hand side 2 components common swizzle operators
        _xvec2<T> _xx() const;
        _xvec2<T> _yx() const;
        _xvec2<T> _zx() const;
        _xvec2<T> _xy() const;
        _xvec2<T> _yy() const;
        _xvec2<T> _zy() const;
        _xvec2<T> _xz() const;
        _xvec2<T> _yz() const;
        _xvec2<T> _zz() const;

        // Left hand side 3 components common swizzle operators
        _xref3<T> _zyx();
        _xref3<T> _yzx();
        _xref3<T> _zxy();
        _xref3<T> _xzy();
        _xref3<T> _yxz();
        _xref3<T> _xyz();

        // Right hand side 3 components common swizzle operators
        _xvec3<T> _xxx() const;
        _xvec3<T> _yxx() const;
        _xvec3<T> _zxx() const;
        _xvec3<T> _xyx() const;
        _xvec3<T> _yyx() const;
        _xvec3<T> _zyx() const;
        _xvec3<T> _xzx() const;
        _xvec3<T> _yzx() const;
        _xvec3<T> _zzx() const;
        _xvec3<T> _xxy() const;
        _xvec3<T> _yxy() const;
        _xvec3<T> _zxy() const;
        _xvec3<T> _xyy() const;
        _xvec3<T> _yyy() const;
        _xvec3<T> _zyy() const;
        _xvec3<T> _xzy() const;
        _xvec3<T> _yzy() const;
        _xvec3<T> _zzy() const;
        _xvec3<T> _xxz() const;
        _xvec3<T> _yxz() const;
        _xvec3<T> _zxz() const;
        _xvec3<T> _xyz() const;
        _xvec3<T> _yyz() const;
        _xvec3<T> _zyz() const;
        _xvec3<T> _xzz() const;
        _xvec3<T> _yzz() const;
        _xvec3<T> _zzz() const;

        // 4 components common swizzle operators
        _xvec4<T> _xxxx() const;
        _xvec4<T> _yxxx() const;
        _xvec4<T> _zxxx() const;
        _xvec4<T> _xyxx() const;
        _xvec4<T> _yyxx() const;
        _xvec4<T> _zyxx() const;
        _xvec4<T> _xzxx() const;
        _xvec4<T> _yzxx() const;
        _xvec4<T> _zzxx() const;
        _xvec4<T> _xxyx() const;
        _xvec4<T> _yxyx() const;
        _xvec4<T> _zxyx() const;
        _xvec4<T> _xyyx() const;
        _xvec4<T> _yyyx() const;
        _xvec4<T> _zyyx() const;
        _xvec4<T> _xzyx() const;
        _xvec4<T> _yzyx() const;
        _xvec4<T> _zzyx() const;
        _xvec4<T> _xxzx() const;
        _xvec4<T> _yxzx() const;
        _xvec4<T> _zxzx() const;
        _xvec4<T> _xyzx() const;
        _xvec4<T> _yyzx() const;
        _xvec4<T> _zyzx() const;
        _xvec4<T> _xzzx() const;
        _xvec4<T> _yzzx() const;
        _xvec4<T> _zzzx() const;
        _xvec4<T> _xxxy() const;
        _xvec4<T> _yxxy() const;
        _xvec4<T> _zxxy() const;
        _xvec4<T> _xyxy() const;
        _xvec4<T> _yyxy() const;
        _xvec4<T> _zyxy() const;
        _xvec4<T> _xzxy() const;
        _xvec4<T> _yzxy() const;
        _xvec4<T> _zzxy() const;
        _xvec4<T> _xxyy() const;
        _xvec4<T> _yxyy() const;
        _xvec4<T> _zxyy() const;
        _xvec4<T> _xyyy() const;
        _xvec4<T> _yyyy() const;
        _xvec4<T> _zyyy() const;
        _xvec4<T> _xzyy() const;
        _xvec4<T> _yzyy() const;
        _xvec4<T> _zzyy() const;
        _xvec4<T> _xxzy() const;
        _xvec4<T> _yxzy() const;
        _xvec4<T> _zxzy() const;
        _xvec4<T> _xyzy() const;
        _xvec4<T> _yyzy() const;
        _xvec4<T> _zyzy() const;
        _xvec4<T> _xzzy() const;
        _xvec4<T> _yzzy() const;
        _xvec4<T> _zzzy() const;
        _xvec4<T> _xxxz() const;
        _xvec4<T> _yxxz() const;
        _xvec4<T> _zxxz() const;
        _xvec4<T> _xyxz() const;
        _xvec4<T> _yyxz() const;
        _xvec4<T> _zyxz() const;
        _xvec4<T> _xzxz() const;
        _xvec4<T> _yzxz() const;
        _xvec4<T> _zzxz() const;
        _xvec4<T> _xxyz() const;
        _xvec4<T> _yxyz() const;
        _xvec4<T> _zxyz() const;
        _xvec4<T> _xyyz() const;
        _xvec4<T> _yyyz() const;
        _xvec4<T> _zyyz() const;
        _xvec4<T> _xzyz() const;
        _xvec4<T> _yzyz() const;
        _xvec4<T> _zzyz() const;
        _xvec4<T> _xxzz() const;
        _xvec4<T> _yxzz() const;
        _xvec4<T> _zxzz() const;
        _xvec4<T> _xyzz() const;
        _xvec4<T> _yyzz() const;
        _xvec4<T> _zyzz() const;
        _xvec4<T> _xzzz() const;
        _xvec4<T> _yzzz() const;
        _xvec4<T> _zzzz() const;
/*
        // 2 components color swizzle operators
        _xvec2<T> _rr() const;
        _xvec2<T> _gr() const;
        _xvec2<T> _br() const;
        _xvec2<T> _rg() const;
        _xvec2<T> _gg() const;
        _xvec2<T> _bg() const;
        _xvec2<T> _rb() const;
        _xvec2<T> _gb() const;
        _xvec2<T> _bb() const;

        // Left hand side 3 components color swizzle operators
        _xref3<T> _bgr();
        _xref3<T> _gbr();
        _xref3<T> _brg();
        _xref3<T> _rbg();
        _xref3<T> _grb();
        _xref3<T> _rgb();

        // 3 components color swizzle operators
        _xvec3<T> _rrr() const;
        _xvec3<T> _grr() const;
        _xvec3<T> _brr() const;
        _xvec3<T> _rgr() const;
        _xvec3<T> _ggr() const;
        _xvec3<T> _bgr() const;
        _xvec3<T> _rbr() const;
        _xvec3<T> _gbr() const;
        _xvec3<T> _bbr() const;
        _xvec3<T> _rrg() const;
        _xvec3<T> _grg() const;
        _xvec3<T> _brg() const;
        _xvec3<T> _rgg() const;
        _xvec3<T> _ggg() const;
        _xvec3<T> _bgg() const;
        _xvec3<T> _rbg() const;
        _xvec3<T> _gbg() const;
        _xvec3<T> _bbg() const;
        _xvec3<T> _rrb() const;
        _xvec3<T> _grb() const;
        _xvec3<T> _brb() const;
        _xvec3<T> _rgb() const;
        _xvec3<T> _ggb() const;
        _xvec3<T> _bgb() const;
        _xvec3<T> _rbb() const;
        _xvec3<T> _gbb() const;
        _xvec3<T> _bbb() const;

        // 4 components color swizzle operators
        _xvec4<T> _rrrr() const;
        _xvec4<T> _grrr() const;
        _xvec4<T> _brrr() const;
        _xvec4<T> _rgrr() const;
        _xvec4<T> _ggrr() const;
        _xvec4<T> _bgrr() const;
        _xvec4<T> _rbrr() const;
        _xvec4<T> _gbrr() const;
        _xvec4<T> _bbrr() const;
        _xvec4<T> _rrgr() const;
        _xvec4<T> _grgr() const;
        _xvec4<T> _brgr() const;
        _xvec4<T> _rggr() const;
        _xvec4<T> _gggr() const;
        _xvec4<T> _bggr() const;
        _xvec4<T> _rbgr() const;
        _xvec4<T> _gbgr() const;
        _xvec4<T> _bbgr() const;
        _xvec4<T> _rrbr() const;
        _xvec4<T> _grbr() const;
        _xvec4<T> _brbr() const;
        _xvec4<T> _rgbr() const;
        _xvec4<T> _ggbr() const;
        _xvec4<T> _bgbr() const;
        _xvec4<T> _rbbr() const;
        _xvec4<T> _gbbr() const;
        _xvec4<T> _bbbr() const;
        _xvec4<T> _rrrg() const;
        _xvec4<T> _grrg() const;
        _xvec4<T> _brrg() const;
        _xvec4<T> _rgrg() const;
        _xvec4<T> _ggrg() const;
        _xvec4<T> _bgrg() const;
        _xvec4<T> _rbrg() const;
        _xvec4<T> _gbrg() const;
        _xvec4<T> _bbrg() const;
        _xvec4<T> _rrgg() const;
        _xvec4<T> _grgg() const;
        _xvec4<T> _brgg() const;
        _xvec4<T> _rggg() const;
        _xvec4<T> _gggg() const;
        _xvec4<T> _bggg() const;
        _xvec4<T> _rbgg() const;
        _xvec4<T> _gbgg() const;
        _xvec4<T> _bbgg() const;
        _xvec4<T> _rrbg() const;
        _xvec4<T> _grbg() const;
        _xvec4<T> _brbg() const;
        _xvec4<T> _rgbg() const;
        _xvec4<T> _ggbg() const;
        _xvec4<T> _bgbg() const;
        _xvec4<T> _rbbg() const;
        _xvec4<T> _gbbg() const;
        _xvec4<T> _bbbg() const;
        _xvec4<T> _rrrb() const;
        _xvec4<T> _grrb() const;
        _xvec4<T> _brrb() const;
        _xvec4<T> _rgrb() const;
        _xvec4<T> _ggrb() const;
        _xvec4<T> _bgrb() const;
        _xvec4<T> _rbrb() const;
        _xvec4<T> _gbrb() const;
        _xvec4<T> _bbrb() const;
        _xvec4<T> _rrgb() const;
        _xvec4<T> _grgb() const;
        _xvec4<T> _brgb() const;
        _xvec4<T> _rggb() const;
        _xvec4<T> _gggb() const;
        _xvec4<T> _bggb() const;
        _xvec4<T> _rbgb() const;
        _xvec4<T> _gbgb() const;
        _xvec4<T> _bbgb() const;
        _xvec4<T> _rrbb() const;
        _xvec4<T> _grbb() const;
        _xvec4<T> _brbb() const;
        _xvec4<T> _rgbb() const;
        _xvec4<T> _ggbb() const;
        _xvec4<T> _bgbb() const;
        _xvec4<T> _rbbb() const;
        _xvec4<T> _gbbb() const;
        _xvec4<T> _bbbb() const;

        // 2 components texcoord swizzle operators
        _xvec2<T> _ss() const;
        _xvec2<T> _ts() const;
        _xvec2<T> _ps() const;
        _xvec2<T> _st() const;
        _xvec2<T> _tt() const;
        _xvec2<T> _pt() const;
        _xvec2<T> _sp() const;
        _xvec2<T> _tp() const;
        _xvec2<T> _pp() const;

        // Left hand side 3 components texcoord swizzle operators
        _xref3<T> _pts();
        _xref3<T> _tps();
        _xref3<T> _pst();
        _xref3<T> _spt();
        _xref3<T> _tsp();
        _xref3<T> _stp();

        // 3 components texcoord swizzle operators
        _xvec3<T> _sss() const;
        _xvec3<T> _tss() const;
        _xvec3<T> _pss() const;
        _xvec3<T> _sts() const;
        _xvec3<T> _tts() const;
        _xvec3<T> _pts() const;
        _xvec3<T> _sps() const;
        _xvec3<T> _tps() const;
        _xvec3<T> _pps() const;
        _xvec3<T> _sst() const;
        _xvec3<T> _tst() const;
        _xvec3<T> _pst() const;
        _xvec3<T> _stt() const;
        _xvec3<T> _ttt() const;
        _xvec3<T> _ptt() const;
        _xvec3<T> _spt() const;
        _xvec3<T> _tpt() const;
        _xvec3<T> _ppt() const;
        _xvec3<T> _ssp() const;
        _xvec3<T> _tsp() const;
        _xvec3<T> _psp() const;
        _xvec3<T> _stp() const;
        _xvec3<T> _ttp() const;
        _xvec3<T> _ptp() const;
        _xvec3<T> _spp() const;
        _xvec3<T> _tpp() const;
        _xvec3<T> _ppp() const;

        // 4 components texcoord swizzle operators
        _xvec4<T> _ssss() const;
        _xvec4<T> _tsss() const;
        _xvec4<T> _psss() const;
        _xvec4<T> _stss() const;
        _xvec4<T> _ttss() const;
        _xvec4<T> _ptss() const;
        _xvec4<T> _spss() const;
        _xvec4<T> _tpss() const;
        _xvec4<T> _ppss() const;
        _xvec4<T> _ssts() const;
        _xvec4<T> _tsts() const;
        _xvec4<T> _psts() const;
        _xvec4<T> _stts() const;
        _xvec4<T> _ttts() const;
        _xvec4<T> _ptts() const;
        _xvec4<T> _spts() const;
        _xvec4<T> _tpts() const;
        _xvec4<T> _ppts() const;
        _xvec4<T> _ssps() const;
        _xvec4<T> _tsps() const;
        _xvec4<T> _psps() const;
        _xvec4<T> _stps() const;
        _xvec4<T> _ttps() const;
        _xvec4<T> _ptps() const;
        _xvec4<T> _spps() const;
        _xvec4<T> _tpps() const;
        _xvec4<T> _ppps() const;
        _xvec4<T> _ssst() const;
        _xvec4<T> _tsst() const;
        _xvec4<T> _psst() const;
        _xvec4<T> _stst() const;
        _xvec4<T> _ttst() const;
        _xvec4<T> _ptst() const;
        _xvec4<T> _spst() const;
        _xvec4<T> _tpst() const;
        _xvec4<T> _ppst() const;
        _xvec4<T> _sstt() const;
        _xvec4<T> _tstt() const;
        _xvec4<T> _pstt() const;
        _xvec4<T> _sttt() const;
        _xvec4<T> _tttt() const;
        _xvec4<T> _pttt() const;
        _xvec4<T> _sptt() const;
        _xvec4<T> _tptt() const;
        _xvec4<T> _pptt() const;
        _xvec4<T> _sspt() const;
        _xvec4<T> _tspt() const;
        _xvec4<T> _pspt() const;
        _xvec4<T> _stpt() const;
        _xvec4<T> _ttpt() const;
        _xvec4<T> _ptpt() const;
        _xvec4<T> _sppt() const;
        _xvec4<T> _tppt() const;
        _xvec4<T> _pppt() const;
        _xvec4<T> _sssp() const;
        _xvec4<T> _tssp() const;
        _xvec4<T> _pssp() const;
        _xvec4<T> _stsp() const;
        _xvec4<T> _ttsp() const;
        _xvec4<T> _ptsp() const;
        _xvec4<T> _spsp() const;
        _xvec4<T> _tpsp() const;
        _xvec4<T> _ppsp() const;
        _xvec4<T> _sstp() const;
        _xvec4<T> _tstp() const;
        _xvec4<T> _pstp() const;
        _xvec4<T> _sttp() const;
        _xvec4<T> _tttp() const;
        _xvec4<T> _pttp() const;
        _xvec4<T> _sptp() const;
        _xvec4<T> _tptp() const;
        _xvec4<T> _pptp() const;
        _xvec4<T> _sspp() const;
        _xvec4<T> _tspp() const;
        _xvec4<T> _pspp() const;
        _xvec4<T> _stpp() const;
        _xvec4<T> _ttpp() const;
        _xvec4<T> _ptpp() const;
        _xvec4<T> _sppp() const;
        _xvec4<T> _tppp() const;
        _xvec4<T> _pppp() const;
*/
    };

    // Binasy opesators
    template <typename T>
	_xvec3<T> operator+ (const _xvec3<T>& v, const T s);

    template <typename T>
    _xvec3<T> operator+ (const T s, const _xvec3<T>& v);

    template <typename T>
    _xvec3<T> operator+ (const _xvec3<T>& v1, const _xvec3<T>& v2);
    
    template <typename T>
	_xvec3<T> operator- (const _xvec3<T>& v, const T s);

    template <typename T>
    _xvec3<T> operator- (const T s, const _xvec3<T>& v);

    template <typename T>
    _xvec3<T> operator- (const _xvec3<T>& v1, const _xvec3<T>& v2);

    template <typename T>
    _xvec3<T> operator* (const _xvec3<T>& v, const T s);

    template <typename T>
    _xvec3<T> operator* (const T s, const _xvec3<T>& v);

    template <typename T>
    _xvec3<T> operator* (const _xvec3<T>& v1, const _xvec3<T>& v2);

    template <typename T>
    _xvec3<T> operator/ (const _xvec3<T>& v, const T s);

    template <typename T>
    _xvec3<T> operator/ (const T s, const _xvec3<T>& v);

    template <typename T>
    _xvec3<T> operator/ (const _xvec3<T>& v1, const _xvec3<T>& v2);

    // Unary constant operators
    template <typename T> 
    inline const _xvec3<T> operator- (const _xvec3<T>& v);

    template <typename T> 
    inline const _xvec3<T> operator-- (const _xvec3<T>& v, int);

    template <typename T> 
    inline const _xvec3<T> operator++ (const _xvec3<T>& v, int);
} //namespace glm

#endif//__xvec3_h__
