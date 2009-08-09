//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_optimum_pow.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_optimum_pow_h__
#define __gtx_optimum_pow_h__

#if (GLM_GTX_optimum_pow == GLM_EXTENSION_DISABLE && defined(GLM_GTX_optimum_pow_required))
#error GLM_GTX_optimum_pow is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_optimum_pow, GLM_EXTENSION_GTX, GLM_GTX_optimum_pow_required))

#define GLM_GTX_optimum_pow_supported 

namespace glm
{
    template <typename genType> genType pow2GTX(const genType& x); //!< \brief Returns x raised to the power of 2. (From GLM_GTX_optimum_pow extension)
    template <typename genType> genType pow3GTX(const genType& x); //!< \brief Returns x raised to the power of 3. (From GLM_GTX_optimum_pow extension)
    template <typename genType> genType pow4GTX(const genType& x); //!< \brief Returns x raised to the power of 4. (From GLM_GTX_optimum_pow extension)
    
    inline bool powOfTwoGTX(int num); //!< \brief Checks if the parameter is a power of 2 number. (From GLM_GTX_optimum_pow extension)
    inline _bvec2 powOfTwoGTX(const _xvec2<int>& x); //!< \brief Checks to determine if the parameter component are power of 2 numbers. (From GLM_GTX_optimum_pow extension)
    inline _bvec3 powOfTwoGTX(const _xvec3<int>& x); //!< \brief Checks to determine if the parameter component are power of 2 numbers. (From GLM_GTX_optimum_pow extension)
    inline _bvec4 powOfTwoGTX(const _xvec4<int>& x); //!< \brief Checks to determine if the parameter component are power of 2 numbers. (From GLM_GTX_optimum_pow extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename genType> genType pow2(const genType& x){return pow2GTX(x);}
    template <typename genType> genType pow3(const genType& x){return pow3GTX(x);}
    template <typename genType> genType pow4(const genType& x){return pow4GTX(x);}
    
    inline bool powOfTwo(int num){return powOfTwoGTX(num);}
    inline _bvec2 powOfTwo(const _xvec2<int>& x){return powOfTwoGTX(x);}
    inline _bvec3 powOfTwo(const _xvec3<int>& x){return powOfTwoGTX(x);}
    inline _bvec4 powOfTwo(const _xvec4<int>& x){return powOfTwoGTX(x);}

#endif
}

#endif//included

#endif//__gtx_optimum_pow_h__
