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

	typedef Array<VGuint>	IndexVector;
	
	template<class VertexType, VGuint PRIM_COUNT>
	struct Geometry
	{
		Array<VertexType>	vertices;
		IndexVector			indices;
		
		GLsizei	offset[PRIM_COUNT], count[PRIM_COUNT];
		
		glm::vec2	mMin, mMax;
		
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
			glCullFace(GL_FRONT);
			drawElements(primType);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glCullFace(GL_BACK);
			drawElements(primType);
		}
	};

	enum FillPrimitiveTypes
	{
		FILL_PRIM_TYPE_TRI,
		FILL_PRIM_TYPE_ARC,
		FILL_PRIM_TYPE_QUAD,
		FILL_PRIM_TYPE_CUBIC,
		FILL_PRIM_TYPE_COUNT,
	};

using namespace shared;

	struct FillGeometry: public Geometry<FillVertex, FILL_PRIM_TYPE_COUNT>
	{
		void RasterizeEvenOdd()
		{
			if (vertices.empty())
				return;

			FillVertex& vtx = vertices[0];
			
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 0, 1);
			glStencilMask(0x01);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, VertexSize, &vtx.p);

			glUseProgram(0);
			drawElements(FILL_PRIM_TYPE_TRI);

			glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3, GL_FLOAT, VertexSize, &vtx.tc);

			glUseProgram(programs[PRG_RAST_FILL_QUAD]);
			drawElements(FILL_PRIM_TYPE_QUAD);

			glUseProgram(programs[PRG_RAST_FILL_CUBIC]);
			drawElements(FILL_PRIM_TYPE_CUBIC);

			glUseProgram(programs[PRG_RAST_FILL_QUAD]);
			drawElements(FILL_PRIM_TYPE_ARC);

			glPopClientAttrib();
			glPopAttrib();
		}

		//Safetly supports up to 127 self intersections
		void RasterizeNonZero()
		{
			if (vertices.empty())
				return;

			FillVertex& vtx = vertices[0];

			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

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

			glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3, GL_FLOAT, VertexSize, &vtx.tc);

			glUseProgram(programs[PRG_RAST_FILL_QUAD]);
			drawElementsNZ(FILL_PRIM_TYPE_QUAD);

			glUseProgram(programs[PRG_RAST_FILL_CUBIC]);
			drawElementsNZ(FILL_PRIM_TYPE_CUBIC);

			glUseProgram(programs[PRG_RAST_FILL_ARC]);
			drawElementsNZ(FILL_PRIM_TYPE_ARC);

			glPopClientAttrib();
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
				calcBBox(v.p);
				return idx;
			}

			void setVertex(VGuint i, const VertexType& v)
			{
				assert(i<mVertices.size());
				mVertices[i] = v;
				calcBBox(v.p);
			}

			VertexType*	reserveRange(VGuint count, VGuint& begin, VGuint& end)
			{
				assert((mVertices.size()+count)<(size_t)VG_MAXINT);
				
				begin = (VGuint)mVertices.size();
				end   = begin+count;
			
				mVertices.resize(end);
				--end;

				return mVertices.begin()+begin;
			}

			void addPrim(VGuint primType, VGuint i0, VGuint i1, VGuint i2)
			{
				assert(primType<PRIM_COUNT);

				if (i0==i1 || i1==i2 || i2==i0)
					return;

				IndexVector& indices = mIndices[primType];

				indices.push_back(i0);
				indices.push_back(i1);
				indices.push_back(i2);
			}
		
			glm::vec2	getPos(VGuint i) {return mVertices[i].p;}
			bool		isFirstPrim()	 {return mCursor==mBase;}

		protected:
			void calcBBox(const glm::vec2& p)
			{
				if (mVertices.empty())
				{
					mMin = p;
					mMax = p;
				}
				else
				{
					mMin = glm::min(mMin, p);
					mMax = glm::max(mMax, p);
				}
			}

		protected:
			static const VGuint	InvalidIndex = ~((VGuint)0);

		protected:
			Array<VertexType>		mVertices;
			IndexVector				mIndices[PRIM_COUNT];
			VGuint					mBase, mCursor;
			glm::vec2				mMin, mMax;
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

				VGuint i0, end;

				reserveRange(3, i0, end);

				setVertex(i0+0, FillVertex(v[0], 0.0f, 0.0f));
				setVertex(i0+1, FillVertex(v[1], 0.5f, 0.0f));
				setVertex(i0+2, FillVertex(v[2], 1.0f, 1.0f));

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
				
				VGuint	begin, end;

				reserveRange((VGuint)pos.size(), begin, end);

				for (size_t i=0; i<pos.size(); ++i)
				{
					setVertex(begin+i, FillVertex(pos[i], tc[i]));
				}

				for (VGuint i0=begin; i0<end; i0+=4)
				{
					addPrim(FILL_PRIM_TYPE_CUBIC, i0,    i0+1, i0+2);
					addPrim(FILL_PRIM_TYPE_CUBIC, i0+2,  i0+3, i0+0);
					addPrim(FILL_PRIM_TYPE_TRI,   begin, i0,   i0+3);
				}
			}

			void addArc(VGPathSegment type, const glm::vec2& radius, VGfloat angle,
					   const glm::vec2& p0, const glm::vec2& p1)
			{
				Array<glm::vec2>	pos;
				Array<glm::vec3>	tc;

				arcTriVertices(type, radius, angle, p0, p1, pos, tc);
				
				assert(pos.size()==tc.size());

				VGuint	begin, end;

				reserveRange((VGuint)pos.size(), begin, end);

				for (size_t i=0; i<pos.size(); ++i)
				{
					setVertex(begin+i, FillVertex(pos[i], tc[i]));
				}

				for (VGuint i0=begin; i<end-1; i+=2)
				{
					addPrim(FILL_PRIM_TYPE_ARC,  i0,    i0+1, i0+2);
					addPrim(FILL_PRIM_TYPE_TRI,  begin, i0,   i0+2);
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
				fillGeom.mMin = mMin;
				fillGeom.mMax = mMax;
			}

	};

	enum StrokePrimitiveTypes
	{
		STROKE_PRIM_TYPE_TRI,
		STROKE_PRIM_TYPE_ARC,
		STROKE_PRIM_TYPE_QUAD,
		STROKE_PRIM_TYPE_CUBIC,
		STROKE_PRIM_TYPE_JOINT_BEVEL,
		STROKE_PRIM_TYPE_JOINT_MITER,
		STROKE_PRIM_TYPE_JOINT_ROUND,
		STROKE_PRIM_TYPE_CAP_ROUND,
		STROKE_PRIM_TYPE_CAP_BEVEL,
		STROKE_PRIM_TYPE_COUNT,
	};

	struct StrokeGeometry: public Geometry<StrokeVertex, STROKE_PRIM_TYPE_COUNT>
	{
		void RasterizePrimitives(float halfWidth)
		{
			glUseProgram(programs[PRG_RAST_STROKE_TRI]);
			glUniform1f(uniforms[UNI_RAST_STROKE_TRI_HALFWIDTH], halfWidth);
			drawElementsNZ(STROKE_PRIM_TYPE_TRI);

			glUseProgram(programs[PRG_RAST_STROKE_QUAD]);
			glUniform1f(uniforms[UNI_RAST_STROKE_QUAD_HALFWIDTH], halfWidth);
			drawElementsNZ(STROKE_PRIM_TYPE_QUAD);

			glUseProgram(programs[PRG_RAST_STROKE_CUBIC]);
			glUniform1f(uniforms[UNI_RAST_STROKE_CUBIC_HALFWIDTH], halfWidth);
			drawElementsNZ(STROKE_PRIM_TYPE_CUBIC);

			glUseProgram(programs[PRG_RAST_STROKE_ARC]);
			glUniform1f(uniforms[UNI_RAST_STROKE_ARC_HALFWIDTH], halfWidth);
			drawElementsNZ(STROKE_PRIM_TYPE_ARC);
		}

		//Safely supports up to 127 self intersections
		//We rasterize strokes in 2 passes, which behave differently for different stroke primitives
		void Rasterize()
		{
			if (vertices.empty())
				return;
			
			StrokeVertex& vtx = vertices[0];

			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
			glStencilMask(0xFF);

 			// Position
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, VertexSize, &vtx.p);
			
			// Perpendicular
			glClientActiveTexture(GL_TEXTURE1);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, VertexSize, &vtx.n);

			// Rasterization parameters
			glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3, GL_FLOAT, VertexSize, &vtx.tc);

			//Chnaging multiplier of normal change winding for tri primitive
			//So we need to change front face to make rasterization correct
			//And for other primitive types we need to change winding to add or remove parts:
			//We rasterize quad as tri, so for proper stroke we need area between tri edges,
			//changing normal multiplier in most cases does not change tri winding, only shifts
			//so if we do not change front face we will have 2 quads one under another
			//We need to change winding for second quad to add/remove pixels.
			
			//First pass
			glFrontFace(GL_CCW);
			RasterizePrimitives(5);

			//Second pass
			glFrontFace(GL_CW);
			RasterizePrimitives(-5);

			glPopClientAttrib();
			glPopAttrib();
		}
	};

	/* Unlike FillGeometryBuilder all methods should be called between begin/end
	/* order is also important
	*/
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
					//lineTo(getFirstVertex().x, getFirstVertex().y);
					//addJointBevel(mCursor-2, mBase+1);
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

				glm::vec2	p0(getPos(mCursor)), p1(x1, y1), n(calcOffset(p0, p1));
				
				VGuint i0, end;

				reserveRange(3, i0, end);

				setVertex(i0+0, StrokeVertex(p0, n));
				setVertex(i0+1, StrokeVertex(p1, n));
				setVertex(i0+2, StrokeVertex(p1));
				
				addStroke(mCursor, i0, i0+1, i0+2);
			}

			//Requires 3 vertices, first should be the same as the last in previous primitive
			void quadTo(const Array<glm::vec2>& v)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);
				assert(getPos(mCursor)==v[0]);

				glm::vec2	n01(calcOffset(v[0], v[1])), n12(calcOffset(v[1], v[2]));
				glm::vec2	n012(calcOffsetN(n01, n12));
				
				VGuint i0, end;

				reserveRange(3, i0, end);

				setVertex(i0+0, StrokeVertex(v[0],  n01, glm::vec3(0.0f, 0.0f, 0.0f)));
				setVertex(i0+1, StrokeVertex(v[1], n012, glm::vec3(0.5f, 0.0f, 0.0f)));
				setVertex(i0+2, StrokeVertex(v[2],  n12, glm::vec3(1.0f, 1.0f, 0.0f)));

				addPrim(FILL_PRIM_TYPE_QUAD, i0, i0+1, i0+2);
				addStroke(mCursor, i0, i0+1, i0+2);
			}

			void addSimpleCubic(VGuint i0, VGuint i1, VGuint i2, VGuint i3)
			{
				addPrim(STROKE_PRIM_TYPE_CUBIC, i0, i1, i2);
				addPrim(STROKE_PRIM_TYPE_CUBIC, i2, i3, i0);
			}
			
			//Requires 4 vertices, first should be the same as the last in previous primitive
			void cubicTo(const Array<glm::vec2>& v)
			{
				assert(mBase   != InvalidIndex);
				assert(mCursor != InvalidIndex);
				assert(glm::equal(getPos(mCursor), v[0]));

				Array<glm::vec2>	pos;
				Array<glm::vec3>	tc;
				
				cubicTriVertices(v, pos, tc);

				assert(pos.size()==tc.size());
				assert(tc.size()%4==0);

				VGuint	begin, end;

				reserveRange((VGuint)pos.size(), begin, end);

				for (size_t i=0; i<pos.size(); ++i)
				{
					glm::vec2	n;

					if (i%4==3)
						n = calcOffset(pos[i-1], pos[i]);
					else if (i%4!=0)
						n = calcOffset(pos[i-1], pos[i], pos[i+1]);
					else
						n = calcOffset(pos[i],   pos[i+1]);

					setVertex(begin+i, StrokeVertex(pos[i], n, tc[i]));
				}

				for (VGuint i=begin; i<end; i+=4)
				{
					addSimpleCubic(i, i+1, i+2, i+3);
					addPrim(STROKE_PRIM_TYPE_TRI, begin, i, i+3);
				}

				VGuint i0 = addVertex(StrokeVertex(v[3]));

				addStroke(mCursor, begin, end, i0);
			}

			void arcTo(VGPathSegment type, const glm::vec2& radius, VGfloat angle, const glm::vec2& p1)
			{
				Array<glm::vec2>	pos;
				Array<glm::vec3>	tc;

				arcTriVertices(type, radius, angle, getPos(mCursor), p1, pos, tc);
				
				assert(pos.size()==tc.size());

				VGuint	begin, end;

				reserveRange((VGuint)pos.size(), begin, end);

				for (size_t i=0; i<pos.size(); ++i)
				{
					glm::vec2	n;

					if (i==pos.size()-1)
						n = calcOffset(pos[i-1], pos[i]);
					else if (i%2==1)
						n = calcOffset(pos[i-1], pos[i], pos[i+1]);
					else
						n = calcOffset(pos[i],   pos[i+1]);

					setVertex(begin+i, StrokeVertex(pos[i], n, tc[i]));
				}

				for (VGuint i=begin; i<end-1; ++i)
				{
					addPrim(FILL_PRIM_TYPE_ARC,  i,     i+1, i+2);
					addPrim(FILL_PRIM_TYPE_TRI,  begin, i,   i+2);
				}

				VGuint i0 = addVertex(p1);

				addStroke(mCursor, begin, end, i0);

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
				strokeGeom.mMin = mMin;
				strokeGeom.mMax = mMax;
			}

		private:
			//Order of indices is very important!!!
			void addStroke(VGuint i0, VGuint i1, VGuint i2, VGuint i3)
			{
				addPrim(STROKE_PRIM_TYPE_TRI, i0, i1, i2);
				addPrim(STROKE_PRIM_TYPE_TRI, i0, i2, i3);

				if (isFirstPrim())
				{
					//addCapButt(mCursor)
					//addCapRound(mCursor)
				}
				else
				{
					//addJointBevel(mCursor);
					//addJointMiter(mCursor);
					//addJointRound(mCursor);
				}

				mCursor = i3;
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
