//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-01-04
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_fast_square_root.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////
// Note:
// - Sqrt optimisation based on Newton's method, 
// www.gamedev.net/community/forums/topic.asp?topic id=139956
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_fast_square_root_h__
#define __gtx_fast_square_root_h__

#if (GLM_GTX_fast_square_root == GLM_EXTENSION_DISABLE && defined(GLM_GTX_fast_square_root_required))
#error GLM_GTX_fast_square_root is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_fast_square_root, GLM_EXTENSION_GTX, GLM_GTX_fast_square_root_required))

#define GLM_GTX_fast_square_root_supported 

namespace glm
{
    float fastSqrtGTX(float x);
    vec2 fastSqrtGTX(const vec2& x);
    vec3 fastSqrtGTX(const vec3& x);
    vec4 fastSqrtGTX(const vec4& x);

    float fastInverseSqrtGTX(float x);
    vec2 fastInverseSqrtGTX(const vec2& x);
    vec3 fastInverseSqrtGTX(const vec3& x);
    vec4 fastInverseSqrtGTX(const vec4& x);

    float fastLengthGTX(float x);
    float fastLengthGTX(const vec2& x);
    float fastLengthGTX(const vec3& x);
    float fastLengthGTX(const vec4& x);

    float fastDistanceGTX(float x, float y);
    float fastDistanceGTX(const vec2& x, const vec2& y);
    float fastDistanceGTX(const vec3& x, const vec3& y);
    float fastDistanceGTX(const vec4& x, const vec4& y);

    float fastNormalizeGTX(float x);
    vec2 fastNormalizeGTX(const vec2& x);
    vec3 fastNormalizeGTX(const vec3& x);
    vec4 fastNormalizeGTX(const vec4& x);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> T fastSqrt(const T& x){return fastSqrtGTX(x);}
    template <typename T> T fastInverseSqrt(const T& x){return fastInverseSqrtGTX(x);}
    template <typename T> T fastLength(const T& x){return fastLengthGTX(x);}
    template <typename T> T fastDistance(const T& x, const T& y){return fastDistanceGTX(x, y);}
    template <typename T> T fastNormalize(const T& x){return fastNormalizeGTX(x);}

#endif
}

#endif//included

#endif//__gtx_fast_square_root_h__
