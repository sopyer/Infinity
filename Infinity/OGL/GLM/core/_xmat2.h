//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-01-27
// Updated : 2006-04-19
// Licence : This source is under GNU LGPL licence
// File    : _mat2.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __mat2x2_h__
#define __mat2x2_h__

namespace glm
{
    template <typename T> class _xvec2;

	//!< \brief Template for 2 * 2 matrix of floating-point numbers. (From GLSL 1.10.59 specification)
    template <typename T> class _xmat2
    {
    private:
        // Data 
        _xvec2<T> value[2];

    private:
        _xmat2<T> _Inverse() const;

    public:
        // Constructors
	    _xmat2();
	    _xmat2(const _xmat2<T> & m);

        explicit _xmat2(const T x);
        explicit _xmat2(const T x1, const T y1, 
                        const T x2, const T y2);
	    explicit _xmat2(const _xvec2<T> & v1, 
                        const _xvec2<T> & v2);
        //explicit _xmat2(const T* a);
        // GL_GTX_euler_angles
        //explicit _xmat2(const _xvec2<T> & angles);

        // Accesses
        _xvec2<T>& operator[](int i) {return value[i];}
        const _xvec2<T> & operator[](int i) const {return value[i];}
        operator T*() {return &value[0][0];}
	    operator const T*() const {return &value[0][0];}

        // Unary updatable operators
        _xmat2<T>& operator=(const _xmat2<T>& m);
        _xmat2<T>& operator+=(const T s);
	    _xmat2<T>& operator+=(const _xmat2<T>& m);
        _xmat2<T>& operator-=(const T s);
	    _xmat2<T>& operator-=(const _xmat2<T>& m);
	    _xmat2<T>& operator*=(const T s);
	    _xmat2<T>& operator*= (const _xmat2<T>& m);
        _xmat2<T>& operator/= (const T s);
	    _xmat2<T>& operator/= (const _xmat2<T>& m);
        _xmat2<T>& operator++ ();
	    _xmat2<T>& operator-- ();

        const _xmat2<T> operator- () const;
        const _xmat2<T> operator++ (int n) const;
        const _xmat2<T> operator-- (int n) const;
    };

	// Binary operators
    template <typename T> 
    inline _xmat2<T> operator+ (const _xmat2<T>& m, const T s);

    template <typename T> 
    inline _xmat2<T> operator+ (const T s, const _xmat2<T>& m);

    template <typename T> 
    inline _xvec2<T> operator+ (const _xmat2<T>& m, const _xvec2<T>& v);

    template <typename T> 
    inline _xvec2<T> operator+ (const _xvec2<T>& v, const _xmat2<T>& m);
  
    template <typename T> 
    inline _xmat2<T> operator+ (const _xmat2<T>& m1, const _xmat2<T>& m2);
    
    template <typename T> 
    inline _xmat2<T> operator- (const _xmat2<T>& m, const T s);

    template <typename T> 
    inline _xmat2<T> operator- (const T s, const _xmat2<T>& m);

    template <typename T> 
    inline _xvec2<T> operator- (const _xmat2<T>& m, const _xvec2<T>& v);

    template <typename T> 
    inline _xvec2<T> operator- (const _xvec2<T>& v, const _xmat2<T>& m);

    template <typename T> 
    inline _xmat2<T> operator- (const _xmat2<T>& m1, const _xmat2<T>& m2);

    template <typename T> 
    inline _xmat2<T> operator* (const _xmat2<T>& m, const T s);

    template <typename T> 
    inline _xmat2<T> operator* (const T s, const _xmat2<T>& m);

    template <typename T> 
    inline _xvec2<T> operator* (const _xmat2<T>& m, const _xvec2<T>& v);

    template <typename T> 
    inline _xvec2<T> operator* (const _xvec2<T>& v, const _xmat2<T>& m);

    template <typename T> 
    inline _xmat2<T> operator* (const _xmat2<T>& m1, const _xmat2<T>& m2);

    template <typename T> 
    inline _xmat2<T> operator/ (const _xmat2<T>& m, const T s);

    template <typename T> 
    inline _xmat2<T> operator/ (const T s, const _xmat2<T>& m);

    template <typename T> 
    inline _xvec2<T> operator/ (const _xmat2<T>& m, const _xvec2<T>& v);

    template <typename T> 
    inline _xvec2<T> operator/ (const _xvec2<T>& v, const _xmat2<T>& m);

    template <typename T> 
    inline _xmat2<T> operator/ (const _xmat2<T>& m1, const _xmat2<T>& m2);

	// Unary constant operators
    template <typename T> 
    inline const _xmat2<T> operator- (const _xmat2<T>& m);

    template <typename T> 
    inline const _xmat2<T> operator-- (const _xmat2<T>& m, int);

    template <typename T> 
    inline const _xmat2<T> operator++ (const _xmat2<T>& m, int);

} //namespace glm

#endif //__mat2x2_h__
