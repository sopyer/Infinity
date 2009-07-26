//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-04-20
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_mul.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////
// Comment:
// - Extension for Cg compatibility
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_mul_h__
#define __gtx_mul_h__

#if (GLM_GTX_mul == GLM_EXTENSION_DISABLE && defined(GLM_GTX_mul_required))
#error GLM_GTX_mul is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_mul, GLM_EXTENSION_GTX, GLM_GTX_mul_required))

//#define GLM_GTX_mul_supported 

namespace glm
{
    template <typename T> _xmat2<T> mulGTX(const _xmat2<T>& a, const _xmat2<T>& b);
    template <typename T> _xmat3<T> mulGTX(const _xmat3<T>& a, const _xmat3<T>& b);
    template <typename T> _xmat4<T> mulGTX(const _xmat4<T>& a, const _xmat4<T>& b);

    template <typename T> _xvec2<T> mulGTX(const _xmat2<T>& m, const _xvec2<T>& v);
    template <typename T> _xvec3<T> mulGTX(const _xmat3<T>& m, const _xvec3<T>& v);
    template <typename T> _xvec4<T> mulGTX(const _xmat4<T>& m, const _xvec4<T>& v);

    template <typename T> _xvec2<T> mulGTX(const _xvec2<T>& v, const _xmat2<T>& m);
    template <typename T> _xvec3<T> mulGTX(const _xvec3<T>& v, const _xmat3<T>& m);
    template <typename T> _xvec4<T> mulGTX(const _xvec4<T>& v, const _xmat4<T>& m);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat2<T> mul(const _xmat2<T>& a, const _xmat2<T>& b){return mulGTX(a, b);}
    template <typename T> _xmat3<T> mul(const _xmat3<T>& a, const _xmat3<T>& b){return mulGTX(a, b);}
    template <typename T> _xmat4<T> mul(const _xmat4<T>& a, const _xmat4<T>& b){return mulGTX(a, b);}

    template <typename T> _xvec2<T> mul(const _xmat2<T>& m, const _xvec2<T>& v){return mulGTX(m, v);}
    template <typename T> _xvec3<T> mul(const _xmat3<T>& m, const _xvec3<T>& v){return mulGTX(m, v);}
    template <typename T> _xvec4<T> mul(const _xmat4<T>& m, const _xvec4<T>& v){return mulGTX(m, v);}

    template <typename T> _xvec2<T> mul(const _xvec2<T>& v, const _xmat2<T>& m){return mulGTX(v, m);}
    template <typename T> _xvec3<T> mul(const _xvec3<T>& v, const _xmat3<T>& m){return mulGTX(v, m);}
    template <typename T> _xvec4<T> mul(const _xvec4<T>& v, const _xmat4<T>& m){return mulGTX(v, m);}

#endif
}

#endif//included

#endif//__gtx_mul_h__
