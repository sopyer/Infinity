#pragma once

#include <vector>
#include <glm/glm.h>
#include <gl/glee.h>
#include <vg/openvg.h>

#include <FTGLTextureFont.h>
#include <Path.h>
#include <Paint.h>
#include <VGCommon.h>

namespace vg
{
	struct Font: public Handle<FTFont*>
	{
		void	setFaceSize(VGuint size) {mHandle->FaceSize(size);}
	};

	Font	createFont(const char* name);
	void	destroyFont(Font font);
	void	drawString(Font font, float x, float y, const char* str);
	void	drawString(Font font, float x, float y, const wchar_t* str);

	Path	createPath();
	void	destroyPath(Path path);

	void	drawPath(Path path, Paint paint);

	class Context
	{
		public:
			Context();
			~Context();

			void drawPath(Path path, Paint fill);

			//Paint createPaint();
			//void destroyPaint(Paint paint);

			/*Font experimental interface*/
			
			//void	setFont(Font font)
			//{
			//	mCurFont = font;
			//}
			//
			//void	getTextMaxExtent(const char* str, float& w, float& h)
			//{

			//}

			//void	getTextMaxExtent(const wchar_t* str, float& w, float& h)
			//{
			//}
			
		private:
			//Font	mCurFont;
	};
}