//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_half.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
// - GLM_GTX_quaternion
//////////////////////////////////////////////////////////////////////////////////
// ToDo:
// - Defined all the extensions
//////////////////////////////////////////////////////////////////////////////////
// Copyright:
// This half implementation is based on OpenEXR witch is Copyright (c) 2002, 
// Industrial Light & Magic, a division of Lucas Digital Ltd. LLC
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_half_h__
#define __gtx_half_h__

#if (GLM_GTX_half == GLM_EXTENSION_DISABLE && defined(GLM_GTX_half_required))
#error GLM_GTX_half is disable but required
#endif

// Dependency:
#include "../glm.h"
#define GLM_GTX_quaternion_required 1
#include "gtx_quaternion.h"

#if(GLM_INCLUDED(GLM_GTX_half, GLM_EXTENSION_GTX, GLM_GTX_half_required))

#define GLM_GTX_half_supported 

namespace glm
{
	//! \brief Type for half-precision floating-point numbers. (From GLM_GTX_half extension)
    class _half
    {
    public:
	    // Constructors
        _half();
        _half(float s);
        _half(double s);
        _half(int s);

        operator float() const;
        operator double() const;
        operator int() const;

        // Operators
        _half& operator=(_half s);
        _half& operator=(float s);
	    _half operator+ (_half s) const;
	    _half& operator+= (_half s);
        _half operator+ (float s) const;
	    _half& operator+= (float s);
	    _half operator- (_half s) const;
	    _half& operator-= (_half s);
	    _half operator- (float s) const;
	    _half& operator-= (float s);
        _half operator* (_half s) const;
	    _half& operator*= (_half s);
        _half operator* (float s) const;
	    _half& operator*= (float s);
	    _half operator/ (_half s) const;
	    _half& operator/= (_half s);
	    _half operator/ (float s) const;
	    _half& operator/= (float s);
        _half operator- () const;
        _half operator++ ();
	    _half operator++ (int n) const;
	    _half operator-- ();
	    _half operator-- (int n) const;

    private:
        short data;

        short _toHalf(int i) const;
        int _toFloat(short h) const;
        float _overflow() const;

    private:
        union uif
        {
    	    int i;
	        float f;
        };
    };

    typedef _half				__halfGTX;  //!< \brief Type for half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xvec2<_half>		__hvec2GTX; //!< \brief Vector of 2 half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xvec3<_half>		__hvec3GTX; //!< \brief Vector of 3 half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xvec4<_half>		__hvec4GTX; //!< \brief Vector of 4 half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xmat2<_half>		__hmat2GTX; //!< \brief 2 * 2 matrix of half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xmat3<_half>		__hmat3GTX; //!< \brief 3 * 3 matrix of half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xmat4<_half>		__hmat4GTX; //!< \brief 4 * 4 matrix of half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xquat<_half>		__hquatGTX; //!< \brief Quaternion of half-precision floating-point numbers. (from GLM_GTX_half extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    typedef _half				half;  //!< \brief Type for half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xvec2<_half>		hvec2; //!< \brief Vector of 2 half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xvec3<_half>		hvec3; //!< \brief Vector of 3 half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xvec4<_half>		hvec4; //!< \brief Vector of 4 half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xmat2<_half>		hmat2; //!< \brief 2 * 2 matrix of half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xmat3<_half>		hmat3; //!< \brief 3 * 3 matrix of half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xmat4<_half>		hmat4; //!< \brief 4 * 4 matrix of half-precision floating-point numbers. (From GLM_GTX_half extension)
    typedef _xquat<_half>		hquat; //!< \brief Quaternion of half-precision floating-point numbers. (from GLM_GTX_half extension)

#endif
}

#endif//included

#endif//__gtx_half_h__
