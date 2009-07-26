//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-10-01
// Updated : 2006-10-01
// Licence : This source is under GNU LGPL licence
// File    : _mat2x3.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __mat2x3_h__
#define __mat2x3_h__

namespace glm
{
    template <typename T> class _xvec2;
    template <typename T> class _xvec3;
    template <typename T> class _xvec4;
    template <typename T> class _xmat2;
    template <typename T> class _xmat3;
    template <typename T> class _xmat4;
    template <typename T> class _xmat2x4;
    template <typename T> class _xmat3x2;
    template <typename T> class _xmat3x4;
    template <typename T> class _xmat4x2;
    template <typename T> class _xmat4x3;

    //!< \brief Template for 2 * 3 matrix of floating-point numbers. (From GLSL 1.20.6 specification)
    template <typename T> class _xmat2x3
    {
    private:
        // Data
        _xvec3<T> value[2];

    public:
        // Constructors
        _xmat2x3();
        explicit _xmat2x3(const T x);
        explicit _xmat2x3(
            const T x0, const T y0, const T z0,
            const T x1, const T y1, const T z1);
        explicit _xmat2x3(
            const _xvec3<T>& v0, 
            const _xvec3<T>& v1);

        // Conversion
        explicit _xmat2x3(const _xmat2<T>& x);
        explicit _xmat2x3(const _xmat3<T>& x);
        explicit _xmat2x3(const _xmat4<T>& x);
        explicit _xmat2x3(const _xmat2x3<T>& x);
        explicit _xmat2x3(const _xmat2x4<T>& x);
        explicit _xmat2x3(const _xmat3x2<T>& x);
        explicit _xmat2x3(const _xmat3x4<T>& x);
        explicit _xmat2x3(const _xmat4x2<T>& x);
        explicit _xmat2x3(const _xmat4x3<T>& x);

        // Accesses
        _xvec3<T>& operator[](int i) {return value[i];}
        const _xvec3<T> & operator[](int i) const {return value[i];}
        operator T*() {return &value[0][0];}
        operator const T*() const {return &value[0][0];}

        // Unary updatable operators
        _xmat2x3<T>& operator=  (const _xmat2x3<T>& m);
        _xmat2x3<T>& operator+= (const T s);
        _xmat2x3<T>& operator+= (const _xmat2x3<T>& m);
        _xmat2x3<T>& operator-= (const T s);
        _xmat2x3<T>& operator-= (const _xmat2x3<T>& m);
        _xmat2x3<T>& operator*= (const T s);
        _xmat2x3<T>& operator*= (const _xmat3x2<T>& m);
        _xmat2x3<T>& operator/= (const T s);
/* ToDo
        _xmat2x3<T>& operator/= (const _xmat3x2<T>& m);
*/
        _xmat2x3<T>& operator++ ();
        _xmat2x3<T>& operator-- ();

        // Unary constant operators
        const _xmat2x3<T> operator- () const;
        const _xmat2x3<T> operator++ (int n) const;
        const _xmat2x3<T> operator-- (int n) const;
    };

    // Binary operators
    template <typename T> 
    _xmat2x3<T> operator+ (const _xmat2x3<T>& m, const T s);
    
    template <typename T> 
    _xmat2x3<T> operator+ (const _xmat2x3<T>& m1, const _xmat2x3<T>& m2);
    
    template <typename T> 
    _xmat2x3<T> operator- (const _xmat2x3<T>& m, const T s);

    template <typename T> 
    _xmat2x3<T> operator- (const _xmat2x3<T>& m1, const _xmat2x3<T>& m2);

    template <typename T> 
    _xmat2x3<T> operator* (const _xmat2x3<T>& m, const T s);

    template <typename T> 
    _xmat2x3<T> operator* (const T s, const _xmat2x3<T>& m);

    template <typename T>
    _xvec3<T> operator* (const _xmat2x3<T>& m, const _xvec2<T>& v);

    template <typename T> 
    _xvec3<T> operator* (const _xvec3<T>& v, const _xmat2x3<T>& m);

    template <typename T>
    _xmat3<T> operator* (const _xmat2x3<T>& m1, const _xmat3x2<T>& m2);

    template <typename T> 
    _xmat3x2<T> operator/ (const _xmat2x3<T>& m, const T s);

    template <typename T> 
    _xmat3x2<T> operator/ (const T s, const _xmat2x3<T>& m);
/* ToDo
    template <typename T> 
    _xvec3<T> operator/ (const _xmat2x3<T>& m, const _xvec2<T>& v);

    template <typename T> 
    _xvec2<T> operator/ (const _xvec3<T>& v, const _xmat2x3<T>& m);

    template <typename T> 
    _xmat3<T> operator/ (const _xmat3x2<T>& m1, const _xmat2x3<T>& m2);
*/
} //namespace glm

#endif //__mat2x3_h__
