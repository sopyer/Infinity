//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-12-06
// Licence : This source is under GNU LGPL licence
// File    : gtx_quaternion.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_quaternion_inl__
#define __gtx_quaternion_inl__

#include "gtx_quaternion.h"
#if(GLM_INCLUDED(GLM_GTX_quaternion, GLM_EXTENSION_GTX, GLM_GTX_quaternion_required))

namespace glm
{
    template <typename T> 
    inline _xquat<T>::_xquat() : 
        w(1),
        x(0),
        y(0),
        z(0)
    {}

    template <typename T> 
    inline _xquat<T>::_xquat(const T s, const _xvec3<T>& v) : 
        w(s),
        x(v.x),
        y(v.y),
        z(v.z)
    {}

    template <typename T> 
    inline _xquat<T>::_xquat(const T w, const T x, const T y, const T z) :
        w(w),
        x(x),
        y(y),
        z(z)
    {}

    //////////////////////////////////////////////////////////////
    // __quatGT conversions

	template <typename T> 
	inline _xquat<T>::_xquat(const T pitch, const T yaw, const T roll)
	{
		_xvec3<T> eulerAngle(pitch * T(0.5), yaw * T(0.5), roll * T(0.5));
		_xvec3<T> c = glm::cos(eulerAngle * T(0.5));
		_xvec3<T> s = glm::sin(eulerAngle * T(0.5));
		
		this->w = c.x * c.y * c.z + s.x * s.y * s.z;
		this->x = s.x * c.y * c.z - c.x * s.y * s.z;
		this->y = c.x * s.y * c.z + s.x * c.y * s.z;
		this->z = c.x * c.y * s.z - s.x * s.y * c.z;
	}

	template <typename T> 
	inline _xquat<T>::_xquat(const _xvec3<T>& eulerAngle)
	{
		_xvec3<T> c = glm::cos(eulerAngle * T(0.5));
		_xvec3<T> s = glm::sin(eulerAngle * T(0.5));
		
		this->w = c.x * c.y * c.z + s.x * s.y * s.z;
		this->x = s.x * c.y * c.z - c.x * s.y * s.z;
		this->y = c.x * s.y * c.z + s.x * c.y * s.z;
		this->z = c.x * c.y * s.z - s.x * s.y * c.z;		
	}

    /*
    template <typename T> 
    inline glm::_xquat<T>::_xquat(const _xvec4<T>& x) :
        w(x.w),
        x(x.x),
        y(x.y),
        z(x.z)
    {}
    */

    template <typename T> 
    inline _xquat<T>::_xquat(const _xmat3<T>& m)
    {
        T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
        T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
        T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
        T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];
        
        int biggestIndex = 0;
        T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
        if(fourXSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourXSquaredMinus1;
            biggestIndex = 1;
        }
        if(fourYSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourYSquaredMinus1;
            biggestIndex = 2;
        }
        if(fourZSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourZSquaredMinus1;
            biggestIndex = 3;
        }

        T biggestVal = glm::sqrt(fourBiggestSquaredMinus1 + T(1)) * T(0.5);
        T mult = T(0.25) / biggestVal;

        switch(biggestIndex)
        {
        case 0:
            this->x = (m[1][2] - m[2][1]) * mult;
            this->y = (m[2][0] - m[0][2]) * mult;
            this->z = (m[0][1] - m[1][0]) * mult;
            this->w = biggestVal;
            break;
        case 1:
            this->x = biggestVal;
            this->y = (m[0][1] + m[1][0]) * mult;
            this->z = (m[2][1] + m[1][2]) * mult;
            this->w = (m[1][2] - m[2][1]) * mult;
            break;
        case 2:
            this->x = (m[0][1] + m[1][0]) * mult;
            this->y = biggestVal;
            this->z = (m[1][2] + m[2][1]) * mult;
            this->w = (m[2][0] - m[0][2]) * mult;
            break;
        case 3:
            this->x = (m[2][0] + m[0][2]) * mult;
            this->y = (m[1][2] + m[2][1]) * mult;
            this->z = biggestVal;
            this->w = (m[0][1] - m[1][0]) * mult;
            break;
        }
    }

    template <typename T> 
    inline _xquat<T>::_xquat(const _xmat4<T>& m)
    {
        T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
        T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
        T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
        T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];
        
        int biggestIndex = 0;
        T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
        if(fourXSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourXSquaredMinus1;
            biggestIndex = 1;
        }
        if(fourYSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourYSquaredMinus1;
            biggestIndex = 2;
        }
        if(fourZSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourZSquaredMinus1;
            biggestIndex = 3;
        }

        T biggestVal = glm::sqrt(fourBiggestSquaredMinus1 + T(1)) * T(0);
        T mult = T(0.25) / biggestVal;

        switch(biggestIndex)
        {
        case 0:
            this->x = (m[1][2] - m[2][1]) * mult;
            this->y = (m[2][0] - m[0][2]) * mult;
            this->z = (m[0][1] - m[1][0]) * mult;
            this->w = biggestVal;
            break;
        case 1:
            this->x = biggestVal;
            this->y = (m[0][1] + m[1][0]) * mult;
            this->z = (m[2][1] + m[1][2]) * mult;
            this->w = (m[1][2] - m[2][1]) * mult;
            break;
        case 2:
            this->x = (m[0][1] + m[1][0]) * mult;
            this->y = biggestVal;
            this->z = (m[1][2] + m[2][1]) * mult;
            this->w = (m[2][0] - m[0][2]) * mult;
            break;
        case 3:
            this->x = (m[2][0] + m[0][2]) * mult;
            this->y = (m[1][2] + m[2][1]) * mult;
            this->z = biggestVal;
            this->w = (m[0][1] - m[1][0]) * mult;
            break;
        }
    }
    /*
    template <typename T> 
    inline _xquat<T>::_xquat(float angle, float x, float y, float z)
    {
        // Axis of rotation must be normalised
        float len = glm::length(glm::_xvec3<T>(x, y, z));
        if(glm::abs(len - 1.0f) > 0.001f)
        {
            float oneOverLen = 1 / len;
            x *= oneOverLen;
            y *= oneOverLen;
            z *= oneOverLen;
        }

        angle = glm::radians(angle);
        float fSin = glm::sin(angle * 0.5f);

        this->x = x * fSin;
        this->y = y * fSin;
        this->z = z * fSin;
        this->w = glm::cos(angle * 0.5f));
    }

    template <typename T> 
    inline _xquat<T>::_xquat(float angle, const glm::_xvec3<T>& v)
    {
        glm::_xvec3<T> v_normalized = glm::normalize(v);

        angle = glm::radians(angle);
        float fSin = glm::sin(angle * 0.5f);

        this->x = v.x * fSin;
        this->y = v.y * fSin;
        this->z = v.z * fSin;
        this->w = glm::cos(angle * 0.5f));
    }
    */
    //////////////////////////////////////////////////////////////
    // __quatGT accesses

    template <typename T> 
    inline T& _xquat<T>::operator [] (int i)
    {
        return (&x)[i];
    }

    template <typename T> 
    inline T _xquat<T>::operator [] (int i) const
    {
        return (&x)[i];
    }

    template <typename T> 
    inline _xquat<T>::operator T* ()
    {
        return &x;
    }

    template <typename T> 
    inline _xquat<T>::operator const T* () const 
    {
        return &x;
    }

    //////////////////////////////////////////////////////////////
    // __quatGT Operators

    template <typename T> 
    inline _xquat<T> _xquat<T>::operator +(const _xquat<T>& q) const
    {
        return glm::_xquat<T>(
            this->w + q.w,
            this->x + q.x,
            this->y + q.y,
            this->z + q.z);
    }

    template <typename T> 
    inline _xquat<T>& _xquat<T>::operator +=(const _xquat<T>& q)
    {
        return *this = *this + q;
    }

    template <typename T> 
    inline _xquat<T> _xquat<T>::operator -(const _xquat<T>& q) const
    {
        return _xquat<T>(
            this->w - q.w,
            this->x - q.x,
            this->y - q.y,
            this->z - q.z);
    }

    template <typename T> 
    inline _xquat<T>& _xquat<T>::operator -=(const _xquat<T>& q)
    {
        return *this = *this - q;
    }

    template <typename T> 
    inline _xquat<T> _xquat<T>::operator *(T s) const
    {
        return _xquat<T>(
            this->w * s,
            this->x * s, 
            this->y * s,
            this->z * s);
    }

    template <typename T> 
    inline _xquat<T>& _xquat<T>::operator *=(T s)
    {
        this->w *= s;
        this->x *= s;
        this->y *= s;
        this->z *= s;
        return *this;
    }

    template <typename T>
    inline _xquat<T> _xquat<T>::operator* (const _xquat<T>& q) const
    {
        return _xquat<T>(this->w * q.w, this->x * q.x, this->y * q.y, this->z * q.z);
    }

    template <typename T> 
    inline _xquat<T>& _xquat<T>::operator *=(const _xquat<T>& q)
    {
        return *this = *this * q;
    }

    template <typename T> 
    inline _xquat<T> _xquat<T>::operator /(T s) const
    {
        return glm::_xquat<T>(
            this->w / s,
            this->x / s, 
            this->y / s, 
            this->z / s);
    }

    template <typename T> 
    inline _xquat<T>& _xquat<T>::operator /=(T s)
    {
        this->w /= s;
        this->x /= s;
        this->y /= s;
        this->z /= s;
        return *this;
    }

    template <typename T> 
    inline _xquat<T> _xquat<T>::operator /(const _xquat<T>& q) const
    {
        return *this * inverseGTX(q);
    }

    template <typename T> 
    inline _xquat<T>& _xquat<T>::operator /=(const _xquat<T>& q)
    {
        return *this = *this * glm::inverseGTX(q);
    }

    template <typename T>
    inline _xquat<T> _xquat<T>::operator- () const
    {
        return _xquat<T>(-this->w, -this->x, -this->y, -this->z);
    }

    // Transformation
    template <typename T>
    inline _xvec3<T> _xquat<T>::operator* (const _xvec3<T>& v) const
    {
        _xvec3<T> uv, uuv;
	    _xvec3<T> QuatVector(x, y, z);
        uv = cross(QuatVector, v);
        uuv = cross(QuatVector, uv);
	    uv *= (T(2) * w); 
	    uuv *= T(2); 

	    return v + uv + uuv;
    }

    template <typename T>
    inline _xvec4<T> _xquat<T>::operator* (const _xvec4<T>& v) const
    {
        _xvec3<T> uv, uuv;
	    _xvec3<T> QuatVector(x, y, z);
        uv = glm::cross(QuatVector, _xvec3<T>(v));
        uuv = glm::cross(QuatVector, uv);
	    uv *= (T(2) * w); 
	    uuv *= T(2); 

	    return _xvec4<T>(v + uv + uuv, v.w);
    }

    ////////////////////////////////////////////////////////
    template <typename T> 
    inline T lengthGTX(const _xquat<T> & q)
    {
        return static_cast<T>(glm::sqrt(
            q.x * q.x + 
            q.y * q.y + 
            q.z * q.z + 
            q.w * q.w));
    }

    template <typename T> 
    inline _xquat<T> normalizeGTX(const _xquat<T>& q)
    {
        T len = static_cast<T>(glm::length(q));
        if(len <= 0) // Problem
            return glm::_xquat<T>(1, 0, 0, 0);
        T oneOverLen = 1 / len;
        return glm::_xquat<T>(q.w * oneOverLen, q.x * oneOverLen, q.y * oneOverLen, q.z * oneOverLen);
    }

    template <typename T> 
    inline T dotGTX(const _xquat<T>& q1, const _xquat<T>& q2)
    {
        return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
    }

    template <typename T> 
    inline _xquat<T> crossGTX(const _xquat<T>& q1, const _xquat<T>& q2)
    {
        return _xquat<T>(
            q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
	        q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
	        q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
	        q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x);
    }
    /*
    template <typename T> 
    inline glm::_xquat<T> crossGTX(const glm::_xquat<T> & q, const glm::_xvec3<T> & v)
    {
        return glm::_xquat<T>(
            (q.w * v.x) + (q.y * v.z) - (q.z * v.y),
            (q.w * v.y) + (q.z * v.x) - (q.x * v.z),
            (q.w * v.z) + (q.x * v.y) - (q.y * v.x),
        -(q.x * v.x) - (q.y * v.y) - (q.z * v.z));
    }
    */
    /*
    template <typename T> 
    inline glm::_xquat<T> crossGTX(const glm::_xvec3<T> & v, const glm::_xquat<T> & q)
    {

    }
    */
    template <typename T>
    inline _xquat<T> mixGTX(const _xquat<T>& x, const _xquat<T>& y, T a)
    {
        if(a <= T(0)) return x;
        if(a >= T(1)) return y;

        float fCos = glm::dot(x, y);
        glm::_xquat<T> y2(y); //BUG!!! glm::_xquat<T> y2;
        if(fCos < T(0))
        {
            y2 = -y;
            fCos = -fCos;
        }

        //if(fCos > 1.0f) // problem
        float k0, k1;
        if(fCos > T(0.9999))
        {
            k0 = T(1) - a;
            k1 = T(0) + a; //BUG!!! 1.0f + a;
        }
        else
        {
            T fSin = glm::sqrt(T(1) - fCos * fCos);
            T fAngle = glm::atan(fSin, fCos);
            T fOneOverSin = T(1) / fSin;
            k0 = glm::sin((T(1) - a) * fAngle) * fOneOverSin;
            k1 = glm::sin((T(0) + a) * fAngle) * fOneOverSin;
        }

        return glm::_xquat<T>(
            k0 * x.w + k1 * y2.w,
            k0 * x.x + k1 * y2.x,
            k0 * x.y + k1 * y2.y,
            k0 * x.z + k1 * y2.z);
    }

    template <typename T> 
    _xquat<T> expGTX(const _xquat<T>& q)
    {
        _xvec3<T> u(q.x, q.y, q.z);
        float a = length(u);
        _xvec3<T> v(u / a);
        return _xquat<T>(cos(a), sin(a) * v);
    }

    template <typename T> 
    _xquat<T> logGTX(const _xquat<T>& q)
    {
        if((q.x == T(0)) && (q.y == T(0)) && (q.z == T(0)))
        {
            if(q.w > T(0))
                return _xquat<T>(log(q.w), T(0), T(0), T(0));
            else if(q.w < T(0))
                return _xquat<T>(log(-q.w), T(3.1415926535897932384626433832795), T(0),T(0));
            else
                return _xquat<T>(std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity());
        } 
        else 
        {
            T Vec3Len = glm::sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
            T QuatLen = glm::sqrt(Vec3Len * Vec3Len + q.w * q.w);
            T t = glm::atan(Vec3Len, T(q.w)) / Vec3Len;
            return _xquat<T>(t * q.x, t * q.y, t * q.z, log(QuatLen));
        }
    }

    template <typename T> 
    inline _xquat<T> powGTX(const _xquat<T>& x, T y)
    {
        if(glm::abs(x.w) > T(0.9999))
            return x;
        float Angle = glm::acos(y);
        float NewAngle = Angle * y;
        float Div = glm::sin(NewAngle) / glm::sin(Angle);
        return glm::_xquat<T>(
            glm::cos(NewAngle),
            x.x * Div,
            x.y * Div,
            x.z * Div);
    }

    template <typename T> 
    inline _xquat<T> conjugateGTX(const _xquat<T>& q)
    {
        return _xquat<T>(q.w, -q.x, -q.y, -q.z);
    }

    template <typename T> 
    inline _xquat<T> inverseGTX(const _xquat<T>& q)
    {
        return conjugateGTX(q) / length(q);
    }

    template <typename T> 
    inline _xquat<T> rotateGTX(const _xquat<T>& q, T angle, T x, T y, T z)
    {
        // Axis of rotation must be normalised
        T len = glm::length(glm::_xvec3<T>(x, y, z));
        if(glm::abs(len - T(1)) > T(0.001))
        {
            T oneOverLen = T(1) / len;
            x *= oneOverLen;
            y *= oneOverLen;
            z *= oneOverLen;
        }

        angle = glm::radians(angle);
        T fSin = glm::sin(angle * T(0.5));

        return glm::_xquat<T>(glm::cos(angle * T(0.5)), x * fSin, y * fSin, z * fSin);
    }

    template <typename T> 
    inline _xvec3<T> rotateGTX(const _xquat<T>& q, const _xvec3<T>& v)
    {
        _xquat<T> Result = q * v * inverseGTX(q);
        return _xvec3<T>(Result.x, Result.y, Result.z);
    }

    template <typename T> 
    inline _xvec4<T> rotateGTX(const _xquat<T>& q, const _xvec4<T>& v)
    {
        _xquat<T> Result = q * v * inverseGTX(q);
        return _xvec4<T>(v.w, Result.x, Result.y, Result.z);
    }

    template <typename T> 
    inline T angleGTX(const _xquat<T>& x)
    {
        return glm::cos(x.w) * T(2);
    }

    template <typename T> 
    inline _xvec3<T> axisGTX(const _xquat<T>& x)
    {
        T tmp1 = T(1) - x.w * x.w;
        if(tmp1 <= T(0))
            return glm::_xvec3<T>(0, 0, 1);
        T tmp2 = T(1) / glm::sqrt(tmp1);
        return glm::_xvec3<T>(x.x * tmp2, x.y * tmp2, x.z * tmp2);
    }

    template <typename T> 
    inline _xmat3<T> mat3GTX(const _xquat<T>& q)
    {
        _xmat3<T> Result(1);
        Result[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
        Result[0][1] = 2 * q.x * q.y + 2 * q.w * q.z;
        Result[0][2] = 2 * q.x * q.z - 2 * q.w * q.y;

        Result[1][0] = 2 * q.x * q.y - 2 * q.w * q.z;
        Result[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
        Result[1][2] = 2 * q.y * q.z + 2 * q.w * q.x;

        Result[2][0] = 2 * q.x * q.z + 2 * q.w * q.y;
        Result[2][1] = 2 * q.y * q.z - 2 * q.w * q.x;
        Result[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
        return Result;
    }

    template <typename T> 
    inline _xmat4<T> mat4GTX(const _xquat<T>& q)
    {
        _xmat4<T> Result = _xmat4<T>(1);
        Result[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
        Result[0][1] = 2 * q.x * q.y + 2 * q.w * q.z;
        Result[0][2] = 2 * q.x * q.z - 2 * q.w * q.y;

        Result[1][0] = 2 * q.x * q.y - 2 * q.w * q.z;
        Result[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
        Result[1][2] = 2 * q.y * q.z + 2 * q.w * q.x;

        Result[2][0] = 2 * q.x * q.z + 2 * q.w * q.y;
        Result[2][1] = 2 * q.y * q.z - 2 * q.w * q.x;
        Result[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
        return Result;
    }

    template <typename T> 
    inline _xquat<T> quatGTX(const _xmat3<T>& m)
    {
        T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
        T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
        T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
        T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];
        
        int biggestIndex = 0;
        T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
        if(fourXSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourXSquaredMinus1;
            biggestIndex = 1;
        }
        if(fourYSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourYSquaredMinus1;
            biggestIndex = 2;
        }
        if(fourZSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourZSquaredMinus1;
            biggestIndex = 3;
        }

        T biggestVal = glm::sqrt(fourBiggestSquaredMinus1 + 1) * T(0.5);
        T mult = T(0.25) / biggestVal;

        glm::_xquat<T> Result;
        switch(biggestIndex)
        {
        case 0:
            Result.w = biggestVal; 
            Result.x = (m[1][2] - m[2][1]) * mult;
            Result.y = (m[2][0] - m[0][2]) * mult;
            Result.z = (m[0][1] - m[1][0]) * mult;
            break;
        case 1:
            Result.w = (m[1][2] - m[2][1]) * mult;
            Result.x = biggestVal;
            Result.y = (m[0][1] + m[1][0]) * mult;
            Result.z = (m[2][1] + m[1][2]) * mult;
            break;
        case 2:
            Result.w = (m[2][0] - m[0][2]) * mult;
            Result.x = (m[0][1] + m[1][0]) * mult;
            Result.y = biggestVal;
            Result.z = (m[1][2] + m[2][1]) * mult;
            break;
        case 3:
            Result.w = (m[0][1] - m[1][0]) * mult;
            Result.x = (m[2][0] + m[0][2]) * mult;
            Result.y = (m[1][2] + m[2][1]) * mult;
            Result.z = biggestVal;
            break;
        }
        return Result;
    }

    template <typename T> 
    inline _xquat<T> quatGTX(const _xmat4<T>& m)
    {
        T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];
        T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
        T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
        T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
        
        int biggestIndex = 0;
        T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
        if(fourXSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourXSquaredMinus1;
            biggestIndex = 1;
        }
        if(fourYSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourYSquaredMinus1;
            biggestIndex = 2;
        }
        if(fourZSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourZSquaredMinus1;
            biggestIndex = 3;
        }

        T biggestVal = glm::sqrt(fourBiggestSquaredMinus1 + 1) * T(0.5);
        T mult = T(0.25) / biggestVal;

        glm::_xquat<T> Result;
        switch(biggestIndex)
        {
        case 0:
            Result.w = biggestVal;
            Result.x = (m[1][2] - m[2][1]) * mult;
            Result.y = (m[2][0] - m[0][2]) * mult;
            Result.z = (m[0][1] - m[1][0]) * mult;
            break;
        case 1:
            Result.w = (m[1][2] - m[2][1]) * mult;
            Result.x = biggestVal;
            Result.y = (m[0][1] + m[1][0]) * mult;
            Result.z = (m[2][1] + m[1][2]) * mult;
            break;
        case 2:
            Result.w = (m[2][0] - m[0][2]) * mult;
            Result.x = (m[0][1] + m[1][0]) * mult;
            Result.y = biggestVal;
            Result.z = (m[1][2] + m[2][1]) * mult;
            break;
        case 3:
            Result.w = (m[0][1] - m[1][0]) * mult;
            Result.x = (m[2][0] + m[0][2]) * mult;
            Result.y = (m[1][2] + m[2][1]) * mult;
            Result.z = biggestVal;
            break;
        }
        return Result;
    }

    template <typename T> 
    inline _xquat<T> angleAxisGTX(T angle, T x, T y, T z)
    {
        _xquat<T> result;
        // Axis of rotation must be normalised
        float len = glm::length(glm::_xvec3<T>(x, y, z));
        if(glm::abs(len - T(1)) > T(0.001))
        {
            T oneOverLen = T(1) / len;
            x *= oneOverLen;
            y *= oneOverLen;
            z *= oneOverLen;
        }

        angle = glm::radians(angle);
        T fSin = glm::sin(angle * T(0.5));

        result.w = glm::cos(angle * T(0.5));
        result.x = x * fSin;
        result.y = y * fSin;
        result.z = z * fSin;
        return result;
    }

    template <typename T> 
    inline _xquat<T> angleAxisGTX(T angle, const _xvec3<T>& v)
    {
        glm::_xquat<T> result;
        glm::_xvec3<T> v_normalized = glm::normalize(v);

        angle = glm::radians(angle);
        T fSin = glm::sin(angle * T(0.5));

        result.w = glm::cos(angle * T(0.5));
        result.x = v.x * fSin;
        result.y = v.y * fSin;
        result.z = v.z * fSin;
        return result;
    }

    template <typename T> T extractRealComponentGTX(const _xquat<T>& q)
    {
        T w = T(1.0) - q.x * q.x - q.y * q.y - q.z * q.z;
        if(w < T(0))
            return T(0);
        else
            return -glm::sqrt(w);
    }
}

#endif//included

#endif//__gtx_quaternion_inl__
