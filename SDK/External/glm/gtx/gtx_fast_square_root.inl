//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-01-04
// Updated : 2006-12-06
// Licence : This source is under GNU LGPL licence
// File    : gtx_fast_square_root.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_fast_square_root_inl__
#define __gtx_fast_square_root_inl__

#include "gtx_fast_square_root.h"
#if(GLM_INCLUDED(GLM_GTX_fast_square_root, GLM_EXTENSION_GTX, GLM_GTX_fast_square_root_required))

namespace glm
{
    // fastSqrtGTX
    inline float fastSqrtGTX(float x)
    {
        return 1.0f / fastInverseSqrtGTX(x);
    }

    inline vec2 fastSqrtGTX(const vec2& x)
    {
        return vec2(
            fastSqrtGTX(x.x), 
            fastSqrtGTX(x.y));
    }

    inline vec3 fastSqrtGTX(const vec3& x)
    {
        return vec3(
            fastSqrtGTX(x.x), 
            fastSqrtGTX(x.y), 
            fastSqrtGTX(x.z));
    }

    inline vec4 fastSqrtGTX(const vec4& x)
    {
        return vec4(
            fastSqrtGTX(x.x), 
            fastSqrtGTX(x.y), 
            fastSqrtGTX(x.z), 
            fastSqrtGTX(x.w));
    }

    // fastInversesqrtGTX
    inline float fastInverseSqrtGTX(float x)
    {
        float xhalf = 0.5f * x;
        int i = *(int*)&x;
        i = 0x5f375a86 - (i >> 1);
        x = *(float*)&i;
        x = x * (1.5f - xhalf * x * x);
        return x;
    }

    inline vec2 fastInverseSqrtGTX(const vec2& x)
    {
        return vec2(
            fastInverseSqrtGTX(x.x), 
            fastInverseSqrtGTX(x.y));
    }

    inline vec3 fastInverseSqrtGTX(const vec3& x)
    {
        return vec3(
            fastInverseSqrtGTX(x.x), 
            fastInverseSqrtGTX(x.y), 
            fastInverseSqrtGTX(x.z));
    }

    inline vec4 fastInverseSqrtGTX(const vec4& x)
    {
        return vec4(
            fastInverseSqrtGTX(x.x), 
            fastInverseSqrtGTX(x.y), 
            fastInverseSqrtGTX(x.z), 
            fastInverseSqrtGTX(x.w));
    }

    // fastLengthGTX
    inline float fastLengthGTX(float x)
    {
        return glm::abs(x);
    }

    inline float fastLengthGTX(const vec2& x)
    {
        float sqr = x.x * x.x + x.y * x.y;
        return glm::sqrt(sqr);
    }

    inline float fastLengthGTX(const vec3& x)
    {
        float sqr = x.x * x.x + x.y * x.y + x.z * x.z;
        return glm::sqrt(sqr);
    }

    inline float fastLengthGTX(const vec4& x)
    {
        float sqr = x.x * x.x + x.y * x.y + x.z * x.z + x.w * x.w;
        return glm::sqrt(sqr);
    }

    // fastDistanceGTX
    inline float fastDistanceGTX(float x, float y)
    {
        return glm::fastLengthGTX(y - x);
    }

    inline float fastDistanceGTX(const vec2& x, const vec2& y)
    {
        return glm::fastLengthGTX(y - x);
    }

    inline float fastDistanceGTX(const vec3& x, const vec3& y)
    {
        return glm::fastLengthGTX(y - x);
    }

    inline float fastDistanceGTX(const vec4& x, const vec4& y)
    {
        return glm::fastLengthGTX(y - x);
    }

    // fastNormalizeGTX
    inline float fastNormalizeGTX(float x)
    {
        return x > 0.0f ? 1.0f : -1.0f;
    }

    inline vec2 fastNormalizeGTX(const vec2& x)
    {
        float sqr = x.x * x.x + x.y * x.y;
	    return x * glm::fastInverseSqrtGTX(sqr);
    }

    inline vec3 fastNormalizeGTX(const vec3& x)
    {
        float sqr = x.x * x.x + x.y * x.y + x.z * x.z;
	    return x * glm::fastInverseSqrtGTX(sqr);
    }

    inline vec4 fastNormalizeGTX(const vec4& x)
    {
        float sqr = x.x * x.x + x.y * x.y + x.z * x.z + x.w * x.w;
	    return x * glm::fastInverseSqrtGTX(sqr);
    }
}

#endif//included

#endif//__gtx_fast_square_root_inl__
