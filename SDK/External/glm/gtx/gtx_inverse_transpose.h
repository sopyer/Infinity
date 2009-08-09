//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-01-04
// Updated : 2006-12-06
// Licence : This source is under GNU LGPL licence
// File    : gtx_inverse_transpose.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_inverse_transpose_h__
#define __gtx_inverse_transpose_h__

#if (GLM_GTX_inverse_transpose == GLM_EXTENSION_DISABLE && defined(GLM_GTX_inverse_transpose_required))
#error GLM_GTX_inverse_transpose is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_inverse_transpose, GLM_EXTENSION_GTX, GLM_GTX_inverse_transpose_required))

#define GLM_GTX_inverse_transpose_supported 

namespace glm
{
    template <typename T> _xmat2<T> inverseTransposeGTX(const _xmat2<T> & m);
    template <typename T> _xmat3<T> inverseTransposeGTX(const _xmat3<T> & m);
    template <typename T> _xmat4<T> inverseTransposeGTX(const _xmat4<T> & m);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> T inverseTranspose(const T & m){return inverseTransposeGTX(m);}

#endif
}

#endif//included

#endif//__gtx_inverse_transpose_h__
