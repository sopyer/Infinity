#include <cassert>
#include <gl/glee.h>
#include <utils.h>

#include "Math.h"
#include "SharedResources.h"
#include "Rasterizer.h"
#include "GeometryBuilders.h"

namespace impl
{
	glm::vec2 readVec2(const VGfloat*& ptr)
	{
		glm::vec2	res(*ptr, *(ptr+1));
		ptr+=2;
		return res;
	}

	float readFloat(const VGfloat*& ptr)
	{
		float	res = *ptr;
		++ptr;
		return res;
	}

	void meshAddBezier3(Geometry& geom, u16 prevIdx, u16 curIdx, const glm::vec2& cp0, const glm::vec2&  cp1, const glm::vec2& cp2, const glm::vec2& cp3)
	{
		glm::vec2	ctrlPt[4*2] = {cp0, cp1, cp2, cp3};
		glm::vec3	klm[4*2];
		int			count;
		float		subdPts[2];

		bezier3MakeImplicit(ctrlPt, klm, count, subdPts);

		if (count>1)
		{
			orderAscend(subdPts[0], subdPts[1]);
		}

		for(int i=0; i<count; ++i)
		{
			float t = i==0?subdPts[i]:(subdPts[i]-subdPts[i-1])/(1-subdPts[i-1]);

			cubic::subdivide(ctrlPt, t, ctrlPt+4, ctrlPt);
			cubic::subdivide(klm, t, klm+4, klm);

			cubic::correctOrient(klm+4);
			geom.bezier3AddVertices(ctrlPt+4, klm+4);

			//Carefully, we changed places of subdivided curves,
			//that's why suitable points are 0 and 7
			u16 idx = geom.shapeAddVertex(ctrlPt[0]);
			geom.shapeAddTri(prevIdx, idx, curIdx);
			prevIdx = idx;
		}

		cubic::correctOrient(klm);
		geom.bezier3AddVertices(ctrlPt, klm);
	}

	void buildFillGeometry(Geometry&      geom,
						   const VGint    count,
						   const VGubyte* commands,
						   const VGfloat* pathData)
	{
		glm::vec2			cp0, cp1, o, p;
		bool				isContourStarted = false;
		u16					startIdx=0, prevIdx=0, curIdx=0; //assign default values to make compiler happy

		for (VGint s=0; s<count; ++s)
		{
			int		segment  = commands[s]&0x1E;
			bool	rel      = commands[s]&1;
			
			if (segment==VG_CLOSE_PATH)
			{
				isContourStarted = false;
			}
			else if (segment==VG_MOVE_TO)
			{
				isContourStarted = false;
				o = p = readVec2(pathData) + (rel?o:glm::vec2(0));
			}
			else
			{
				//Here starts non control commands
				//So we can handle start path case
				if (!isContourStarted)
				{
					isContourStarted = true;
					startIdx = prevIdx = curIdx = geom.shapeAddVertex(o);
				}

				switch (segment)
				{
					case VG_LINE_TO:
						o = p = readVec2(pathData) + (rel?o:glm::vec2(0));
						break;

					case VG_HLINE_TO:
						o.x = p.x = readFloat(pathData) + (rel?o.x:0);
						break;

					case VG_VLINE_TO:
						o.y = p.y = readFloat(pathData) + (rel?o.y:0);
						break;

					case VG_QUAD_TO:
						p = readVec2(pathData) + (rel?o:glm::vec2(0));
						o = readVec2(pathData) + (rel?o:glm::vec2(0));
						break;

					case VG_SQUAD_TO:
						p = 2.0f*o - p;
						o = readVec2(pathData) + (rel?o:glm::vec2(0));
						break;

					case VG_CUBIC_TO:
						cp0 = o;
						cp1 = readVec2(pathData) + (rel?o:glm::vec2(0));
						p = readVec2(pathData) + (rel?o:glm::vec2(0));
						o = readVec2(pathData) + (rel?o:glm::vec2(0));
						break;

					case VG_SCUBIC_TO:
						cp0 = o;
						cp1 = 2.0f*o - p;
						p = readVec2(pathData) + (rel?o:glm::vec2(0));
						o = readVec2(pathData) + (rel?o:glm::vec2(0));
						break;

					case VG_SCWARC_TO:
					case VG_SCCWARC_TO:
					case VG_LCWARC_TO:
					case VG_LCCWARC_TO:
						{
							glm::vec2	r = readVec2(pathData);
							float		a = readFloat(pathData);
							glm::vec2	e = readVec2(pathData) + (rel?o:glm::vec2(0));

							o = p = e;
						}
						break;

					default:
						assert(0);
				}

				assert(isContourStarted);
				prevIdx = curIdx;
				curIdx = geom.shapeAddVertex(o);
				geom.shapeAddTri(startIdx, prevIdx, curIdx);

				switch (segment)
				{
					case VG_CUBIC_TO:
					case VG_SCUBIC_TO:
						meshAddBezier3(geom, prevIdx, curIdx, cp0, cp1, p, o);
						break;

					case VG_QUAD_TO:
					case VG_SQUAD_TO:
						//TODO: implement quadratic curves
						assert(0);
						break;

					case VG_SCWARC_TO:
					case VG_SCCWARC_TO:
					case VG_LCWARC_TO:
					case VG_LCCWARC_TO:
						//TODO: implement elliptic arcs
						assert(0);
						break;
				};
			}
		}

		geom.bezier3GenIndices();

		//stupid initialization
		if (!geom.shapeVertices.empty())
		{
			geom.xmin = geom.xmax = geom.shapeVertices[0].x;
			geom.ymin = geom.ymax = geom.shapeVertices[0].y;
		}
		else if (!geom.bezier3Vertices.empty())
		{
			geom.xmin = geom.xmax = geom.bezier3Vertices[0].x;
			geom.ymin = geom.ymax = geom.bezier3Vertices[0].y;
		}
		else
		{
			geom.xmin = geom.ymin = geom.xmax = geom.ymax = 0;
			return;
		}

		for(size_t i=0; i<geom.shapeVertices.size(); ++i)
		{
			geom.xmin = std::min(geom.xmin, geom.shapeVertices[i].x);
			geom.ymin = std::min(geom.ymin, geom.shapeVertices[i].y);
			geom.xmax = std::max(geom.xmax, geom.shapeVertices[i].x);
			geom.ymax = std::max(geom.ymax, geom.shapeVertices[i].y);
		}

		for(size_t i=0; i<geom.bezier3Vertices.size(); ++i)
		{
			geom.xmin = std::min(geom.xmin, geom.bezier3Vertices[i].x);
			geom.ymin = std::min(geom.ymin, geom.bezier3Vertices[i].y);
			geom.xmax = std::max(geom.xmax, geom.bezier3Vertices[i].x);
			geom.ymax = std::max(geom.ymax, geom.bezier3Vertices[i].y);
		}
	}

	void meshAddBezier3(FillGeometry& geom, u16 prevIdx, u16 curIdx, const glm::vec2& cp0, const glm::vec2&  cp1, const glm::vec2& cp2, const glm::vec2& cp3)
	{
		glm::vec2	ctrlPt[4*2] = {cp0, cp1, cp2, cp3};
		glm::vec3	klm[4*2];
		int			count;
		float		subdPts[2];

		bezier3MakeImplicit(ctrlPt, klm, count, subdPts);

		if (count>1)
		{
			orderAscend(subdPts[0], subdPts[1]);
		}

		for(int i=0; i<count; ++i)
		{
			float t = i==0?subdPts[i]:(subdPts[i]-subdPts[i-1])/(1-subdPts[i-1]);

			cubic::subdivide(ctrlPt, t, ctrlPt+4, ctrlPt);
			cubic::subdivide(klm, t, klm+4, klm);

			cubic::correctOrient(klm+4);
			geom.bezier3AddVertices(ctrlPt+4, klm+4);

			//Carefully, we changed places of subdivided curves,
			//that's why suitable points are 0 and 7
			u16 idx = geom.shapeAddVertex(ctrlPt[0]);
			geom.shapeAddTri(prevIdx, idx, curIdx);
			prevIdx = idx;
		}

		cubic::correctOrient(klm);
		geom.bezier3AddVertices(ctrlPt, klm);
	}

	void buildFillGeometry(FillGeometry&  geom,
						   const VGint    count,
						   const VGubyte* commands,
						   const VGfloat* pathData)
	{
		FillGeometryBuilder	builder;
		Array<glm::vec2>	points;
		glm::vec2			cp0, cp1, o, p;
		bool				isContourStarted = false;
		u16					startIdx=0, prevIdx=0, curIdx=0; //assign default values to make compiler happy

		for (VGint s=0; s<count; ++s)
		{
			int		segment  = commands[s]&0x1E;
			bool	rel      = commands[s]&1;
			
			points.clear();
			points.push_back(o);
			if (segment==VG_CLOSE_PATH)
			{
				if (isContourStarted)
				{
					builder.end();
					isContourStarted = false;
				}
			}
			else if (segment==VG_MOVE_TO)
			{
				if (isContourStarted)
				{
					builder.end();
					isContourStarted = false;
				}

				o = p = readVec2(pathData) + (rel?o:glm::vec2(0));
			}
			else
			{
				//Here starts non control commands
				//So we can handle start path case
				if (!isContourStarted)
				{
					isContourStarted = true;
					builder.begin(o.x, o.y);
					startIdx = prevIdx = curIdx = geom.shapeAddVertex(o);
				}

				switch (segment)
				{
					case VG_LINE_TO:
						{
							o = p = readVec2(pathData) + (rel?o:glm::vec2(0));
						}
						break;

					case VG_HLINE_TO:
						{
							float x = readFloat(pathData) + (rel?o.x:0);
							o.x = p.x = x;
						}
						break;

					case VG_VLINE_TO:
						{
							float y = readFloat(pathData) + (rel?o.y:0);
							o.y = p.y = y;
						}
						break;

					case VG_QUAD_TO:
						{
							glm::vec2	p1 = readVec2(pathData) + (rel?o:glm::vec2(0));
							glm::vec2	p2 = readVec2(pathData) + (rel?o:glm::vec2(0));
							
							points.push_back(p1);
							points.push_back(p2);

							builder.addQuad(points);
							
							p = p1;
							o = p2;
						}
						break;

					case VG_SQUAD_TO:
						{
							glm::vec2	p1 = 2.0f*o - p;
							glm::vec2	p2 = readVec2(pathData) + (rel?o:glm::vec2(0));

							points.push_back(p1);
							points.push_back(p2);

							builder.addQuad(points);
							
							p = p1;
							o = p2;
						}
						break;

					case VG_CUBIC_TO:
						{
							glm::vec2	p1 = readVec2(pathData) + (rel?o:glm::vec2(0));
							glm::vec2	p2 = readVec2(pathData) + (rel?o:glm::vec2(0));
							glm::vec2	p3 = readVec2(pathData) + (rel?o:glm::vec2(0));
							
							points.push_back(p1);
							points.push_back(p2);
							points.push_back(p3);

							builder.addCubic(points);
							
							cp0 = o;
							cp1 = p1;
							p = p2;
							o = p3;
						}
						break;

					case VG_SCUBIC_TO:
						{
							glm::vec2	p1 = 2.0f*o - p;
							glm::vec2	p2 = readVec2(pathData) + (rel?o:glm::vec2(0));
							glm::vec2	p3 = readVec2(pathData) + (rel?o:glm::vec2(0));
							
							points.push_back(p1);
							points.push_back(p2);
							points.push_back(p3);

							builder.addCubic(points);
							
							cp0 = o;
							cp1 = p1;
							p = p2;
							o = p3;
						}
						break;

					case VG_SCWARC_TO:
					case VG_SCCWARC_TO:
					case VG_LCWARC_TO:
					case VG_LCCWARC_TO:
						{
							glm::vec2	r = readVec2(pathData);
							float		a = readFloat(pathData);
							glm::vec2	e = readVec2(pathData) + (rel?o:glm::vec2(0));

							builder.addArc((VGPathSegment)segment, r, a, o, e);

							o = p = e;
						}
						break;

					default:
						assert(0);
				}

				builder.lineTo(o.x, o.y);

				assert(isContourStarted);
				prevIdx = curIdx;
				curIdx = geom.shapeAddVertex(o);
				geom.shapeAddTri(startIdx, prevIdx, curIdx);

				switch (segment)
				{
					case VG_CUBIC_TO:
					case VG_SCUBIC_TO:
						meshAddBezier3(geom, prevIdx, curIdx, cp0, cp1, p, o);
						break;

					case VG_SCWARC_TO:
					case VG_SCCWARC_TO:
					case VG_LCWARC_TO:
					case VG_LCCWARC_TO:
						//TODO: implement elliptic arcs
						assert(0);
						break;
				};
			}
		}

		if (isContourStarted)
			builder.end();

		builder.copyDataTo(geom);
		geom.bezier3GenIndices();
	}

	void buildStrokeGeometry(const VGint      numSegments,
							 const VGubyte*   pathSegments,
							 const VGfloat*   pathData,
							 StrokeGeometry&  geom)
	{
		StrokeGeometryBuilder	builder;
		Array<glm::vec2>		points;
		glm::vec2				o, p;
		bool					segmentStarted = false;

		for (VGint s=0; s<numSegments; ++s)
		{
			int		segment  = pathSegments[s]&0x1E;
			bool	rel      = pathSegments[s]&1;
			
			points.clear();
			points.push_back(o);
			if (segment==VG_CLOSE_PATH)
			{
				if (segmentStarted)
				{
					builder.end(true);
					segmentStarted = false;
				}
			}
			else if (segment==VG_MOVE_TO)
			{
				if (segmentStarted)
				{
					builder.end(false);
					segmentStarted = false;
				}

				o = p = readVec2(pathData) + (rel?o:glm::vec2(0));
			}
			else
			{
				//Here starts non control commands
				//So we can handle start path case
				if (!segmentStarted)
				{
					segmentStarted = true;
					builder.begin(o.x, o.y);
				}

				switch (segment)
				{
					case VG_LINE_TO:
						{
							o = p = readVec2(pathData) + (rel?o:glm::vec2(0));
							builder.lineTo(o.x, o.y);
						}
						break;

					case VG_HLINE_TO:
						{
							float x = readFloat(pathData) + (rel?o.x:0);
							o.x = p.x = x;
							builder.lineTo(o.x, o.y);
						}
						break;

					case VG_VLINE_TO:
						{
							float y = readFloat(pathData) + (rel?o.y:0);
							o.y = p.y = y;
							builder.lineTo(o.x, o.y);
						}
						break;

					case VG_QUAD_TO:
						{
							glm::vec2	p1 = readVec2(pathData) + (rel?o:glm::vec2(0));
							glm::vec2	p2 = readVec2(pathData) + (rel?o:glm::vec2(0));
							
							points.push_back(p1);
							points.push_back(p2);

							builder.quadTo(points);
							
							p = p1;
							o = p2;
						}
						break;

					case VG_SQUAD_TO:
						{
							glm::vec2	p1 = 2.0f*o - p;
							glm::vec2	p2 = readVec2(pathData) + (rel?o:glm::vec2(0));

							points.push_back(p1);
							points.push_back(p2);

							builder.quadTo(points);
							
							p = p1;
							o = p2;
						}
						break;

					case VG_CUBIC_TO:
						{
							glm::vec2	p1 = readVec2(pathData) + (rel?o:glm::vec2(0));
							glm::vec2	p2 = readVec2(pathData) + (rel?o:glm::vec2(0));
							glm::vec2	p3 = readVec2(pathData) + (rel?o:glm::vec2(0));
							
							points.push_back(p1);
							points.push_back(p2);
							points.push_back(p3);

							builder.cubicTo(points);
							
							p = p2;
							o = p3;
						}
						break;

					case VG_SCUBIC_TO:
						{
							glm::vec2	p1 = 2.0f*o - p;
							glm::vec2	p2 = readVec2(pathData) + (rel?o:glm::vec2(0));
							glm::vec2	p3 = readVec2(pathData) + (rel?o:glm::vec2(0));
							
							points.push_back(p1);
							points.push_back(p2);
							points.push_back(p3);

							builder.cubicTo(points);
							
							p = p2;
							o = p3;
						}
						break;

					case VG_SCWARC_TO:
					case VG_SCCWARC_TO:
					case VG_LCWARC_TO:
					case VG_LCCWARC_TO:
						{
							glm::vec2	r = readVec2(pathData);
							float		a = readFloat(pathData);
							glm::vec2	e = readVec2(pathData) + (rel?o:glm::vec2(0));

							builder.arcTo((VGPathSegment)segment, r, a, o, e);

							o = p = e;
						}
						break;

					default:
						assert(0);
				}
			}
		}

		if (segmentStarted)
			builder.end(false);

		builder.copyDataTo(geom);
	}

	enum
	{
		PRIM_TYPE_TRI = 1,
		PRIM_TYPE_ARC = 2,
		PRIM_TYPE_QUAD = 4,
		PRIM_TYPE_CUBIC = 8,
		PRIM_TYPE_ALL = 15
	};

	template<class VertexType, VGuint PRIM_COUNT>
	void drawElements(GeometryOld<VertexType, PRIM_COUNT>& geom, VGuint primType)
	{
		assert(primType<PRIM_COUNT);
		if (!geom.count[primType]) return;
		
		glDrawElements(GL_TRIANGLES, geom.count[primType], GL_UNSIGNED_INT, &geom.indices[0]+geom.offset[primType]);
	}

	template<class VertexType, VGuint PRIM_COUNT>
	void drawElementsNZ(GeometryOld<VertexType, PRIM_COUNT>& geom, VGuint primType)
	{
		assert(primType<PRIM_COUNT);
		if (!geom.count[primType]) return;
		
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glCullFace(GL_FRONT);
		drawElements(geom, primType);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		glCullFace(GL_BACK);
		drawElements(geom, primType);
	}

	void rasterizeEvenOddA2C(Geometry& geom)
	{
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

		if (!geom.shapeIndices.empty())
		{
			glUseProgram(0);
			glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), geom.shapeVertices.begin());
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.shapeIndices.size(), GL_UNSIGNED_SHORT, geom.shapeIndices.begin());
		}

		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (!geom.bezier3Indices.empty())
		{
			glVertexPointer(2, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].x);
			glTexCoordPointer(3, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].k);
			glUseProgram(programs[PRG_RAST_FILL_CUBIC_AA]);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.bezier3Indices.size(), GL_UNSIGNED_SHORT, geom.bezier3Indices.begin());
		}

		//TODO: implement second order bezier
		//glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		//drawElements(geom, FILL_PRIM_TYPE_QUAD);

		//TODO: implement elliptic arcs
		//glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		//drawElements(geom, FILL_PRIM_TYPE_ARC);
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

		glPopClientAttrib();
		glPopAttrib();
	}

	void rasterizeNonZeroA2C(Geometry& geom)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
		glStencilMask(0xFF);

		glEnableClientState(GL_VERTEX_ARRAY);

		if (!geom.shapeIndices.empty())
		{
			glUseProgram(0);
			glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), geom.shapeVertices.begin());
			
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glCullFace(GL_FRONT);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.shapeIndices.size(), GL_UNSIGNED_SHORT, geom.shapeIndices.begin());
			
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glCullFace(GL_BACK);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.shapeIndices.size(), GL_UNSIGNED_SHORT, geom.shapeIndices.begin());
		}

		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (!geom.bezier3Indices.empty())
		{
			glVertexPointer(2, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].x);
			glTexCoordPointer(3, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].k);
			glUseProgram(programs[PRG_RAST_FILL_CUBIC_AA]);

			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glCullFace(GL_FRONT);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.bezier3Indices.size(), GL_UNSIGNED_SHORT, geom.bezier3Indices.begin());

			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glCullFace(GL_BACK);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.bezier3Indices.size(), GL_UNSIGNED_SHORT, geom.bezier3Indices.begin());
		}

		//glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		//drawElementsNZ(geom, FILL_PRIM_TYPE_QUAD);

		//glUseProgram(programs[PRG_RAST_FILL_ARC]);
		//drawElementsNZ(geom, FILL_PRIM_TYPE_ARC);
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

		glPopClientAttrib();
		glPopAttrib();
	}

	void rasterizeEvenOdd(Geometry& geom)
	{
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

		if (!geom.shapeIndices.empty())
		{
			glUseProgram(0);
			glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), geom.shapeVertices.begin());
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.shapeIndices.size(), GL_UNSIGNED_SHORT, geom.shapeIndices.begin());
		}

		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (!geom.bezier3Indices.empty())
		{
			glVertexPointer(2, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].x);
			glTexCoordPointer(3, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].k);
			glUseProgram(programs[PRG_RAST_FILL_CUBIC]);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.bezier3Indices.size(), GL_UNSIGNED_SHORT, geom.bezier3Indices.begin());
		}

		//TODO: implement second order bezier
		//glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		//drawElements(geom, FILL_PRIM_TYPE_QUAD);

		//TODO: implement elliptic arcs
		//glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		//drawElements(geom, FILL_PRIM_TYPE_ARC);

		glPopClientAttrib();
		glPopAttrib();
	}

	void rasterizeEvenOddAA(Geometry& geom)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);

		glEnableClientState(GL_VERTEX_ARRAY);

		if (!geom.shapeIndices.empty())
		{
			glUseProgram(0);
			glColor4ub(0, 0, 0, 255);
			glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), geom.shapeVertices.begin());
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.shapeIndices.size(), GL_UNSIGNED_SHORT, geom.shapeIndices.begin());
		}

		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (!geom.bezier3Indices.empty())
		{
			glVertexPointer(2, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].x);
			glTexCoordPointer(3, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].k);
			glUseProgram(programs[PRG_RAST_FILL_CUBIC_AA]);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.bezier3Indices.size(), GL_UNSIGNED_SHORT, geom.bezier3Indices.begin());
		}

		//TODO: implement second order bezier
		//glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		//drawElements(geom, FILL_PRIM_TYPE_QUAD);

		//TODO: implement elliptic arcs
		//glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		//drawElements(geom, FILL_PRIM_TYPE_ARC);

		glPopClientAttrib();
		glPopAttrib();
	}

	void rasterizeNonZero(Geometry& geom)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
		glStencilMask(0xFF);

		glEnableClientState(GL_VERTEX_ARRAY);

		if (!geom.shapeIndices.empty())
		{
			glUseProgram(0);
			glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), geom.shapeVertices.begin());
			
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glCullFace(GL_FRONT);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.shapeIndices.size(), GL_UNSIGNED_SHORT, geom.shapeIndices.begin());
			
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glCullFace(GL_BACK);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.shapeIndices.size(), GL_UNSIGNED_SHORT, geom.shapeIndices.begin());
		}

		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (!geom.bezier3Indices.empty())
		{
			glVertexPointer(2, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].x);
			glTexCoordPointer(3, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].k);
			glUseProgram(programs[PRG_RAST_FILL_CUBIC]);

			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glCullFace(GL_FRONT);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.bezier3Indices.size(), GL_UNSIGNED_SHORT, geom.bezier3Indices.begin());

			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glCullFace(GL_BACK);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom.bezier3Indices.size(), GL_UNSIGNED_SHORT, geom.bezier3Indices.begin());
		}

		//glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		//drawElementsNZ(geom, FILL_PRIM_TYPE_QUAD);

		//glUseProgram(programs[PRG_RAST_FILL_ARC]);
		//drawElementsNZ(geom, FILL_PRIM_TYPE_ARC);

		glPopClientAttrib();
		glPopAttrib();
	}

	void RasterizeEvenOdd(GeometryOld<FillVertex, FILL_PRIM_TYPE_COUNT>& geom)
	{
		if (geom.vertices.empty())
			return;

		FillVertex& vtx = geom.vertices[0];
		
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
		glVertexPointer(2, GL_FLOAT, sizeof(FillVertex), &vtx.p);
		
		glUseProgram(0);
		drawElements(geom, FILL_PRIM_TYPE_TRI);

		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, sizeof(FillVertex), &vtx.tc);

		glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		drawElements(geom, FILL_PRIM_TYPE_QUAD);

		glUseProgram(programs[PRG_RAST_FILL_CUBIC]);
		drawElements(geom, FILL_PRIM_TYPE_CUBIC);

		glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		drawElements(geom, FILL_PRIM_TYPE_ARC);

		glPopClientAttrib();
		glPopAttrib();
	}

	void RasterizeEvenOddAA(GeometryOld<FillVertex, FILL_PRIM_TYPE_COUNT>& geom)
	{
		if (geom.vertices.empty())
			return;

		FillVertex& vtx = geom.vertices[0];
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(FillVertex), &vtx.p);
	
		glUseProgram(0);
		glColor4ub(0, 0, 0, 255);
		drawElements(geom, FILL_PRIM_TYPE_TRI);

		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, sizeof(FillVertex), &vtx.tc);

		glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		drawElements(geom, FILL_PRIM_TYPE_QUAD);

		glUseProgram(programs[PRG_RAST_FILL_CUBIC_AA]);
		drawElements(geom, FILL_PRIM_TYPE_CUBIC);

		glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		drawElements(geom, FILL_PRIM_TYPE_ARC);

		glPopClientAttrib();
		glPopAttrib();
	}

	void RasterizeNonZero(GeometryOld<FillVertex, FILL_PRIM_TYPE_COUNT>& geom)
	{
		if (geom.vertices.empty())
			return;

		FillVertex& vtx = geom.vertices[0];

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
		glStencilMask(0xFF);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(FillVertex), &vtx.p);

		glUseProgram(0);
		drawElementsNZ(geom, FILL_PRIM_TYPE_TRI);

		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, sizeof(FillVertex), &vtx.tc);

		glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		drawElementsNZ(geom, FILL_PRIM_TYPE_QUAD);

		glUseProgram(programs[PRG_RAST_FILL_CUBIC]);
		drawElementsNZ(geom, FILL_PRIM_TYPE_CUBIC);

		glUseProgram(programs[PRG_RAST_FILL_ARC]);
		drawElementsNZ(geom, FILL_PRIM_TYPE_ARC);

		glPopClientAttrib();
		glPopAttrib();
	}

	void StrokeGeometry::RasterizePrimitives(float halfWidth, bool jointMiter, bool jointRound)
	{
		glUseProgram(programs[PRG_RAST_STROKE_TRI]);
		glUniform1f(uniforms[UNI_RAST_STROKE_TRI_HALFWIDTH], halfWidth);
		drawElementsNZ(*this, STROKE_PRIM_TYPE_TRI);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glCullFace(GL_FRONT);
		drawElements(*this, STROKE_PRIM_TYPE_JOINT_BEVEL);
		if (jointMiter)
			drawElements(*this, STROKE_PRIM_TYPE_JOINT_MITER);

		glUseProgram(programs[PRG_RAST_STROKE_QUAD]);
		glUniform1f(uniforms[UNI_RAST_STROKE_QUAD_HALFWIDTH], halfWidth);
		drawElementsNZ(*this, STROKE_PRIM_TYPE_QUAD);

		glUseProgram(programs[PRG_RAST_STROKE_CUBIC]);
		glUniform1f(uniforms[UNI_RAST_STROKE_CUBIC_HALFWIDTH], halfWidth);
		drawElementsNZ(*this, STROKE_PRIM_TYPE_CUBIC);

		glUseProgram(programs[PRG_RAST_STROKE_ARC]);
		glUniform1f(uniforms[UNI_RAST_STROKE_ARC_HALFWIDTH], halfWidth);
		drawElementsNZ(*this, STROKE_PRIM_TYPE_ARC);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glCullFace(GL_FRONT);
		if (jointRound)
			drawElements(*this, STROKE_PRIM_TYPE_JOINT_ROUND);
	}

	void StrokeGeometry::Rasterize(float halfWidth, bool capRound, bool capButt)
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

		//Changing multiplier of normal change winding for tri primitive
		//So we need to change front face to make rasterization correct
		//And for other primitive types we need to change winding to add or remove parts:
		//We rasterize quad as tri, so for proper stroke we need area between tri edges,
		//changing normal multiplier in most cases does not change tri winding, only shifts
		//so if we do not change front face we will have 2 quads one under another
		//We need to change winding for second quad to add/remove pixels.
		
		if (capRound || capButt)
		{
			if (capRound)
				glUseProgram(programs[PRG_RAST_STROKE_ARC]);
			if (capButt)
				glUseProgram(programs[PRG_RAST_STROKE_TRI]);
			glUniform1f(uniforms[UNI_RAST_STROKE_TRI_HALFWIDTH], halfWidth);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glCullFace(GL_FRONT);
			drawElements(*this, STROKE_PRIM_TYPE_CAP);
		}

		//First pass
		glFrontFace(GL_CCW);
		RasterizePrimitives(halfWidth);

		//Second pass
		glFrontFace(GL_CW);
		RasterizePrimitives(-halfWidth);

		glPopClientAttrib();
		glPopAttrib();
	}

}
