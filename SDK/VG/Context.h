#pragma once

#include <vector>
#include <glm/glm.h>
#include <gl/glee.h>
#include <vg/openvg.h>
#include ".\types.h"

#include <FTGLTextureFont.h>

namespace vg
{
	struct Font
	{
		void	setFaceSize(VGuint size) {object->FaceSize(size);}

		FTFont*	object;
	};

	Font	createFont(const char* name);
	void	destroyFont(Font font);
	void	drawString(Font font, float x, float y, const char* str);
	void	drawString(Font font, float x, float y, const wchar_t* str);

	class Context
	{
		public:
			Context();
			~Context();

			//Path createPath(float scale, float bias);
			//void drawPath(Path path, ProgramRef fill);
			//void drawPath(Path path, Paint fill);
			//void destroyPath(Path path);

			//Paint createPaint();
			//void destroyPaint(Paint paint);

			/*Font experimental interface*/
			
			void	setFont(Font font)
			{
				mCurFont = font;
			}
			
			void	getTextMaxExtent(const char* str, float& w, float& h)
			{

			}

			void	getTextMaxExtent(const wchar_t* str, float& w, float& h)
			{
			}
			
		private:
			Font	mCurFont;
	};
}