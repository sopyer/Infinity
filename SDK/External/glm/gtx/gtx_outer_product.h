//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-03-30
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_outer_product.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_outer_product_h__
#define __gtx_outer_product_h__

#if (GLM_GTX_outer_product == GLM_EXTENSION_DISABLE && defined(GLM_GTX_outer_product_required))
#error GLM_GTX_outer_product is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_outer_product, GLM_EXTENSION_GTX, GLM_GTX_outer_product_required))

#define GLM_GTX_outer_product_supported 

namespace glm
{
    template <typename T> _xmat2<T> outerProductGTX(const _xvec2<T>& x, const _xvec2<T>& y); //! Product of x extended to a matrix with the y extended to a transposed matrix
    template <typename T> _xmat3<T> outerProductGTX(const _xvec3<T>& x, const _xvec3<T>& y); //! Product of x extended to a matrix with the y extended to a transposed matrix
    template <typename T> _xmat4<T> outerProductGTX(const _xvec4<T>& x, const _xvec4<T>& y); //! Product of x extended to a matrix with the y extended to a transposed matrix

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat2<T> outerProduct(const _xvec2<T>& x, const _xvec2<T>& y){return outerProductGTX(x, y);}
    template <typename T> _xmat3<T> outerProduct(const _xvec3<T>& x, const _xvec3<T>& y){return outerProductGTX(x, y);}
    template <typename T> _xmat4<T> outerProduct(const _xvec4<T>& x, const _xvec4<T>& y){return outerProductGTX(x, y);}

#endif
}

#endif//included

#endif//__gtx_outer_product_h__
