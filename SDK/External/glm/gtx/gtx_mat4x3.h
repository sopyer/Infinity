//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-01-27
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : _mat4.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __mat4x3_h__
#define __mat4x3_h__

#if (GLM_GTX_mat4x3 == GLM_EXTENSION_DISABLE && defined(GLM_GTX_mat4x3_required))
#error GLM_GTX_mat4x3 is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_mat4x3, GLM_EXTENSION_GTX, GLM_GTX_mat4x3_required))

#define GLM_GTX_mat4x3_supported 

namespace glm
{
    template <typename T> class _xvec3;
    template <typename T> class _xvec4;

    //!< \brief Template for 4 * 3 matrix of floating-point numbers.
    template <typename T> class _xmat4x3
    {
    private:
        // Data
        _xvec3<T> value[4];

    public:
        // Constructors
        _xmat4x3();
        explicit _xmat4x3(const T x);
        explicit _xmat4x3(
            const T x0, const T y0, const T z0,
            const T x1, const T y1, const T z1,
            const T x2, const T y2, const T z2,
            const T x3, const T y3, const T z3);
        explicit _xmat4x3(
            const _xvec3<T>& v0, 
            const _xvec3<T>& v1,
            const _xvec3<T>& v2,
            const _xvec3<T>& v3);

        // Accesses
        _xvec3<T>& operator[](int i) {return value[i];}
        const _xvec3<T> & operator[](int i) const {return value[i];}
        operator T*() {return &value[0][0];}
        operator const T*() const {return &value[0][0];}

        // Unary updatable operators
        _xmat4x3<T>& operator= (const _xmat4x3<T>& m);
        _xmat4x3<T>& operator+= (const T s);
        _xmat4x3<T>& operator+= (const _xmat4x3<T>& m);
        _xmat4x3<T>& operator-= (const T s);
        _xmat4x3<T>& operator-= (const _xmat4x3<T>& m);
        _xmat4x3<T>& operator*= (const T s);
        _xmat4x3<T>& operator*= (const _xmat3x4<T>& m);
        _xmat4x3<T>& operator/= (const T s);
        // ToDo
        //_xmat4x3<T>& operator/= (const _xmat3x4<T>& m);
        _xmat4x3<T>& operator++ ();
        _xmat4x3<T>& operator-- ();

        // Unary constant operators
        const _xmat4x3<T> operator- () const;
        const _xmat4x3<T> operator++ (int n) const;
        const _xmat4x3<T> operator-- (int n) const;
    };

    // Binary operators
    template <typename T> 
    _xmat4x3<T> operator+ (const _xmat4x3<T>& m, const T s);
    
    template <typename T> 
    _xmat4x3<T> operator+ (const _xmat4x3<T>& m1, const _xmat4x3<T>& m2);
    
    template <typename T> 
    _xmat4x3<T> operator- (const _xmat4x3<T>& m, const T s);

    template <typename T> 
    _xmat4x3<T> operator- (const _xmat4x3<T>& m1, const _xmat4x3<T>& m2);

    template <typename T> 
    _xmat4x3<T> operator* (const _xmat4x3<T>& m, const T s);

    template <typename T> 
    _xmat4x3<T> operator* (const T s, const _xmat4x3<T>& m);

    template <typename T>
    _xvec3<T> operator* (const _xmat4x3<T>& m, const _xvec4<T>& v);

    template <typename T> 
    _xvec3<T> operator* (const _xvec4<T>& v, const _xmat4x3<T>& m);

    template <typename T> 
    _xmat4x3<T> operator* (const _xmat4x3<T>& m1, const _xmat4x3<T>& m2);

    template <typename T> 
    _xmat4x3<T> operator/ (const _xmat4x3<T>& m, const T s);

    template <typename T> 
    _xmat4x3<T> operator/ (const T s, const _xmat4x3<T>& m);
/*
    template <typename T> 
    _xvec3<T> operator/ (const _xmat4x3<T>& m, const _xvec4<T>& v);

    template <typename T> 
    _xvec3<T> operator/ (const _xvec4<T>& v, const _xmat4x3<T>& m);
*/
    template <typename T> 
    _xmat4x3<T> operator/ (const _xmat4x3<T>& m1, const _xmat4x3<T>& m2);
} //namespace glm

#endif//included

#endif //__mat4x3_h__
