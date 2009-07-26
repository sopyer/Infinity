//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2007 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-01-12
// Updated : 2007-01-05
// Licence : This source is under GNU LGPL licence
// File    : _func.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __func_h__
#define __func_h__

#include "./_xvec2.h"
#include "./_xvec3.h"
#include "./_xvec4.h"
#include "./_bvec2.h"
#include "./_bvec3.h"
#include "./_bvec4.h"
#include "./_xmat2.h"
#include "./_xmat3.h"
#include "./_xmat4.h"
#include "./_xmat2x3.h"
#include "./_xmat3x2.h"
#include "./_xmat2x4.h"
#include "./_xmat4x2.h"
#include "./_xmat3x4.h"
#include "./_xmat4x3.h"
/*
#ifdef WIN32
#pragma warning (disable : 4244)
#endif //WIN32
*/
namespace glm
{
    ////////////////////////////////////////////////////////////////////////
    // Trigonometric Functions

    template <typename T> T radians(T degrees); //!< \brief Converts degrees to radians and returns the result. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> radians(const _xvec2<T>& degrees); //!< \brief Converts degrees to radians and returns the result. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> radians(const _xvec3<T>& degrees); //!< \brief Converts degrees to radians and returns the result. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> radians(const _xvec4<T>& degrees); //!< \brief Converts degrees to radians and returns the result. (From GLSL 1.10.59 specification)

    template <typename T> T degrees(T radians); //! \brief Converts radians to degrees and returns the result. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> degrees(const _xvec2<T>& radians); //! \brief Converts radians to degrees and returns the result. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> degrees(const _xvec3<T>& radians); //! \brief Converts radians to degrees and returns the result. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> degrees(const _xvec4<T>& radians); //! \brief Converts radians to degrees and returns the result. (From GLSL 1.10.59 specification)

    template <typename T> T sin(T angle); //!< \brief The standard trigonometric sine function. The values returned by this function will range from [-1, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> sin(const _xvec2<T>& angle); //!< \brief The standard trigonometric sine function. The values returned by this function will range from [-1, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> sin(const _xvec3<T>& angle); //!< \brief The standard trigonometric sine function. The values returned by this function will range from [-1, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> sin(const _xvec4<T>& angle); //!< \brief The standard trigonometric sine function. The values returned by this function will range from [-1, 1]. (From GLSL 1.10.59 specification)

    template <typename T> T cos(T angle); //!< \brief The standard trigonometric cosine function. The values returned by this function will range from [-1, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> cos(const _xvec2<T>& angle); //!< \brief The standard trigonometric cosine function. The values returned by this function will range from [-1, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> cos(const _xvec3<T>& angle); //!< \brief The standard trigonometric cosine function. The values returned by this function will range from [-1, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> cos(const _xvec4<T>& angle); //!< \brief The standard trigonometric cosine function. The values returned by this function will range from [-1, 1]. (From GLSL 1.10.59 specification)

    template <typename T> T tan(T angle); //!< \brief The standard trigonometric tangent function. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> tan(const _xvec2<T>& angle); //!< \brief The standard trigonometric tangent function. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> tan(const _xvec3<T>& angle); //!< \brief The standard trigonometric tangent function. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> tan(const _xvec4<T>& angle); //!< \brief The standard trigonometric tangent function. (From GLSL 1.10.59 specification)

    template <typename T> T asin(T x); //!< \brief Arc sine. Returns an angle whose sine is x. The range of values returned by this function is [-PI/2, PI/2]. Results are undefined if |x| > 1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> asin(const _xvec2<T>& x); //!< \brief Arc sine. Returns an angle whose sine is x. The range of values returned by this function is [-PI/2, PI/2]. Results are undefined if |x| > 1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> asin(const _xvec3<T>& x); //!< \brief Arc sine. Returns an angle whose sine is x. The range of values returned by this function is [-PI/2, PI/2]. Results are undefined if |x| > 1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> asin(const _xvec4<T>& x); //!< \brief Arc sine. Returns an angle whose sine is x. The range of values returned by this function is [-PI/2, PI/2]. Results are undefined if |x| > 1. (From GLSL 1.10.59 specification)

    template <typename T> T acos(T x); //!< \brief Arc cosine. Returns an angle whose sine is x. The range of values returned by this function is [0, PI]. Results are undefined if |x| > 1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> acos(const _xvec2<T>& x); //!< \brief Arc cosine. Returns an angle whose sine is x. The range of values returned by this function is [0, PI]. Results are undefined if |x| > 1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> acos(const _xvec3<T>& x); //!< \brief Arc cosine. Returns an angle whose sine is x. The range of values returned by this function is [0, PI]. Results are undefined if |x| > 1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> acos(const _xvec4<T>& x); //!< \brief Arc cosine. Returns an angle whose sine is x. The range of values returned by this function is [0, PI]. Results are undefined if |x| > 1. (From GLSL 1.10.59 specification)

    template <typename T> T atan(T y, T x); //!< \brief Arc tangent. Returns an angle whose tangent is y/x. The signs of x and y are used to determine what quadrant the angle is in. The range of values returned by this function is [-PI, PI]. Results are undefined if x and y are both 0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> atan(const _xvec2<T>& y, const _xvec2<T>& x); //!< \brief Arc tangent. Returns an angle whose tangent is y/x. The signs of x and y are used to determine what quadrant the angle is in. The range of values returned by this function is [-PI, PI]. Results are undefined if x and y are both 0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> atan(const _xvec3<T>& y, const _xvec3<T>& x); //!< \brief Arc tangent. Returns an angle whose tangent is y/x. The signs of x and y are used to determine what quadrant the angle is in. The range of values returned by this function is [-PI, PI]. Results are undefined if x and y are both 0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> atan(const _xvec4<T>& y, const _xvec4<T>& x); //!< \brief Arc tangent. Returns an angle whose tangent is y/x. The signs of x and y are used to determine what quadrant the angle is in. The range of values returned by this function is [-PI, PI]. Results are undefined if x and y are both 0. (From GLSL 1.10.59 specification)

    template <typename T> T atan(T y_over_x); //!< \brief Arc tangent. Returns an angle whose tangent is y_over_x. The range of values returned by this function is [-PI/2, PI/2]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> atan(const _xvec2<T>& y_over_x); //!< \brief Arc tangent. Returns an angle whose tangent is y_over_x. The range of values returned by this function is [-PI/2, PI/2]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> atan(const _xvec3<T>& y_over_x); //!< \brief Arc tangent. Returns an angle whose tangent is y_over_x. The range of values returned by this function is [-PI/2, PI/2]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> atan(const _xvec4<T>& y_over_x); //!< \brief Arc tangent. Returns an angle whose tangent is y_over_x. The range of values returned by this function is [-PI/2, PI/2]. (From GLSL 1.10.59 specification)

    ////////////////////////////////////////////////////////////////////////
    // Exponential Functions

    template <typename T> T pow(T x, T y); //!< \brief Returns x raised to the y power. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> pow(const _xvec2<T>& x, const _xvec2<T>& y); //!< \brief Returns x raised to the y power. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> pow(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Returns x raised to the y power. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> pow(const _xvec4<T>& x, const _xvec4<T>& y); //!< \brief Returns x raised to the y power. (From GLSL 1.10.59 specification)

    template <typename T> T exp(T x); //!< \brief Returns the natural exponentiation of x, i.e., e^x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> exp(const _xvec2<T>& x); //!< \brief Returns the natural exponentiation of x, i.e., e^x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> exp(const _xvec3<T>& x); //!< \brief Returns the natural exponentiation of x, i.e., e^x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> exp(const _xvec4<T>& x); //!< \brief Returns the natural exponentiation of x, i.e., e^x. (From GLSL 1.10.59 specification)

    template <typename T> T log(T x); //!< \brief Returns the natural logarithm of x, i.e., returns the value y which satisfies the equation x = e^y. Results are undefined if x <= 0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> log(const _xvec2<T>& x); //!< \brief Returns the natural logarithm of x, i.e., returns the value y which satisfies the equation x = e^y. Results are undefined if x <= 0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> log(const _xvec3<T>& x); //!< \brief Returns the natural logarithm of x, i.e., returns the value y which satisfies the equation x = e^y. Results are undefined if x <= 0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> log(const _xvec4<T>& x); //!< \brief Returns the natural logarithm of x, i.e., returns the value y which satisfies the equation x = e^y. Results are undefined if x <= 0. (From GLSL 1.10.59 specification)

    template <typename T> T exp2(T x); //!< \brief Returns 2 raised to the x power. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> exp2(const _xvec2<T>& x); //!< \brief Returns 2 raised to the x power. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> exp2(const _xvec3<T>& x); //!< \brief Returns 2 raised to the x power. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> exp2(const _xvec4<T>& x); //!< \brief Returns 2 raised to the x power. (From GLSL 1.10.59 specification)

    template <typename T> T log2(T x); //!< \brief Returns the base 2 log of x, i.e., returns the value y, which satisfies the equation x = 2 ^ y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> log2(const _xvec2<T>& x); //!< \brief Returns the base 2 log of x, i.e., returns the value y, which satisfies the equation x = 2 ^ y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> log2(const _xvec3<T>& x); //!< \brief Returns the base 2 log of x, i.e., returns the value y, which satisfies the equation x = 2 ^ y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> log2(const _xvec4<T>& x); //!< \brief Returns the base 2 log of x, i.e., returns the value y, which satisfies the equation x = 2 ^ y. (From GLSL 1.10.59 specification)

    template <typename T> T sqrt(T x); //!< \brief Returns the positive square root of x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> sqrt(const _xvec2<T>& x); //!< \brief Returns the positive square root of x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> sqrt(const _xvec3<T>& x); //!< \brief Returns the positive square root of x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> sqrt(const _xvec4<T>& x); //!< \brief Returns the positive square root of x. (From GLSL 1.10.59 specification)
    
    template <typename T> T inversesqrt(T x); //!< \brief Returns the reciprocal of the positive square root of x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> inversesqrt(const _xvec2<T>& x); //!< \brief Returns the reciprocal of the positive square root of x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> inversesqrt(const _xvec3<T>& x); //!< \brief Returns the reciprocal of the positive square root of x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> inversesqrt(const _xvec4<T>& x); //!< \brief Returns the reciprocal of the positive square root of x. (From GLSL 1.10.59 specification)

    ////////////////////////////////////////////////////////////////////////
    // Common Functions

    template <typename T> T abs(T x); //!< \brief Returns x if x >= 0; otherwise, it returns -x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> abs(const _xvec2<T>& x); //!< \brief Returns x if x >= 0; otherwise, it returns -x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> abs(const _xvec3<T>& x); //!< \brief Returns x if x >= 0; otherwise, it returns -x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> abs(const _xvec4<T>& x); //!< \brief Returns x if x >= 0; otherwise, it returns -x. (From GLSL 1.10.59 specification)

    template <typename T> T sign(T x); //!< \brief Returns 1.0 if x > 0, 0.0 if x = 0, or -1.0 if x < 0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> sign(const _xvec2<T>& x); //!< \brief Returns 1.0 if x > 0, 0.0 if x = 0, or -1.0 if x < 0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> sign(const _xvec3<T>& x); //!< \brief Returns 1.0 if x > 0, 0.0 if x = 0, or -1.0 if x < 0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> sign(const _xvec4<T>& x); //!< \brief Returns 1.0 if x > 0, 0.0 if x = 0, or -1.0 if x < 0. (From GLSL 1.10.59 specification)

    template <typename T> T floor(T x); //!< \brief Returns a value equal to the nearest integer that is less then or equal to x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> floor(const _xvec2<T>& x); //!< \brief Returns a value equal to the nearest integer that is less then or equal to x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> floor(const _xvec3<T>& x); //!< \brief Returns a value equal to the nearest integer that is less then or equal to x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> floor(const _xvec4<T>& x); //!< \brief Returns a value equal to the nearest integer that is less then or equal to x. (From GLSL 1.10.59 specification)

    template <typename T> T ceil(T x); //!< \brief Returns a value equal to the nearest integer that is greater than or equal to x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> ceil(const _xvec2<T>& x); //!< \brief Returns a value equal to the nearest integer that is greater than or equal to x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> ceil(const _xvec3<T>& x); //!< \brief Returns a value equal to the nearest integer that is greater than or equal to x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> ceil(const _xvec4<T>& x); //!< \brief Returns a value equal to the nearest integer that is greater than or equal to x. (From GLSL 1.10.59 specification)

    template <typename T> T fract(T x); //!< \brief Return x - floor(x). (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> fract(const _xvec2<T>& x); //!< \brief Return x - floor(x). (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> fract(const _xvec3<T>& x); //!< \brief Return x - floor(x). (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> fract(const _xvec4<T>& x); //!< \brief Return x - floor(x). (From GLSL 1.10.59 specification)

    template <typename T> T mod(T x, T y); //!< \brief Modulus. Returns x - y * floor(x / y) for each component in x using the floating point value y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> mod(const _xvec2<T>& x, T y); //!< \brief Modulus. Returns x - y * floor(x / y) for each component in x using the floating point value y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> mod(const _xvec3<T>& x, T y); //!< \brief Modulus. Returns x - y * floor(x / y) for each component in x using the floating point value y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> mod(const _xvec4<T>& x, T y); //!< \brief Modulus. Returns x - y * floor(x / y) for each component in x using the floating point value y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> mod(const _xvec2<T>& x, const _xvec2<T>& y); //!< \brief Modulus. Returns x - y * floor(x / y) for each component in x using the corresponding component of y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> mod(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Modulus. Returns x - y * floor(x / y) for each component in x using the corresponding component of y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> mod(const _xvec4<T>& x, const _xvec4<T>& y); //!< \brief Modulus. Returns x - y * floor(x / y) for each component in x using the corresponding component of y. (From GLSL 1.10.59 specification)

    template <typename T> T min(T x, T y); //!< \brief Returns y if y < x; otherwise, it returns x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> min(const _xvec2<T>& x, T y); //!< \brief Returns y if y < x; otherwise, it returns x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> min(const _xvec3<T>& x, T y); //!< \brief Returns y if y < x; otherwise, it returns x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> min(const _xvec4<T>& x, T y); //!< \brief Returns y if y < x; otherwise, it returns x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> min(const _xvec2<T>& x, const _xvec2<T>& y); //!< \brief Returns minimum of each component of x compared with the floating-point value y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> min(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Returns minimum of each component of x compared with the floating-point value y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> min(const _xvec4<T>& x, const _xvec4<T>& y); //!< \brief Returns minimum of each component of x compared with the floating-point value y. (From GLSL 1.10.59 specification)

    template <typename T> T max(T x, T y); //!< \brief Returns y if x < y; otherwise, it returns x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> max(const _xvec2<T>& x, T y); //!< \brief Returns y if x < y; otherwise, it returns x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> max(const _xvec3<T>& x, T y); //!< \brief Returns y if x < y; otherwise, it returns x. (From GLSL 1.10.59 specification) 
    template <typename T> _xvec4<T> max(const _xvec4<T>& x, T y); //!< \brief Returns y if x < y; otherwise, it returns x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> max(const _xvec2<T>& x, const _xvec2<T>& y); //!< \brief Returns maximum of each component of x compared with the floating-point value y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> max(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Returns maximum of each component of x compared with the floating-point value y. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> max(const _xvec4<T>& x, const _xvec4<T>& y); //!< \brief Returns maximum of each component of x compared with the floating-point value y. (From GLSL 1.10.59 specification)

    template <typename T> T clamp(T x, T minVal, T maxVal); //!< \brief Returns min(max(x, minVal), maxVal) for each component in x using the floating-point values minVal and maxVal. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> clamp(const _xvec2<T>& x, T minVal, T maxVal); //!< \brief Returns min(max(x, minVal), maxVal) for each component in x using the floating-point values minVal and maxVal. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> clamp(const _xvec3<T>& x, T minVal, T maxVal); //!< \brief Returns min(max(x, minVal), maxVal) for each component in x using the floating-point values minVal and maxVal. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> clamp(const _xvec4<T>& x, T minVal, T maxVal); //!< \brief Returns min(max(x, minVal), maxVal) for each component in x using the floating-point values minVal and maxVal. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> clamp(const _xvec2<T>& x, const _xvec2<T>& minVal, const _xvec2<T>& maxVal); //!< \brief Returns the component-wise result of min(max(x, minVal), maxVal). (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> clamp(const _xvec3<T>& x, const _xvec3<T>& minVal, const _xvec3<T>& maxVal); //!< \brief Returns the component-wise result of min(max(x, minVal), maxVal). (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> clamp(const _xvec4<T>& x, const _xvec4<T>& minVal, const _xvec4<T>& maxVal); //!< \brief Returns the component-wise result of min(max(x, minVal), maxVal). (From GLSL 1.10.59 specification)

    template <typename T> T mix(T x, T y, T a); //!< \brief Returns x * (1.0 - a) + y * a, i.e., the linear blend of x and y using the floating-point value a. The value for a is not restricted to the range [0, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> mix(const _xvec2<T>& x, const _xvec2<T>& y, T a); //!< \brief Returns x * (1.0 - a) + y * a, i.e., the linear blend of x and y using the floating-point value a. The value for a is not restricted to the range [0, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> mix(const _xvec3<T>& x, const _xvec3<T>& y, T a); //!< \brief Returns x * (1.0 - a) + y * a, i.e., the linear blend of x and y using the floating-point value a. The value for a is not restricted to the range [0, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> mix(const _xvec4<T>& x, const _xvec4<T>& y, T a); //!< \brief Returns x * (1.0 - a) + y * a, i.e., the linear blend of x and y using the floating-point value a. The value for a is not restricted to the range [0, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> mix(const _xvec2<T>& x, const _xvec2<T>& y, const _xvec2<T>& a); //!< \brief Returns the component-wise result of x * (1.0 - a) + y * a, i.e., the linear blend of x and y using vector a. The value for a is not restricted to the range [0, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> mix(const _xvec3<T>& x, const _xvec3<T>& y, const _xvec3<T>& a); //!< \brief Returns the component-wise result of x * (1.0 - a) + y * a, i.e., the linear blend of x and y using vector a. The value for a is not restricted to the range [0, 1]. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> mix(const _xvec4<T>& x, const _xvec4<T>& y, const _xvec4<T>& a); //!< \brief Returns the component-wise result of x * (1.0 - a) + y * a, i.e., the linear blend of x and y using vector a. The value for a is not restricted to the range [0, 1]. (From GLSL 1.10.59 specification)

    template <typename T> T step(T edge, T x); //!< \brief Returns 0.0 if x <= edge; otherwise, it returns 1.0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> step(T edge, const _xvec2<T>& x); //!< \brief Returns 0.0 if x <= edge; otherwise, it returns 1.0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> step(T edge, const _xvec3<T>& x); //!< \brief Returns 0.0 if x <= edge; otherwise, it returns 1.0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> step(T edge, const _xvec4<T>& x); //!< \brief Returns 0.0 if x <= edge; otherwise, it returns 1.0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> step(const _xvec2<T>& edge, const _xvec2<T>& x); //!< \brief Returns 0.0 if x <= edge; otherwise, it returns 1.0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> step(const _xvec3<T>& edge, const _xvec3<T>& x); //!< \brief Returns 0.0 if x <= edge; otherwise, it returns 1.0. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> step(const _xvec4<T>& edge, const _xvec4<T>& x); //!< \brief Returns 0.0 if x <= edge; otherwise, it returns 1.0. (From GLSL 1.10.59 specification)

    template <typename T> T smoothstep(T edge0, T edge1, T x); //!< \brief Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth Hermite interpolation between 0 and 1 when edge0 < x, edge1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> smoothstep(T edge0, T edge1, const _xvec2<T>& x); //!< \brief Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth Hermite interpolation between 0 and 1 when edge0 < x, edge1. (From GLSL 1.10.59 specification) 
    template <typename T> _xvec3<T> smoothstep(T edge0, T edge1, const _xvec3<T>& x); //!< \brief Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth Hermite interpolation between 0 and 1 when edge0 < x, edge1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> smoothstep(T edge0, T edge1, const _xvec4<T>& x); //!< \brief Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth Hermite interpolation between 0 and 1 when edge0 < x, edge1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> smoothstep(const _xvec2<T>& edge0, const _xvec2<T>& edge1, const _xvec2<T>& x); //!< \brief Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth Hermite interpolation between 0 and 1 when edge0 < x, edge1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> smoothstep(const _xvec3<T>& edge0, const _xvec3<T>& edge1, const _xvec3<T>& x); //!< \brief Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth Hermite interpolation between 0 and 1 when edge0 < x, edge1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> smoothstep(const _xvec4<T>& edge0, const _xvec4<T>& edge1, const _xvec4<T>& x); //!< \brief Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth Hermite interpolation between 0 and 1 when edge0 < x, edge1. (From GLSL 1.10.59 specification)

    ////////////////////////////////////////////////////////////////////////
    // Geometric Functions

    template <typename T> T length(T x); //!< \brief Returns the length of x, i.e., sqrt(x * x). (From GLSL 1.10.59 specification)
    template <typename T> T length(const _xvec2<T>& x); //!< \brief Returns the length of x, i.e., sqrt(x[0] * x[0] + x[1] * x[1]). (From GLSL 1.10.59 specification)
    template <typename T> T length(const _xvec3<T>& x); //!< \brief Returns the length of x, i.e., sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]). (From GLSL 1.10.59 specification)
    template <typename T> T length(const _xvec4<T>& x); //!< \brief Returns the length of x, i.e., sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2] + x[3] * x[3]). (From GLSL 1.10.59 specification)

    template <typename T> T distance(T p0, T p1); //!< \brief Returns the distance betwwen p0 and p1, i.e., length(p0 - p1). (From GLSL 1.10.59 specification)
    template <typename T> T distance(const _xvec2<T>& p0, const _xvec2<T>& p1); //!< \brief Returns the distance betwwen p0 and p1, i.e., length(p0 - p1). (From GLSL 1.10.59 specification)
    template <typename T> T distance(const _xvec3<T>& p0, const _xvec3<T>& p1); //!< \brief Returns the distance betwwen p0 and p1, i.e., length(p0 - p1). (From GLSL 1.10.59 specification)
    template <typename T> T distance(const _xvec4<T>& p0, const _xvec4<T>& p1); //!< \brief Returns the distance betwwen p0 and p1, i.e., length(p0 - p1). (From GLSL 1.10.59 specification)

    template <typename T> T dot(T v1, T v2); //!< \brief Returns the dot product of x and y, i.e., result = x * y. (From GLSL 1.10.59 specification)
    template <typename T> T dot(const _xvec2<T>& x, const _xvec2<T>& y); //!< \brief Returns the dot product of x and y, i.e., result = x[0] * y[0] + x[1] * y[1]. (From GLSL 1.10.59 specification)
    template <typename T> T dot(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Returns the dot product of x and y, i.e., result = x[0] * y[0] + x[1] * y[1] + x[2] * y[2]. (From GLSL 1.10.59 specification)
    template <typename T> T dot(const _xvec4<T>& x, const _xvec4<T>& y); //!< \brief Returns the dot product of x and y, i.e., result = x[0] * y[0] + x[1] * y[1] + x[2] * y[2] + x[3] * y[3]. (From GLSL 1.10.59 specification)

    template <typename T> _xvec3<T> cross(const _xvec3<T>& x, const _xvec3<T>& y); //!< brief Returns the cross product of x and y. (From GLSL 1.10.59 specification)

	template <typename T> T normalize(T x); //!< \brief Returns a vector in the same direction as x but with length of 1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> normalize(const _xvec2<T>& x); //!< \brief Returns a vector in the same direction as x but with length of 1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> normalize(const _xvec3<T>& x); //!< \brief Returns a vector in the same direction as x but with length of 1. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> normalize(const _xvec4<T>& x); //!< \brief Returns a vector in the same direction as x but with length of 1. (From GLSL 1.10.59 specification)

    template <typename T> T faceforward(T N, T I, T Nref); //!< \brief If dot(Nref, I) < 0.0, return N, otherwise, return -N. (From GLSL 1.10.59 specification) 
    template <typename T> _xvec2<T> faceforward(const _xvec2<T>& N, const _xvec2<T>& I, const _xvec2<T>& Nref); //!< \brief If dot(Nref, I) < 0.0, return N, otherwise, return -N. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> faceforward(const _xvec3<T>& N, const _xvec3<T>& I, const _xvec3<T>& Nref); //!< \brief If dot(Nref, I) < 0.0, return N, otherwise, return -N. (From GLSL 1.10.59 specification) 
    template <typename T> _xvec4<T> faceforward(const _xvec4<T>& N, const _xvec4<T>& I, const _xvec4<T>& Nref); //!< \brief If dot(Nref, I) < 0.0, return N, otherwise, return -N. (From GLSL 1.10.59 specification) 
    
    template <typename T> T reflect(T I, T N); //!< \brief For the incident vector I and surface orientation N, returns the reflection direction : result = I - 2.0 * dot(N, I) * N. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> reflect(const _xvec2<T>& I, const _xvec2<T>& N); //!< \brief For the incident vector I and surface orientation N, returns the reflection direction : result = I - 2.0 * dot(N, I) * N. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> reflect(const _xvec3<T>& I, const _xvec3<T>& N); //!< \brief For the incident vector I and surface orientation N, returns the reflection direction : result = I - 2.0 * dot(N, I) * N. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> reflect(const _xvec4<T>& I, const _xvec4<T>& N); //!< \brief For the incident vector I and surface orientation N, returns the reflection direction : result = I - 2.0 * dot(N, I) * N. (From GLSL 1.10.59 specification)
    
    template <typename T> T refract(T I, T N, T eta); //!< \brief For the incident vector I and surface normal N, and the ratio of indices of refraction eta, return the refraction vector. (From GLSL 1.10.59 specification)  
    template <typename T> _xvec2<T> refract(const _xvec2<T>& I, const _xvec2<T>& N, T eta); //!< \brief For the incident vector I and surface normal N, and the ratio of indices of refraction eta, return the refraction vector. (From GLSL 1.10.59 specification) 
    template <typename T> _xvec3<T> refract(const _xvec3<T>& I, const _xvec3<T>& N, T eta); //!< \brief For the incident vector I and surface normal N, and the ratio of indices of refraction eta, return the refraction vector. (From GLSL 1.10.59 specification) 
    template <typename T> _xvec4<T> refract(const _xvec4<T>& I, const _xvec4<T>& N, T eta); //!< \brief For the incident vector I and surface normal N, and the ratio of indices of refraction eta, return the refraction vector. (From GLSL 1.10.59 specification) 

    ////////////////////////////////////////////////////////////////////////
    // Matrix Functions

    template <typename T> _xmat2<T> matrixCompMult(const _xmat2<T>& x, const _xmat2<T>& y); //!< \brief Multiply matrix x by matrix y component-wise, i.e., result[i][j] is the scalar product of x[i][j] and y[i][j]. (From GLSL 1.10.59 specification)  
    template <typename T> _xmat3<T> matrixCompMult(const _xmat3<T>& x, const _xmat3<T>& y); //!< \brief Multiply matrix x by matrix y component-wise, i.e., result[i][j] is the scalar product of x[i][j] and y[i][j]. (From GLSL 1.10.59 specification)  
    template <typename T> _xmat4<T> matrixCompMult(const _xmat4<T>& x, const _xmat4<T>& y); //!< \brief Multiply matrix x by matrix y component-wise, i.e., result[i][j] is the scalar product of x[i][j] and y[i][j]. (From GLSL 1.10.59 specification)  
    template <typename T> _xmat2x3<T> matrixCompMult(const _xmat2x3<T>& x, const _xmat2x3<T>& y); //!< \brief Multiply matrix x by matrix y component-wise, i.e., result[i][j] is the scalar product of x[i][j] and y[i][j]. (From GLSL 1.20.6 specification)  
    template <typename T> _xmat3x2<T> matrixCompMult(const _xmat3x2<T>& x, const _xmat3x2<T>& y); //!< \brief Multiply matrix x by matrix y component-wise, i.e., result[i][j] is the scalar product of x[i][j] and y[i][j]. (From GLSL 1.20.6 specification)  
	template <typename T> _xmat2x4<T> matrixCompMult(const _xmat2x4<T>& x, const _xmat2x4<T>& y); //!< \brief Multiply matrix x by matrix y component-wise, i.e., result[i][j] is the scalar product of x[i][j] and y[i][j]. (From GLSL 1.20.6 specification)  
    template <typename T> _xmat4x2<T> matrixCompMult(const _xmat4x2<T>& x, const _xmat4x2<T>& y); //!< \brief Multiply matrix x by matrix y component-wise, i.e., result[i][j] is the scalar product of x[i][j] and y[i][j]. (From GLSL 1.20.6 specification)  
	template <typename T> _xmat3x4<T> matrixCompMult(const _xmat3x4<T>& x, const _xmat3x4<T>& y); //!< \brief Multiply matrix x by matrix y component-wise, i.e., result[i][j] is the scalar product of x[i][j] and y[i][j]. (From GLSL 1.20.6 specification)  
    template <typename T> _xmat4x3<T> matrixCompMult(const _xmat4x3<T>& x, const _xmat4x3<T>& y); //!< \brief Multiply matrix x by matrix y component-wise, i.e., result[i][j] is the scalar product of x[i][j] and y[i][j]. (From GLSL 1.20.6 specification)  

    template <typename T> _xmat2<T> outerProduct(const _xvec2<T>& c, const _xvec2<T>& r); //!< \brief Treats the first parameter c as a column vector and the second parameter r as a row vector and does a linear algebraic matrix multiply c * r. (From GLSL 1.20.6 specification) 
    template <typename T> _xmat3<T> outerProduct(const _xvec3<T>& c, const _xvec3<T>& r); //!< \brief Treats the first parameter c as a column vector and the second parameter r as a row vector and does a linear algebraic matrix multiply c * r. (From GLSL 1.20.6 specification) 
    template <typename T> _xmat4<T> outerProduct(const _xvec4<T>& c, const _xvec4<T>& r); //!< \brief Treats the first parameter c as a column vector and the second parameter r as a row vector and does a linear algebraic matrix multiply c * r. (From GLSL 1.20.6 specification) 
    template <typename T> _xmat2x3<T> outerProduct(const _xvec3<T>& c, const _xvec2<T>& r); //!< \brief Treats the first parameter c as a column vector and the second parameter r as a row vector and does a linear algebraic matrix multiply c * r. (From GLSL 1.20.6 specification) 
    template <typename T> _xmat3x2<T> outerProduct(const _xvec2<T>& c, const _xvec3<T>& r); //!< \brief Treats the first parameter c as a column vector and the second parameter r as a row vector and does a linear algebraic matrix multiply c * r. (From GLSL 1.20.6 specification) 
	template <typename T> _xmat2x4<T> outerProduct(const _xvec2<T>& c, const _xvec4<T>& r); //!< \brief Treats the first parameter c as a column vector and the second parameter r as a row vector and does a linear algebraic matrix multiply c * r. (From GLSL 1.20.6 specification) 
	template <typename T> _xmat4x2<T> outerProduct(const _xvec4<T>& c, const _xvec2<T>& r); //!< \brief Treats the first parameter c as a column vector and the second parameter r as a row vector and does a linear algebraic matrix multiply c * r. (From GLSL 1.20.6 specification) 
	template <typename T> _xmat3x4<T> outerProduct(const _xvec4<T>& c, const _xvec3<T>& r); //!< \brief Treats the first parameter c as a column vector and the second parameter r as a row vector and does a linear algebraic matrix multiply c * r. (From GLSL 1.20.6 specification) 
	template <typename T> _xmat4x3<T> outerProduct(const _xvec3<T>& c, const _xvec4<T>& r); //!< \brief Treats the first parameter c as a column vector and the second parameter r as a row vector and does a linear algebraic matrix multiply c * r. (From GLSL 1.20.6 specification) 

    template <typename T> _xmat2<T> transpose(const _xmat2<T>& x); //!< Returns the transposed matrix of x (From GLSL 1.20.6 specification) 
    template <typename T> _xmat3<T> transpose(const _xmat3<T>& x); //!< Returns the transposed matrix of x (From GLSL 1.20.6 specification) 
    template <typename T> _xmat4<T> transpose(const _xmat4<T>& x); //!< Returns the transposed matrix of x (From GLSL 1.20.6 specification) 
    template <typename T> _xmat2x3<T> transpose(const _xmat3x2<T>& x); //!< Returns the transposed matrix of x (From GLSL 1.20.6 specification) 
    template <typename T> _xmat3x2<T> transpose(const _xmat2x3<T>& x); //!< Returns the transposed matrix of x (From GLSL 1.20.6 specification) 
    template <typename T> _xmat2x4<T> transpose(const _xmat4x2<T>& x); //!< Returns the transposed matrix of x (From GLSL 1.20.6 specification) 
    template <typename T> _xmat4x2<T> transpose(const _xmat2x4<T>& x); //!< Returns the transposed matrix of x (From GLSL 1.20.6 specification) 
    template <typename T> _xmat3x4<T> transpose(const _xmat4x3<T>& x); //!< Returns the transposed matrix of x (From GLSL 1.20.6 specification) 
    template <typename T> _xmat4x3<T> transpose(const _xmat3x4<T>& x); //!< Returns the transposed matrix of x (From GLSL 1.20.6 specification) 

    ////////////////////////////////////////////////////////////////////////
    // Vector Relational Functions

    template <typename T> _bvec2 lessThan(const _xvec2<T>& x, const _xvec2<T>& y); //!< \brief Returns the component-wise compare of x < y. (From GLSL 1.10.59 specification)  
    template <typename T> _bvec3 lessThan(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Returns the component-wise compare of x < y. (From GLSL 1.10.59 specification)  
    template <typename T> _bvec4 lessThan(const _xvec4<T>& x, const _xvec4<T>& y); //!< \brief Returns the component-wise compare of x < y. (From GLSL 1.10.59 specification)

    template <typename T> _bvec2 lessThanEqual(const _xvec2<T>& x, const _xvec2<T>& y); //!< \brief Returns the component-wise compare of x <= y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec3 lessThanEqual(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Returns the component-wise compare of x <= y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec4 lessThanEqual(const _xvec4<T>& x, const _xvec4<T>& y); //!< \brief Returns the component-wise compare of x <= y. (From GLSL 1.10.59 specification)

    template <typename T> _bvec2 greaterThan(const _xvec2<T>& x, const _xvec2<T>& y); //!< \brief Returns the component-wise compare of x > y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec3 greaterThan(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Returns the component-wise compare of x > y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec4 greaterThan(const _xvec4<T>& x, const _xvec4<T>& y); //!< \brief Returns the component-wise compare of x > y. (From GLSL 1.10.59 specification)

    template <typename T> _bvec2 greaterThanEqual(const _xvec2<T>& x, const _xvec2<T>& y); //!< \brief Returns the component-wise compare of x >= y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec3 greaterThanEqual(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Returns the component-wise compare of x >= y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec4 greaterThanEqual(const _xvec4<T>& x, const _xvec4<T>& y); //!< \brief Returns the component-wise compare of x >= y. (From GLSL 1.10.59 specification)

    _bvec2 equal(const _bvec2& x, const _bvec2& y); //!< \brief Returns the component-wise compare of x == y. (From GLSL 1.10.59 specification)
    _bvec3 equal(const _bvec3& x, const _bvec3& y); //!< \brief Returns the component-wise compare of x == y. (From GLSL 1.10.59 specification)
    _bvec4 equal(const _bvec4& x, const _bvec4& y); //!< \brief Returns the component-wise compare of x == y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec2 equal(const _xvec2<T>& x, const _xvec2<T>& y); //!< \brief Returns the component-wise compare of x == y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec3 equal(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Returns the component-wise compare of x == y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec4 equal(const _xvec4<T>& x, const _xvec4<T>& y); //!< \brief Returns the component-wise compare of x == y. (From GLSL 1.10.59 specification)

    _bvec2 notEqual(const _bvec2& x, const _bvec2& y); //!< \brief Returns the component-wise compare of x != y. (From GLSL 1.10.59 specification)
    _bvec3 notEqual(const _bvec3& x, const _bvec3& y); //!< \brief Returns the component-wise compare of x != y. (From GLSL 1.10.59 specification)
    _bvec4 notEqual(const _bvec4& x, const _bvec4& y); //!< \brief Returns the component-wise compare of x != y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec2 notEqual(const _xvec2<T>& x, const _xvec2<T>& y); //!< \brief Returns the component-wise compare of x != y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec3 notEqual(const _xvec3<T>& x, const _xvec3<T>& y); //!< \brief Returns the component-wise compare of x != y. (From GLSL 1.10.59 specification)
    template <typename T> _bvec4 notEqual(const _xvec4<T>& x, const _xvec4<T>& y); //!< \brief Returns the component-wise compare of x != y. (From GLSL 1.10.59 specification)

    bool any(const _bvec2& x); //!< \brief Returns true if any component of x is true. (From GLSL 1.10.59 specification)
    bool any(const _bvec3& x); //!< \brief Returns true if any component of x is true. (From GLSL 1.10.59 specification)
    bool any(const _bvec4& x); //!< \brief Returns true if any component of x is true. (From GLSL 1.10.59 specification)

    bool all(const _bvec2& x); //!< \brief Returns true if all component of x is true. (From GLSL 1.10.59 specification)
    bool all(const _bvec3& x); //!< \brief Returns true if all component of x is true. (From GLSL 1.10.59 specification)
    bool all(const _bvec4& x); //!< \brief Returns true if all component of x is true. (From GLSL 1.10.59 specification)

#ifdef CC_VC71 // VC7.1 compiler doesn't support the C++ key word 'not'
    bool not(bool x); //!< \brief Returns the component-wise logical complement of x. (From GLSL 1.10.59 specification)
    _bvec2 not(const _bvec2& v); //!< \brief Returns the component-wise logical complement of x. (From GLSL 1.10.59 specification)
    _bvec3 not(const _bvec3& v); //!< \brief Returns the component-wise logical complement of x. (From GLSL 1.10.59 specification)
    _bvec4 not(const _bvec4& v); //!< \brief Returns the component-wise logical complement of x. (From GLSL 1.10.59 specification)
#elif CC_GCC3
    _bvec2 operator not(const _bvec2& v); //!< \brief Returns the component-wise logical complement of x. (From GLSL 1.10.59 specification)
    _bvec3 operator not(const _bvec3& v); //!< \brief Returns the component-wise logical complement of x. (From GLSL 1.10.59 specification)
    _bvec4 operator not(const _bvec4& v); //!< \brief Returns the component-wise logical complement of x. (From GLSL 1.10.59 specification)
#endif

    ////////////////////////////////////////////////////////////////////////
    // Noise Functions

    template <typename T> T noise1(T x); //!< \brief Returns a 1D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> T noise1(const _xvec2<T>& x); //!< \brief Returns a 1D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> T noise1(const _xvec3<T>& x); //!< \brief Returns a 1D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> T noise1(const _xvec4<T>& x); //!< \brief Returns a 1D noise value based on the input value x. (From GLSL 1.10.59 specification)

    template <typename T> _xvec2<T> noise2(T x); //!< \brief Returns a 2D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> noise2(const _xvec2<T>& x); //!< \brief Returns a 2D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> noise2(const _xvec3<T>& x); //!< \brief Returns a 2D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec2<T> noise2(const _xvec4<T>& x); //!< \brief Returns a 2D noise value based on the input value x. (From GLSL 1.10.59 specification)

    template <typename T> _xvec3<T> noise3(T x); //!< \brief Returns a 3D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> noise3(const _xvec2<T>& x); //!< \brief Returns a 3D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> noise3(const _xvec3<T>& x); //!< \brief Returns a 3D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec3<T> noise3(const _xvec4<T>& x); //!< \brief Returns a 3D noise value based on the input value x. (From GLSL 1.10.59 specification)

    template <typename T> _xvec4<T> noise4(T x); //!< \brief Returns a 4D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> noise4(const _xvec2<T>& x); //!< \brief Returns a 4D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> noise4(const _xvec3<T>& x); //!< \brief Returns a 4D noise value based on the input value x. (From GLSL 1.10.59 specification)
    template <typename T> _xvec4<T> noise4(const _xvec4<T>& x); //!< \brief Returns a 4D noise value based on the input value x. (From GLSL 1.10.59 specification)
} //namespace glm

#endif //__func_h__
