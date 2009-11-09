#include <framework.h>

#include <VG\path.h>
#include <VG\Context.h>
#include <VG\GPUData.h>
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
			mPattern.create("pattern.jpg");

			assert(testLoad(mPattern));

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(35.0, (float)mWidth/mHeight,1,2048);

			mFill = mContext.createPaint();
			assert(testLoad(mPattern));
			//mFill.setColorPaint(glm::vec4(0.45f, 0.33f, 0.67f, 1.0f));
			mFill.setPatternPaint((GLuint)mPattern, glm::vec4(1,0,0,1), VG_TILE_REPEAT);

			assert(testLoad(mPattern));
			mPolygon = mContext.createPath(1.0f, 0.0f);

			ubyte segs[] = {VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, /*VG_LINE_TO_ABS,*/ VG_QUAD_TO_ABS, VG_CLOSE_PATH};
			float data[] = {150, 100, 150, 0, 50, 100, /*0, 0,*/ 0, 50, 50, 0};

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

			ubyte sampleTest[] = {/*VG_MOVE_TO_ABS,
								  VG_LINE_TO_ABS,
								  VG_LINE_TO_ABS,
								  VG_LINE_TO_ABS,
								  VG_CLOSE_PATH,*/
								  VG_MOVE_TO_ABS,
								  VG_CUBIC_TO_ABS,
								  VG_CUBIC_TO_ABS, 
								  VG_CLOSE_PATH};
			float sampleData[] = {/*20, 20,
								  20, 80,
								  80, 80,
								  80, 20,*/
								  0,  0,
								  99, 0,  50, 50,  99, 99,
								  0, 99,  50, 50,  0, 0};

			mPolygon.appendData(sizeof(segs), segs, data);
			//mPolygon.appendData(sizeof(quadTest), quadTest, quadData);
			//mPolygon.appendData(sizeof(lineQuadTest), lineQuadTest, lineQuadData);
			//mPolygon.appendData(sizeof(arcTest), arcTest, arcData);
			//mPolygon.appendData(sizeof(squadTest), squadTest, squadData);
			//mPolygon.appendData(sizeof(cubicTest), cubicTest, cubicData);
			//mPolygon.appendData(sizeof(sampleTest), sampleTest, sampleData);

			glClearDepth(1.0);
			glClearStencil(0);

			impl::shared::Acquire();
			mRegion.begin(0,0);
			mRegion.lineTo(150, 100);
			mRegion.lineTo(150, 0);
			mRegion.lineTo(50, 100);
			//mRegion.lineTo(50, 0);
			mRegion.quadTo(0, 50, 50, 0);
			mRegion.quadTo(100, -50, 50, -100);
			//mRegion.cubic(50, 0, 90, -50, 120, 0, 150, 0); 
			mRegion.end(true);

			impl::FillGeometryBuilder	fillBuilder;

			//fillBuilder.begin(0,0);
			//fillBuilder.lineTo(150, 100);
			//fillBuilder.lineTo(150, 0);
			//fillBuilder.lineTo(50, 100);
			////mRegion.lineTo(50, 0);
			//fillBuilder.quadTo(0, 50, 50, 0);
			//fillBuilder.quadTo(100, -50, 50, -100);
			////mRegion.cubic(50, 0, 90, -50, 120, 0, 150, 0); 
			//fillBuilder.end();

			fillBuilder.begin(0, 0);
			fillBuilder.cubicTo(50, 50, 100, -50, 150, 0);
			//fillBuilder.lineTo(50, 100);
			//fillBuilder.lineTo(0, 0);
			fillBuilder.end();

			fillBuilder.copyDataTo(fill);

			impl::StrokeGeometryBuilder	strokeBuilder;

			//strokeBuilder.begin(0,0);
			//strokeBuilder.lineTo(150, 100);
			//strokeBuilder.lineTo(150, 0);
			//strokeBuilder.lineTo(50, 100);
			////mRegion.lineTo(50, 0);
			//strokeBuilder.quadTo(0, 50, 50, 0);
			//strokeBuilder.quadTo(100, -50, 50, -100);
			////mRegion.cubic(50, 0, 90, -50, 120, 0, 150, 0); 
			//strokeBuilder.end(true);

			strokeBuilder.begin(0, 0);
			strokeBuilder.cubicTo(50, 50, 100, -50, 150, 0);
			//fillBuilder.lineTo(50, 100);
			//fillBuilder.lineTo(0, 0);
			strokeBuilder.end(false);

			strokeBuilder.copyDataTo(stroke);
		}

		~VGSample()
		{
			impl::shared::Release();
			mContext.destroyPaint(mFill);
			mContext.destroyPath(mPolygon);
		}

	protected:
		void onPaint(VG& vg)
		{
 			glStencilMask(0xFF);
			glClearStencil(0x80);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(400.0f, 300.0f, -4.0f);
			glScalef(2, -2, 1);
			glCullFace(GL_FRONT_AND_BACK);

			//fill.RasterizeEvenOdd();
			//fill.RasterizeNonZero();
			//impl::RasterizeFillEvenOdd(mRegion.data);

			glColor4f(1, 1, 1, 1);
			glUseProgram(0);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_EQUAL, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glBegin(GL_QUADS);
			glVertex2f(0,-100);
			glVertex2f(0,100);
			glVertex2f(150,100);
			glVertex2f(150,-100);
			glEnd();

			glClear(GL_STENCIL_BUFFER_BIT);

			stroke.Rasterize();
			//impl::RasterizeStroke(mRegion.data);

			glColor4f(1, 0, 0, 1);
			glUseProgram(0);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glBegin(GL_QUADS);
			glVertex2f(-400,-400);
			glVertex2f(-400,400);
			glVertex2f(400,400);
			glVertex2f(400,-400);
			glEnd();

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
		vg::Path	mPolygon;
		vg::Paint	mFill;
		TextureRef	mPattern;
		impl::GPUData	mRegion;
};

int main()
{
	VGSample app;
	app.run();
	return 0;
}
