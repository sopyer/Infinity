#pragma once

#include <vector>
#include <glm/glm.h>
#include <gl/glee.h>
#include <vg/openvg.h>
#include ".\types.h"

#include <Path.h>
#include <Paint.h>

namespace vg
{
	class Context
	{
		public:
			Context();
			~Context();

			Path createPath(float scale, float bias);
			//void drawPath(Path path, ProgramRef fill);
			void drawPath(Path path, Paint fill);
			void destroyPath(Path path);

			Paint createPaint();
			void destroyPaint(Paint paint);

		private:
			void maskPathRegion(Path path);
	};
}