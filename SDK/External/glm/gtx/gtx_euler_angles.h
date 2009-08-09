//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_euler_angles.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
// - GLM_GTX_double
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_euler_angles_h__
#define __gtx_euler_angles_h__

#if (GLM_GTX_euler_angles == GLM_EXTENSION_DISABLE && defined(GLM_GTX_euler_angles_required))
#error GLM_GTX_euler_angles is disable but required
#endif

// Dependency:
#include "../glm.h"
#define GLM_GTX_double_required 1
#include "../gtx/gtx_double.h"

#if(GLM_INCLUDED(GLM_GTX_euler_angles, GLM_EXTENSION_GTX, GLM_GTX_euler_angles_required))

#define GLM_GTX_euler_angles_supported 

namespace glm
{
    template <typename T> _xmat2<T> mat2GTX(const T angle); //!< \brief Creates a 2 * 2 rotation matrix from angle (from GLM_GTX_euler_angles extension)
    template <typename T> _xmat3<T> mat3GTX(const T angle); //!< \brief Creates a 3 * 3 rotation matrix from angle (from GLM_GTX_euler_angles extension)
    template <typename T> _xmat3<T> mat3GTX(const _xvec3<T>& angles); //!< \brief Creates a 3 * 3 rotation matrix from angles (from GLM_GTX_euler_angles extension)
    template <typename T> _xmat4<T> mat4GTX(const _xvec4<T>& angles); //!< \brief Creates a 4 * 4 rotation matrix from angles (from GLM_GTX_euler_angles extension)
}

#endif//included

#endif//__gtx_euler_angles_h__
