//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-24
// Updated : 2006-11-14
// Licence : This source is under GNU LGPL licence
// File    : gtx_integer.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_integer_h__
#define __gtx_integer_h__

#if (GLM_GTX_integer == GLM_EXTENSION_DISABLE && defined(GLM_GTX_integer_required))
#error GLM_GTX_integer is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_integer, GLM_EXTENSION_GTX, GLM_GTX_integer_required))

#define GLM_GTX_integer_supported 

namespace glm
{
    int absGTX(int x);
    template <typename T> T abs(T x);
    template <typename T> _xvec2<T> absGTX(const _xvec2<T>& x);
    template <typename T> _xvec3<T> absGTX(const _xvec3<T>& x);
    template <typename T> _xvec4<T> absGTX(const _xvec4<T>& x);
    
    template <typename genType> genType signGTX(const genType& x);
	
    template <typename T> T minGTX(T x, T y);
    template <typename T> _xvec2<T> minGTX(const _xvec2<T>& x, T y);
    template <typename T> _xvec3<T> minGTX(const _xvec3<T>& x, T y);
    template <typename T> _xvec4<T> minGTX(const _xvec4<T>& x, T y);
    template <typename T> _xvec2<T> minGTX(const _xvec2<T>& x, const _xvec2<T>& y);
    template <typename T> _xvec3<T> minGTX(const _xvec3<T>& x, const _xvec3<T>& y);
    template <typename T> _xvec4<T> minGTX(const _xvec4<T>& x, const _xvec4<T>& y);

    template <typename T> T maxGTX(T x, T y);
    template <typename T> _xvec2<T> maxGTX(const _xvec2<T>& x, T y);
    template <typename T> _xvec3<T> maxGTX(const _xvec3<T>& x, T y);
    template <typename T> _xvec4<T> maxGTX(const _xvec4<T>& x, T y);
    template <typename T> _xvec2<T> maxGTX(const _xvec2<T>& x, const _xvec2<T>& y);
    template <typename T> _xvec3<T> maxGTX(const _xvec3<T>& x, const _xvec3<T>& y);
    template <typename T> _xvec4<T> maxGTX(const _xvec4<T>& x, const _xvec4<T>& y);

    template <typename T> T clampGTX(T x, T minVal, T maxVal);
    template <typename T> _xvec2<T> clampGTX(const _xvec2<T>& x, T minVal, T maxVal);
    template <typename T> _xvec3<T> clampGTX(const _xvec3<T>& x, T minVal, T maxVal);
    template <typename T> _xvec4<T> clampGTX(const _xvec4<T>& x, T minVal, T maxVal);
    template <typename T> _xvec2<T> clampGTX(const _xvec2<T>& x, const _xvec2<T>& minVal, const _xvec2<T>& maxVal);
    template <typename T> _xvec3<T> clampGTX(const _xvec3<T>& x, const _xvec3<T>& minVal, const _xvec3<T>& maxVal);
    template <typename T> _xvec4<T> clampGTX(const _xvec4<T>& x, const _xvec4<T>& minVal, const _xvec4<T>& maxVal);

    int powGTX(int x, int y);
    template <typename T> T powGTX(T x, T y);
    template <typename T> _xvec2<T> powGTX(const _xvec2<T>& x, const _xvec2<T>& y);
    template <typename T> _xvec3<T> powGTX(const _xvec3<T>& x, const _xvec3<T>& y);
    template <typename T> _xvec4<T> powGTX(const _xvec4<T>& x, const _xvec4<T>& y);

    int sqrtGTX(int x);
    template <typename T> T sqrtGTX(T x);
    template <typename T> _xvec2<T> sqrtGTX(const _xvec2<T>& x);
    template <typename T> _xvec3<T> sqrtGTX(const _xvec3<T>& x);
    template <typename T> _xvec4<T> sqrtGTX(const _xvec4<T>& x);

    template <typename T> T modGTX(T x, T y);
    template <typename T> _xvec2<T> modGTX(const _xvec2<T>& x, T y);
    template <typename T> _xvec3<T> modGTX(const _xvec3<T>& x, T y);
    template <typename T> _xvec4<T> modGTX(const _xvec4<T>& x, T y);
    template <typename T> _xvec2<T> modGTX(const _xvec2<T>& x, const _xvec2<T>& y);
    template <typename T> _xvec3<T> modGTX(const _xvec3<T>& x, const _xvec3<T>& y);
    template <typename T> _xvec4<T> modGTX(const _xvec4<T>& x, const _xvec4<T>& y);

    template <typename T> T factorialGTX(T x);
    template <typename T> _xvec2<T> factorialGTX(const _xvec2<T>& x);
    template <typename T> _xvec3<T> factorialGTX(const _xvec3<T>& x);
    template <typename T> _xvec4<T> factorialGTX(const _xvec4<T>& x);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    int pow(int x, int y);
    int sqrt(int x);
    int mod(int x, int y);
    template <typename T> T factorial(T x);
    template <typename T> _xvec2<T> factorial(const _xvec2<T>& x);
    template <typename T> _xvec3<T> factorial(const _xvec3<T>& x);
    template <typename T> _xvec4<T> factorial(const _xvec4<T>& x);

#endif	
}

#endif//included

#endif//__gtx_integer_h__
