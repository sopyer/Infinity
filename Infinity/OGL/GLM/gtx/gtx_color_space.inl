//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_color_space.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_color_space_inl__
#define __gtx_color_space_inl__

#include "gtx_color_space.h"
#if(GLM_INCLUDED(GLM_GTX_color_space, GLM_EXTENSION_GTX, GLM_GTX_color_space_required))

namespace glm
{
    template <typename T> 
    inline _xvec3<T> rgbGTX(const _xvec3<T>& hsv)
    {
        _xvec3<T> rgb;

	    if(hsv.y == 0.0f)
		    // achromatic (grey)
            rgb = _xvec3<T>(hsv.z);
	    else
	    {
            // sector 0 to 5
            hsv.x /= 60.0f;                        

            int i = glm::floor(hsv.x);
            // factorial part of h
            float f = hsv.x - i;
            float p = hsv.z * (1.0f - hsv.y);
            float q = hsv.z * (1.0f - hsv.y * f);
            float t = hsv.z * (1.0f - hsv.y * (1.0f - f));

            switch(i)
            {
            default:
                rgb.r = hsv.v; 
                rgb.g = p; 
                rgb.b = q;
                break;
            case 0:
                rgb.r = hsv.v;
                rgb.g = t;
                rgb.b = p;
                break;
            case 1:
                rgb.r = q;
                rgb.g = hsv.v;
                rgb.b = p;
                break;
            case 2:
                rgb.r = p;
                rgb.g = hsv.v;
                rgb.b = t;
                break;
            case 3:
                rgb.r = p;
                rgb.g = q;
                rgb.b = hsv.v;
                break;
            case 4:
                rgb.r = t; 
                rgb.g = p; 
                rgb.b = hsv.v;
                break;
            }
	    }
    }

    template <typename T> 
    inline _xvec3<T> hsvGTX(const _xvec3<T>& rgb)
    {
        _xvec3<T> hvs = rgb;
	    float Min   = min(min(rgb.r, rgb.g), rgb.b);
	    float Max   = max(max(rgb.r, rgb.g), rgb.b);
	    float Delta = Max - Min;

	    hvs.z = Max;                               
    	
	    if(Max != 0.0f)
	    {
		    hvs.y = Delta / Max;              

		    if(rgb.r == Max)
			    // between yellow & magenta
			    hvs.x = (rgb.g - rgb.b) / Delta;
		    else
			    if(rgb.g == Max)
				    // between cyan & yellow
				    hvs.x = 2.0f + (rgb.b - rgb.r) / Delta;
			    else
				    // between magenta & cyan
				    hvs.x = 4.0f + (rgb.r - rgb.g) / Delta;
            
		    // Conversion in degrees
		    hvs.x *= 60.0f;                               
		    if(hvs.x < 0.0f) 
                hvs.x += 360.0f;
	    }
	    else
	    {
		    // If r = g = b = 0 then s = 0, h is undefined
		    hvs.y =  0.0f;
		    hvs.x = -1.0f;
	    }
    }
}

#endif//included

#endif//__gtx_color_space_inl__
