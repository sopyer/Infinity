#include <cassert>
#include <gl\glee.h>
#include <utils.h>
#include "Cubic.h"
#include "SharedResources.h"
#include "Rasterizer.h"
#include "GeometryBuilders.h"

namespace impl
{
	using namespace shared;

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

	void buildFillGeometry(const VGint    numSegments,
						   const VGubyte* pathSegments,
						   const VGfloat* pathData,
						   FillGeometry&  geom)
	{
		FillGeometryBuilder	builder;
		Array<glm::vec2>	points;
		glm::vec2			o, p;
		bool				segmentStarted = false;

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
					builder.end();
					segmentStarted = false;
				}
			}
			else if (segment==VG_MOVE_TO)
			{
				if (segmentStarted)
				{
					builder.end();
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
							float y = readFloat(pathData) + (rel?o.x:0);
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
			}
		}

		if (segmentStarted)
			builder.end();

		builder.copyDataTo(geom);
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
							float y = readFloat(pathData) + (rel?o.x:0);
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

	void FillGeometry::RasterizeEvenOdd()
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

	void FillGeometry::RasterizeNonZero()
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

	void StrokeGeometry::RasterizePrimitives(float halfWidth, bool jointMiter, bool jointRound)
	{
		glUseProgram(programs[PRG_RAST_STROKE_TRI]);
		glUniform1f(uniforms[UNI_RAST_STROKE_TRI_HALFWIDTH], halfWidth);
		drawElementsNZ(STROKE_PRIM_TYPE_TRI);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glCullFace(GL_FRONT);
		drawElements(STROKE_PRIM_TYPE_JOINT_BEVEL);
		if (jointMiter)
			drawElements(STROKE_PRIM_TYPE_JOINT_MITER);

		glUseProgram(programs[PRG_RAST_STROKE_QUAD]);
		glUniform1f(uniforms[UNI_RAST_STROKE_QUAD_HALFWIDTH], halfWidth);
		drawElementsNZ(STROKE_PRIM_TYPE_QUAD);

		glUseProgram(programs[PRG_RAST_STROKE_CUBIC]);
		glUniform1f(uniforms[UNI_RAST_STROKE_CUBIC_HALFWIDTH], halfWidth);
		drawElementsNZ(STROKE_PRIM_TYPE_CUBIC);

		glUseProgram(programs[PRG_RAST_STROKE_ARC]);
		glUniform1f(uniforms[UNI_RAST_STROKE_ARC_HALFWIDTH], halfWidth);
		drawElementsNZ(STROKE_PRIM_TYPE_ARC);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glCullFace(GL_FRONT);
		if (jointRound)
			drawElements(STROKE_PRIM_TYPE_JOINT_ROUND);
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
			drawElements(STROKE_PRIM_TYPE_CAP);
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
