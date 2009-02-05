//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-30
// Updated : 2005-12-30
// Licence : This source is under GNU LGPL licence
// File    : gtx_vector_angle.inl
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GL_GTX_vector_angle
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_vector_angle_inl__
#define __gtx_vector_angle_inl__

#include "gtx_vector_angle.h"
#include "gtx_quaternion.h"
#include "gtx_epsilon.h"
#if(GLM_INCLUDED(GLM_GTX_vector_angle, GLM_EXTENSION_GTX, GLM_GTX_vector_angle_required))

namespace glm
{
    template <typename T> 
    inline T angleGTX(T x, T y)
    {
        return degrees(glm::acos(glm::dot(x, y)));
    }

    template <typename T> 
    inline T angleGTX(const _xvec2<T>& x, const _xvec2<T>& y)
    {
        return degrees(glm::acos(glm::dot(x, y)));
    }

    template <typename T> 
    inline T angleGTX(const _xvec3<T>& x, const _xvec3<T>& y)
    {
        return degrees(acos(dot(x, y)));
    }

    template <typename T> 
    inline T angleGTX(const _xvec4<T>& x, const _xvec4<T>& y)
    {
        return degrees(acos(glm::dot(x, y)));
    }

    template <typename T> 
    inline T orientedAngleGTX(const _xvec2<T>& x, const _xvec2<T>& y)
    {
        float Angle = glm::acos(glm::dot(x, y));
        float c = glm::cos(Angle);
        float s = glm::sin(Angle);
        _xvec2<T> TransformedVector = _xvec2<T>(c * y.x - s * y.y, s * y.x + c * y.y);
        if(glm::all(glm::equalEpsilonGTX(x, TransformedVector, 0.01f)))
            return -glm::degrees(Angle);
        else
            return glm::degrees(Angle);
    }

    template <typename T> 
    inline T orientedAngleGTX(const _xvec3<T>& x, const _xvec3<T>& y)
    {
        float Angle = glm::degrees(glm::acos(glm::dot(x, y)));
        _xvec3<T> TransformedVector = glm::rotateGTX(__quatGTX(), Angle, glm::cross(x, y)) * y;
        if(glm::all(glm::equalEpsilonGTX(x, TransformedVector, 0.01f)))
            return -glm::degrees(Angle);
        else
            return glm::degrees(Angle);
    }

    template <typename T>
    inline T orientedAngleGTX(const _xvec4<T>& x, const _xvec4<T>& y)
    {
        float Angle = glm::degrees(glm::acos(glm::dot(x, y)));
        _xvec4<T> TransformedVector = glm::rotateGTX(__quatGTX(), Angle, glm::cross(x, y)) * y;
        if(glm::all(glm::equalEpsilonGTX(x, TransformedVector, 0.01f)))
            return -glm::degrees(Angle);
        else
            return glm::degrees(Angle);
    }
}

#endif//included

#endif//__gtx_vector_angle_inl__
