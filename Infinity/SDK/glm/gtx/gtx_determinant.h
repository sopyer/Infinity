//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_determinant.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_determinant_h__
#define __gtx_determinant_h__

#if (GLM_GTX_determinant == GLM_EXTENSION_DISABLE && defined(GLM_GTX_determinant_required))
#error GLM_GTX_determinant is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_determinant, GLM_EXTENSION_GTX, GLM_GTX_determinant_required))

#define GLM_GTX_determinant_supported 

namespace glm
{
    template <typename T> T determinantGTX(const _xmat2<T>& m); //!< \brief Returns the determinant of a 2 * 2 matrix. (From GLM_GTX_determinant extension)
    template <typename T> T determinantGTX(const _xmat3<T>& m); //!< \brief Returns the determinant of a 3 * 3 matrix. (From GLM_GTX_determinant extension)
    template <typename T> T determinantGTX(const _xmat4<T>& m); //!< \brief Returns the determinant of a 4 * 4 matrix. (From GLM_GTX_determinant extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> T determinant(const _xmat2<T>& m){return determinant(m);}
    template <typename T> T determinant(const _xmat3<T>& m){return determinant(m);}
    template <typename T> T determinant(const _xmat4<T>& m){return determinant(m);}

#endif
}

#endif//included

#endif//__gtx_determinant_h__
