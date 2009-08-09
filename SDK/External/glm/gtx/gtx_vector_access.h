//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-01-16
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_vector_access.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
// - GL_GTX_double
// - GL_GTX_half
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_vector_access_h__
#define __gtx_vector_access_h__

#if (GLM_GTX_vector_access == GLM_EXTENSION_DISABLE && defined(GLM_GTX_vector_access_required))
#error GLM_GTX_vector_access is disable but required
#endif

// Dependency:
#include "../glm.h"
#define GLM_GTX_half_required 1
#include "../gtx/gtx_half.h"
#define GLM_GTX_double_required 1
#include "../gtx/gtx_double.h"

#if(GLM_INCLUDED(GLM_GTX_vector_access, GLM_EXTENSION_GTX, GLM_GTX_vector_access_required))

//#define GLM_GTX_vector_access_supported 

namespace glm
{
/*
    void setGTX(float& v, float x);
    void setGTX(int& v, int x);
    void setGTX(bool& v, bool x);
    void setGTX(double& v, double x);
    void setGTX(__halfGTX& v, __halfGTX x);
*/
    template <typename T> void setGTX(_xvec2<T>& v, T x, T y);				//!< \brief Sets values to a 2 components vector of floating-point numbers or integers (from GLM_GTX_vector_access extension)
    template <typename T> void setGTX(_xvec3<T>& v, T x, T y, T z);			//!< \brief Sets values to a 3 components vector of floating-point numbers or integers (from GLM_GTX_vector_access extension)
    template <typename T> void setGTX(_xvec4<T>& v, T x, T y, T z, T w);	//!< \brief Sets values to a 4 components vector of floating-point numbers or integers (from GLM_GTX_vector_access extension)

    void setGTX(_bvec2& v, bool x, bool y);									//!< \brief Sets values to a 2 components vector of booleans (from GLM_GTX_vector_access extension)
    void setGTX(_bvec3& v, bool x, bool y, bool z);							//!< \brief Sets values to a 3 components vector of booleans (from GLM_GTX_vector_access extension)
    void setGTX(_bvec4& v, bool x, bool y, bool z, bool w);			//!< \brief Sets values to a 4 components vector of booleans (from GLM_GTX_vector_access extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> void set(_xvec2<T>& v, T x, T y){setGTX(v, x, y);}
    template <typename T> void set(_xvec3<T>& v, T x, T y, T z){setGTX(v, x, y, z);}
    template <typename T> void set(_xvec4<T>& v, T x, T y, T z, T w){setGTX(v, x, y, z, w);}

    inline void set(_bvec2& v, bool x, bool y){setGTX(v, x, y);}
    inline void set(_bvec3& v, bool x, bool y, bool z){setGTX(v, x, y, z);}
    inline void set(_bvec4& v, bool x, bool y, bool z, bool w){setGTX(v, x, y, z, w);}

#endif
}

#endif//included

#endif//__gtx_vector_access_h__
