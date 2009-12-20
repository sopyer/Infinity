#include <framework.h>

#include <VG\path.h>
#include <VG\Context.h>
#include <VG\Rasterizer.h>
#include <VG\GeometryBuilders.h>
#include <VG\SharedResources.h>

GLchar buf[1024];
GLsizei len;

#define PRINT_SHADER_LOG(obj)	obj.getLog(1023, &len, buf);\
	logMessage(#obj##" info\n%s", buf)

float cubicData[] = {0, 0, 50, 50, 67, 20, 48, 30};

class VGSample: public UI::GLFWStage
{
	public:
		VGSample()
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(35.0, (float)mWidth/mHeight,1,2048);

			ubyte segs[] = {VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_QUAD_TO_ABS, VG_CLOSE_PATH};
			float data[] = {150, 100, 150, 0, 50, 100, 0, 0, 0, 50, 50, 0};

			ubyte quadTest[] = {VG_QUAD_TO_ABS, VG_CLOSE_PATH};
			float quadData[] = {50, 100, 100, 0};

			ubyte lineQuadTest[] = {VG_QUAD_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_CLOSE_PATH};
			float lineQuadData[] = {50, 100, 100, 0, 0, 100, 100, 100};


			//ubyte arcTest[] = {VG_LCCWARC_TO_ABS,/* VG_MOVE_TO_ABS,*/ VG_SCCWARC_TO_ABS, VG_CLOSE_PATH};
			////float arcData[] = {0.3, 0.3, 0, 0.5, 0, 0, 0, 0.3, 0.3, 0, 0.5, 0};
			//float arcData[] = {40, 50, 0, 60, 0, /*60, 0,*/ 40, 50, 0, 0, 0};

			//Bug!!!! if main geom actually has zero geometry in fact in covers some pixels on screen - bug was in wrong path points
			//ubyte arcTest[] = {/*VG_MOVE_TO_REL,*/ VG_SCCWARC_TO_REL, VG_SCCWARC_TO_REL, VG_SCCWARC_TO_REL, VG_SCCWARC_TO_REL, VG_CLOSE_PATH};
			//float arcData[] = {/*-25, -25,*/ 25, 25, 0, 0, 50, 25, 25, 0, 50, 0, 25, 25, 0, 0, -50, 25, 25, 0, -50, 0};

			//Bug!!!! - Not a bug but feature :D
			ubyte arcTest[] = {/*VG_MOVE_TO_REL,*/ VG_SCCWARC_TO_REL, VG_LINE_TO_REL, VG_SCCWARC_TO_REL, VG_CLOSE_PATH};
			float arcData[] = {25, 25, 0, 0, 50, -25, -25, 25, 25, 0, -50, 0};

			//ubyte arcTest[] = {/*VG_MOVE_TO_REL,*/ VG_SCCWARC_TO_REL, VG_LINE_TO_REL, VG_SCCWARC_TO_REL, VG_CLOSE_PATH};
			//float arcData[] = {25, 25, 0, 0, 50, 50, -50, 25, 25, 0, -50, 0};

			ubyte squadTest[] = {VG_QUAD_TO_ABS, VG_SQUAD_TO_ABS, VG_CLOSE_PATH};
			float squadData[] = {50, 100, 100, 0, 200, 0};

			ubyte cubicTest[] = {VG_MOVE_TO_ABS, VG_CUBIC_TO_ABS, VG_CLOSE_PATH};
			//float cubicData[] = {0, 0, 50, 70, 150, -20, 200, 50};
			//float cubicData[] = {-200, 200, -100, 100, -200, 0, 150, 0};
			//float cubicData[] = {200, 100, -100, 100, 100, 0, 150, 0};
			//float cubicData[] = {50, 40, 100, 0, 150, 50, 150, 0};
			//Loop
			//float cubicData[] = {0, 0, 60, 50, -10, 50, 50, 0};
			//Cusp
			//float cubicData[] = {0, 0, 50, 50, 0, 50, 50, 0};
			//Serpentine
			//float cubicData[] = {0, 0, 50, 50, 50, 0, 100, 50};
			//Normal
			//float cubicData[] = {0, 0, 0, 50, 50, 50, 50, 0};
			//!!!!!BUG!!!!!
			//float cubicData[] = {0, 0, 50, 50, 70, 0, 45, 30};

			ubyte sampleTest[] = {VG_MOVE_TO_ABS,
								  VG_LINE_TO_ABS,
								  VG_LINE_TO_ABS,
								  VG_LINE_TO_ABS,
								  VG_CLOSE_PATH,
								  VG_MOVE_TO_ABS,
								  VG_CUBIC_TO_ABS,
								  VG_CUBIC_TO_ABS, 
								  VG_CLOSE_PATH};
			float sampleData[] = {20, 20,
								  20, 80,
								  80, 80,
								  80, 20,
								  0,  0,
								  99, 0,  50, 50,  99, 99,
								  0, 99,  50, 50,  0, 0};

			//mPolygon.appendData(sizeof(segs), segs, data);
			//mPolygon.appendData(sizeof(quadTest), quadTest, quadData);
			//mPolygon.appendData(sizeof(lineQuadTest), lineQuadTest, lineQuadData);
			//mPolygon.appendData(sizeof(arcTest), arcTest, arcData);
			//mPolygon.appendData(sizeof(squadTest), squadTest, squadData);
			//mPolygon.appendData(sizeof(cubicTest), cubicTest, cubicData);
			//mPolygon.appendData(sizeof(sampleTest), sampleTest, sampleData);

			glClearDepth(1.0);
			glClearStencil(0);

			impl::shared::Acquire();

			impl::buildFillGeometry(ARRAY_SIZE(segs), segs, data, fill);

			//impl::FillGeometryBuilder	fillBuilder;

			Array<glm::vec2>	v;

			//fillBuilder.begin(0,0);
			//fillBuilder.lineTo(150, 100);
			//fillBuilder.lineTo(150, 0);
			//v.clear();
			//v.pushBack(glm::vec2(50, 100));
			//v.pushBack(glm::vec2(0, 50));
			//v.pushBack(glm::vec2(50, 0));
			//fillBuilder.addQuad(v);
			//fillBuilder.lineTo(50, 100);
			//fillBuilder.lineTo(50, 0);
			//fillBuilder.lineTo(50, -100);
			//fillBuilder.end();

			//v.clear();
			//v.pushBack(glm::vec2(50, 0));
			//v.pushBack(glm::vec2(100, -50));
			//v.pushBack(glm::vec2(50, -100));
			//fillBuilder.addQuad(v);

			//fillBuilder.begin(0, 0);
			//fillBuilder.arcTo(arc::LARGE_CW, 50, 50, 0, 100, 0);
			//fillBuilder.arcTo(arc::LARGE_CW, 80, 50, 45, 100, 0);
			//v.pushBack(glm::vec2(0, 0));
			//v.pushBack(glm::vec2(50, 50));
			//v.pushBack(glm::vec2(100, -50));
			//v.pushBack(glm::vec2(150, 0));
			//fillBuilder.addCubic(v);
			//fillBuilder.lineTo(50, 100);
			//fillBuilder.lineTo(0, 0);
			//fillBuilder.end();

			//fillBuilder.copyDataTo(fill);

			impl::StrokeGeometryBuilder	strokeBuilder;

			//VGubyte	ss[] = {VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS,
			//				VG_QUAD_TO_ABS, VG_SQUAD_TO_ABS, VG_CLOSE_PATH};
			//float	ssd[] = {150, 100, 150, 0, 50, 100, 0, 50, 50, 0, 50, -100};
			VGubyte	ss[] = {VG_QUAD_TO_ABS/*, VG_CLOSE_PATH*/};
			float	ssd[] = {50, 139, 70, 30};
			impl::buildStrokeGeometry(ARRAY_SIZE(ss), ss, ssd, stroke);

			//strokeBuilder.begin(50,100);
			////strokeBuilder.begin(0,0);
			////strokeBuilder.lineTo(150, 100);
			////strokeBuilder.lineTo(150, 0);
			////strokeBuilder.lineTo(50, 100);
			////strokeBuilder.quadTo(0, 50, 50, 0);
			////strokeBuilder.quadTo(100, -50, 50, -100);
			//strokeBuilder.end(/*true*/false);

			//strokeBuilder.begin(0,0);
			//strokeBuilder.lineTo(150, 100);
			//strokeBuilder.lineTo(150, 0);
			//strokeBuilder.lineTo(50, 100);
			//v.clear();
			//v.pushBack(glm::vec2(50, 100));
			//v.pushBack(glm::vec2(0, 50));
			//v.pushBack(glm::vec2(50, 0));
			//strokeBuilder.quadTo(v);
			//v.clear();
			//v.pushBack(glm::vec2(50, 0));
			//v.pushBack(glm::vec2(100, -50));
			//v.pushBack(glm::vec2(50, -100));
			//strokeBuilder.quadTo(v);
			//strokeBuilder.end(true);

			//strokeBuilder.begin(0, 0);
			////v.clear();
			////v.pushBack(glm::vec2(0, 0));
			////v.pushBack(glm::vec2(50, 50));
			////v.pushBack(glm::vec2(100, -50));
			////v.pushBack(glm::vec2(150, 0));
			////strokeBuilder.cubicTo(v);
			////strokeBuilder.arcTo(VG_LCWARC_TO, glm::vec2(50, 50), 0, glm::vec2(0, 0), glm::vec2(100, 0));
			//strokeBuilder.arcTo(VG_LCWARC_TO, glm::vec2(80, 50), 45, glm::vec2(0, 0), glm::vec2(100, 0));
			////strokeBuilder.cubicTo(50, 50, 100, -50, 150, 0);
			////strokeBuilder.lineTo(150, 0);
			////strokeBuilder.lineTo(50, 100);
			////strokeBuilder.lineTo(0, 0);
			//strokeBuilder.end(false);

			//strokeBuilder.copyDataTo(stroke);
		}

		~VGSample()
		{
			impl::shared::Release();
		}

	protected:
		void drawQuad(const glm::vec2& min, const glm::vec2& max, float offset)
		{
			glBegin(GL_QUADS);
			glVertex2f(min.x-offset, min.y-offset);
			glVertex2f(min.x-offset, max.y+offset);
			glVertex2f(max.x+offset, max.y+offset);
			glVertex2f(max.x+offset, min.y-offset);
			glEnd();
		}

		void clearStencil(const glm::vec2& min, const glm::vec2& max, float offset)
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);

			glEnable(GL_STENCIL_TEST);
			glDisable(GL_DEPTH_TEST);

			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
			glStencilMask(0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			glUseProgram(0);

			drawQuad(min, max, offset);

			glPopAttrib();
		}

		void onPaint(VG& vg)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(400.0f, 300.0f, -4.0f);
			glScalef(2, -2, 1);

			clearStencil(fill.mMin, fill.mMax, 0);
			fill.RasterizeNonZero();

			glColor4f(1, 1, 1, 1);
			glUseProgram(0);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			drawQuad(fill.mMin, fill.mMax, 0);

			clearStencil(stroke.mMin, stroke.mMax, 5+10/*Take miter joints into account*/);
			stroke.Rasterize();

			glColor4f(1, 0, 0, 1);
			glUseProgram(0);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			drawQuad(stroke.mMin, stroke.mMax, 5+10/*Take miter joints into account*/);

			glFlush();
		}

		void onUpdate(float frame_time)
		{
			if( glfwGetKey(GLFW_KEY_ESC) )
				close();
		}
		
	private:
		impl::FillGeometry			fill;
		impl::StrokeGeometry		stroke;

		vg::Context	mContext;
};

int main()
{
	VGSample app;
	app.run();
	return 0;
}
