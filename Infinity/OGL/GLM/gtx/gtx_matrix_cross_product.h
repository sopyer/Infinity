//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_matrix_cross_product.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_matrix_cross_product_h__
#define __gtx_matrix_cross_product_h__

#if (GLM_GTX_matrix_cross_product == GLM_EXTENSION_DISABLE && defined(GLM_GTX_matrix_cross_product_required))
#error GLM_GTX_matrix_cross_product is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_matrix_cross_product, GLM_EXTENSION_GTX, GLM_GTX_matrix_cross_product_required))

#define GLM_GTX_matrix_cross_product_supported 

namespace glm
{
    template <typename T> _xmat3<T> matrixCross3GTX(const _xvec3<T>& x);
    template <typename T> _xmat4<T> matrixCross4GTX(const _xvec3<T>& x);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat3<T> matrixCross3(const _xvec3<T>& x){return matrixCross3GTX(x);}
    template <typename T> _xmat4<T> matrixCross4(const _xvec3<T>& x){return matrixCross4GTX(x);}

#endif
}

#endif//included

#endif//__gtx_matrix_cross_product_h__
