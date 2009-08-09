//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-27
// Updated : 2006-12-06
// Licence : This source is under GNU LGPL licence
// File    : gtx_matrix_access.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_matrix_access_h__
#define __gtx_matrix_access_h__

#if (GLM_GTX_matrix_access == GLM_EXTENSION_DISABLE && defined(GLM_GTX_matrix_access_required))
#error GLM_GTX_matrix_access is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_matrix_access, GLM_EXTENSION_GTX, GLM_GTX_matrix_access_required))

#define GLM_GTX_matrix_access_supported 

namespace glm
{
    template <typename T> _xmat2<T> rowGTX(const _xmat2<T> & m, int index, const _xvec2<T>& x);
    template <typename T> _xmat3<T> rowGTX(const _xmat3<T> & m, int index, const _xvec3<T>& x);
    template <typename T> _xmat4<T> rowGTX(const _xmat4<T> & m, int index, const _xvec4<T>& x);
    template <typename T> _xmat2<T> columnGTX(const _xmat2<T> & m, int index, const _xvec2<T>& x);
    template <typename T> _xmat3<T> columnGTX(const _xmat3<T> & m, int index, const _xvec3<T>& x);
    template <typename T> _xmat4<T> columnGTX(const _xmat4<T> & m, int index, const _xvec4<T>& x);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat2<T> row(const _xmat2<T> & m, int index, const _xvec2<T>& x){return rowGTX(m, index, x);}
    template <typename T> _xmat3<T> row(const _xmat3<T> & m, int index, const _xvec3<T>& x){return rowGTX(m, index, x);}
    template <typename T> _xmat4<T> row(const _xmat4<T> & m, int index, const _xvec4<T>& x){return rowGTX(m, index, x);}
    template <typename T> _xmat2<T> column(const _xmat2<T> & m, int index, const _xvec2<T>& x){return columnGTX(m, index, x);}
    template <typename T> _xmat3<T> column(const _xmat3<T> & m, int index, const _xvec3<T>& x){return columnGTX(m, index, x);}
    template <typename T> _xmat4<T> column(const _xmat4<T> & m, int index, const _xvec4<T>& x){return columnGTX(m, index, x);}

#endif
}

#endif//included

#endif//__gtx_matrix_access_h__
