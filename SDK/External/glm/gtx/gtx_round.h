//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_round.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
// - GL_GTX_double
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_round_h__
#define __gtx_round_h__

#if (GLM_GTX_round == GLM_EXTENSION_DISABLE && defined(GLM_GTX_round_required))
#error GLM_GTX_round is disable but required
#endif

// Dependency:
#include "../glm.h"
#define GLM_GTX_double_required 1
#include "../gtx/gtx_double.h"

#if(GLM_INCLUDED(GLM_GTX_round, GLM_EXTENSION_GTX, GLM_GTX_round_required))

#define GLM_GTX_round_supported 

namespace glm
{
    template <typename genType> genType roundGTX(const genType & x); //!< \brief Computes the component-wise round value of x (from GLM_GTX_round extension)
    float roundGTX(float x); //!< \brief Computes the round value of x (from GLM_GTX_round extension)
    double roundGTX(double x); //!< \brief Computes the round value of x (from GLM_GTX_round extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename genType> genType round(const genType & x){return roundGTX(x);}
    inline float round(float x){return roundGTX(x);}
    inline double round(double x){return roundGTX(x);}

#endif
}

#endif//included

#endif//__gtx_round_h__
