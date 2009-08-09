//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-01-09
// Updated : 2006-01-09
// Licence : This source is under GNU LGPL licence
// File    : gtx_fast_exponential.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_fast_exponential_inl__
#define __gtx_fast_exponential_inl__

#include "gtx_fast_exponential.h"
#if(GLM_INCLUDED(GLM_GTX_fast_exponential, GLM_EXTENSION_GTX, GLM_GTX_fast_exponential_required))

namespace glm
{
    // fastPowGTX: 
    inline float fastPowGTX(float x, float y)
    {
        return glm::exp(y * glm::log(x));
    }

    template <typename T> 
    inline _xvec2<T> fastPowGTX(const _xvec2<T>& x, const _xvec2<T>& y)
    {
        return _xvec2<T>(
            fastPowGTX(x.x, y.x),
            fastPowGTX(x.y, y.y));
    }

    template <typename T> 
    inline _xvec3<T> fastPowGTX(const _xvec3<T>& x, const _xvec3<T>& y)
    {
        return _xvec3<T>(
            fastPowGTX(x.x, y.x),
            fastPowGTX(x.y, y.y),
            fastPowGTX(x.z, y.z));
    }

    template <typename T> 
    inline _xvec4<T> fastPowGTX(const _xvec4<T>& x, const _xvec4<T>& y)
    {
        return _xvec4<T>(
            fastPowGTX(x.x, y.x),
            fastPowGTX(x.y, y.y),
            fastPowGTX(x.z, y.z),
            fastPowGTX(x.w, y.w));
    }

    inline float fastPowGTX(float x, int y)
    {
        float f = 1.0f;
        for(int i = 0; i < y; ++i)
            f *= x;
        return f;
    }

    template <typename T> 
    inline _xvec2<T> fastPowGTX(const _xvec2<T>& x, const _xvec2<int>& y)
    {
        return _xvec2<T>(
            fastPowGTX(x.x, y.x),
            fastPowGTX(x.y, y.y));
    }

    template <typename T> 
    inline _xvec3<T> fastPowGTX(const _xvec3<T>& x, const _xvec3<int>& y)
    {
        return _xvec3<T>(
            fastPowGTX(x.x, y.x),
            fastPowGTX(x.y, y.y),
            fastPowGTX(x.z, y.z));
    }

    template <typename T> 
    inline _xvec4<T> fastPowGTX(const _xvec4<T>& x, const _xvec4<int>& y)
    {
        return _xvec4<T>(
            fastPowGTX(x.x, y.x),
            fastPowGTX(x.y, y.y),
            fastPowGTX(x.z, y.z),
            fastPowGTX(x.w, y.w));
    }

    // fastExpGTX
    // Note: This function provides accurate results only for value between -1 and 1, else avoid it.
    inline float fastExpGTX(float x)
    {
        // This has a better looking and same performance in release mode than the following code. However, in debug mode it's slower.
        // return 1.0f + x * (1.0f + x * 0.5f * (1.0f + x * 0.3333333333f * (1.0f + x * 0.25 * (1.0f + x * 0.2f))));
        float x2 = x * x;
        float x3 = x2 * x;
        float x4 = x3 * x;
        float x5 = x4 * x;
        return 1.0f + x + (x2 * 0.5f) + (x3 * 0.1666666667f) + (x4 * 0.041666667f) + (x5 * 0.008333333333f);
    }
/*  // Try to handle all values of float... but often shower than std::exp, glm::floor and the loop kill the performance
    inline float fastExpGTX(float x)
    {
        const float e = 2.718281828f;
        const float IntegerPart = glm::floor(x);
        const float FloatPart = x - IntegerPart;
        float z = 1.f;

        for(int i = 0; i < int(IntegerPart); ++i)
            z *= e;

        const float x2 = FloatPart * FloatPart;
        const float x3 = x2 * FloatPart;
        const float x4 = x3 * FloatPart;
        const float x5 = x4 * FloatPart;
        return z * (1.0f + FloatPart + (x2 * 0.5f) + (x3 * 0.1666666667f) + (x4 * 0.041666667f) + (x5 * 0.008333333333f));
    }

    // Increase accuracy on number bigger that 1 and smaller than -1 but it's not enough for high and negative numbers
    inline float fastExpGTX(float x)
    {
        // This has a better looking and same performance in release mode than the following code. However, in debug mode it's slower.
        // return 1.0f + x * (1.0f + x * 0.5f * (1.0f + x * 0.3333333333f * (1.0f + x * 0.25 * (1.0f + x * 0.2f))));
        float x2 = x * x;
        float x3 = x2 * x;
        float x4 = x3 * x;
        float x5 = x4 * x;
        float x6 = x5 * x;
        float x7 = x6 * x;
        float x8 = x7 * x;
        return 1.0f + x + (x2 * 0.5f) + (x3 * 0.1666666667f) + (x4 * 0.041666667f) + (x5 * 0.008333333333f)+ (x6 * 0.00138888888888f) + (x7 * 0.000198412698f) + (x8 * 0.0000248015873f);;
    }
*/
    template <typename T> 
    inline _xvec2<T> fastExpGTX(const _xvec2<T>& x)
    {
        return _xvec2<T>(
            fastExpGTX(x.x),
            fastExpGTX(x.y));
    }

    template <typename T> 
    inline _xvec3<T> fastExpGTX(const _xvec3<T>& x)
    {
        return _xvec3<T>(
            fastExpGTX(x.x),
            fastExpGTX(x.y),
            fastExpGTX(x.z));
    }

    template <typename T> 
    inline _xvec4<T> fastExpGTX(const _xvec4<T>& x)
    {
        return _xvec4<T>(
            fastExpGTX(x.x),
            fastExpGTX(x.y),
            fastExpGTX(x.z),
            fastExpGTX(x.w));
    }

    // fastLogGTX
    inline float fastLogGTX(float x)
    {
        return std::log(x);
    }

    /* Slower than the VC7.1 function...
    inline float fastLogGTX(float x)
    {
        float y1 = (x - 1.0f) / (x + 1.0f);
        float y2 = y1 * y1;
        return 2.0f * y1 * (1.0f + y2 * (0.3333333333f + y2 * (0.2f + y2 * 0.1428571429f)));
    }
    */

    template <typename T> 
    inline _xvec2<T> fastLogGTX(const _xvec2<T>& x)
    {
        return _xvec2<T>(
            fastLnGTX(x.x),
            fastLnGTX(x.y));
    }

    template <typename T> 
    inline _xvec3<T> fastLogGTX(const _xvec3<T>& x)
    {
        return _xvec3<T>(
            fastLnGTX(x.x),
            fastLnGTX(x.y),
            fastLnGTX(x.z));
    }

    template <typename T> 
    inline _xvec4<T> fastLogGTX(const _xvec4<T>& x)
    {
        return _xvec4<T>(
            fastLnGTX(x.x),
            fastLnGTX(x.y),
            fastLnGTX(x.z),
            fastLnGTX(x.w));
    }

    //fastExp2GTX, ln2 = 0.69314718055994530941723212145818f
    inline float fastExp2GTX(float x)
    {
        return fastExpGTX(0.69314718055994530941723212145818f * x);
    }

    template <typename T> 
    inline _xvec2<T> fastExp2GTX(const _xvec2<T>& x)
    {
        return _xvec2<T>(
            fastExp2GTX(x.x),
            fastExp2GTX(x.y));
    }

    template <typename T> 
    inline _xvec3<T> fastExp2GTX(const _xvec3<T>& x)
    {
        return _xvec3<T>(
            fastExp2GTX(x.x),
            fastExp2GTX(x.y),
            fastExp2GTX(x.z));
    }

    template <typename T> 
    inline _xvec4<T> fastExp2GTX(const _xvec4<T>& x)
    {
        return _xvec4<T>(
            fastExp2GTX(x.x),
            fastExp2GTX(x.y),
            fastExp2GTX(x.z),
            fastExp2GTX(x.w));
    }

    // fastLog2GTX, ln2 = 0.69314718055994530941723212145818f
    inline float fastLog2GTX(float x)
    {
        return fastLogGTX(x) / 0.69314718055994530941723212145818f;
    }

    template <typename T> 
    inline _xvec2<T> fastLog2GTX(const _xvec2<T>& x)
    {
        return _xvec2<T>(
            fastLog2GTX(x.x),
            fastLog2GTX(x.y));
    }

    template <typename T> 
    inline _xvec3<T> fastLog2GTX(const _xvec3<T>& x)
    {
        return _xvec3<T>(
            fastLog2GTX(x.x),
            fastLog2GTX(x.y),
            fastLog2GTX(x.z));
    }

    template <typename T> 
    inline _xvec4<T> fastLog2GTX(const _xvec4<T>& x)
    {
        return _xvec4<T>(
            fastLog2GTX(x.x),
            fastLog2GTX(x.y),
            fastLog2GTX(x.z),
            fastLog2GTX(x.w));
    }
}

#endif//included

#endif//__gtx_fast_exponential_inl__
