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

	typedef std::vector<VGuint>	IndexVector;
	
#define DECLARE_PRIM_TYPE(name)	PT_##name,							\
								PT_##name##_CW  = PT_##name,		\
								PT_##name##_CCW

	enum PrimitiveTypes
	{
		DECLARE_PRIM_TYPE(FILL_TRI),
		DECLARE_PRIM_TYPE(FILL_ARC),
		DECLARE_PRIM_TYPE(FILL_QUAD),
		DECLARE_PRIM_TYPE(FILL_CUBIC),
		DECLARE_PRIM_TYPE(STROKE_TRI),
		DECLARE_PRIM_TYPE(STROKE_ARC),
		DECLARE_PRIM_TYPE(STROKE_QUAD),
		DECLARE_PRIM_TYPE(STROKE_CUBIC),
		DECLARE_PRIM_TYPE(JOINT_BEVEL),
		DECLARE_PRIM_TYPE(JOINT_MITER),
		DECLARE_PRIM_TYPE(JOINT_ROUND),
		DECLARE_PRIM_TYPE(CAP_ROUND),
		DECLARE_PRIM_TYPE(CAP_BEVEL),
		PT_COUNT,
	};

	template<class VertexType, VGuint PRIM_COUNT>
	struct Geometry
	{
		std::vector<VertexType>	vertices;
		IndexVector				indices;
		
		GLsizei	offset[PRIM_COUNT], count[PRIM_COUNT];
		
		static const VGuint VertexSize = sizeof(VertexType);

		void drawElements(VGuint primType) const
		{
			//assert(primType<PRIM_COUNT);
			if (count[primType])
				glDrawElements(GL_TRIANGLES, count[primType], GL_UNSIGNED_INT, &indices[0]+offset[primType]);
		}
	};

#define DECLARE_FILL_PRIM_TYPE(name)	FILL_PRIM_TYPE_##name,									\
										FILL_PRIM_TYPE_##name##_CW  = FILL_PRIM_TYPE_##name,	\
										FILL_PRIM_TYPE_##name##_CCW

#define CCW(primType)	((primType)+1)
#define CW(primType)	((primType)+0)

	enum FillPrimitiveTypes
	{
		DECLARE_FILL_PRIM_TYPE(TRI),
		DECLARE_FILL_PRIM_TYPE(ARC),
		DECLARE_FILL_PRIM_TYPE(QUAD),
		DECLARE_FILL_PRIM_TYPE(CUBIC),
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
			drawElements(FILL_PRIM_TYPE_TRI_CCW);
			drawElements(FILL_PRIM_TYPE_TRI_CW);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3, GL_FLOAT, VertexSize, &vtx.tc);

			glUseProgram(shared::prgMaskQuad);
			drawElements(FILL_PRIM_TYPE_QUAD_CCW);
			drawElements(FILL_PRIM_TYPE_QUAD_CW);

			glUseProgram(shared::prgMaskCubic);
			drawElements(FILL_PRIM_TYPE_CUBIC_CCW);
			drawElements(FILL_PRIM_TYPE_CUBIC_CW);

			glUseProgram(shared::prgMaskArc);
			drawElements(FILL_PRIM_TYPE_ARC_CCW);
			drawElements(FILL_PRIM_TYPE_ARC_CW);

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

			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_FRONT_AND_BACK);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
			glStencilMask(0xFF);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, VertexSize, &vtx.p);

			glUseProgram(0);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			drawElements(FILL_PRIM_TYPE_TRI_CCW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			drawElements(FILL_PRIM_TYPE_TRI_CW);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3, GL_FLOAT, VertexSize, &vtx.tc);

			glUseProgram(shared::prgMaskQuad);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			drawElements(FILL_PRIM_TYPE_QUAD_CCW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			drawElements(FILL_PRIM_TYPE_QUAD_CW);

			glUseProgram(shared::prgMaskCubic);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			drawElements(FILL_PRIM_TYPE_CUBIC_CCW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			drawElements(FILL_PRIM_TYPE_CUBIC_CW);

			glUseProgram(shared::prgMaskArc);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			drawElements(FILL_PRIM_TYPE_ARC_CCW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			drawElements(FILL_PRIM_TYPE_ARC_CW);

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
				mVertices.insert(mVertices.end(), v, v+count);
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

				IndexVector& indices = mIndices[det>0?CCW(primType):CW(primType)];

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
			std::vector<VertexType>	mVertices;
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
				
				assert(mCursor == i1-1);

				addPrim(FILL_PRIM_TYPE_TRI, mBase, mCursor, i1);

				mCursor = i1;
			}

			void quadTo(VGfloat x1, VGfloat y1, VGfloat x2, VGfloat y2)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);

				FillVertex v[] = {
					FillVertex(getLastVertex(), 0.0f, 0.0f),
					FillVertex(x1, y1,          0.5f, 0.0f),
					FillVertex(x2, y2,			1.0f, 1.0f),
				};

				VGuint i0 = addVertices(ARRAY_SIZE(v), v);
				VGuint i1 = i0+1;
				VGuint i2 = i0+2;

				assert(mCursor == i0-1);

				addPrim(FILL_PRIM_TYPE_TRI,  mBase, i0, i2);
				addPrim(FILL_PRIM_TYPE_QUAD, i0,    i1, i2);

				mCursor = i2;
			}

			void addSimpleCubic(VGuint i0, VGuint i1, VGuint i2, VGuint i3)
			{
				FillVertex& v0 = mVertices[i0];
				FillVertex& v1 = mVertices[i1];
				FillVertex& v2 = mVertices[i2];
				FillVertex& v3 = mVertices[i3];

				if (cubic::calcImplicit(v1.tc)<0 || cubic::calcImplicit(v2.tc)<0)
				{
					cubic::changeOrient(v0.tc);
					cubic::changeOrient(v1.tc);
					cubic::changeOrient(v2.tc);
					cubic::changeOrient(v3.tc);
				}

				addPrim(FILL_PRIM_TYPE_CUBIC, i0, i1, i2);
				addPrim(FILL_PRIM_TYPE_CUBIC, i2, i3, i0);

			}

			void cubicTo(VGfloat x1, VGfloat y1, VGfloat x2, VGfloat y2, VGfloat x3, VGfloat y3)
			{
				glm::vec2 pts[4] = 
				{
					getLastVertex(),
					glm::vec2(x1, y1),
					glm::vec2(x2, y2),
					glm::vec2(x3, y3),
				};

				VGint count;
				glm::vec2	pos[10];
				glm::vec3	tc[10];

				cubic::calcTriVertices(pts, count, pos, tc);

				VGuint	last;

				for (VGint i=0; i<count; ++i)
				{
					VGuint	i0 = addVertex(FillVertex(pos[i*3+0], tc[i*3+0]));
					VGuint	i1 = addVertex(FillVertex(pos[i*3+1], tc[i*3+1]));
					VGuint	i2 = addVertex(FillVertex(pos[i*3+2], tc[i*3+2]));
					VGuint	i3 = addVertex(FillVertex(pos[i*3+3], tc[i*3+3]));
					
					addSimpleCubic(i0, i1, i2, i3);
					addPrim(FILL_PRIM_TYPE_TRI, mCursor, i0, i3);

					last = i3;
				}

				addPrim(FILL_PRIM_TYPE_TRI,  mBase, mCursor, last);

				mCursor = last;
			}

			void arcTo(arc::Type type, VGfloat rx, VGfloat ry, VGfloat angle, VGfloat x1, VGfloat y1)
			{
				VGint count;
				glm::vec2	pos[9];
				glm::vec3	tc[9];

				arc::calcTriVertices(type, rx, ry, angle, getLastVertex(), glm::vec2(x1, y1), count, pos, tc);
				
				VGuint	last;

				for (VGint i=0; i<count; ++i)
				{
					last = addVertex(FillVertex(pos[i], tc[i]));
				}

				assert(last==mCursor+count);

				for (VGint i=0; i<count-1; i+=2)
				{
					addPrim(FILL_PRIM_TYPE_ARC,  mCursor+i+1, mCursor+i+2, mCursor+i+3);
					addPrim(FILL_PRIM_TYPE_TRI,  mCursor,     mCursor+i+1, mCursor+i+3);
				}

				addPrim(FILL_PRIM_TYPE_TRI,  mBase, mCursor+1, last);

				mCursor = last;
			}
			
			void copyDataTo(FillGeometry& fillGeom)
			{
				VGuint offset = 0, count;

				for (size_t i=0; i<FILL_PRIM_TYPE_COUNT; ++i)
				{
					fillGeom.offset[i] = offset;

					assert(mIndices[i].size() < VG_MAXINT);
					
					count = (VGuint)mIndices[i].size();
					fillGeom.count[i] = count;
					fillGeom.indices.insert(
						fillGeom.indices.end(), 
						mIndices[i].begin(),
						mIndices[i].end());
					offset += count;
				}

				fillGeom.vertices.insert(
					fillGeom.vertices.end(), 
					mVertices.begin(),
					mVertices.end());
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

				glm::vec2	p0(getLastVertex()), p1(x1, y1), n(makeNormal(p0, p1));

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
				glm::vec2	n01(makeNormal(p0, p1)), n12(makeNormal(p1, p2));
				float		det = n01.y*n12.x-n01.x*n12.y;
				
				VGuint	begin, end;

				if (det == 0)
				{
					glm::vec2 n(makeNormal(p0, p2));
					StrokeVertex v[] = {
						StrokeVertex(p0, n), StrokeVertex(p0, -n),
						StrokeVertex(p2, n), StrokeVertex(p2, -n),
						StrokeVertex(p2)
					};

					begin  = addVertices(ARRAY_SIZE(v), v);
					end    = begin+2;
				}
				else
				{
					glm::vec2	offset(makeNormal(n01, n12)/det);

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
				}

				assert(mCursor == begin-1);

				addStroke(begin, end);
			}

			void addSimpleCubic(VGuint i0, VGuint i1, VGuint i2, VGuint i3)
			{
				StrokeVertex& v0 = mVertices[i0];
				StrokeVertex& v1 = mVertices[i1];
				StrokeVertex& v2 = mVertices[i2];
				StrokeVertex& v3 = mVertices[i3];

				if (cubic::calcImplicit(v1.tc)<0 || cubic::calcImplicit(v2.tc)<0)
				{
					cubic::changeOrient(v0.tc);
					cubic::changeOrient(v1.tc);
					cubic::changeOrient(v2.tc);
					cubic::changeOrient(v3.tc);
				}

				addPrim(STROKE_PRIM_TYPE_CUBIC, i0, i1, i2);
				addPrim(STROKE_PRIM_TYPE_CUBIC, i2, i3, i0);

			}

			void cubicTo(VGfloat x1, VGfloat y1, VGfloat x2, VGfloat y2, VGfloat x3, VGfloat y3)
			{
				glm::vec2 pts[4] = 
				{
					getLastVertex(),
					glm::vec2(x1, y1),
					glm::vec2(x2, y2),
					glm::vec2(x3, y3),
				};

				VGint count;
				glm::vec2	pos[10];
				glm::vec3	tc[10];

				cubic::calcTriVertices(pts, count, pos, tc);

				GLuint base1, base2;

				for (VGint i=0; i<count; ++i)
				{
					glm::vec2	n01(makeNormal(pos[i*3+0], pos[i*3+1])),
								n12(makeNormal(pos[i*3+1], pos[i*3+2])),
								n23(makeNormal(pos[i*3+2], pos[i*3+3]));
					float		det012 = n01.y*n12.x-n01.x*n12.y;
					glm::vec2	offset012(det012!=0?makeNormal(n01, n12)/det012:makeNormal(pos[i*3+0], pos[i*3+2]));
					float		det123 = n12.y*n23.x-n12.x*n23.y;
					glm::vec2	offset123(det123!=0?makeNormal(n12, n23)/det123:makeNormal(pos[i*3+1], pos[i*3+3]));

					VGuint	i0 = addVertex(StrokeVertex(pos[i*3+0], n01, tc[i*3+0]));
					addVertex(StrokeVertex(pos[i*3+0], -n01, tc[i*3+0]));
					VGuint	i1 = addVertex(StrokeVertex(pos[i*3+1], offset012, tc[i*3+1]));
					addVertex(StrokeVertex(pos[i*3+1], -offset012, tc[i*3+1]));
					VGuint	i2 = addVertex(StrokeVertex(pos[i*3+2], offset123, tc[i*3+2]));
					addVertex(StrokeVertex(pos[i*3+2], -offset123, tc[i*3+2]));
					VGuint	i3 = addVertex(StrokeVertex(pos[i*3+3], n23, tc[i*3+3]));
					addVertex(StrokeVertex(pos[i*3+3], -n23, tc[i*3+3]));
					
					if (i==0)
					{
						base1 = i0;
						base2 = i0+1;
					}

					addSimpleCubic(i0, i1, i2, i3);
					addPrim(STROKE_PRIM_TYPE_TRI, base1, i0, i3);

					addSimpleCubic(i3+1, i2+1, i1+1, i0+1);
					addPrim(STROKE_PRIM_TYPE_TRI, base2, i3+1, i0+1);
				}

				VGuint	last = addVertex(StrokeVertex(pos[i*3+3]));

				addStroke(mCursor+1, last-2);
			}

			void arcTo(arc::Type type, VGfloat rx, VGfloat ry, VGfloat angle, VGfloat x1, VGfloat y1)
			{
				VGint count;
				glm::vec2	pos[9];
				glm::vec3	tc[9];

				arc::calcTriVertices(type, rx, ry, angle, getLastVertex(), glm::vec2(x1, y1), count, pos, tc);
				
				VGuint	last;

				for (VGint i=0; i<count; ++i)
				{
					glm::vec2	n;

					if (i==count-1)
						n = calcOffset(pos[i-1],   pos[i]);
					else if (i%2==1)
						n = calcOffset(pos[i-1], pos[i], pos[i+1]);
					else
						n = calcOffset(pos[i],   pos[i+1]);

					last = addVertex(StrokeVertex(pos[i], n, tc[i]));
					last = addVertex(StrokeVertex(pos[i], -n, tc[i]));
				}

				assert(last==mCursor+count*2);

				for (VGint i=0; i<count-1; i+=2)
				{
					addPrim(STROKE_PRIM_TYPE_ARC,  mCursor+i*2+1, mCursor+i*2+3, mCursor+i*2+5);
					addPrim(STROKE_PRIM_TYPE_TRI,  mCursor+1,     mCursor+i*2+1, mCursor+i*2+5);

					addPrim(STROKE_PRIM_TYPE_ARC,  mCursor+i*2+6, mCursor+i*2+4, mCursor+i*2+2);
					addPrim(STROKE_PRIM_TYPE_TRI,  mCursor+2,     mCursor+i*2+6, mCursor+i*2+2);
				}

				last = addVertex(StrokeVertex(pos[count-1]));

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
					strokeGeom.indices.insert(
						strokeGeom.indices.end(), 
						mIndices[i].begin(),
						mIndices[i].end());
					offset += count;
				}

				strokeGeom.vertices.insert(
					strokeGeom.vertices.end(), 
					mVertices.begin(),
					mVertices.end());
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

	struct GData
	{
		std::vector<Vertex>	vertices;
		IndexVector		indices[PT_COUNT];
		
		GLsizei	offset[PT_COUNT], count[PT_COUNT];

		void drawElements(PrimitiveTypes prim) const
		{
			if (count[prim])
				glDrawElements(GL_TRIANGLES, count[prim], GL_UNSIGNED_INT, &indices[prim][0]);
		}
	};

	//Uses fixed vertex layout
	//We share first, last non-displaced vertices
	//Order of vertices and indices is very important
	struct GPUData
	{
		public:
			enum
			{
				FILL = 1,	//(1<<0)
				STROKE = 2,	//(1<<1)
				ALL = FILL|STROKE,
			};

		public:
			GPUData();

			void begin(float x, float y);
			void end(bool closePath = false);
			
			void lineTo(float x1, float y1);
			void quadTo(float x1, float y1, float x2, float y2);
			void cubic(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
			void arc(VGPathSegment type, float rx, float ry, float angle, float x0, float y0, float x1, float y1);

		public:
			GData	data;

		private:
			u32 addVertex(const Vertex& v);
			u32 addVertices(u32 count, Vertex v[]);
			
			const glm::vec2& getCursor()
			{return data.vertices[mCursor].p;}

			bool isFirstPrim(u32 primVtx)
			{return data.vertices.size()-primVtx<2;}
			
			
			void addStroke(u32 first, u32 last);
			void addJointBevel(u32 end, u32 start);
			void addPrim(PrimitiveTypes prim, u32 i0, u32 i1, u32 i2, bool displaced=false);
			void addSimpleCubic(u32& firstIdx, u32& lastIdx,
								const glm::vec2& p1, const glm::vec3& tc1,
								const glm::vec2& p2, const glm::vec3& tc2,
								const glm::vec2& p3, const glm::vec3& tc3,
								const glm::vec2& p4, const glm::vec3& tc4);

		private:
			u32 mBase, mCursor;
	};

	void RasterizeFillNonZero(const GData& data);
	void RasterizeFillEvenOdd(const GData& data);
	void RasterizeStroke(const GData& data);
}
