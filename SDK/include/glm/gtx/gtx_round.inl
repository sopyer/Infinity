//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_round.inl
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GL_GTX_double
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_round_inl__
#define __gtx_round_inl__

#include "gtx_round.h"
#if(GLM_INCLUDED(GLM_GTX_round, GLM_EXTENSION_GTX, GLM_GTX_round_required))

namespace glm
{
    template <typename genType>
    inline genType roundGTX(const genType& x)
    {
        genType result;
        int i = x._size();
        while(i--)
            result[i] = float(int(x[i] + 0.5f));
        return result;
    }

    inline float roundGTX(float x)
    {
        return float(int(x + 0.5f));
    }

    inline double roundGTX(double x)
    {
        return double(int(x + 0.5));
    }
}

#endif//included

#endif//__gtx_round_inl__
