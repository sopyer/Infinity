//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-11-13
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_gpu_shader4.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
// - GLM_GTX_round
// - GLM_GTX_half
// - GLM_GTX_double
// - GLM_GTX_integer
//////////////////////////////////////////////////////////////////////////////////
// ToDo:
// - Defined all the extensions
//////////////////////////////////////////////////////////////////////////////////
// Comment:
// This extension is based on the extension GL_EXT_gpu_shader4
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_gpu_shader4_h__
#define __gtx_gpu_shader4_h__

#if (GLM_GTX_gpu_shader4 == GLM_EXTENSION_DISABLE && defined(GLM_GTX_gpu_shader4_required))
#error GLM_GTX_gpu_shader4 is disable but required
#endif

// Dependency:
#include "../glm.h"
#define GLM_GTX_round_required 1
#include "../gtx/gtx_round.h"
#define GLM_GTX_half_required 1
#include "../gtx/gtx_half.h"

#if(GLM_INCLUDED(GLM_GTX_gpu_shader4, GLM_EXTENSION_GTX, GLM_GTX_gpu_shader4_required))

//#define GLM_GTX_gpu_shader4_supported 

namespace glm
{
    __halfGTX truncateGTX(__halfGTX x); //!< \brief Computes the truncate value of x (from GLM_GTX_gpu_shader4 extension) 
    float truncateGTX(float x); //!< \brief Computes the truncate value of x (from GLM_GTX_gpu_shader4 extension)
    double truncateGTX(double x); //!< \brief Computes the truncate value of x (from GLM_GTX_gpu_shader4 extension)
    template <typename genType> genType truncateGTX(const genType& x); //!< \brief Computes the component-wise truncate value of x (from GLM_GTX_gpu_shader4 extension)

    unsigned int powGTX(unsigned int x, unsigned int y);
    unsigned int sqrtGTX(unsigned int x);

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    half truncate(half x); //!< \brief Computes the truncate value of x (from GLM_GTX_gpu_shader4 extension) 
    float truncate(float x); //!< \brief Computes the truncate value of x (from GLM_GTX_gpu_shader4 extension)
    double truncate(double x); //!< \brief Computes the truncate value of x (from GLM_GTX_gpu_shader4 extension)
    template <typename genType> genType truncate(const genType& x); //!< \brief Computes the component-wise truncate value of x (from GLM_GTX_gpu_shader4 extension)

#endif

    typedef _xvec2<unsigned int>		__uvec2GTX; //!< \brief Vector of 2 half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xvec3<unsigned int>		__uvec3GTX; //!< \brief Vector of 3 half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xvec4<unsigned int>		__uvec4GTX; //!< \brief Vector of 4 half-precision floating-point numbers. (From GLM_GTX_half extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    typedef _xvec2<unsigned int>		uvec2; //!< \brief Vector of 2 half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xvec3<unsigned int>		uvec3; //!< \brief Vector of 3 half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xvec4<unsigned int>		uvec4; //!< \brief Vector of 4 half-precision floating-point numbers. (From GLM_GTX_half extension)

#endif	
}

#endif//included

#endif//__gtx_gpu_shader4_h__
