//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_vector_comp_mult.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////
// Note:
// mat4 m;
// vec4 v;
// ...
// vec4 Result = transposeGTX(m) * v;
// Same result as the following, but it's faster by avoiding the matrix transpose: 
// vec4 Result = vectorCompMultGTX(m, v);
//////////////////////////////////////////////////////////////////////////////////
// Note:
// - This extension is deprecated, use the operator vec* * mat* instead.
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_vector_comp_mult_h__
#define __gtx_vector_comp_mult_h__

#if (GLM_GTX_vector_comp_mult == GLM_EXTENSION_DISABLE && defined(GLM_GTX_vector_comp_mult_required))
#error GLM_GTX_vector_comp_mult is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_vector_comp_mult, GLM_EXTENSION_GTX, GLM_GTX_vector_comp_mult_required))

#define GLM_GTX_rotate_vector_supported 

namespace glm
{
    template <typename T> _xvec2<T> vectorCompMultGTX(const _xmat2<T>& x, const _xvec2<T>& y); //!< Returns the result of the product between the transposed 2 * 2 matrix of x and the 2 component vector y. Deprecated, prefer vec2 * mat2 operator (From GLM_GTX_vector_comp_mult extension)
    template <typename T> _xvec3<T> vectorCompMultGTX(const _xmat3<T>& x, const _xvec3<T>& y); //!< Returns the result of the product between the transposed 3 * 3 matrix of x and the 3 component vector y. Deprecated, prefer vec3 * mat3 operator (From GLM_GTX_vector_comp_mult extension)
    template <typename T> _xvec4<T> vectorCompMultGTX(const _xmat4<T>& x, const _xvec4<T>& y); //!< Returns the result of the product between the transposed 4 * 4 matrix of x and the 4 component vector y. Deprecated, prefer vec4 * mat4 operator (From GLM_GTX_vector_comp_mult extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xvec2<T> vectorCompMult(const _xmat2<T>& x, const _xvec2<T>& y){return vectorCompMultGTX(x, y);}
    template <typename T> _xvec3<T> vectorCompMult(const _xmat3<T>& x, const _xvec3<T>& y){return vectorCompMultGTX(x, y);}
    template <typename T> _xvec4<T> vectorCompMult(const _xmat4<T>& x, const _xvec4<T>& y){return vectorCompMultGTX(x, y);}

#endif
}

#endif//included

#endif//__gtx_vector_comp_mult_h__
