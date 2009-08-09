//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_perpendicular.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_perpendicular_inl__
#define __gtx_perpendicular_inl__

#include "gtx_perpendicular.h"
#include "gtx_projection.h"
#if(GLM_INCLUDED(GLM_GTX_perpendicular, GLM_EXTENSION_GTX, GLM_GTX_perpendicular_required))

namespace glm
{
    template <typename T> 
    inline _xvec2<T> perpGTX(const _xvec2<T>& x, const _xvec2<T>& Normal)
    {
	    return x - projGTX(x, Normal);
    }

    template <typename T> 
    inline _xvec3<T> perpGTX(const _xvec3<T>& x, const _xvec3<T>& Normal)
    {
	    return x - projGTX(x, Normal);
    }

    template <typename T> 
    inline _xvec4<T> perpGTX(const _xvec4<T>& x, const _xvec4<T>& Normal)
    {
	    return x - projGTX(x, Normal);
    }
}

#endif//included

#endif//__gtx_perpendicular_inl__
