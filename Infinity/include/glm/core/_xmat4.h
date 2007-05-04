//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-01-27
// Updated : 2006-04-19
// Licence : This source is under GNU LGPL licence
// File    : _xmat4.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __xmat4_h__
#define __xmat4_h__

namespace glm
{
    template <typename T> class _xvec3;
    template <typename T> class _xvec4;
    template <typename T> class _xquat;

	//!< \brief Template for 4 * 4 matrix of floating-point numbers. (From GLSL 1.10.59 specification)
    template <typename T> class _xmat4
    {
    private:
        // Data
        _xvec4<T> value[4];

    private:
        _xmat4<T> _Inverse() const;

    public:
        // Constructor
        _xmat4();
        explicit _xmat4(const T x);
        explicit _xmat4(
            const T x0, const T y0, const T z0, const T w0,
            const T x1, const T y1, const T z1, const T w1,
            const T x2, const T y2, const T z2, const T w2,
            const T x3, const T y3, const T z3, const T w3);
        explicit _xmat4(const _xvec4<T> & v0, 
            const _xvec4<T> & v1,
            const _xvec4<T> & v2,
            const _xvec4<T> & v3);
        //explicit _xmat4(const T* a);
        // GL_GTX_euler_angles
        //explicit _xmat4(const _xvec3<T> & angle);

        // Conversion
        //explicit _xmat4(const glm::_xquat<T> & q);

        // Accesses
        _xvec4<T>& operator[](int i) {return value[i];}
        const _xvec4<T> & operator[](int i) const {return value[i];}
        operator T*() {return &value[0][0];}
        operator const T*() const {return &value[0][0];}

        // Unary updatable operators
        _xmat4<T>& operator= (const _xmat4<T>& m);
        _xmat4<T>& operator+= (const T s);
        _xmat4<T>& operator+= (const _xmat4<T>& m);
        _xmat4<T>& operator-= (const T s);
        _xmat4<T>& operator-= (const _xmat4<T>& m);
        _xmat4<T>& operator*= (const T s);
        _xmat4<T>& operator*= (const _xmat4<T>& m);
        _xmat4<T>& operator/= (const T s);
        _xmat4<T>& operator/= (const _xmat4<T>& m);
        _xmat4<T>& operator++ ();
        _xmat4<T>& operator-- ();
    };

	// Binary operators
    template <typename T> 
    inline _xmat4<T> operator+ (const _xmat4<T>& m, const T s);

    template <typename T> 
    inline _xmat4<T> operator+ (const T s, const _xmat4<T>& m);

    template <typename T> 
    inline _xvec4<T> operator+ (const _xmat4<T>& m, const _xvec4<T>& v);

    template <typename T> 
    inline _xvec4<T> operator+ (const _xvec4<T>& v, const _xmat4<T>& m);
 
    template <typename T> 
    inline _xmat4<T> operator+ (const _xmat4<T>& m1, const _xmat4<T>& m2);
    
    template <typename T> 
    inline _xmat4<T> operator- (const _xmat4<T>& m, const T s);

    template <typename T> 
    inline _xmat4<T> operator- (const T s, const _xmat4<T>& m);

    template <typename T> 
    inline _xvec4<T> operator- (const _xmat4<T>& m, const _xvec4<T>& v);

    template <typename T> 
    inline _xvec4<T> operator- (const _xvec4<T>& v, const _xmat4<T>& m);

    template <typename T> 
    inline _xmat4<T> operator- (const _xmat4<T>& m1, const _xmat4<T>& m2);

    template <typename T> 
    inline _xmat4<T> operator* (const _xmat4<T>& m, const T s);

    template <typename T> 
    inline _xmat4<T> operator* (const T s, const _xmat4<T>& m);

    template <typename T> 
    inline _xvec4<T> operator* (const _xmat4<T>& m, const _xvec4<T>& v);

    template <typename T> 
    inline _xvec4<T> operator* (const _xvec4<T>& v, const _xmat4<T>& m);

    template <typename T> 
    inline _xmat4<T> operator* (const _xmat4<T>& m1, const _xmat4<T>& m2);

    template <typename T> 
    inline _xmat4<T> operator/ (const _xmat4<T>& m, const T s);

    template <typename T> 
    inline _xmat4<T> operator/ (const T s, const _xmat4<T>& m);

    template <typename T> 
    inline _xvec4<T> operator/ (const _xmat4<T>& m, const _xvec4<T>& v);

    template <typename T> 
    inline _xvec4<T> operator/ (const _xvec4<T>& v, const _xmat4<T>& m);

    template <typename T> 
    inline _xmat4<T> operator/ (const _xmat4<T>& m1, const _xmat4<T>& m2);

	// Unary constant operators
    template <typename T> 
    inline const _xmat4<T> operator- (const _xmat4<T>& m);

    template <typename T> 
    inline const _xmat4<T> operator-- (const _xmat4<T>& m, int);

    template <typename T> 
    inline const _xmat4<T> operator++ (const _xmat4<T>& m, int);

} //namespace glm

#endif //__xmat4_h__
