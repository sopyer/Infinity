#ifndef     __FTBBox__
#define     __FTBBox__

#include <ft2build.h>
#include FT_FREETYPE_H
//#include FT_GLYPH_H
#include FT_OUTLINE_H

#include "FTGL.h"
#include "FTPoint.h"
#ifndef _FTGL_NATIVE_
#	include <glm\glmext.h>
#endif

/**
 * FTBBox is a convenience class for handling bounding boxes.
 */
class FTGL_EXPORT FTBBox
{
    public:
        /**
         * Default constructor. Bounding box is set to zero.
         */
        FTBBox():lower(0.0f), upper(0.0f){}
        
        /**
         * Constructor.
         */
        FTBBox( float lx, float ly, float lz, float ux, float uy, float uz)
        :lower(lx, ly, lz), upper(ux, uy, uz)
        {}
        
        /**
         * Constructor. Extracts a bounding box from a freetype glyph. Uses
         * the control box for the glyph. <code>FT_Glyph_Get_CBox()</code>
         *
         * @param glyph A freetype glyph
         */
        FTBBox( FT_GlyphSlot glyph)
        :   lower(0.0f), upper(0.0f)
        {
            FT_BBox bbox;
            FT_Outline_Get_CBox( &(glyph->outline), &bbox);

            lower.x = static_cast<float>( bbox.xMin) / 64.0f;
            lower.y = static_cast<float>( bbox.yMin) / 64.0f;
            lower.z = 0.0f;
            upper.x = static_cast<float>( bbox.xMax) / 64.0f;
            upper.y = static_cast<float>( bbox.yMax) / 64.0f;
            upper.z = 0.0f;
            
        }       

        /**
         * Destructor
         */
        ~FTBBox()
        {}
        

        /**
         * Move the Bounding Box by a vector.
         *
         * @param distance The distance to move the bbox in 3D space.
         */
        FTBBox& Move( FTPoint distance)
        {
            lower += distance;
            upper += distance;
            return *this;
        }

        FTBBox& operator += ( const FTBBox& bbox) 
        {
			lower = glm::min(lower, bbox.lower);
			upper = glm::max(upper, bbox.upper);
            return *this;
        }
        
        void SetDepth( float depth)
        {
            upper.z = lower.z + depth;
        }
        
		bool IsEmpty() {return glm::all(glm::equal(lower, upper));}
        /**
         * The bounds of the box
         */
		glm::vec3	lower, upper;
    protected:
    
    
    private:

};

#endif  //  __FTBBox__

