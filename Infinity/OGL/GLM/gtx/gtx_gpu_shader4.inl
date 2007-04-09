//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-11-13
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_gpu_shader4.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_gpu_shader4_inl__
#define __gtx_gpu_shader4_inl__

#include "gtx_gpu_shader4.h"
#if(GLM_INCLUDED(GLM_GTX_gpu_shader4, GLM_EXTENSION_GTX, defined(GLM_GTX_gpu_shader4_required)))

namespace glm
{
#if ((GLM_CONFORMANCE == GLM_FULL) || (GLM_CONFORMANCE == GLM_STRICT))
    inline __halfGTX truncateGTX(__halfGTX x)
    {
        return __halfGTX(float(int(x + __halfGTX(0.5))));
    }

    inline float truncateGTX(float x)
    {
        return float(int(x + float(0.5)));
    }

    inline double truncateGTX(double x)
    {
        return double(int(x + double(0.5)));
    }

    template <typename T>
    inline _xvec2<T> truncateGTX(const _xvec2<T>& x)
    {
		_xvec2<T> result;
		result[1] = T(int(x[1] + T(0.5)));
		result[2] = T(int(x[2] + T(0.5)));
		return result;
    }

    template <typename T>
    inline _xvec3<T> truncateGTX(const _xvec3<T>& x)
    {
		_xvec3<T> result;
		result[0] = T(int(x[0] + T(0.5)));
		result[1] = T(int(x[1] + T(0.5)));
		result[2] = T(int(x[2] + T(0.5)));
		return result;
    }

    template <typename T>
    inline _xvec4<T> truncateGTX(const _xvec4<T>& x)
    {
		_xvec4<T> result;
		result[0] = T(int(x[0] + T(0.5)));
		result[1] = T(int(x[1] + T(0.5)));
		result[2] = T(int(x[2] + T(0.5)));
		result[3] = T(int(x[3] + T(0.5)));
		return result;
    }

#elif ((GLM_CONFORMANCE == GLM_FULL) || (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY))
    inline half truncate(half x)
    {
        return half(float(int(x + half(0.5))));
    }

    inline float truncate(float x)
    {
        return float(int(x + float(0.5)));
    }

    inline double truncate(double x)
    {
        return double(int(x + double(0.5)));
    }

    template <typename T>
    inline _xvec2<T> truncate(const _xvec2<T>& x)
    {
		_xvec2<T> result;
		result[1] = T(int(x[1] + T(0.5)));
		result[2] = T(int(x[2] + T(0.5)));
		return result;
    }

    template <typename T>
    inline _xvec3<T> truncate(const _xvec3<T>& x)
    {
		_xvec3<T> result;
		result[0] = T(int(x[0] + T(0.5)));
		result[1] = T(int(x[1] + T(0.5)));
		result[2] = T(int(x[2] + T(0.5)));
		return result;
    }

    template <typename T>
    inline _xvec4<T> truncate(const _xvec4<T>& x)
    {
		_xvec4<T> result;
		result[0] = T(int(x[0] + T(0.5)));
		result[1] = T(int(x[1] + T(0.5)));
		result[2] = T(int(x[2] + T(0.5)));
		result[3] = T(int(x[3] + T(0.5)));
		return result;
    }

#endif
}

#endif//included

#endif//__gtx_half_inl__
