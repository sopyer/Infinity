//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_half.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_half_inl__
#define __gtx_half_inl__

#include "gtx_half.h"
#if(GLM_INCLUDED(GLM_GTX_half, GLM_EXTENSION_GTX, GLM_GTX_half_required))

namespace glm
{
    inline _half::_half()
    {}

    inline _half::_half(float s)
    {
        if(s == 0)
        {
	        data = 0;
        }
        else
        {
	        uif x;
	        x.f = s;
            data = _toHalf(x.i);
        }
    }

    inline _half::_half(double s)
    {
        if(s == 0)
        {
	        data = 0;
        }
        else
        {
	        uif x;
	        x.f = float(s);
            data = _toHalf(x.i);
        }
    }

    inline _half::operator float() const
    {
        uif x;
        x.i = _toFloat(data);
        return x.f;
    }

    inline _half::operator double() const
    {
        uif x;
        x.i = _toFloat(data);
        return double(x.f);
    }

    inline _half::operator int() const
    {
        uif x;
        x.i = _toFloat(data);
        return int(x.f);
    }

    inline _half& _half::operator=(_half h)
    {
        data = h.data;
        return *this;
    }

    inline _half& _half::operator=(float f)
    {
        *this = _half(f);
        return *this;
    }

    inline _half _half::operator+(_half h) const
    {
        return _half(float(*this) + float(h));
    }

    inline _half& _half::operator+=(_half h)
    {
        *this = _half(float(*this) + float(h));
        return *this;
    }

    inline _half _half::operator+(float f) const
    {
        return _half(float(*this) + f);
    }

    inline _half& _half::operator+=(float f)
    {
        *this = _half(float(*this) + f);
        return *this;
    }

    inline _half _half::operator-(_half h) const
    {
        return _half(float(*this) - float(h));
    }

    inline _half& _half::operator-=(_half h)
    {
        *this = _half(float(*this) - float(h));
        return *this;
    }

    inline _half _half::operator-(float f) const
    {
        return _half(float(*this) - f);
    }

    inline _half& _half::operator-=(float f)
    {
        *this = _half (float (*this) - f);
        return *this;
    }

    inline _half _half::operator*(_half h) const
    {
        return _half(float(*this) * float(h));
    }

    inline _half& _half::operator*=(_half h)
    {
        *this = _half (float (*this) * float (h));
        return *this;
    }

    inline _half _half::operator*(float f) const
    {
        return _half(float(*this) * f);
    }

    inline _half& _half::operator*=(float f)
    {
        *this = _half (float (*this) * f);
        return *this;
    }

    inline _half _half::operator/(_half h) const
    {
        return _half(float(*this) / float(h));
    }

    inline _half& _half::operator/=(_half h)
    {
        *this = _half (float (*this) / float (h));
        return *this;
    }

    inline _half _half::operator/(float f) const
    {
        return _half(float(*this) / f);
    }

    inline _half& _half::operator/=(float f)
    {
        *this = _half (float (*this) / f);
        return *this;
    }

    inline _half _half::operator-() const
    {
        _half Result;
        Result.data = data ^ 0x8000;
        return Result;
    }

    inline int _half::_toFloat(short y) const
    {
        int s = (y >> 15) & 0x00000001;
        int e = (y >> 10) & 0x0000001f;
        int m =  y        & 0x000003ff;

        if(e == 0)
        {
	        if(m == 0)
	        {
	            //
	            // Plus or minus zero
	            //

	            return s << 31;
	        }
	        else
	        {
	            //
	            // Denormalized number -- renormalize it
	            //

	            while(!(m & 0x00000400))
	            {
		            m <<= 1;
		            e -=  1;
	            }

	            e += 1;
	            m &= ~0x00000400;
	        }
        }
        else if(e == 31)
        {
	        if(m == 0)
	        {
	            //
	            // Positive or negative infinity
	            //

	            return(s << 31) | 0x7f800000;
	        }
	        else
	        {
	            //
	            // Nan -- preserve sign and significand bits
	            //

	            return(s << 31) | 0x7f800000 | (m << 13);
	        }
        }

        //
        // Normalized number
        //

        e = e + (127 - 15);
        m = m << 13;

        //
        // Assemble s, e and m.
        //

        return(s << 31) | (e << 23) | m;
    }

    inline short _half::_toHalf(int i) const
    {
        //
        // Our floating point number, f, is represented by the bit
        // pattern in integer i.  Disassemble that bit pattern into
        // the sign, s, the exponent, e, and the significand, m.
        // Shift s into the position where it will go in in the
        // resulting _half number.
        // Adjust e, accounting for the different exponent bias
        // of float and _half (127 versus 15).
        //

        register int s =  (i >> 16) & 0x00008000;
        register int e = ((i >> 23) & 0x000000ff) - (127 - 15);
        register int m =   i        & 0x007fffff;

        //
        // Now reassemble s, e and m into a _half:
        //

        if(e <= 0)
        {
	        if(e < -10)
	        {
	            //
	            // E is less than -10.  The absolute value of f is
	            // less than __halfGTX_MIN (f may be a small normalized
	            // float, a denormalized float or a zero).
	            //
	            // We convert f to a _half zero.
	            //

	            return 0;
	        }

	        //
	        // E is between -10 and 0.  F is a normalized float,
	        // whose magnitude is less than __halfGTX_NRM_MIN.
	        //
	        // We convert f to a denormalized _half.
	        // 

	        m = (m | 0x00800000) >> (1 - e);

	        //
	        // Round to nearest, round "0.5" up.
	        //
	        // Rounding may cause the significand to overflow and make
	        // our number normalized.  Because of the way a _half's bits
	        // are laid out, we don't have to treat this case separately;
	        // the code below will handle it correctly.
	        // 

	        if(m &  0x00001000) m += 0x00002000;

	        //
	        // Assemble the _half from s, e (zero) and m.
	        //

	        return s | (m >> 13);
        }
        else if(e == 0xff - (127 - 15))
        {
	        if(m == 0)
	        {
	            //
	            // F is an infinity; convert f to a _half
	            // infinity with the same sign as f.
	            //

	            return s | 0x7c00;
	        }
	        else
	        {
	            //
	            // F is a NAN; we produce a _half NAN that preserves
	            // the sign bit and the 10 leftmost bits of the
	            // significand of f, with one exception: If the 10
	            // leftmost bits are all zero, the NAN would turn 
	            // into an infinity, so we have to set at least one
	            // bit in the significand.
	            //

	            m >>= 13;
	            return s | 0x7c00 | m | (m == 0);
	        }
        }
        else
        {
	        //
	        // E is greater than zero.  F is a normalized float.
	        // We try to convert f to a normalized _half.
	        //

	        //
	        // Round to nearest, round "0.5" up
	        //

	        if(m &  0x00001000)
	        {
	            m += 0x00002000;

	            if(m & 0x00800000)
	            {
		            m =  0;     // overflow in significand,
		            e += 1;     // adjust exponent
	            }
	        }

	        //
	        // Handle exponent overflow
	        //

	        if (e > 30)
	        {
	            _overflow();        // Cause a hardware floating point overflow;
	            return s | 0x7c00;  // if this returns, the _half becomes an
	        }                       // infinity with the same sign as f.

	        //
	        // Assemble the _half from s, e and m.
	        //

	        return s | (e << 10) | (m >> 13);
        }
    }

    inline float _half::_overflow() const
    {
        volatile float f = 1e10;

        for(int i = 0; i < 10; ++i)	
	        f *= f;             // this will overflow before
                                // the for­loop terminates
        return f;
    }
}

#endif//included

#endif//__gtx_half_inl__
