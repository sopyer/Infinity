//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_epsilon.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
// - GLM_GTX_double
// - GLM_GTX_half
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_epsilon_h__
#define __gtx_epsilon_h__

#if (GLM_GTX_epsilon == GLM_EXTENSION_DISABLE && defined(GLM_GTX_epsilon_required))
#error GLM_GTX_epsilon is disable but required
#endif

// Dependency:
#include "../glm.h"
#define GLM_GTX_double_required 1
#include "../gtx/gtx_double.h"
#define GLM_GTX_half_required 1
#include "../gtx/gtx_half.h"

#if(GLM_INCLUDED(GLM_GTX_epsilon, GLM_EXTENSION_GTX, GLM_GTX_epsilon_required))

#define GLM_GTX_epsilon_supported 

namespace glm
{
    template <typename T> T equalEpsilonGTX(T x, T y, T epsilon); //!< \brief Returns the component-wise compare of |x - y| < epsilon (from GLM_GTX_epsilon extension)
    template <typename T> T notEqualEpsilonGTX(T x, T y, T epsilon); //!< \brief Returns the component-wise compare of |x - y| >= epsilon (from GLM_GTX_epsilon extension)

    template <typename T> _bvec2 equalEpsilonGTX(const _xvec2<T>& x, const _xvec2<T>& y, T epsilon); //!< \brief Returns the component-wise compare of |x - y| < epsilon (from GLM_GTX_epsilon extension)
    template <typename T> _bvec3 equalEpsilonGTX(const _xvec3<T>& x, const _xvec3<T>& y, T epsilon); //!< \brief Returns the component-wise compare of |x - y| < epsilon (from GLM_GTX_epsilon extension)
    template <typename T> _bvec4 equalEpsilonGTX(const _xvec4<T>& x, const _xvec4<T>& y, T epsilon); //!< \brief Returns the component-wise compare of |x - y| < epsilon (from GLM_GTX_epsilon extension)
    template <typename T> _bvec2 notEqualEpsilonGTX(const _xvec2<T>& x, const _xvec2<T>& y, T epsilon); //!< \brief Returns the component-wise compare of |x - y| >= epsilon (from GLM_GTX_epsilon extension)
    template <typename T> _bvec3 notEqualEpsilonGTX(const _xvec3<T>& x, const _xvec3<T>& y, T epsilon); //!< \brief Returns the component-wise compare of |x - y| >= epsilon (from GLM_GTX_epsilon extension)
    template <typename T> _bvec4 notEqualEpsilonGTX(const _xvec4<T>& x, const _xvec4<T>& y, T epsilon); //!< \brief Returns the component-wise compare of |x - y| >= epsilon (from GLM_GTX_epsilon extension)

    template <typename T> _bvec2 equalEpsilonGTX(const _xvec2<T>& x, const _xvec2<T>& y, const _xvec2<T>& epsilon); //!< \brief Returns the component-wise compare of |x - y| < epsilon (from GLM_GTX_epsilon extension)
    template <typename T> _bvec3 equalEpsilonGTX(const _xvec3<T>& x, const _xvec3<T>& y, const _xvec3<T>& epsilon); //!< \brief Returns the component-wise compare of |x - y| < epsilon (from GLM_GTX_epsilon extension)
    template <typename T> _bvec4 equalEpsilonGTX(const _xvec4<T>& x, const _xvec4<T>& y, const _xvec4<T>& epsilon); //!< \brief Returns the component-wise compare of |x - y| < epsilon (from GLM_GTX_epsilon extension)
    template <typename T> _bvec2 notEqualEpsilonGTX(const _xvec2<T>& x, const _xvec2<T>& y, const _xvec2<T>& epsilon); //!< \brief Returns the component-wise compare of |x - y| >= epsilon (from GLM_GTX_epsilon extension)
    template <typename T> _bvec3 notEqualEpsilonGTX(const _xvec3<T>& x, const _xvec3<T>& y, const _xvec3<T>& epsilon); //!< \brief Returns the component-wise compare of |x - y| >= epsilon (from GLM_GTX_epsilon extension)
    template <typename T> _bvec4 notEqualEpsilonGTX(const _xvec4<T>& x, const _xvec4<T>& y, const _xvec4<T>& epsilon); //!< \brief Returns the component-wise compare of |x - y| >= epsilon (from GLM_GTX_epsilon extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> T equalEpsilon(T x, T y, T epsilon){return equalEpsilonGTX(x, y, epsilon);}
    template <typename T> T notEqualEpsilon(T x, T y, T epsilon){return notEqualEpsilonGTX(x, y, epsilon);}

    template <typename T> _bvec2 equalEpsilon(const _xvec2<T>& x, const _xvec2<T>& y, T epsilon){return equalEpsilonGTX(x, y, epsilon);}
    template <typename T> _bvec3 equalEpsilon(const _xvec3<T>& x, const _xvec3<T>& y, T epsilon){return equalEpsilonGTX(x, y, epsilon);}
    template <typename T> _bvec4 equalEpsilon(const _xvec4<T>& x, const _xvec4<T>& y, T epsilon){return equalEpsilonGTX(x, y, epsilon);}
    template <typename T> _bvec2 notEqualEpsilon(const _xvec2<T>& x, const _xvec2<T>& y, T epsilon){return notEqualEpsilonGTX(x, y, epsilon);}
    template <typename T> _bvec3 notEqualEpsilon(const _xvec3<T>& x, const _xvec3<T>& y, T epsilon){return notEqualEpsilonGTX(x, y, epsilon);}
    template <typename T> _bvec4 notEqualEpsilon(const _xvec4<T>& x, const _xvec4<T>& y, T epsilon){return notEqualEpsilonGTX(x, y, epsilon);}

    template <typename T> _bvec2 equalEpsilon(const _xvec2<T>& x, const _xvec2<T>& y, const _xvec2<T>& epsilon){return equalEpsilonGTX(x, y, epsilon);}
    template <typename T> _bvec3 equalEpsilon(const _xvec3<T>& x, const _xvec3<T>& y, const _xvec3<T>& epsilon){return equalEpsilonGTX(x, y, epsilon);}
    template <typename T> _bvec4 equalEpsilon(const _xvec4<T>& x, const _xvec4<T>& y, const _xvec4<T>& epsilon){return equalEpsilonGTX(x, y, epsilon);}
    template <typename T> _bvec2 notEqualEpsilon(const _xvec2<T>& x, const _xvec2<T>& y, const _xvec2<T>& epsilon){return notEqualEpsilonGTX(x, y, epsilon);}
    template <typename T> _bvec3 notEqualEpsilon(const _xvec3<T>& x, const _xvec3<T>& y, const _xvec3<T>& epsilon){return notEqualEpsilonGTX(x, y, epsilon);}
    template <typename T> _bvec4 notEqualEpsilon(const _xvec4<T>& x, const _xvec4<T>& y, const _xvec4<T>& epsilon){return notEqualEpsilonGTX(x, y, epsilon);}

#endif
}

#endif//included

#endif//__gtx_epsilon_h__
