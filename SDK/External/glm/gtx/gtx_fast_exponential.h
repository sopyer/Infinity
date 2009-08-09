//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-01-09
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_fast_exponential.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_fast_exponential_h__
#define __gtx_fast_exponential_h__

#if (GLM_GTX_fast_exponential == GLM_EXTENSION_DISABLE && defined(GLM_GTX_fast_exponential_required))
#error GLM_GTX_fast_exponential is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_fast_exponential, GLM_EXTENSION_GTX, GLM_GTX_fast_exponential_required))

#define GLM_GTX_fast_exponential_supported 

namespace glm
{
    float fastPowGTX(float x, float y);
    template <typename T> _xvec2<T> fastPowGTX(const _xvec2<T>& x, const _xvec2<T>& y);
    template <typename T> _xvec3<T> fastPowGTX(const _xvec3<T>& x, const _xvec3<T>& y);
    template <typename T> _xvec4<T> fastPowGTX(const _xvec4<T>& x, const _xvec4<T>& y);

    float fastPowGTX(float x, int y);
    template <typename T> _xvec2<T> fastPowGTX(const _xvec2<T>& x, const _xvec2<int>& y);
    template <typename T> _xvec3<T> fastPowGTX(const _xvec3<T>& x, const _xvec3<int>& y);
    template <typename T> _xvec4<T> fastPowGTX(const _xvec4<T>& x, const _xvec4<int>& y);

    float fastExpGTX(float x);
    template <typename T> _xvec2<T> fastExpGTX(const _xvec2<T>& x);
    template <typename T> _xvec3<T> fastExpGTX(const _xvec3<T>& x);
    template <typename T> _xvec4<T> fastExpGTX(const _xvec4<T>& x);

    float fastLogGTX(float x);
    template <typename T> _xvec2<T> fastLogGTX(const _xvec2<T>& x);
    template <typename T> _xvec3<T> fastLogGTX(const _xvec3<T>& x);
    template <typename T> _xvec4<T> fastLogGTX(const _xvec4<T>& x);

    float fastExp2GTX(float x);
    template <typename T> _xvec2<T> fastExp2GTX(const _xvec2<T>& x);
    template <typename T> _xvec3<T> fastExp2GTX(const _xvec3<T>& x);
    template <typename T> _xvec4<T> fastExp2GTX(const _xvec4<T>& x);

    float fastLog2GTX(float x);
    template <typename T> _xvec2<T> fastLog2GTX(const _xvec2<T>& x);
    template <typename T> _xvec3<T> fastLog2GTX(const _xvec3<T>& x);
    template <typename T> _xvec4<T> fastLog2GTX(const _xvec4<T>& x);

    float fastLnGTX(float x);
    template <typename T> _xvec2<T> fastLnGTX(const _xvec2<T>& x);
    template <typename T> _xvec3<T> fastLnGTX(const _xvec3<T>& x);
    template <typename T> _xvec4<T> fastLnGTX(const _xvec4<T>& x);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> T fastPow(const T& x, const T& y){return fastPowGTX(x, y);}
    template <typename T, typename U> T fastPow(const T& x, const U& y){return fastPowGTX(x, y);}
    template <typename T> T fastExp(const T& x){return fastExpGTX(x);}
    template <typename T> T fastLog(const T& x){return fastLogGTX(x);}
    template <typename T> T fastExp2(const T& x){return fastExp2GTX(x);}
    template <typename T> T fastLog2(const T& x){return fastLog2GTX(x);}
    template <typename T> T fastLn(const T& x){return fastLnGTX(x);}

#endif
}

#endif//included

#endif//__gtx_fast_exponential_h__
