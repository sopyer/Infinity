//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-24
// Updated : 2006-12-06
// Licence : This source is under GNU LGPL licence
// File    : gtx_integer.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_integer_inl__
#define __gtx_integer_inl__

#include "gtx_integer.h"
#if(GLM_INCLUDED(GLM_GTX_integer, GLM_EXTENSION_GTX, GLM_GTX_integer_required))

namespace glm
{
    // absGTX
    inline int absGTX(int x)
    {
        return ((x >> 31) ^ x) - (x >> 31);
    }

    template <typename T> 
	inline T absGTX(T x)
    {
        return x >= T(0) ? x : -x;
    }

    template <typename T> 
	inline _xvec2<T> absGTX(const _xvec2<T>& x)
    {
        return _xvec2<T>(
            absGTX(x.x),
            absGTX(x.y));
    }

    template <typename T> 
	inline _xvec3<T> absGTX(const _xvec3<T>& x)
    {
        return _xvec3<T>(
            absGTX(x.x),
            absGTX(x.y),
            absGTX(x.z));
    }

    template <typename T> 
	inline _xvec4<T> absGTX(const _xvec4<T>& x)
    {
        return _xvec4<T>(
            absGTX(x.x),
            absGTX(x.y),
            absGTX(x.z),
            absGTX(x.w));
    }

    // signGTX
	template <typename genType> inline genType signGTX(const genType& x) {return sign(x);}

    // minGTX
    // test: 
    /*
    inline int minGTX(int x, int y)
    {
        return (x + y - glm::absGTX(x - y)) >> 1;
    }
    */
	template <typename T> inline T minGTX(T x, T y) {return min(x, y);}
	template <typename T> inline _xvec2<T> minGTX(const _xvec2<T>& x, T y) {return min(x, y);}
	template <typename T> inline _xvec3<T> minGTX(const _xvec3<T>& x, T y) {return min(x, y);}
	template <typename T> inline _xvec4<T> minGTX(const _xvec4<T>& x, T y) {return min(x, y);}
	template <typename T> inline _xvec2<T> minGTX(const _xvec2<T>& x, const _xvec2<T>& y) {return min(x, y);}
	template <typename T> inline _xvec3<T> minGTX(const _xvec3<T>& x, const _xvec3<T>& y) {return min(x, y);}
	template <typename T> inline _xvec4<T> minGTX(const _xvec4<T>& x, const _xvec4<T>& y) {return min(x, y);}

	// maxGTX
	template <typename T> inline T maxGTX(T x, T y) {return max(x, y);}
	template <typename T> inline _xvec2<T> maxGTX(const _xvec2<T>& x, T y) {return max(x, y);}
	template <typename T> inline _xvec3<T> maxGTX(const _xvec3<T>& x, T y) {return max(x, y);}
	template <typename T> inline _xvec4<T> maxGTX(const _xvec4<T>& x, T y) {return max(x, y);}
	template <typename T> inline _xvec2<T> maxGTX(const _xvec2<T>& x, const _xvec2<T>& y) {return max(x, y);}
	template <typename T> inline _xvec3<T> maxGTX(const _xvec3<T>& x, const _xvec3<T>& y) {return max(x, y);}
	template <typename T> inline _xvec4<T> maxGTX(const _xvec4<T>& x, const _xvec4<T>& y) {return max(x, y);}

    // clampGTX
    template <typename T> inline T clampGTX(T x, T minVal, T maxVal) {return min(max(x, minVal), maxVal);}
    template <typename T> inline _xvec2<T> clampGTX(const _xvec2<T>& x, T minVal, T maxVal) {return min(max(x, minVal), maxVal);}
    template <typename T> inline _xvec3<T> clampGTX(const _xvec3<T>& x, T minVal, T maxVal) {return min(max(x, minVal), maxVal);}
    template <typename T> inline _xvec4<T> clampGTX(const _xvec4<T>& x, T minVal, T maxVal) {return min(max(x, minVal), maxVal);}
    template <typename T> inline _xvec2<T> clampGTX(const _xvec2<T>& x, const _xvec2<T>& minVal, const _xvec2<T>& maxVal) {return min(max(x, minVal), maxVal);}
    template <typename T> inline _xvec3<T> clampGTX(const _xvec3<T>& x, const _xvec3<T>& minVal, const _xvec3<T>& maxVal) {return min(max(x, minVal), maxVal);}
    template <typename T> inline _xvec4<T> clampGTX(const _xvec4<T>& x, const _xvec4<T>& minVal, const _xvec4<T>& maxVal) {return min(max(x, minVal), maxVal);}

    // powGTX
    inline int powGTX(int x, int y)
    {
        int result = x;
        for(int i = 1; i < y; ++i)
            result *= x;
        return result;
    }

	template <typename T> 
    inline T powGTX(T x, T y)
    {
        return std::pow(x, y);
    }

	template <typename T> 
    inline _xvec2<T> powGTX(const _xvec2<T>& x, const _xvec2<T>& y)
    {
        return _xvec2<T>(
            powGTX(x.x, y.x),
            powGTX(x.y, y.y));
    }

	template <typename T> 
    inline _xvec3<T> powGTX(const _xvec3<T>& x, const _xvec3<T>& y)
    {
        return _xvec3<T>(
            powGTX(x.x, y.x),
            powGTX(x.y, y.y),
            powGTX(x.z, y.z));
    }

	template <typename T> 
    inline _xvec4<T> powGTX(const _xvec4<T>& x, const _xvec4<T>& y)
    {
        return _xvec4<T>(
            powGTX(x.x, y.x),
            powGTX(x.y, y.y),
            powGTX(x.z, y.z),
            powGTX(x.w, y.w));
    }

    // sqrtGTX: From Christopher J. Musial, An integer square root, Graphics Gems, 1990, page 387
    inline int sqrtGTX(int x)
    {
        if(x <= 1) return x;

        int NextTrial = x >> 1;
        int CurrentAnswer;

        do
        {
            CurrentAnswer = NextTrial;
            NextTrial = (NextTrial + x / NextTrial) >> 1;
        } while(NextTrial < CurrentAnswer);

        return CurrentAnswer;
    }

    template <typename T> 
    inline T sqrtGTX(T x)
    {
        return std::sqrt(x);
    }

    template <typename T> 
	inline _xvec2<T> sqrtGTX(const _xvec2<T>& x)
    {
        return _xvec2<T>(
            sqrtGTX(x.x),
            sqrtGTX(x.y));
    }

    template <typename T> 
	inline _xvec3<T> sqrtGTX(const _xvec3<T>& x)
    {
        return _xvec3<T>(
            sqrtGTX(x.x),
            sqrtGTX(x.y),
            sqrtGTX(x.z));
    }

    template <typename T> 
	inline _xvec4<T> sqrtGTX(const _xvec4<T>& x)
    {
        return _xvec4<T>(
            sqrtGTX(x.x),
            sqrtGTX(x.y),
            sqrtGTX(x.z),
            sqrtGTX(x.w));
    }

    // modGTX
	template <typename T> 
    inline T modGTX(T x, T y)
    {
        return x - y * (x / y);
    }

	template <typename T> 
    inline _xvec2<T> modGTX(const _xvec2<T>& x, T y)
    {
        return _xvec2<T>(
            modGTX(x.x, y),
            modGTX(x.y, y));
    }

	template <typename T> 
    inline _xvec3<T> modGTX(const _xvec3<T>& x, T y)
    {
        return _xvec3<T>(
            modGTX(x.x, y),
            modGTX(x.y, y),
            modGTX(x.z, y));
    }

	template <typename T> 
    inline _xvec4<T> modGTX(const _xvec4<T>& x, T y)
    {
        return _xvec4<T>(
            modGTX(x.x, y),
            modGTX(x.y, y),
            modGTX(x.z, y),
            modGTX(x.w, y));
    }

	template <typename T> 
    inline _xvec2<T> modGTX(const _xvec2<T>& x, const _xvec2<T>& y)
    {
        return _xvec2<T>(
            modGTX(x.x, y.x),
            modGTX(x.y, y.y));
    }

	template <typename T> 
    inline _xvec3<T> modGTX(const _xvec3<T>& x, const _xvec3<T>& y)
    {
        return _xvec3<T>(
            modGTX(x.x, y.x),
            modGTX(x.y, y.y),
            modGTX(x.z, y.z));
    }

	template <typename T> 
    inline _xvec4<T> modGTX(const _xvec4<T>& x, const _xvec4<T>& y)
    {
        return _xvec4<T>(
            modGTX(x.x, y.x),
            modGTX(x.y, y.y),
            modGTX(x.z, y.z),
            modGTX(x.w, y.w));
    }

    // factorial (!12 max, integer only)
	template <typename T> 
    inline T factorialGTX(T x)
    {
        T Result;
        for(Result = 1; x > 1; --x)
            Result *= x;
        return Result;
    }

	template <typename T> 
    inline _xvec2<T> factorialGTX(const _xvec2<T>& x)
    {
        return _xvec2<T>(
            factorialGTX(x.x),
            factorialGTX(x.y));
    }

	template <typename T> 
    inline _xvec3<T> factorialGTX(const _xvec3<T>& x)
    {
        return _xvec3<T>(
            factorialGTX(x.x),
            factorialGTX(x.y),
            factorialGTX(x.z));
    }

	template <typename T> 
    inline _xvec4<T> factorialGTX(const _xvec4<T>& x)
    {
        return _xvec4<T>(
            factorialGTX(x.x),
            factorialGTX(x.y),
            factorialGTX(x.z),
            factorialGTX(x.w));
    }

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    // absGTX
    inline int abs(int x)
    {
        return ((x >> 31) ^ x) - (x >> 31);
    }

	// pow
	inline int pow(int x, int y) {return powGTX(x, y);}

    // sqrt: From Christopher J. Musial, An integer square root, Graphics Gems, 1990, page 387
	inline int sqrt(int x) {return sqrtGTX(x);}

    // mod
	inline int mod(int x, int y) {return modGTX(x, y);}

    // factorial (!12 max, integer only)
	template <typename T> inline T factorial(T x) {return factorialGTX(x);}
	template <typename T> inline _xvec2<T> factorial(const _xvec2<T>& x) {return factorialGTX(x);}
	template <typename T> inline _xvec3<T> factorial(const _xvec3<T>& x) {return factorialGTX(x);}
    template <typename T> inline _xvec4<T> factorial(const _xvec4<T>& x) {return factorialGTX(x);}

#endif//GLM_CONFORMANCE
}

#endif//included

#endif//__gtx_integer_inl__
