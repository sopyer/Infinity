//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-01-16
// Updated : 2006-01-16
// Licence : This source is under GNU LGPL licence
// File    : gtx_random.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_random_inl__
#define __gtx_random_inl__

#include "gtx_random.h"
#include <ctime>
#if(GLM_INCLUDED(GLM_GTX_random, GLM_EXTENSION_GTX, GLM_GTX_random_required))

namespace glm
{
    inline float rand1GTX()
    {
        return 2.0f * float(std::rand() - (RAND_MAX >> 1)) / float(RAND_MAX);
    }

    inline float rand1GTX(float Min, float Max)
    {
        return (glm::rand1GTX() * (Max - Min)) + Min;
    }
    
    inline _xvec2<float> rand2GTX()
    {
        float a = glm::rand1GTX(0.0f, 6.283185307179586476925286766559f);
        return _xvec2<float>(glm::cos(a), glm::sin(a));
    }

    inline _xvec2<float> rand2GTX(float Min, float Max)
    {
        return glm::rand2GTX() * rand1GTX(Min, Max);
    }

    inline _xvec3<float> rand3GTX()
    {
	    float z = glm::rand1GTX(-1.0f, 1.0f);
	    float a = glm::rand1GTX(0.0f, 3.1415926535897932384626433832795f);

        float r = glm::sqrt(1.0f - z * z);

	    float x = r * glm::cos(a);
	    float y = r * glm::sin(a);

        return _xvec3<float>(x, y, z);
    }

    inline _xvec3<float> rand3GTX(float Min, float Max)
    {
        return glm::rand3GTX() * rand1GTX(Min, Max);
    }

    inline _xvec4<float> rand4GTX()
    {
        return _xvec4<float>(rand3GTX(), 0.0f);
    }

    inline _xvec4<float> rand4GTX(float Min, float Max)
    {
        return rand4GTX() * rand1GTX(Min, Max);
    }

    template <typename T>
    inline T gauss1GTX(T mean, T std_deviation)
    {
        T w, x1, x2;

        do 
        {
            x1 = rand1GTX(T(-1), T(1));
            x2 = rand1GTX(T(-1), T(1));

            w = x1 * x1 + x2 * x2;
        } while(w > T(1));

        return x2 * std_deviation * std_deviation * sqrt((T(-2) * log(w)) / w) + mean; 
    }

    template <typename T> 
    inline _xvec2<T> gauss2GTX(T mean, T std_deviation)
    {
        return _xvec2<T>(
            gauss1GTX(mean, std_deviation),
            gauss1GTX(mean, std_deviation));
    }

    template <typename T> 
    inline _xvec3<T> gauss3GTX(T mean, T std_deviation)
    {
        return _xvec3<T>(
            gauss1GTX(mean, std_deviation),
            gauss1GTX(mean, std_deviation),
            gauss1GTX(mean, std_deviation));
    }

    template <typename T> 
    inline _xvec4<T> gauss4GTX(T mean, T std_deviation)
    {
        return _xvec4<T>(
            gauss1GTX(mean, std_deviation),
            gauss1GTX(mean, std_deviation),
            gauss1GTX(mean, std_deviation),
            gauss1GTX(mean, std_deviation));
    }
}

#endif//included

#endif//__gtx_random_inl__
