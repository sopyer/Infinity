//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_transform.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_transform_h__
#define __gtx_transform_h__

#if (GLM_GTX_transform == GLM_EXTENSION_DISABLE && defined(GLM_GTX_transform_required))
#error GLM_GTX_transform is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_transform, GLM_EXTENSION_GTX, GLM_GTX_transform_required))

#define GLM_GTX_transform_supported 

namespace glm
{
    template <typename T> _xmat4<T> translateGTX(T x, T y, T z); //!< /brief Builds a translation 4 * 4 matrix created from 3 scalars (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> translateGTX(const _xvec3<T> & v); //!< /brief Builds a translation 4 * 4 matrix created from a vector of 3 components (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> translateGTX(const _xmat4<T> & m, T x, T y, T z); //!< /brief Transforms a matrix with a translation 4 * 4 matrix created from 3 scalars (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> translateGTX(const _xmat4<T> & m, const _xvec3<T> & v); //!< /brief Transforms a matrix with a translation 4 * 4 matrix created from a vector of 3 components (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> rotateGTX(T angle, T x, T y, T z); //!< /brief Builds a rotation 4 * 4 matrix created from an axis of 3 scalars and an angle expressed in degrees (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> rotateGTX(T angle, const _xvec3<T> & v); //!< /brief Builds a rotation 4 * 4 matrix created from an axis vector and an angle expressed in degrees (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> rotateGTX(const _xmat4<T> & m, T angle, T x, T y, T z); //!< /brief Transforms a matrix with a rotation 4 * 4 matrix created from an axis of 3 scalars and an angle expressed in degrees (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> rotateGTX(const _xmat4<T> & m, T angle, const _xvec3<T> & v); //!< /brief Transforms a matrix with a rotation 4 * 4 matrix created from an axis vector and an angle expressed in degrees (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> scaleGTX(T x, T y, T z); //!< /brief Builds a scale 4 * 4 matrix created from 3 scalars (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> scaleGTX(const _xvec3<T> & v); //!< /brief Builds a scale 4 * 4 matrix created from a vector of 3 components (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> scaleGTX(const _xmat4<T> & m, T x, T y, T z); //!< /brief Transforms a matrix with a scale 4 * 4 matrix created from 3 scalars (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> scaleGTX(const _xmat4<T> & m, const _xvec3<T> & v); //!< /brief Transforms a matrix with a scale 4 * 4 matrix created from a vector of 3 components (from GLM_GTX_transform extension)

    // 3D
    template <typename T> _xmat4<T> translate3DGTX(const _xmat4<T> & m, T x, T y, T z); //!< /brief Transforms a matrix with a translation 4 * 4 matrix created from 3 scalars (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> translate3DGTX(const _xmat4<T> & m, const _xvec3<T> & v); //!< /brief Transforms a matrix with a translation 4 * 4 matrix created from a vector of 3 components (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> rotate3DGTX(const _xmat4<T> & m, T angle, T x, T y, T z); //!< /brief Transforms a matrix with a rotation 4 * 4 matrix created from an axis of 3 scalars and an angle expressed in degrees (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> rotate3DGTX(const _xmat4<T> & m, T angle, const _xvec3<T> & v); //!< /brief Transforms a matrix with a rotation 4 * 4 matrix created from an axis vector and an angle expressed in degrees (from GLM_GTX_transform extension)
    template <typename T> _xmat3<T> rotate3DGTX(const _xmat3<T> & m, T angle, T x, T y, T z); //!< /brief Transforms a matrix with a rotation 3 * 3 matrix created from an axis of 3 scalars and an angle expressed in degrees (from GLM_GTX_transform extension)
    template <typename T> _xmat3<T> rotate3DGTX(const _xmat3<T> & m, T angle, const _xvec3<T> & v); //!< /brief Transforms a matrix with a rotation 3 * 3 matrix created from an axis vector and an angle expressed in degrees (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> scale3DGTX(const _xmat4<T> & m, T x, T y, T z); //!< /brief Transforms a matrix with a scale 4 * 4 matrix created from 3 scalars (from GLM_GTX_transform extension)
    template <typename T> _xmat4<T> scale3DGTX(const _xmat4<T> & m, const _xvec3<T> & v); //!< /brief Transforms a matrix with a scale 4 * 4 matrix created from a vector of 3 components (from GLM_GTX_transform extension)
    template <typename T> _xmat3<T> scale3DGTX(const _xmat3<T> & m, T x, T y, T z); //!< /brief Transforms a matrix with a scale 3 * 3 matrix created from 3 scalars (from GLM_GTX_transform extension)
    template <typename T> _xmat3<T> scale3DGTX(const _xmat3<T> & m, const _xvec3<T> & v); //!< /brief Transforms a matrix with a scale 3 * 3 matrix created from a vector of 3 components (from GLM_GTX_transform extension)

    // 2D
    template <typename T> _xmat3<T> translate2DGTX(const _xmat3<T> & m, T x, T y); //!< /brief Translates a 3 * 3 matrix from 2 scalars (from GLM_GTX_transform extension)
    template <typename T> _xmat3<T> translate2DGTX(const _xmat3<T> & m, const _xvec2<T> & v); //!< /brief Translates a 3 * 3 matrix from a vector of 2 components (from GLM_GTX_transform extension)
    template <typename T> _xmat3<T> rotate2DGTX(const _xmat3<T> & m, T angle); //!< /brief Rotates a 3 * 3 matrix from an angle expressed in degrees (from GLM_GTX_transform extension)
    template <typename T> _xmat2<T> rotate2DGTX(const _xmat2<T> & m, T angle); //!< /brief Rotates a 2 * 2 matrix from an angle expressed in degrees (from GLM_GTX_transform extension)
    template <typename T> _xmat3<T> scale2DGTX(const _xmat3<T> & m, T x, T y); //!< /brief Scales a 3 * 3 matrix using 2 scalars (from GLM_GTX_transform extension)
    template <typename T> _xmat3<T> scale2DGTX(const _xmat3<T> & m, const _xvec2<T> & v); //!< /brief Scales a 3 * 3 matrix using a vector of 2 components (from GLM_GTX_transform extension)
    template <typename T> _xmat2<T> scale2DGTX(const _xmat2<T> & m, T x, T y); //!< /brief Scales a 2 * 2 matrix using 2 scalars (from GLM_GTX_transform extension)
    template <typename T> _xmat2<T> scale2DGTX(const _xmat2<T> & m, const _xvec2<T> & v); //!< /brief Scales a 2 * 2 matrix using a vector of 2 components (from GLM_GTX_transform extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat4<T> translate(T x, T y, T z){return translateGTX(x, y, z);}
    template <typename T> _xmat4<T> translate(const _xvec3<T> & v){return translateGTX(v);}
    template <typename T> _xmat4<T> translate(const _xmat4<T> & m, T x, T y, T z){return translateGTX(m, x, y, z);}
    template <typename T> _xmat4<T> translate(const _xmat4<T> & m, const _xvec3<T> & v){return translateGTX(m, v);}
    template <typename T> _xmat4<T> rotate(T angle, T x, T y, T z){return rotateGTX(angle, x, y, z);}
    template <typename T> _xmat4<T> rotate(T angle, const _xvec3<T> & v){return rotateGTX(angle, v);}
    template <typename T> _xmat4<T> rotate(const _xmat4<T> & m, T angle, T x, T y, T z){return rotateGTX(m, angle, x, y, z);}
    template <typename T> _xmat4<T> rotate(const _xmat4<T> & m, T angle, const _xvec3<T> & v){return rotateGTX(m, angle, v);}
    template <typename T> _xmat4<T> scale(T x, T y, T z){return scaleGTX(x, y, z);}
    template <typename T> _xmat4<T> scale(const _xvec3<T> & v){return scaleGTX(v);}
    template <typename T> _xmat4<T> scale(const _xmat4<T> & m, T x, T y, T z){return scaleGTX(m, x, y, z);}
    template <typename T> _xmat4<T> scale(const _xmat4<T> & m, const _xvec3<T> & v){return scaleGTX(m, v);}

    // 3D
    template <typename T> _xmat4<T> translate3D(const _xmat4<T> & m, T x, T y, T z){return translate3DGTX(m, x, y, z);}
    template <typename T> _xmat4<T> translate3D(const _xmat4<T> & m, const _xvec3<T> & v){return translate3DGTX(m, v);}
    template <typename T> _xmat4<T> rotate3D(const _xmat4<T> & m, T angle, T x, T y, T z){return rotate3DGTX(m, angle, x, y, z);}
    template <typename T> _xmat4<T> rotate3D(const _xmat4<T> & m, T angle, const _xvec3<T> & v){return rotate3DGTX(m, angle, v);}
    template <typename T> _xmat3<T> rotate3D(const _xmat3<T> & m, T angle, T x, T y, T z){return rotate3DGTX(m, angle, x, y, z);}
    template <typename T> _xmat3<T> rotate3D(const _xmat3<T> & m, T angle, const _xvec3<T> & v){return rotate3DGTX(m, angle, v);}
    template <typename T> _xmat4<T> scale3D(const _xmat4<T> & m, T x, T y, T z){return scale3DGTX(m, x, y, z);}
    template <typename T> _xmat4<T> scale3D(const _xmat4<T> & m, const _xvec3<T> & v){return scale3DGTX(m, v);}
    template <typename T> _xmat3<T> scale3D(const _xmat3<T> & m, T x, T y, T z){return scale3DGTX(m, x, y, z);}
    template <typename T> _xmat3<T> scale3D(const _xmat3<T> & m, const _xvec3<T> & v){return scale3DGTX(m, v);}

    // 2D
    template <typename T> _xmat3<T> translate2D(const _xmat3<T> & m, T x, T y){return translate2DGTX(m, x, y);}
    template <typename T> _xmat3<T> translate2D(const _xmat3<T> & m, const _xvec2<T> & v){return translate2DGTX(m, v);}
    template <typename T> _xmat3<T> rotate2D(const _xmat3<T> & m, T angle){return rotate2DGTX(m, angle);}
    template <typename T> _xmat2<T> rotate2D(const _xmat2<T> & m, T angle){return rotate2DGTX(m, angle);}
    template <typename T> _xmat3<T> scale2D(const _xmat3<T> & m, T x, T y){return scale2DGTX(m, x, y);}
    template <typename T> _xmat3<T> scale2D(const _xmat3<T> & m, const _xvec2<T> & v){return scale2DGTX(m, v);}
    template <typename T> _xmat2<T> scale2D(const _xmat2<T> & m, T x, T y){return scale2DGTX(m, x, y);}
    template <typename T> _xmat2<T> scale2D(const _xmat2<T> & m, const _xvec2<T> & v){return scale2DGTX(m, v);}

#endif
}

#endif//included

#endif//__gtx_transform_h__
