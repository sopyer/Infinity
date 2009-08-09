//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_color_space.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_color_space_h__
#define __gtx_color_space_h__

#if (GLM_GTX_color_space == GLM_EXTENSION_DISABLE && defined(GLM_GTX_color_space_required))
#error GLM_GTX_color_space is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_color_space, GLM_EXTENSION_GTX, GLM_GTX_color_space_required))

#define GLM_GTX_color_space_supported 

namespace glm
{
    template <typename T> _xvec3<T> rgbGTX(const _xvec3<T>& hsv); //!< \brief Converts a color from HSV color space to its color in RGB color space (from GLM_GTX_color_space extension)
    template <typename T> _xvec3<T> hsvGTX(const _xvec3<T>& rgb); //!< \brief Converts a color from RGB color space to its color in HSV color space (from GLM_GTX_color_space extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xvec3<T> rgb(const _xvec3<T>& hsv){return rgbGTX(hsv);}
    template <typename T> _xvec3<T> hsv(const _xvec3<T>& rgb){return hsvGTX(rgb);}

#endif
}

#endif//included

#endif//__gtx_color_space_h__
