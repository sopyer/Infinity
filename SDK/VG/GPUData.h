#pragma once

#include <vector>
#include <glm/glm.h>
#include <glm/glmext.h>
#include <vg/openvg.h>
#include <utils.h>
#include ".\types.h"
#include "SharedResources.h"
#include "Cubic.h"

namespace impl
{
	struct Vertex
	{
		glm::vec2	p;
		glm::vec2	n;
		glm::vec3	tc;

		Vertex(const glm::vec2& _p, const glm::vec2& _n, const glm::vec3& _tc):
			p(_p), n(_n), tc(_tc)
		{}

		Vertex(const glm::vec2& _p, const glm::vec2& _n):
			p(_p), n(_n)
		{}

		Vertex(const glm::vec2& _p, const glm::vec3& _tc):
			p(_p), tc(_tc)
		{}

		Vertex(const glm::vec2& _p): p(_p)
		{}

		Vertex()
		{
		}
	};

	struct FillVertex
	{
		glm::vec2	p;
		glm::vec3	tc;

		FillVertex(const glm::vec2& _p, const glm::vec3& _tc):
			p(_p), tc(_tc)
		{}

		FillVertex(const glm::vec2& _p, const float tcx, const float tcy):
			p(_p), tc(tcx, tcy, 0)
		{}

		FillVertex(const float x, const float y, const float tcx, const float tcy)
			: p(x, y), tc(tcx, tcy, 0)
		{}
		FillVertex(const glm::vec2& _p): p(_p)
		{}

		FillVertex(const float x, const float y): p(x, y)
		{}

		FillVertex()
		{}
	};

	struct StrokeVertex
	{
		glm::vec2	p;
		glm::vec2	n;
		glm::vec3	tc;

		StrokeVertex(const glm::vec2& _p, const glm::vec2& _n, const glm::vec3& _tc):
			p(_p), n(_n), tc(_tc)
		{}

		StrokeVertex(const glm::vec2& _p, const glm::vec2& _n):
			p(_p), n(_n)
		{}

		StrokeVertex(const glm::vec2& _p, const glm::vec3& _tc):
			p(_p), tc(_tc)
		{}

		StrokeVertex(const glm::vec2& _p): p(_p)
		{}

		StrokeVertex()
		{}
	};


	inline glm::vec2 evalVertex(FillVertex v)
	{return v.p;}

	inline glm::vec2 evalVertex(StrokeVertex v)
	{return v.p+v.n;}

	typedef Array<VGuint>	IndexVector;
	
	template<class VertexType, VGuint PRIM_COUNT>
	struct Geometry
	{
		Array<VertexType>	vertices;
		IndexVector			indices;
		
		GLsizei	offset[PRIM_COUNT], count[PRIM_COUNT];
		
		static const VGuint VertexSize = sizeof(VertexType);

		void drawElements(VGuint primType) const
		{
			assert(primType<PRIM_COUNT);
			if (count[primType])
				glDrawElements(GL_TRIANGLES, count[primType], GL_UNSIGNED_INT, &indices[0]+offset[primType]);
		}

		void drawElementsNZ(VGuint primType)
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glCullFace(GL_BACK);
			drawElements(primType);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glCullFace(GL_FRONT);
			drawElements(primType);
		}
	};

#define CCW(primType)	((primType)+1)
#define CW(primType)	((primType)+0)

	enum FillPrimitiveTypes
	{
		FILL_PRIM_TYPE_TRI,
		FILL_PRIM_TYPE_ARC,
		FILL_PRIM_TYPE_QUAD,
		FILL_PRIM_TYPE_CUBIC,
		FILL_PRIM_TYPE_COUNT,
	};

	struct FillGeometry: public Geometry<FillVertex, FILL_PRIM_TYPE_COUNT>
	{
		void RasterizeEvenOdd()
		{
			if (vertices.empty())
				return;

			FillVertex& vtx = vertices[0];

			//// Clear stencil
			//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			//glBegin(GL_QUADS);
			//glVertex2f(path.mObject->min.x, path.mObject->min.y);
			//glVertex2f(path.mObject->max.x, path.mObject->min.y);
			//glVertex2f(path.mObject->max.x, path.mObject->max.y);
			//glVertex2f(path.mObject->min.x, path.mObject->max.y);
			//glEnd();

			glPushAttrib(GL_ALL_ATTRIB_BITS);

			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_FRONT_AND_BACK);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 0, 1);
			glStencilMask(0x01);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, VertexSize, &vtx.p);

			glUseProgram(0);
			drawElements(FILL_PRIM_TYPE_TRI);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3, GL_FLOAT, VertexSize, &vtx.tc);

			glUseProgram(shared::prgMaskQuad);
			drawElements(FILL_PRIM_TYPE_QUAD);

			glUseProgram(shared::prgMaskCubic);
			drawElements(FILL_PRIM_TYPE_CUBIC);

			glUseProgram(shared::prgMaskArc);
			drawElements(FILL_PRIM_TYPE_ARC);

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			
			glPopAttrib();
		}

		void RasterizeNonZero()
		{
			if (vertices.empty())
				return;

			//// Clear stencil
			//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			//glBegin(GL_QUADS);
			//glVertex2f(path.mObject->min.x, path.mObject->min.y);
			//glVertex2f(path.mObject->max.x, path.mObject->min.y);
			//glVertex2f(path.mObject->max.x, path.mObject->max.y);
			//glVertex2f(path.mObject->min.x, path.mObject->max.y);
			//glEnd();

			FillVertex& vtx = vertices[0];

			glPushAttrib(GL_ALL_ATTRIB_BITS);

			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
			glStencilMask(0xFF);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, VertexSize, &vtx.p);

			glUseProgram(0);
			drawElementsNZ(FILL_PRIM_TYPE_TRI);

			glCullFace(GL_FRONT_AND_BACK);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3, GL_FLOAT, VertexSize, &vtx.tc);

			glUseProgram(shared::prgMaskQuad);
			drawElementsNZ(FILL_PRIM_TYPE_QUAD);

			glUseProgram(shared::prgMaskCubic);
			drawElementsNZ(FILL_PRIM_TYPE_CUBIC);

			glUseProgram(shared::prgMaskArc);
			drawElementsNZ(FILL_PRIM_TYPE_ARC);

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			glPopAttrib();
		}
	};

	template<class VertexType, VGuint PRIM_COUNT>
	class GeometryBuilder
	{
		protected:
			GeometryBuilder():mBase(InvalidIndex), mCursor(InvalidIndex)
			{}

			VGuint addVertex(const VertexType& v)
			{
				assert(mVertices.size()<(size_t)VG_MAXINT);
				VGuint idx = (VGuint)mVertices.size();
				mVertices.push_back(v);
				return idx;
			}

			VGuint addVertices(VGuint count, VertexType v[])
			{
				assert(mVertices.size()<(size_t)VG_MAXINT);
				VGuint idx = (VGuint)mVertices.size();
				mVertices.pushBack(v, v+count);
				return idx;
			}
			
			void addPrim(VGuint primType, VGuint i0, VGuint i1, VGuint i2)
			{
				assert(primType<PRIM_COUNT);
				glm::vec2	v1 = evalVertex(mVertices[i1])-evalVertex(mVertices[i0]);
				glm::vec2	v2 = evalVertex(mVertices[i2])-evalVertex(mVertices[i0]);

				float	det = v1.x*v2.y - v1.y*v2.x;

				if (det==0)
					return;

				IndexVector& indices = mIndices[/*det>0?CCW(primType):CW(primType)*/primType];

				indices.push_back(i0);
				indices.push_back(i1);
				indices.push_back(i2);
			}
		
		protected:
			glm::vec2	getLastVertex()  {return mVertices[mCursor].p;}
			glm::vec2	getFirstVertex() {return mVertices[mBase].p;}
			bool		isFirstPrim()	 {return mCursor==mBase;}

		protected:
			static const VGuint	InvalidIndex = ~((VGuint)0);

		protected:
			Array<VertexType>		mVertices;
			IndexVector				mIndices[PT_COUNT];
			VGuint					mBase, mCursor;
	};

	class FillGeometryBuilder: public GeometryBuilder<FillVertex, FILL_PRIM_TYPE_COUNT>
	{
		public:
			void begin(VGfloat x, VGfloat y)
			{
				assert(mBase   == InvalidIndex);
				assert(mCursor == InvalidIndex);
				mCursor = mBase = addVertex(FillVertex(x, y));
			}

			void end()
			{
				mCursor = mBase=InvalidIndex;
			}
			
			void lineTo(VGfloat x1, VGfloat y1)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);

				VGuint i1 = addVertex(FillVertex(x1, y1));
				
				addPrim(FILL_PRIM_TYPE_TRI, mBase, mCursor, i1);

				mCursor = i1;
			}

			void addQuad(const Array<glm::vec2>& v)
			{
				assert(v.size()==3);
				
				//TODO: check for degenerate case...

				VGuint i0 = 
				addVertex(FillVertex(v[0], 0.0f, 0.0f));
				addVertex(FillVertex(v[1], 0.5f, 0.0f));
				addVertex(FillVertex(v[2], 1.0f, 1.0f));

				addPrim(FILL_PRIM_TYPE_QUAD, i0, i0+1, i0+2);
			}

			void addCubic(const Array<glm::vec2>& v)
			{
				assert(v.size()==4);

				Array<glm::vec2>	pos;
				Array<glm::vec3>	tc;

				cubicTriVertices(v, pos, tc);

				assert(pos.size()==tc.size());
				assert(tc.size()%4==0);

				for (size_t i=0; i<pos.size(); ++i)
				{
					addVertex(FillVertex(pos[i], tc[i]));
				}

				VGuint	first = (VGuint)mVertices.size()-pos.size(),
						last  = first+(VGuint)pos.size()-1;

				for (VGuint i0=first; i0<last; i0+=4)
				{
					addPrim(FILL_PRIM_TYPE_CUBIC, i0,    i0+1, i0+2);
					addPrim(FILL_PRIM_TYPE_CUBIC, i0+2,  i0+3, i0+0);
					addPrim(FILL_PRIM_TYPE_TRI,   first, i0,   i0+3);
				}
			}

			void addArc(VGPathSegment type, const glm::vec2& radius, VGfloat angle,
					   const glm::vec2& p0, const glm::vec2& p1)
			{
				Array<glm::vec2>	pos;
				Array<glm::vec3>	tc;

				arcTriVertices(type, radius, angle, p0, p1, pos, tc);
				
				assert(pos.size()==tc.size());

				size_t count = pos.size();

				for (size_t i=0; i<count; ++i)
				{
					addVertex(FillVertex(pos[i], tc[i]));
				}

				VGuint	first = (VGuint)mVertices.size()-pos.size(),
						last  = first+(VGuint)pos.size()-1;

				for (VGuint i0=first; i<last-1; i+=2)
				{
					addPrim(FILL_PRIM_TYPE_ARC,  i0,    i0+1, i0+2);
					addPrim(FILL_PRIM_TYPE_TRI,  first, i0,   i0+2);
				}
			}
			
			void copyDataTo(FillGeometry& fillGeom)
			{
				VGuint offset = 0, count;

				for (size_t i=0; i<FILL_PRIM_TYPE_COUNT; ++i)
				{
					assert(mIndices[i].size() < VG_MAXINT);
					count = (VGuint)mIndices[i].size();

					fillGeom.offset[i] = offset;
					fillGeom.count[i] = count;
					fillGeom.indices.pushBack(mIndices[i].begin(), mIndices[i].end());

					offset += count;
				}

				fillGeom.vertices.pushBack(mVertices.begin(), mVertices.end());
			}

	};

#define DECLARE_STROKE_PRIM_TYPE(name)	STROKE_PRIM_TYPE_##name,									\
										STROKE_PRIM_TYPE_##name##_CW  = STROKE_PRIM_TYPE_##name,	\
										STROKE_PRIM_TYPE_##name##_CCW

	enum StrokePrimitiveTypes
	{
		DECLARE_STROKE_PRIM_TYPE(TRI),
		DECLARE_STROKE_PRIM_TYPE(ARC),
		DECLARE_STROKE_PRIM_TYPE(QUAD),
		DECLARE_STROKE_PRIM_TYPE(CUBIC),
		DECLARE_STROKE_PRIM_TYPE(JOINT_BEVEL),
		DECLARE_STROKE_PRIM_TYPE(JOINT_MITER),
		DECLARE_STROKE_PRIM_TYPE(JOINT_ROUND),
		DECLARE_STROKE_PRIM_TYPE(CAP_ROUND),
		DECLARE_STROKE_PRIM_TYPE(CAP_BEVEL),
		STROKE_PRIM_TYPE_COUNT,
	};

	struct StrokeGeometry: public Geometry<StrokeVertex, STROKE_PRIM_TYPE_COUNT>
	{
		void Rasterize()
		{
			if (vertices.empty())
				return;

			StrokeVertex& vtx = vertices[0];

			glPushAttrib(GL_ALL_ATTRIB_BITS);

			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_FRONT_AND_BACK);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
			glStencilMask(0xFF);

			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

 			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &vtx.p);

			glUseProgram(shared::prgMaskStrokeSeg);
			glEnableVertexAttribArray(shared::locOffsetAttrib);
			glVertexAttribPointer(shared::locOffsetAttrib, 2, GL_FLOAT, false, VertexSize, &vtx.n);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			drawElements(STROKE_PRIM_TYPE_TRI_CCW);
			drawElements(STROKE_PRIM_TYPE_JOINT_BEVEL_CCW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			drawElements(STROKE_PRIM_TYPE_TRI_CW);
			drawElements(STROKE_PRIM_TYPE_JOINT_BEVEL_CW);
			glDisableVertexAttribArray(shared::locOffsetAttrib);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), &vtx.tc);

			glUseProgram(shared::prgStrokeMaskQuad);
			glEnableVertexAttribArray(shared::locOffsetAttribQuad);
			glVertexAttribPointer(shared::locOffsetAttribQuad, 2, GL_FLOAT, false, VertexSize, &vtx.n);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			drawElements(STROKE_PRIM_TYPE_QUAD_CCW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			drawElements(STROKE_PRIM_TYPE_QUAD_CW);
			glDisableVertexAttribArray(shared::locOffsetAttribQuad);

			glUseProgram(shared::prgStrokeMaskCubic);
			glEnableVertexAttribArray(shared::attrOffsetCubic);
			glVertexAttribPointer(shared::attrOffsetCubic, 2, GL_FLOAT, false, VertexSize, &vtx.n);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			drawElements(STROKE_PRIM_TYPE_CUBIC_CCW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			drawElements(STROKE_PRIM_TYPE_CUBIC_CW);
			glDisableVertexAttribArray(shared::attrOffsetCubic);

			glUseProgram(shared::prgStrokeMaskArc);
			glEnableVertexAttribArray(shared::attrOffsetArc);
			glVertexAttribPointer(shared::attrOffsetArc, 2, GL_FLOAT, false, VertexSize, &vtx.n);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			drawElements(STROKE_PRIM_TYPE_ARC_CCW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			drawElements(STROKE_PRIM_TYPE_ARC_CW);
			glDisableVertexAttribArray(shared::attrOffsetArc);

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			glPopAttrib();
		}
	};

	class StrokeGeometryBuilder: public GeometryBuilder<StrokeVertex, STROKE_PRIM_TYPE_COUNT>
	{
		public:
			void begin(VGfloat x, VGfloat y)
			{
				assert(mBase   == InvalidIndex);
				assert(mCursor == InvalidIndex);
				mCursor = mBase = addVertex(StrokeVertex(glm::vec2(x, y)));
			}

			void end(bool closePath)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);

				if (closePath)
				{
					//Add closing geometry to the strokes
					lineTo(getFirstVertex().x, getFirstVertex().y);
					addJointBevel(mCursor-2, mBase+1);
				}
				else
				{
					//generate caps
				}

				mCursor = mBase=InvalidIndex;
			}
			
			void lineTo(VGfloat x1, VGfloat y1)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);

				glm::vec2	p0(getLastVertex()), p1(x1, y1), n(calcOffset(p0, p1));

				StrokeVertex v[] = {
					StrokeVertex(p0, n), StrokeVertex(p0, -n),
					StrokeVertex(p1, n), StrokeVertex(p1, -n),
					StrokeVertex(p1)
				};
				
				VGuint begin = addVertices(ARRAY_SIZE(v), v);
				VGuint end   = begin+2;

				assert(mCursor == begin-1);

				addStroke(begin, end);
			}

			void quadTo(VGfloat x1, VGfloat y1, VGfloat x2, VGfloat y2)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);

				glm::vec2	p0(getLastVertex()), p1(x1, y1), p2(x2, y2);
				glm::vec2	n01(calcOffset(p0, p1)), n12(calcOffset(p1, p2));
				glm::vec2	offset(calcOffsetN(n01, n12));
				
				VGuint	begin, end;

				StrokeVertex v[] = {
					StrokeVertex(p0, n01,     glm::vec3(0.0f, 0.0f, 0.0f)),
					StrokeVertex(p0, -n01,    glm::vec3(0.0f, 0.0f, 0.0f)),
					StrokeVertex(p1, offset,  glm::vec3(0.5f, 0.0f, 0.0f)),
					StrokeVertex(p1, -offset, glm::vec3(0.5f, 0.0f, 0.0f)),
					StrokeVertex(p2, n12,     glm::vec3(1.0f, 1.0f, 0.0f)),
					StrokeVertex(p2, -n12,    glm::vec3(1.0f, 1.0f, 0.0f)),
					StrokeVertex(p2)
				};

				begin = addVertices(ARRAY_SIZE(v), v);
				end   = begin+4;

				addPrim(STROKE_PRIM_TYPE_QUAD, begin,   begin+2, begin+4);
				addPrim(STROKE_PRIM_TYPE_QUAD, begin+5, begin+3, begin+1);

				assert(mCursor == begin-1);

				addStroke(begin, end);
			}

			void addSimpleCubic(VGuint i0, VGuint i1, VGuint i2, VGuint i3)
			{
				addPrim(STROKE_PRIM_TYPE_CUBIC, i0, i1, i2);
				addPrim(STROKE_PRIM_TYPE_CUBIC, i2, i3, i0);
			}

			void cubicTo(VGfloat x1, VGfloat y1, VGfloat x2, VGfloat y2, VGfloat x3, VGfloat y3)
			{
				Array<glm::vec2>	pts;
				Array<glm::vec2>	pos;
				Array<glm::vec3>	tc;

				pts.pushBack(getLastVertex());
				pts.pushBack(glm::vec2(x1, y1));
				pts.pushBack(glm::vec2(x2, y2));
				pts.pushBack(glm::vec2(x3, y3));

				cubicTriVertices(pts, pos, tc);

				VGuint	first = (VGuint)mVertices.size(),
						last  = first+(VGuint)pos.size()*2-1;

				for (size_t i=0; i<pos.size(); ++i)
				{
					glm::vec2	n;

					if (i%4==3)
						n = calcOffset(pos[i-1], pos[i]);
					else if (i%4!=0)
						n = calcOffset(pos[i-1], pos[i], pos[i+1]);
					else
						n = calcOffset(pos[i],   pos[i+1]);

					last = addVertex(StrokeVertex(pos[i],  n, tc[i]));
					last = addVertex(StrokeVertex(pos[i], -n, tc[i]));
				}

				for (VGuint i0=first; i0<last; i0+=8)
				{
					addSimpleCubic(i0, i0+2, i0+4, i0+6);
					addPrim(STROKE_PRIM_TYPE_TRI, mCursor+1, i0, i0+6);

					addSimpleCubic(i0+7, i0+5, i0+3, i0+1);
					addPrim(STROKE_PRIM_TYPE_TRI, mCursor+2, i0+7, i0+1);
				}

				last = addVertex(StrokeVertex(glm::vec2(x3, y3)));

				addStroke(mCursor+1, last-2);
			}

			void arcTo(VGPathSegment type, VGfloat rx, VGfloat ry, VGfloat angle, VGfloat x1, VGfloat y1)
			{
				Array<glm::vec2>	pos;
				Array<glm::vec3>	tc;

				arcTriVertices(type, glm::vec2(rx, ry), angle, getLastVertex(), glm::vec2(x1, y1), pos, tc);
				
				VGuint	last;

				for (size_t i=0; i<pos.size(); ++i)
				{
					glm::vec2	n;

					if (i==pos.size()-1)
						n = calcOffset(pos[i-1], pos[i]);
					else if (i%2==1)
						n = calcOffset(pos[i-1], pos[i], pos[i+1]);
					else
						n = calcOffset(pos[i],   pos[i+1]);

					last = addVertex(StrokeVertex(pos[i],  n, tc[i]));
					last = addVertex(StrokeVertex(pos[i], -n, tc[i]));
				}

				assert(last==mCursor+pos.size()*2);

				for (size_t i=0; i<pos.size()-1; i+=2)
				{
					addPrim(STROKE_PRIM_TYPE_ARC,  mCursor+i*2+1, mCursor+i*2+3, mCursor+i*2+5);
					addPrim(STROKE_PRIM_TYPE_TRI,  mCursor+1,     mCursor+i*2+1, mCursor+i*2+5);

					addPrim(STROKE_PRIM_TYPE_ARC,  mCursor+i*2+6, mCursor+i*2+4, mCursor+i*2+2);
					addPrim(STROKE_PRIM_TYPE_TRI,  mCursor+2,     mCursor+i*2+6, mCursor+i*2+2);
				}

				last = addVertex(glm::vec2(x1, y1));

				addStroke(mCursor+1, last-2);
			}
			
			void copyDataTo(StrokeGeometry& strokeGeom)
			{
				VGuint offset = 0, count;

				for (size_t i=0; i<STROKE_PRIM_TYPE_COUNT; ++i)
				{
					strokeGeom.offset[i] = offset;

					assert(mIndices[i].size() < VG_MAXINT);
					
					count = (VGuint)mIndices[i].size();
					strokeGeom.count[i] = count;
					strokeGeom.indices.pushBack(mIndices[i].begin(), mIndices[i].end());
					offset += count;
				}

				strokeGeom.vertices.pushBack(mVertices.begin(),	mVertices.end());
			}

		private:
			void addStroke(u32 start, u32 end)
			{
				//Order of indices is very important!!!
				addPrim(STROKE_PRIM_TYPE_TRI, start, end,     end+1);
				addPrim(STROKE_PRIM_TYPE_TRI, end+1, start+1, start);

				if (isFirstPrim())
				{
					//addCapButt(mCursor)
					//addCapRound(mCursor)
				}
				else
				{
					addJointBevel(mCursor);
					//addJointMiter(mCursor);
					//addJointRound(mCursor);
				}

				mCursor = end+2;
			}

			void addJointBevel(VGuint center)
			{
				addJointBevel(center-2, center+1);
			}

			void addJointBevel(VGuint first, VGuint second)
			{
				glm::vec2	v1 = mVertices[first].n,
							v2 = mVertices[second].n;
				
				float		det = v1.x*v2.y - v1.y*v2.x;
				
				if (det<0)
					addPrim(STROKE_PRIM_TYPE_JOINT_BEVEL, first,   second,   second-1);
				else
					addPrim(STROKE_PRIM_TYPE_JOINT_BEVEL, first+1, second+1, second-1);
			}
	};
}
