//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-27
// Licence : This source is under GNU LGPL licence
// File    : gtx_optimum_pow.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_optimum_pow_inl__
#define __gtx_optimum_pow_inl__

#include "gtx_optimum_pow.h"
#if(GLM_INCLUDED(GLM_GTX_optimum_pow, GLM_EXTENSION_GTX, GLM_GTX_optimum_pow_required))

namespace glm
{
    template <typename genType> 
    inline genType pow2GTX(const genType& x)
    {
        return x * x;
    }

    template <typename genType> 
    inline genType pow3GTX(const genType& x)
    {
        return x * x * x;
    }

    template <typename genType> 
    inline genType pow4GTX(const genType& x)
    {
        return x * x * x * x;
    }

    inline bool powOfTwoGTX(int x)
    {
        return !(x & (x - 1));
    }

    inline _bvec2 powOfTwoGTX(const _xvec2<int>& x)
    {
        return _bvec2(
            powOfTwoGTX(x.x),
            powOfTwoGTX(x.y));
    }

    inline _bvec3 powOfTwoGTX(const _xvec3<int>& x)
    {
        return _bvec3(
            powOfTwoGTX(x.x),
            powOfTwoGTX(x.y),
            powOfTwoGTX(x.z));
    }

    inline _bvec4 powOfTwoGTX(const _xvec4<int>& x)
    {
        return _bvec4(
            powOfTwoGTX(x.x),
            powOfTwoGTX(x.y),
            powOfTwoGTX(x.z),
            powOfTwoGTX(x.w));
    }
}

#endif//included

#endif//__gtx_optimum_pow_inl__
