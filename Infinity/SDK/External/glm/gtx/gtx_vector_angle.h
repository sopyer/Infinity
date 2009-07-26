//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-30
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_vector_angle.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
// - GL_GTX_vector_angles
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_vector_angle_h__
#define __gtx_vector_angle_h__

#if (GLM_GTX_vector_angle == GLM_EXTENSION_DISABLE && defined(GLM_GTX_vector_angle_required))
#error GLM_GTX_vector_angle is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_vector_angle, GLM_EXTENSION_GTX, GLM_GTX_vector_angle_required))

#define GLM_GTX_vector_angle_supported 

namespace glm
{
    template <typename T> T angleGTX(T x, T y); //!< Returns the absolute angle between x and y (From GLM_GTX_vector_angle extension)
    template <typename T> T angleGTX(const _xvec2<T>& x, const _xvec2<T>& y); //!< Returns the absolute angle between x and y (From GLM_GTX_vector_angle extension)
    template <typename T> T angleGTX(const _xvec3<T>& x, const _xvec3<T>& y); //!< Returns the absolute angle between x and y (From GLM_GTX_vector_angle extension)
    template <typename T> T angleGTX(const _xvec4<T>& x, const _xvec4<T>& y); //!< Returns the absolute angle between x and y (From GLM_GTX_vector_angle extension)
    template <typename T> T orientedAngleGTX(const _xvec2<T>& x, const _xvec2<T>& y); //!< Returns the oriented angle between x and y (From GLM_GTX_vector_angle extension)
    template <typename T> T orientedAngleGTX(const _xvec3<T>& x, const _xvec3<T>& y); //!< Returns the oriented angle between x and y (From GLM_GTX_vector_angle extension)
    template <typename T> T orientedAngleGTX(const _xvec4<T>& x, const _xvec4<T>& y); //!< Returns the oriented angle between x and y (From GLM_GTX_vector_angle extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> T angle(T x, T y){return angleGTX(x, y);}
    template <typename T> T angle(const _xvec2<T>& x, const _xvec2<T>& y){return angleGTX(x, y);}
    template <typename T> T angle(const _xvec3<T>& x, const _xvec3<T>& y){return angleGTX(x, y);}
    template <typename T> T angle(const _xvec4<T>& x, const _xvec4<T>& y){return angleGTX(x, y);}
    template <typename T> T orientedAngle(const _xvec2<T>& x, const _xvec2<T>& y){return orientedAngleGTX(x, y);}
    template <typename T> T orientedAngle(const _xvec3<T>& x, const _xvec3<T>& y){return orientedAngleGTX(x, y);}
    template <typename T> T orientedAngle(const _xvec4<T>& x, const _xvec4<T>& y){return orientedAngleGTX(x, y);}

#endif
}

#endif//included

#endif//__gtx_vector_angle_h__
