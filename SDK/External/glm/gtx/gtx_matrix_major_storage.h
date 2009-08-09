//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-19
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_matrix_major_storage.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_matrix_major_storage_h__
#define __gtx_matrix_major_storage_h__

#if (GLM_GTX_matrix_major_storage == GLM_EXTENSION_DISABLE && defined(GLM_GTX_matrix_major_storage_required))
#error GLM_GTX_matrix_major_storage is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_matrix_major_storage, GLM_EXTENSION_GTX, GLM_GTX_matrix_major_storage_required))

#define GLM_GTX_matrix_major_storage_supported 

namespace glm
{
    template <typename T> _xmat2<T> rowMajor2GTX(const _xvec2<T>& v1, const _xvec2<T>& v2);
    template <typename T> _xmat2<T> rowMajor2GTX(const _xmat2<T>& m);
    template <typename T> _xmat3<T> rowMajor3GTX(const _xvec3<T>& v1, const _xvec3<T>& v2, const _xvec3<T>& v3);
    template <typename T> _xmat3<T> rowMajor3GTX(const _xmat3<T>& m);
    template <typename T> _xmat4<T> rowMajor4GTX(const _xvec4<T>& v1, const _xvec4<T>& v2, const _xvec4<T>& v3, const _xvec4<T>& v4);
    template <typename T> _xmat4<T> rowMajor4GTX(const _xmat4<T>& m);

    template <typename T> _xmat2<T> colMajor2GTX(const _xvec2<T>& v1, const _xvec2<T>& v2);
    template <typename T> _xmat2<T> colMajor2GTX(const _xmat2<T>& m);
    template <typename T> _xmat3<T> colMajor3GTX(const _xvec3<T>& v1, const _xvec3<T>& v2, const _xvec3<T>& v3);
    template <typename T> _xmat3<T> colMajor3GTX(const _xmat3<T>& m);
    template <typename T> _xmat4<T> colMajor4GTX(const _xvec4<T>& v1, const _xvec4<T>& v2, const _xvec4<T>& v3, const _xvec4<T>& v4);
    template <typename T> _xmat4<T> colMajor4GTX(const _xmat4<T>& m);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat2<T> rowMajor2(const _xvec2<T>& v1, const _xvec2<T>& v2){return rowMajor2GTX(v1, v2);}
    template <typename T> _xmat2<T> rowMajor2(const _xmat2<T>& m){return rowMajor2GTX(m);}
    template <typename T> _xmat3<T> rowMajor3(const _xvec3<T>& v1, const _xvec3<T>& v2, const _xvec3<T>& v3){return rowMajor3GTX(v1, v2, v3);}
    template <typename T> _xmat3<T> rowMajor3(const _xmat3<T>& m){return rowMajor3GTX(m);}
    template <typename T> _xmat4<T> rowMajor4(const _xvec4<T>& v1, const _xvec4<T>& v2, const _xvec4<T>& v3, const _xvec4<T>& v4){return rowMajor4GTX(v1, v2, v3, v4);}
    template <typename T> _xmat4<T> rowMajor4(const _xmat4<T>& m){return rowMajor4GTX(m);}

    template <typename T> _xmat2<T> colMajor2(const _xvec2<T>& v1, const _xvec2<T>& v2){return colMajor2GTX(v1, v2);}
    template <typename T> _xmat2<T> colMajor2(const _xmat2<T>& m){return colMajor2GTX(m);}
    template <typename T> _xmat3<T> colMajor3(const _xvec3<T>& v1, const _xvec3<T>& v2, const _xvec3<T>& v3){return colMajor2GTX(v1, v2, v3);}
    template <typename T> _xmat3<T> colMajor3(const _xmat3<T>& m){return colMajor3GTX(m);}
    template <typename T> _xmat4<T> colMajor4(const _xvec4<T>& v1, const _xvec4<T>& v2, const _xvec4<T>& v3, const _xvec4<T>& v4){return colMajor2GTX(v1, v2, v3, v4);}
    template <typename T> _xmat4<T> colMajor4(const _xmat4<T>& m){return colMajor4GTX(m);}

#endif
}

#endif//included

#endif//__gtx_matrix_major_storage_h__
