//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-12-06
// Licence : This source is under GNU LGPL licence
// File    : gtx_inverse.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_inverse_h__
#define __gtx_inverse_h__

#if (GLM_GTX_inverse == GLM_EXTENSION_DISABLE && defined(GLM_GTX_inverse_required))
#error GLM_GTX_inverse is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_inverse, GLM_EXTENSION_GTX, GLM_GTX_inverse_required))

#define GLM_GTX_inverse_supported 

namespace glm
{
    template <typename T> _xmat2<T> inverseGTX(const _xmat2<T>& m);
    template <typename T> _xmat3<T> inverseGTX(const _xmat3<T>& m);
    template <typename T> _xmat4<T> inverseGTX(const _xmat4<T>& m);

    template <typename T> _xmat2<T> affineInverseGTX(const _xmat2<T>& m);
    template <typename T> _xmat3<T> affineInverseGTX(const _xmat3<T>& m);
    template <typename T> _xmat4<T> affineInverseGTX(const _xmat4<T>& m);

    template <typename T> _xmat4<T> fastInverseGTX(const _xmat4<T>& m);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat2<T> inverse(const _xmat2<T>& m){return inverseGTX(m);}
    template <typename T> _xmat3<T> inverse(const _xmat3<T>& m){return inverseGTX(m);}
    template <typename T> _xmat4<T> inverse(const _xmat4<T>& m){return inverseGTX(m);}

    template <typename T> _xmat2<T> affineInverse(const _xmat2<T>& m){return affineInverseGTX(m);}
    template <typename T> _xmat3<T> affineInverse(const _xmat3<T>& m){return affineInverseGTX(m);}
    template <typename T> _xmat4<T> affineInverse(const _xmat4<T>& m){return affineInverseGTX(m);}

    template <typename T> _xmat4<T> fastInverse(const _xmat4<T>& m){return fastInverseGTX(m);}

#endif
}

#endif//included

#endif//__gtx_inverse_h__
