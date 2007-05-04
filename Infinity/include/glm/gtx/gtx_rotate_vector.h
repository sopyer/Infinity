//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-11-02
// Updated : 2006-12-15
// Licence : This source is under GNU LGPL licence
// File    : gtx_rotate_vector.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_rotate_vector_h__
#define __gtx_rotate_vector_h__

#if (GLM_GTX_rotate_vector == GLM_EXTENSION_DISABLE && defined(GLM_GTX_rotate_vector_required))
#error GLM_GTX_rotate_vector is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_rotate_vector, GLM_EXTENSION_GTX, GLM_GTX_rotate_vector_required))

#define GLM_GTX_rotate_vector_supported 

namespace glm
{
    template <typename T> _xvec2<T> rotateGTX(const _xvec2<T>& v, T angle); //!< /brief Rotate a two dimentionnals vector (from GLM_GTX_rotate_vector extension)
    template <typename T> _xvec3<T> rotateGTX(const _xvec3<T>& v, T angle, const _xvec3<T>& normal); //!< /brief Rotate a three dimentionnals vector around an axis (from GLM_GTX_rotate_vector extension)
    template <typename T> _xvec4<T> rotateGTX(const _xvec4<T>& v, T angle, const _xvec3<T>& normal); //!< /brief Rotate a four dimentionnals vector around an axis (from GLM_GTX_rotate_vector extension)
    template <typename T> _xvec3<T> rotateXGTX(const _xvec3<T>& v, T angle); //!< /brief Rotate a three dimentionnals vector around the X axis (from GLM_GTX_rotate_vector extension)
    template <typename T> _xvec3<T> rotateYGTX(const _xvec3<T>& v, T angle); //!< /brief Rotate a three dimentionnals vector around the Y axis (from GLM_GTX_rotate_vector extension)
    template <typename T> _xvec3<T> rotateZGTX(const _xvec3<T>& v, T angle); //!< /brief Rotate a three dimentionnals vector around the Z axis (from GLM_GTX_rotate_vector extension)
    template <typename T> _xvec4<T> rotateXGTX(const _xvec4<T>& v, T angle); //!< /brief Rotate a four dimentionnals vector around the X axis (from GLM_GTX_rotate_vector extension)
    template <typename T> _xvec4<T> rotateYGTX(const _xvec4<T>& v, T angle); //!< /brief Rotate a four dimentionnals vector around the X axis (from GLM_GTX_rotate_vector extension)
    template <typename T> _xvec4<T> rotateZGTX(const _xvec4<T>& v, T angle); //!< /brief Rotate a four dimentionnals vector around the X axis (from GLM_GTX_rotate_vector extension)
	template <typename T> _xmat4<T> orientationGTX(const _xvec3<T>& Normal, const _xvec3<T>& Up); //!< /brief Build a rotation matrix from a normal and a up vector (from GLM_GTX_rotate_vector extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xvec2<T> rotate(const _xvec2<T>& v, T angle){return rotateGTX(v, angle);}
    template <typename T> _xvec3<T> rotate(const _xvec3<T>& v, T angle, const _xvec3<T>& normal){return rotateGTX(v, angle, normal);}
    template <typename T> _xvec4<T> rotate(const _xvec4<T>& v, T angle, const _xvec3<T>& normal){return rotateGTX(v, angle, normal);}
    template <typename T> _xvec3<T> rotateX(const _xvec3<T>& v, T angle){return rotateXGTX(v, angle);}
    template <typename T> _xvec3<T> rotateY(const _xvec3<T>& v, T angle){return rotateYGTX(v, angle);}
    template <typename T> _xvec3<T> rotateZ(const _xvec3<T>& v, T angle){return rotateZGTX(v, angle);}
    template <typename T> _xvec4<T> rotateX(const _xvec4<T>& v, T angle){return rotateXGTX(v, angle);}
    template <typename T> _xvec4<T> rotateY(const _xvec4<T>& v, T angle){return rotateYGTX(v, angle);}
    template <typename T> _xvec4<T> rotateZ(const _xvec4<T>& v, T angle){return rotateZGTX(v, angle);}
	template <typename T> _xmat4<T> orientation(const _xvec3<T>& Normal, const _xvec3<T>& Up){return orientationGTX(Normal, Up);}

#endif
}

#endif//included

#endif//__gtx_rotate_vector_h__
