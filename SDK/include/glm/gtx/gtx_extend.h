//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-01-07
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_extend.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_extend_h__
#define __gtx_extend_h__

#if (GLM_GTX_extend == GLM_EXTENSION_DISABLE && defined(GLM_GTX_extend_required))
#error GLM_GTX_extend is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_extend, GLM_EXTENSION_GTX, GLM_GTX_extend_required))

#define GLM_GTX_extend_supported 

namespace glm
{
	template <typename T> T extendGTX(const T Origin, const T Source, const T Length); //!< \brief Extends of Length the Origin position using the (Source - Origin) direction (from GLM_GTX_extend extension)
	template <typename T> _xvec2<T> extendGTX(const _xvec2<T>& Origin, const _xvec2<T>& Source, const T Length); //!< \brief Extends of Length the Origin position using the (Source - Origin) direction (from GLM_GTX_extend extension)
	template <typename T> _xvec3<T> extendGTX(const _xvec3<T>& Origin, const _xvec3<T>& Source, const T Length); //!< \brief Extends of Length the Origin position using the (Source - Origin) direction (from GLM_GTX_extend extension)
	template <typename T> _xvec4<T> extendGTX(const _xvec4<T>& Origin, const _xvec4<T>& Source, const T Length); //!< \brief Extends of Length the Origin position using the (Source - Origin) direction (from GLM_GTX_extend extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

	template <typename T> T extend(const T Origin, const T Source, const T Length){return extendGTX(Origin, Source, Length);}
	template <typename T> _xvec2<T> extend(const _xvec2<T>& Origin, const _xvec2<T>& Source, const T Length){return extendGTX(Origin, Source, Length);}
	template <typename T> _xvec3<T> extend(const _xvec3<T>& Origin, const _xvec3<T>& Source, const T Length){return extendGTX(Origin, Source, Length);}
	template <typename T> _xvec4<T> extend(const _xvec4<T>& Origin, const _xvec4<T>& Source, const T Length){return extendGTX(Origin, Source, Length);}

#endif
}

#endif//included

#endif//__gtx_extend_h__
