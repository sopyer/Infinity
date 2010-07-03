//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-01-08
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_fast_trigonometry.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_fast_trigonometry_h__
#define __gtx_fast_trigonometry_h__

#if (GLM_GTX_fast_trigonometry == GLM_EXTENSION_DISABLE && defined(GLM_GTX_fast_trigonometry_required))
#error GLM_GTX_fast_trigonometry is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_fast_trigonometry, GLM_EXTENSION_GTX, GLM_GTX_fast_trigonometry_required))

#define GLM_GTX_fast_trigonometry_supported 

namespace glm
{
    template <typename T> T fastSinGTX(T angle);
    template <typename T> _xvec2<T> fastSinGTX(const _xvec2<T>& angle);
    template <typename T> _xvec3<T> fastSinGTX(const _xvec3<T>& angle);
    template <typename T> _xvec4<T> fastSinGTX(const _xvec4<T>& angle);

    template <typename T> T fastCosGTX(T angle);
    template <typename T> _xvec2<T> fastCosGTX(const _xvec2<T>& angle);
    template <typename T> _xvec3<T> fastCosGTX(const _xvec3<T>& angle);
    template <typename T> _xvec4<T> fastCosGTX(const _xvec4<T>& angle);

    template <typename T> T fastTanGTX(T angle);
    template <typename T> _xvec2<T> fastTanGTX(const _xvec2<T>& angle);
    template <typename T> _xvec3<T> fastTanGTX(const _xvec3<T>& angle);
    template <typename T> _xvec4<T> fastTanGTX(const _xvec4<T>& angle);

    float fastAsinGTX(float x);
    template <typename T> _xvec2<T> fastAsinGTX(const _xvec2<T>& x);
    template <typename T> _xvec3<T> fastAsinGTX(const _xvec3<T>& x);
    template <typename T> _xvec4<T> fastAsinGTX(const _xvec4<T>& x);

    float fastAcosGTX(float x);
    template <typename T> _xvec2<T> fastAcosGTX(const _xvec2<T>& x);
    template <typename T> _xvec3<T> fastAcosGTX(const _xvec3<T>& x);
    template <typename T> _xvec4<T> fastAcosGTX(const _xvec4<T>& x);

    float fastAtanGTX(float y, float x);
    template <typename T> _xvec2<T> fastAtanGTX(const _xvec2<T>& y, const _xvec2<T>& x);
    template <typename T> _xvec3<T> fastAtanGTX(const _xvec3<T>& y, const _xvec3<T>& x);
    template <typename T> _xvec4<T> fastAtanGTX(const _xvec4<T>& y, const _xvec4<T>& x);

    float fastAtanGTX(float y_over_x);
    template <typename T> _xvec2<T> fastAtanGTX(const _xvec2<T>& y_over_x);
    template <typename T> _xvec3<T> fastAtanGTX(const _xvec3<T>& y_over_x);
    template <typename T> _xvec4<T> fastAtanGTX(const _xvec4<T>& y_over_x);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> T fastSin(const T& angle){return fastSinGTX(angle);}
    template <typename T> T fastCos(const T& angle){return fastCosGTX(angle);}
    template <typename T> T fastTan(const T& angle){return fastTanGTX(angle);}
    template <typename T> T fastAsin(const T& angle){return fastAsinGTX(angle);}
    template <typename T> T fastAcos(const T& angle){return fastAcosGTX(angle);}
    template <typename T> T fastAtan(const T& y, const T& x){return fastAtanGTX(y, x);}
    template <typename T> T fastAtan(const T& angle){return fastAtanGTX(angle);}

#endif
}

#endif//included

#endif//__gtx_fast_trigonometry_h__
