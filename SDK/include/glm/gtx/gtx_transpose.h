//////////////////////////////////////////////////////////////////////////////////

// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_transpose.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_transpose_h__
#define __gtx_transpose_h__

#if (GLM_GTX_transpose == GLM_EXTENSION_DISABLE && defined(GLM_GTX_transpose_required))
#error GLM_GTX_transpose is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_transpose, GLM_EXTENSION_GTX, GLM_GTX_transpose_required))

#define GLM_GTX_transpose_supported 

namespace glm
{
    template <typename T> _xmat2<T> transposeGTX(const _xmat2<T>& x); //!< Returns the transposed 2 * 2 matrix of x (From GLM_GTX_transpose extension)
    template <typename T> _xmat3<T> transposeGTX(const _xmat3<T>& x); //!< Returns the transposed 3 * 3 matrix of x (From GLM_GTX_transpose extension)
    template <typename T> _xmat4<T> transposeGTX(const _xmat4<T>& x); //!< Returns the transposed 4 * 4 matrix of x (From GLM_GTX_transpose extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat2<T> transpose(const _xmat2<T>& x){return transposeGTX(x);} //!< Returns the transposed 2 * 2 matrix of x (From GLM_GTX_transpose extension)
    template <typename T> _xmat3<T> transpose(const _xmat3<T>& x){return transposeGTX(x);} //!< Returns the transposed 3 * 3 matrix of x (From GLM_GTX_transpose extension)
    template <typename T> _xmat4<T> transpose(const _xmat4<T>& x){return transposeGTX(x);} //!< Returns the transposed 4 * 4 matrix of x (From GLM_GTX_transpose extension)

#endif
}

#endif//included

#endif//__gtx_transpose_h__
