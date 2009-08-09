//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-01-16
// Updated : 2006-01-16
// Licence : This source is under GNU LGPL licence
// File    : gtx_vector_access.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_vector_access_inl__
#define __gtx_vector_access_inl__

#include "gtx_vector_access.h"
#if(GLM_INCLUDED(GLM_GTX_vector_access, GLM_EXTENSION_GTX, GLM_GTX_vector_access_required))

namespace glm
{
/*
    inline void setGTX(float& v, float x)
    {
        v = x;
    }

    inline void setGTX(int& v, int x)
    {
        v = x;
    }

    inline void setGTX(bool& v, bool x)
    {
        v = x;
    }

    inline void setGTX(double& v, double x)
    {
        v = x;
    }

    inline void setGTX(__halfGTX& v, __halfGTX x)
    {
        v = x;
    }
*/
    template <typename T> 
    inline void setGTX(_xvec2<T>& v, T x, T y)
    {
        v.x = x;
        v.y = y;
    }

    template <typename T> 
    inline void setGTX(_xvec3<T>& v, T x, T y, T z)
    {
        v.x = x;
        v.y = y;
        v.z = z;
    }

    template <typename T>
    inline void setGTX(_xvec4<T>& v, T x, T y, T z, T w)
    {
        v.x = x;
        v.y = y;
        v.z = z;
        v.w = w;
    }

    inline void setGTX(_bvec2& v, bool x, bool y)
    {
        v.x = x;
        v.y = y;
    }

    inline void setGTX(_bvec3& v, bool x, bool y, bool z)
    {
        v.x = x;
        v.y = y;
        v.z = z;
    }

    inline void setGTX(_bvec4& v, bool x, bool y, bool z, bool w)
    {
        v.x = x;
        v.y = y;
        v.z = z;
        v.w = w;
    }
}

#endif//included

#endif//__gtx_vector_access_inl__
