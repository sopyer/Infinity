//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_quaternion.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////
// ToDo:
// - Study construction with angles and axis
// - Study construction with vec3 that are the imaginary component of quaternion
// - Include _xquat<T> quatGTX(const _xmat*<T>& x) into _xquat type as contructor
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_quaternion_h__
#define __gtx_quaternion_h__

#if (GLM_GTX_quaternion == GLM_EXTENSION_DISABLE && defined(GLM_GTX_quaternion_required))
#error GLM_GTX_quaternion is disable but required
#endif

// Dependency:
#include "../glm.h"
#define GLM_GTX_double_required 1
#include "../gtx/gtx_double.h"
#include <limits>

#if(GLM_INCLUDED(GLM_GTX_quaternion, GLM_EXTENSION_GTX, GLM_GTX_quaternion_required))

#define GLM_GTX_quaternion_supported 

namespace glm
{
    //! \brief Template for quaternion. (From GLM_GTX_quaternion extension)
    template <typename T> class _xquat
    {
    public:
        T x, y, z, w;

        // Constructors
        _xquat();
        explicit _xquat(const T s, const _xvec3<T>& v);
        explicit _xquat(const T w, const T x, const T y, const T z);

        // Convertions
        explicit _xquat(const T pitch, const T yaw, const T roll);
        explicit _xquat(const _xvec3<T>& eulerAngles);
        //explicit _xquat(const _xvec4<T>& x);
        explicit _xquat(const _xmat3<T>& m);
        explicit _xquat(const _xmat4<T>& m);
        //explicit _xquat(float angle, float x, float y, float z);
        //explicit _xquat(float angle, const _xvec3<T>& v);

        // Accesses
        T& operator[](int i);
        T operator[](int i) const;
        operator T*();
        operator const T*() const;

        // Operators
        _xquat<T>  operator+ (const _xquat<T>& q) const;
        _xquat<T>& operator+=(const _xquat<T>& q);
        _xquat<T>  operator- (const _xquat<T>& q) const;
        _xquat<T>& operator-=(const _xquat<T>& q);
        _xquat<T>  operator* (T s) const;
        _xquat<T>& operator*=(T s);
        _xquat<T>  operator* (const _xquat<T>& q) const;
        _xquat<T>& operator*=(const _xquat<T>& q);
        _xquat<T>  operator/ (T s) const;
        _xquat<T>& operator/=(T s);
        _xquat<T>  operator/ (const _xquat<T>& q) const;
        _xquat<T>& operator/=(const _xquat<T>& q);
        _xquat<T>  operator- () const;

        // Transformations
        _xvec3<T> operator* (const _xvec3<T>& v) const;
        _xvec4<T> operator* (const _xvec4<T>& v) const;
    };

    template <typename T> _xquat<T> operator* (const T s, const _xquat<T> & q)
    {
        return q * s;
    }

    template <typename T> T lengthGTX(const _xquat<T>& q); //!< \brief Returns the length of the quaternion x. (from GLM_GTX_quaternion extension)
    template <typename T> _xquat<T> normalizeGTX(const _xquat<T>& q); //!< \brief Returns the normalized quaternion of from x. (from GLM_GTX_quaternion extension)
    template <typename T> T dotGTX(const _xquat<T>& q1, const _xquat<T> & q2); //!< \brief Returns dot product of q1 and q2, i.e., q1[0] * q2[0] + q1[1] * q2[1] + ... (from GLM_GTX_quaternion extension)
    template <typename T> _xquat<T> crossGTX(const _xquat<T>& q1, const _xquat<T>& q2); //!< \brief Returns the cross product of q1 and q2. (from GLM_GTX_quaternion extension)
    //template <typename T> _xquat<T> crossGTX(const _xquat<T> & q, const _xvec3<T> & v);
    //template <typename T> _xquat<T> crossGTX(const _xvec3<T> & v, const _xquat<T> & q);
    template <typename T> _xquat<T> mixGTX(const _xquat<T>& x, const _xquat<T>& y, T a); //!< \brief Returns a LERP interpolated quaternion of x and y according a. (from GLM_GTX_quaternion extension)
    template <typename T> _xquat<T> expGTX(const _xquat<T>& q);
    template <typename T> _xquat<T> logGTX(const _xquat<T>& q);
    template <typename T> _xquat<T> powGTX(const _xquat<T>& x, T y); //!< \brief Returns x raised to the y power. (from GLM_GTX_quaternion extension)
    template <typename T> _xquat<T> conjugateGTX(const _xquat<T> & q); //!< \brief Returns the q conjugate. (from GLM_GTX_quaternion extension)
    template <typename T> _xquat<T> inverseGTX(const _xquat<T> & q); //!< \brief Returns the q inverse. (from GLM_GTX_quaternion extension)
    template <typename T> _xquat<T> rotateGTX(const _xquat<T> & q, T angle, T x, T y, T z); //!< /brief Rotates a quaternion from an axis of 3 scalars and an angle expressed in degrees (from GLM_GTX_transform extension)
	template <typename T> _xquat<T> rotateGTX(const _xquat<T> & q, T angle, const _xvec3<T>& v); //!< /brief Rotates a quaternion from an vector of 3 components axis and an angle expressed in degrees (from GLM_GTX_transform extension)
    template <typename T> _xvec3<T> rotateGTX(const _xquat<T> & q, const _xvec3<T> & v); //!< /brief Rotates a 3 components vector by a quaternion. (from GLM_GTX_transform extension)
    template <typename T> _xvec4<T> rotateGTX(const _xquat<T> & q, const _xvec4<T> & v); //!< /brief Rotates a 4 components vector by a quaternion. (from GLM_GTX_transform extension)
    //template <typename T> _xquat<T> squadGTX(const _xquat<T> & x, const _xquat<T> & y, float s1, float s2, float a);
    template <typename T> T angleGTX(const _xquat<T> & x); //!< \brief Returns the q rotation angle. (from GLM_GTX_quaternion extension)
    template <typename T> _xvec3<T> axisGTX(const _xquat<T> & x); //!< \brief Returns the q rotation axis. (from GLM_GTX_quaternion extension)
    template <typename T> _xmat3<T> mat3GTX(const _xquat<T> & x); //!< \brief Converts a quaternion to a 3 * 3 matrix. (from GLM_GTX_quaternion extension)
    template <typename T> _xmat4<T> mat4GTX(const _xquat<T> & x); //!< \brief Converts a quaternion to a 4 * 4 matrix. (from GLM_GTX_quaternion extension)
    template <typename T> _xquat<T> quatGTX(const _xmat3<T> & x); //!< \brief Converts a 3 * 3 matrix to a quaternion. (from GLM_GTX_quaternion extension)
    template <typename T> _xquat<T> quatGTX(const _xmat4<T> & x); //!< \brief Converts a 4 * 4 matrix to a quaternion. (from GLM_GTX_quaternion extension)
    template <typename T> _xquat<T> angleAxisGTX(T angle, T x, T y, T z); //!< Build a quaternion from an angle and an axis. (from GLM_GTX_quaternion extension)
    template <typename T> _xquat<T> angleAxisGTX(T angle, const _xvec3<T>& v); //!< Build a quaternion from an angle and an axis. (from GLM_GTX_quaternion extension)
    template <typename T> T extractRealComponentGTX(const _xquat<T>& q); //!< \brief Extract the real component of a quaternion. (from GLM_GTX_quaternion extension)

    typedef _xquat<float> __quatGTX; //!< \brief Quaternion of floating-point numbers. (from GLM_GTX_quaternion extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> T length(const _xquat<T>& q){return lengthGTX(q);}
    template <typename T> _xquat<T> normalize(const _xquat<T>& q){return normalizeGTX(q);}
    template <typename T> T dot(const _xquat<T>& q1, const _xquat<T> & q2){return dotGTX(q1, q2);}
    template <typename T> _xquat<T> cross(const _xquat<T>& q1, const _xquat<T>& q2){return crossGTX(q1, q2);}
    //template <typename T> _xquat<T> cross(const _xquat<T> & q, const _xvec3<T> & v){return crossGTX(q, v);}
    //template <typename T> _xquat<T> cross(const _xvec3<T> & v, const _xquat<T> & q){return crossGTX(v, q);}
    template <typename T> _xquat<T> mix(const _xquat<T>& x, const _xquat<T>& y, T a){return mixGTX(x, y, a);}
    template <typename T> _xquat<T> exp(const _xquat<T>& q){return expGTX(q);}
    template <typename T> _xquat<T> log(const _xquat<T>& q){return logGTX(q);}
    template <typename T> _xquat<T> pow(const _xquat<T>& x, T y){return powGTX(x, y);}
    template <typename T> _xquat<T> conjugate(const _xquat<T> & q){return conjugateGTX(q);}
    template <typename T> _xquat<T> inverse(const _xquat<T> & q){return inverseGTX(q);}
    template <typename T> _xquat<T> rotate(const _xquat<T> & q, T angle, T x, T y, T z){return rotateGTX(q, angle, x, y ,z);}
    template <typename T> _xquat<T> rotate(const _xquat<T> & q, T angle, const _xvec3<T>& v){return rotateGTX(q, angle, v);}
    template <typename T> _xvec3<T> rotate(const _xquat<T> & q, const _xvec3<T> & v){return rotateGTX(q, v);}
    template <typename T> _xvec4<T> rotate(const _xquat<T> & q, const _xvec4<T> & v){return rotateGTX(q, v);}
    //template <typename T> _xquat<T> squad(const _xquat<T> & x, const _xquat<T> & y, float s1, float s2, float a){return squadGTX(x, y, s1, s2, a);}
    template <typename T> T angle(const _xquat<T> & x){return angleGTX(x);}
    template <typename T> _xvec3<T> axis(const _xquat<T> & x){return axisGTX(x);}
    //template <typename T> _xmat3<T> mat3(const _xquat<T> & x){return mat3GTX(x);}
    //template <typename T> _xmat4<T> mat4(const _xquat<T> & x){return mat4GTX(x);}
    //template <typename T> _xquat<T> quat(const _xmat3<T> & x){return quatGTX(x);}
    //template <typename T> _xquat<T> quat(const _xmat4<T> & x){return quatGTX(x);}
    template <typename T> _xquat<T> angleAxis(T angle, T x, T y, T z){return angleAxisGTX(angle, x, y, z);}
    template <typename T> _xquat<T> angleAxis(T angle, const _xvec3<T>& v){return angleAxisGTX(angle, v);}
    template <typename T> T extractRealComponent(const _xquat<T>& q){return extractRealComponentGTX(q);}

    typedef _xquat<float> quat; //!< \brief Quaternion of floating-point numbers. (from GLM_GTX_quaternion extension)

#endif
}

#endif//included

#endif//__gtx_quaternion_h__
