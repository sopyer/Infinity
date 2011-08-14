#include <cassert>
#include <opengl.h>
#include <utils.h>

#include "Math.h"
#include "SharedResources.h"
#include "Rasterizer.h"

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
			glUseProgram(stencilCubicAreaAAProgram);
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
			glUseProgram(stencilCubicAreaAAProgram);

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
			glUseProgram(stencilCubicAreaProgram);
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
			glUseProgram(stencilCubicAreaAAProgram);
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
			glUseProgram(stencilCubicAreaProgram);

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

	void stencilPath(Geometry* geom, int useAA, int useNonZero)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0, 1);
		glStencilMask(0xFF); //TODO: make mask configurable
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, useNonZero?GL_INCR_WRAP:GL_INVERT);
		glStencilOpSeparate(GL_BACK,  GL_KEEP, GL_KEEP, useNonZero?GL_DECR_WRAP:GL_INVERT);

		glEnableClientState(GL_VERTEX_ARRAY);

		if (!geom->shapeIndices.empty())
		{
			glUseProgram(0);
			glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), geom->shapeVertices.begin());
			glDrawElements(GL_TRIANGLES, (GLsizei)geom->shapeIndices.size(), GL_UNSIGNED_SHORT, geom->shapeIndices.begin());
		}

		if (useAA) glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (!geom->bezier3Indices.empty())
		{
			glVertexPointer(2, GL_FLOAT, sizeof(Bezier3Vertex), &geom->bezier3Vertices[0].x);
			glTexCoordPointer(3, GL_FLOAT, sizeof(Bezier3Vertex), &geom->bezier3Vertices[0].k);
			glUseProgram(useAA?stencilCubicAreaAAProgram:stencilCubicAreaProgram);
			glDrawElements(GL_TRIANGLES, (GLsizei)geom->bezier3Indices.size(), GL_UNSIGNED_SHORT, geom->bezier3Indices.begin());
		}

		//TODO: implement second order bezier
		//glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		//drawElements(geom, FILL_PRIM_TYPE_QUAD);

		//TODO: implement elliptic arcs
		//glUseProgram(programs[PRG_RAST_FILL_QUAD]);
		//drawElements(geom, FILL_PRIM_TYPE_ARC);
		if (useAA) glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

		glPopClientAttrib();
		glPopAttrib();
	}
}
