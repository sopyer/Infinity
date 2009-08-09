//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-27
// Updated : 2006-04-27
// Licence : This source is under GNU LGPL licence
// File    : _bvec3.h
//////////////////////////////////////////////////////////////////////////////////
// Optimisation note: Don't use vec3 class instead of vec4 because you think it 
// whould be faster. It whouldn't be the case especially if you use SSE 
// intructions set in your compiler option and/or mat4 transformations.
// An advide: test your code speed with vec3 and vec4 to select the faster.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __bvec3_h__
#define __bvec3_h__

namespace glm
{
    template <typename T> class _xvec2;
    template <typename T> class _xvec3;
    template <typename T> class _xvec4;
    class _bvec2;
    class _bvec4;
    struct _bref3;

    //!< \brief Class for Vector of 3 booleans. (From GLSL 1.10.59 specification)
    class _bvec3
    {
    public:
        // Data
        union{bool x, r, s;};
        union{bool y, g, t;};
        union{bool z, b, p;};

        // Common constructors
	    _bvec3();
        _bvec3(const _bvec3& v);

        // Swizzle constructors
        _bvec3(const _bref3& r);

        // Bool constructors
        explicit _bvec3(const bool a);
        explicit _bvec3(const bool a, const bool b, const bool c);
        explicit _bvec3(const bool a, const bool b, const _bvec2& c);
        explicit _bvec3(const bool a, const bool b, const _bvec3& c);
        explicit _bvec3(const bool a, const bool b, const _bvec4& c);
        explicit _bvec3(const bool a, const _bvec2& b);
        explicit _bvec3(const bool a, const _bvec3& b);
        explicit _bvec3(const bool a, const _bvec4& b);
        explicit _bvec3(const _bvec2& a, bool b);
        explicit _bvec3(const _bvec2& a, const _bvec2& b);
        explicit _bvec3(const _bvec2& a, const _bvec3& b);
        explicit _bvec3(const _bvec2& a, const _bvec4& b);
        explicit _bvec3(const _bvec4& a);

        // U constructors
        template <typename U> explicit _bvec3(const U x);
        template <typename U> explicit _bvec3(const U a, const U b, const U c);
        template <typename U> explicit _bvec3(const U a, const U b, const _xvec2<U>& c);
        template <typename U> explicit _bvec3(const U a, const U b, const _xvec3<U>& c);
        template <typename U> explicit _bvec3(const U a, const U b, const _xvec4<U>& c);
        template <typename U> explicit _bvec3(const U a, const _xvec2<U>& b);
        template <typename U> explicit _bvec3(const U a, const _xvec3<U>& b);
        template <typename U> explicit _bvec3(const U a, const _xvec4<U>& b);
        template <typename U> explicit _bvec3(const _xvec2<U>& a, U b);
        template <typename U> explicit _bvec3(const _xvec2<U>& a, const _xvec2<U>& b);
        template <typename U> explicit _bvec3(const _xvec2<U>& a, const _xvec3<U>& b);
        template <typename U> explicit _bvec3(const _xvec2<U>& a, const _xvec4<U>& b);
        template <typename U> explicit _bvec3(const _xvec3<U>& a);
        template <typename U> explicit _bvec3(const _xvec4<U>& a);

        // Accesses
        bool& operator [] (int i);
        bool operator [] (int i) const;
        operator bool* ();
	    operator const bool* () const;

        // Operators
        _bvec3& operator=(const _bvec3& x);
/*
	    _bvec3 operator-- ();
		const _bvec3 operator-- (int n) const;
		_bvec3 operator++ ();
	    const _bvec3 operator++ (int n) const;
*/
	    _bvec3 operator! () const;

        // Right hand side 2 components common swizzle operators
        _bvec2 _xx() const;
        _bvec2 _yx() const;
        _bvec2 _zx() const;
        _bvec2 _xy() const;
        _bvec2 _yy() const;
        _bvec2 _zy() const;
        _bvec2 _xz() const;
        _bvec2 _yz() const;
        _bvec2 _zz() const;

        // Left hand side 3 components common swizzle operators
        _bref3 _zyx();
        _bref3 _yzx();
        _bref3 _zxy();
        _bref3 _xzy();
        _bref3 _yxz();
        _bref3 _xyz();

        // Right hand side 3 components common swizzle operators
        _bvec3 _xxx() const;
        _bvec3 _yxx() const;
        _bvec3 _zxx() const;
        _bvec3 _xyx() const;
        _bvec3 _yyx() const;
        _bvec3 _zyx() const;
        _bvec3 _xzx() const;
        _bvec3 _yzx() const;
        _bvec3 _zzx() const;
        _bvec3 _xxy() const;
        _bvec3 _yxy() const;
        _bvec3 _zxy() const;
        _bvec3 _xyy() const;
        _bvec3 _yyy() const;
        _bvec3 _zyy() const;
        _bvec3 _xzy() const;
        _bvec3 _yzy() const;
        _bvec3 _zzy() const;
        _bvec3 _xxz() const;
        _bvec3 _yxz() const;
        _bvec3 _zxz() const;
        _bvec3 _xyz() const;
        _bvec3 _yyz() const;
        _bvec3 _zyz() const;
        _bvec3 _xzz() const;
        _bvec3 _yzz() const;
        _bvec3 _zzz() const;

        // 4 components swizzles operators
        _bvec4 _xxxx() const;
        _bvec4 _yxxx() const;
        _bvec4 _zxxx() const;
        _bvec4 _xyxx() const;
        _bvec4 _yyxx() const;
        _bvec4 _zyxx() const;
        _bvec4 _xzxx() const;
        _bvec4 _yzxx() const;
        _bvec4 _zzxx() const;
        _bvec4 _xxyx() const;
        _bvec4 _yxyx() const;
        _bvec4 _zxyx() const;
        _bvec4 _xyyx() const;
        _bvec4 _yyyx() const;
        _bvec4 _zyyx() const;
        _bvec4 _xzyx() const;
        _bvec4 _yzyx() const;
        _bvec4 _zzyx() const;
        _bvec4 _xxzx() const;
        _bvec4 _yxzx() const;
        _bvec4 _zxzx() const;
        _bvec4 _xyzx() const;
        _bvec4 _yyzx() const;
        _bvec4 _zyzx() const;
        _bvec4 _xzzx() const;
        _bvec4 _yzzx() const;
        _bvec4 _zzzx() const;
        _bvec4 _xxxy() const;
        _bvec4 _yxxy() const;
        _bvec4 _zxxy() const;
        _bvec4 _xyxy() const;
        _bvec4 _yyxy() const;
        _bvec4 _zyxy() const;
        _bvec4 _xzxy() const;
        _bvec4 _yzxy() const;
        _bvec4 _zzxy() const;
        _bvec4 _xxyy() const;
        _bvec4 _yxyy() const;
        _bvec4 _zxyy() const;
        _bvec4 _xyyy() const;
        _bvec4 _yyyy() const;
        _bvec4 _zyyy() const;
        _bvec4 _xzyy() const;
        _bvec4 _yzyy() const;
        _bvec4 _zzyy() const;
        _bvec4 _xxzy() const;
        _bvec4 _yxzy() const;
        _bvec4 _zxzy() const;
        _bvec4 _xyzy() const;
        _bvec4 _yyzy() const;
        _bvec4 _zyzy() const;
        _bvec4 _xzzy() const;
        _bvec4 _yzzy() const;
        _bvec4 _zzzy() const;
        _bvec4 _xxxz() const;
        _bvec4 _yxxz() const;
        _bvec4 _zxxz() const;
        _bvec4 _xyxz() const;
        _bvec4 _yyxz() const;
        _bvec4 _zyxz() const;
        _bvec4 _xzxz() const;
        _bvec4 _yzxz() const;
        _bvec4 _zzxz() const;
        _bvec4 _xxyz() const;
        _bvec4 _yxyz() const;
        _bvec4 _zxyz() const;
        _bvec4 _xyyz() const;
        _bvec4 _yyyz() const;
        _bvec4 _zyyz() const;
        _bvec4 _xzyz() const;
        _bvec4 _yzyz() const;
        _bvec4 _zzyz() const;
        _bvec4 _xxzz() const;
        _bvec4 _yxzz() const;
        _bvec4 _zxzz() const;
        _bvec4 _xyzz() const;
        _bvec4 _yyzz() const;
        _bvec4 _zyzz() const;
        _bvec4 _xzzz() const;
        _bvec4 _yzzz() const;
        _bvec4 _zzzz() const;
/*
        // 2 components color swizzle operators
        _bvec2 _rr() const;
        _bvec2 _gr() const;
        _bvec2 _br() const;
        _bvec2 _rg() const;
        _bvec2 _gg() const;
        _bvec2 _bg() const;
        _bvec2 _rb() const;
        _bvec2 _gb() const;
        _bvec2 _bb() const;

        // Left hand side 3 components color swizzle operators
        _bref3 _bgr();
        _bref3 _gbr();
        _bref3 _brg();
        _bref3 _rbg();
        _bref3 _grb();
        _bref3 _rgb();

        // 3 components color swizzle operators
        _bvec3 _rrr() const;
        _bvec3 _grr() const;
        _bvec3 _brr() const;
        _bvec3 _rgr() const;
        _bvec3 _ggr() const;
        _bvec3 _bgr() const;
        _bvec3 _rbr() const;
        _bvec3 _gbr() const;
        _bvec3 _bbr() const;
        _bvec3 _rrg() const;
        _bvec3 _grg() const;
        _bvec3 _brg() const;
        _bvec3 _rgg() const;
        _bvec3 _ggg() const;
        _bvec3 _bgg() const;
        _bvec3 _rbg() const;
        _bvec3 _gbg() const;
        _bvec3 _bbg() const;
        _bvec3 _rrb() const;
        _bvec3 _grb() const;
        _bvec3 _brb() const;
        _bvec3 _rgb() const;
        _bvec3 _ggb() const;
        _bvec3 _bgb() const;
        _bvec3 _rbb() const;
        _bvec3 _gbb() const;
        _bvec3 _bbb() const;

        // 4 components color swizzle operators
        _bvec4 _rrrr() const;
        _bvec4 _grrr() const;
        _bvec4 _brrr() const;
        _bvec4 _rgrr() const;
        _bvec4 _ggrr() const;
        _bvec4 _bgrr() const;
        _bvec4 _rbrr() const;
        _bvec4 _gbrr() const;
        _bvec4 _bbrr() const;
        _bvec4 _rrgr() const;
        _bvec4 _grgr() const;
        _bvec4 _brgr() const;
        _bvec4 _rggr() const;
        _bvec4 _gggr() const;
        _bvec4 _bggr() const;
        _bvec4 _rbgr() const;
        _bvec4 _gbgr() const;
        _bvec4 _bbgr() const;
        _bvec4 _rrbr() const;
        _bvec4 _grbr() const;
        _bvec4 _brbr() const;
        _bvec4 _rgbr() const;
        _bvec4 _ggbr() const;
        _bvec4 _bgbr() const;
        _bvec4 _rbbr() const;
        _bvec4 _gbbr() const;
        _bvec4 _bbbr() const;
        _bvec4 _rrrg() const;
        _bvec4 _grrg() const;
        _bvec4 _brrg() const;
        _bvec4 _rgrg() const;
        _bvec4 _ggrg() const;
        _bvec4 _bgrg() const;
        _bvec4 _rbrg() const;
        _bvec4 _gbrg() const;
        _bvec4 _bbrg() const;
        _bvec4 _rrgg() const;
        _bvec4 _grgg() const;
        _bvec4 _brgg() const;
        _bvec4 _rggg() const;
        _bvec4 _gggg() const;
        _bvec4 _bggg() const;
        _bvec4 _rbgg() const;
        _bvec4 _gbgg() const;
        _bvec4 _bbgg() const;
        _bvec4 _rrbg() const;
        _bvec4 _grbg() const;
        _bvec4 _brbg() const;
        _bvec4 _rgbg() const;
        _bvec4 _ggbg() const;
        _bvec4 _bgbg() const;
        _bvec4 _rbbg() const;
        _bvec4 _gbbg() const;
        _bvec4 _bbbg() const;
        _bvec4 _rrrb() const;
        _bvec4 _grrb() const;
        _bvec4 _brrb() const;
        _bvec4 _rgrb() const;
        _bvec4 _ggrb() const;
        _bvec4 _bgrb() const;
        _bvec4 _rbrb() const;
        _bvec4 _gbrb() const;
        _bvec4 _bbrb() const;
        _bvec4 _rrgb() const;
        _bvec4 _grgb() const;
        _bvec4 _brgb() const;
        _bvec4 _rggb() const;
        _bvec4 _gggb() const;
        _bvec4 _bggb() const;
        _bvec4 _rbgb() const;
        _bvec4 _gbgb() const;
        _bvec4 _bbgb() const;
        _bvec4 _rrbb() const;
        _bvec4 _grbb() const;
        _bvec4 _brbb() const;
        _bvec4 _rgbb() const;
        _bvec4 _ggbb() const;
        _bvec4 _bgbb() const;
        _bvec4 _rbbb() const;
        _bvec4 _gbbb() const;
        _bvec4 _bbbb() const;

        // 2 components texcoord swizzle operators
        _bvec2 _ss() const;
        _bvec2 _ts() const;
        _bvec2 _ps() const;
        _bvec2 _st() const;
        _bvec2 _tt() const;
        _bvec2 _pt() const;
        _bvec2 _sp() const;
        _bvec2 _tp() const;
        _bvec2 _pp() const;

        // Left hand side 3 components texcoord swizzle operators
        _bref3 _pts();
        _bref3 _tps();
        _bref3 _pst();
        _bref3 _spt();
        _bref3 _tsp();
        _bref3 _stp();

        // 3 components texcoord swizzle operators
        _bvec3 _sss() const;
        _bvec3 _tss() const;
        _bvec3 _pss() const;
        _bvec3 _sts() const;
        _bvec3 _tts() const;
        _bvec3 _pts() const;
        _bvec3 _sps() const;
        _bvec3 _tps() const;
        _bvec3 _pps() const;
        _bvec3 _sst() const;
        _bvec3 _tst() const;
        _bvec3 _pst() const;
        _bvec3 _stt() const;
        _bvec3 _ttt() const;
        _bvec3 _ptt() const;
        _bvec3 _spt() const;
        _bvec3 _tpt() const;
        _bvec3 _ppt() const;
        _bvec3 _ssp() const;
        _bvec3 _tsp() const;
        _bvec3 _psp() const;
        _bvec3 _stp() const;
        _bvec3 _ttp() const;
        _bvec3 _ptp() const;
        _bvec3 _spp() const;
        _bvec3 _tpp() const;
        _bvec3 _ppp() const;

        // 4 components texcoord swizzle operators
        _bvec4 _ssss() const;
        _bvec4 _tsss() const;
        _bvec4 _psss() const;
        _bvec4 _stss() const;
        _bvec4 _ttss() const;
        _bvec4 _ptss() const;
        _bvec4 _spss() const;
        _bvec4 _tpss() const;
        _bvec4 _ppss() const;
        _bvec4 _ssts() const;
        _bvec4 _tsts() const;
        _bvec4 _psts() const;
        _bvec4 _stts() const;
        _bvec4 _ttts() const;
        _bvec4 _ptts() const;
        _bvec4 _spts() const;
        _bvec4 _tpts() const;
        _bvec4 _ppts() const;
        _bvec4 _ssps() const;
        _bvec4 _tsps() const;
        _bvec4 _psps() const;
        _bvec4 _stps() const;
        _bvec4 _ttps() const;
        _bvec4 _ptps() const;
        _bvec4 _spps() const;
        _bvec4 _tpps() const;
        _bvec4 _ppps() const;
        _bvec4 _ssst() const;
        _bvec4 _tsst() const;
        _bvec4 _psst() const;
        _bvec4 _stst() const;
        _bvec4 _ttst() const;
        _bvec4 _ptst() const;
        _bvec4 _spst() const;
        _bvec4 _tpst() const;
        _bvec4 _ppst() const;
        _bvec4 _sstt() const;
        _bvec4 _tstt() const;
        _bvec4 _pstt() const;
        _bvec4 _sttt() const;
        _bvec4 _tttt() const;
        _bvec4 _pttt() const;
        _bvec4 _sptt() const;
        _bvec4 _tptt() const;
        _bvec4 _pptt() const;
        _bvec4 _sspt() const;
        _bvec4 _tspt() const;
        _bvec4 _pspt() const;
        _bvec4 _stpt() const;
        _bvec4 _ttpt() const;
        _bvec4 _ptpt() const;
        _bvec4 _sppt() const;
        _bvec4 _tppt() const;
        _bvec4 _pppt() const;
        _bvec4 _sssp() const;
        _bvec4 _tssp() const;
        _bvec4 _pssp() const;
        _bvec4 _stsp() const;
        _bvec4 _ttsp() const;
        _bvec4 _ptsp() const;
        _bvec4 _spsp() const;
        _bvec4 _tpsp() const;
        _bvec4 _ppsp() const;
        _bvec4 _sstp() const;
        _bvec4 _tstp() const;
        _bvec4 _pstp() const;
        _bvec4 _sttp() const;
        _bvec4 _tttp() const;
        _bvec4 _pttp() const;
        _bvec4 _sptp() const;
        _bvec4 _tptp() const;
        _bvec4 _pptp() const;
        _bvec4 _sspp() const;
        _bvec4 _tspp() const;
        _bvec4 _pspp() const;
        _bvec4 _stpp() const;
        _bvec4 _ttpp() const;
        _bvec4 _ptpp() const;
        _bvec4 _sppp() const;
        _bvec4 _tppp() const;
        _bvec4 _pppp() const;
*/
    };
} //namespace glm

#endif//__bvec3_h__
