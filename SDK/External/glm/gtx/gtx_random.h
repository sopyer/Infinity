//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-01-16
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_random.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_random_h__
#define __gtx_random_h__

#if (GLM_GTX_random == GLM_EXTENSION_DISABLE && defined(GLM_GTX_random_required))
#error GLM_GTX_random is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_random, GLM_EXTENSION_GTX, GLM_GTX_random_required))

#define GLM_GTX_random_supported 

namespace glm
{
    float rand1GTX(); //!< \brief Returns a random value will range from [0.0, 1.0](from GLM_GTX_random extension)
    float rand1GTX(float min, float max); //!< \brief Returns a random value will range from [min, max](from GLM_GTX_random extension)
    
    _xvec2<float> rand2GTX(); //!< \brief Returns a random normalized vector of 2 components (from GLM_GTX_random extension)
    _xvec2<float> rand2GTX(float min, float max); //!< \brief Returns a random vector of 2 components witch length will range from [min, max] (from GLM_GTX_random extension)
    
    _xvec3<float> rand3GTX(); //!< \brief Returns a random normalized vector of 3 components (from GLM_GTX_random extension)
    _xvec3<float> rand3GTX(float min, float max); //!< \brief Returns a random vector of 3 components witch length will range from [min, max] (from GLM_GTX_random extension)

    _xvec4<float> rand4GTX(); //!< \brief Returns a random normalized vector of 4 components (from GLM_GTX_random extension)
    _xvec4<float> rand4GTX(float min, float max); //!< \brief Returns a random vector of 4 components witch length will range from [min, max] (from GLM_GTX_random extension)

    template <typename T> T gauss1GTX(T mean, T std_deviation);
    template <typename T> _xvec2<T> gauss2GTX(T mean, T std_deviation);
    template <typename T> _xvec3<T> gauss3GTX(T mean, T std_deviation);
    template <typename T> _xvec4<T> gauss4GTX(T mean, T std_deviation);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    inline float rand1(){return rand1GTX();}
    inline float rand1(float min, float max){return rand1GTX(min, max);}
    
    inline _xvec2<float> rand2(){return rand2GTX();}
    inline _xvec2<float> rand2(float min, float max){return rand2GTX(min, max);}
    
    inline _xvec3<float> rand3(){return rand3GTX();}
    inline _xvec3<float> rand3(float min, float max){return rand3GTX(min, max);}

    inline _xvec4<float> rand4(){return rand4GTX();}
    inline _xvec4<float> rand4(float min, float max){return rand4GTX(min, max);}

    template <typename T> T gauss1(T mean, T std_deviation){return gauss1GTX(mean, std_deviation);}
    template <typename T> _xvec2<T> gauss2(T mean, T std_deviation){return gauss2GTX(mean, std_deviation);}
    template <typename T> _xvec3<T> gauss3(T mean, T std_deviation){return gauss3GTX(mean, std_deviation);}
    template <typename T> _xvec4<T> gauss4(T mean, T std_deviation){return gauss4GTX(mean, std_deviation);}

#endif
}

#endif//included

#endif//__gtx_random_h__
