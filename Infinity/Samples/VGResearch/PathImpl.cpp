#include "PathImpl.h"

#pragma once

namespace vg
{
	ProgramRef	maskQuad;
	ProgramRef	maskCubic;
	ProgramRef	maskArc;

	void init(ProgramRef maskQuad, ProgramRef maskCubic, ProgramRef maskArc)
	{
		vg::maskQuad = maskQuad;
		vg::maskCubic = maskCubic;
		vg::maskArc = maskArc;
	}

	void deinit()
	{
		vg::maskQuad = 0;
		vg::maskCubic = 0;
		vg::maskArc = 0;
	}
}

namespace vg
{
	const int coordsPerCommand[] =
	{
	  0, /* VG_CLOSE_PATH */
	  2, /* VG_MOTE_TO */
	  2, /* VG_LINE_TO */
	  1, /* VG_HLINE_TO */
	  1, /* VG_VLINE_TO */
	  4, /* VG_QUAD_TO */
	  6, /* VG_CUBIC_TO */
	  2, /* VG_SQUAD_TO */
	  4, /* VG_SCUBIC_TO */
	  5, /* VG_SCCWARC_TO */
	  5, /* VG_SCWARC_TO */
	  5, /* VG_LCCWARC_TO */
	  5  /* VG_LCWARC_TO */
	};

	PathImpl::PathImpl(float pathScale, float pathBias):
		scale(pathScale), bias(pathBias)
	{}
	
	void PathImpl::startPath(const glm::vec2& start)
	{
		startNewPath = true;
		numPathVertices = 0;
		ptP = ptO = start;
	}

	void PathImpl::closePath()
	{
		if (numPathVertices > 0)
			subPathes.push_back(numPathVertices);

		startNewPath = true;
		numPathVertices = 0;
	}

	void PathImpl::addPathPoint(const glm::vec2& p)
	{
		addPathPoint(vertices, p);
	}

	void PathImpl::addPathPoint(std::vector<glm::vec2>& pts, const glm::vec2& p)
	{
		pts.push_back(p);
		numPathVertices += 1;
		min = glm::min(min, p);
		max = glm::max(max, p);
	}

	void PathImpl::addSpecialTriangle(
		std::vector<glm::vec2>& pts,
		std::vector<glm::vec2>& tcs,
		const glm::vec2& p1, const glm::vec2& tc1,
		const glm::vec2& p2, const glm::vec2& tc2,
		const glm::vec2& p3, const glm::vec2& tc3)
	{
		addPathPoint(pts, p1);
		tcs.push_back(tc1);

		addPathPoint(pts, p2);
		tcs.push_back(tc2);

		addPathPoint(pts, p3);
		tcs.push_back(tc3);
	}

	void PathImpl::addCubicTriangle(const glm::vec2& p1, const glm::vec3& tc1,
									const glm::vec2& p2, const glm::vec3& tc2,
									const glm::vec2& p3, const glm::vec3& tc3)
	{
		addPathPoint(cubics, p1);
		cubicsTC.push_back(tc1);

		addPathPoint(cubics, p2);
		cubicsTC.push_back(tc2);

		addPathPoint(cubics, p3);
		cubicsTC.push_back(tc3);
	}

	void PathImpl::moveTo(const glm::vec2& newPos)
	{
		closePath();
		ptP = ptO = newPos;
	}

	void PathImpl::lineTo(const glm::vec2& point)
	{
		if (startNewPath)
		{
			startNewPath = false;
			addPathPoint(ptO);
		}

		ptP = ptO = point;
		addPathPoint(ptO);
	}

	void PathImpl::quadTo(const glm::vec2& p1, const glm::vec2& p2)
	{
		if (startNewPath)
		{
			startNewPath = false;
			addPathPoint(ptO);
		}

		addPathPoint(p2);

		addQuadTriangle(ptO, glm::vec2(0.0f, 0.0f),
						p1,     glm::vec2(0.5f, 0.0f),
						p2,     glm::vec2(1.0f, 1.0f));
		ptP = p1;
		ptO = p2;
	}

	void PathImpl::cubicTo(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
	{
		if (startNewPath)
		{
			startNewPath = false;
			addPathPoint(ptO);
		}

		addPathPoint(p3);

		//glm::vec3 b0(ptO, 1);
		//glm::vec3 b1(p1, 1);
		//glm::vec3 b2(p2, 1);
		//glm::vec3 b3(p3, 1);
		
		glm::vec3 cp0(ptO, 1);
		glm::vec3 cp1(p1, 1);
		glm::vec3 cp2(p2, 1);
		glm::vec3 cp3(p3, 1);

		glm::vec3 b0(cp0);
		glm::vec3 b1(-3.0f*cp0 + 3.0f*cp1);
		glm::vec3 b2( 3.0f*cp0 - 6.0f*cp1 + 3.0f*cp2);
		glm::vec3 b3(     -cp0 + 3.0f*cp1 - 3.0f*cp2 + cp3);

		float d0 = glm::dot(b3, glm::cross(b2, b1));	
		float d1 = -glm::dot(b3, glm::cross(b2, b0));	
		float d2 = glm::dot(b3, glm::cross(b1, b0));	
		float d3 = -glm::dot(b2, glm::cross(b1, b0));
		//float a1 = glm::dot(b0, glm::cross(b3, b2));	
		//float a2 = glm::dot(b1, glm::cross(b0, b3));	
		//float a3 = glm::dot(b2, glm::cross(b1, b0));

		//float d1 = a1 - 2*a2 + 3*a3;
		//float d2 = -a2 + 3*a3;
		//float d3 = 3*a3;

		float discr = d1*d1*(3*d2*d2 - 4*d1*d3);
		
		assert(discr>0);

		//See Loop, Blinn. Rendering resolution independent curves using programmable hardware
		if (discr>0)
		{
			//Serpentine
			float ls = d2 - sqrt(d2*d2 - 4.0f/3.0f*d1*d3);
			float lt = 2*d1;
			float ms = d2 + sqrt(d2*d2 - 4.0f/3.0f*d1*d3);
			float mt = 2*d1;

			glm::vec2 r1(ls, lt);
			//r1 = glm::normalize(r1);
			ls = r1.x/r1.y;
			lt = 1;

			glm::vec2 r2(ms, mt);
			//r1 = glm::normalize(r2);
			ms = r2.x/r2.y;
			mt = 1;

			//Calculating linear factors
			float L03 = ls;						float M03 = ms;
			float L13 = ls - 1.0f/3.0f * lt;	float M13 = ms - 1.0f/3.0f * mt;
			float L23 = ls - 2.0f/3.0f * lt;	float M23 = ms - 2.0f/3.0f * mt;
			float L33 = ls - lt;				float M33 = ms - mt;

			//Calculating functions k, l, m along cubic at positions (0,3), (1,3), (2,3), (3,3)
			float k03 = L03*M03;	float l03 = L03*L03*L03;	float m03 = M03*M03*M03;
			float k13 = L13*M13;	float l13 = L13*L13*L13;	float m13 = M13*M13*M13;
			float k23 = L23*M23;	float l23 = L23*L23*L23;	float m23 = M23*M23*M23;
			float k33 = L33*M33;	float l33 = L33*L33*L33;	float m33 = M33*M33*M33;
			
			glm::vec4	kv(k03, k13, k23, k33);
			glm::vec4	lv(l03, l13, l23, l33);
			glm::vec4	mv(m03, m13, m23, m33);

			glm::vec4	ic13(-5.0f/6.0f, 3.0f, -3.0f/2.0f, 1.0f/3.0f);
			glm::vec4	ic23(1.0f/3.0f, -3.0f/2.0f, 3.0f, -5.0f/6.0f);
			
			float d;
			d = 8.0f/27.0f*k03 + 12.0f/27.0f*glm::dot(kv, ic13) + 6.0f/27.0f*glm::dot(kv, ic23) + 1.0f/27.0f*k33;
			//assert(d == k13);
			d = 1.0f/27.0f*k03 + 6.0f/27.0f*glm::dot(kv, ic13) + 12.0f/27.0f*glm::dot(kv, ic23) + 8.0f/27.0f*k33;
			//assert(d == k23);

			//Calculation linear functionals at control points
			glm::vec3 tc03(-k03, -l03, m03);
			glm::vec3 tc13(-glm::dot(kv, ic13), -glm::dot(lv, ic13), glm::dot(mv, ic13));
			glm::vec3 tc23(-glm::dot(kv, ic23), -glm::dot(lv, ic23), glm::dot(mv, ic23));
			glm::vec3 tc33(-k33, -l33, m33);

			//glm::vec3 tc03(k03, l03, m03);
			//glm::vec3 tc13(glm::dot(kv, ic13), glm::dot(lv, ic13), glm::dot(mv, ic13));
			//glm::vec3 tc23(glm::dot(kv, ic23), glm::dot(lv, ic23), glm::dot(mv, ic23));
			//glm::vec3 tc33(k33, l33, m33);

			//addCubicTriangle(ptO, tc03, p1, tc13, p3, tc33);
			//addCubicTriangle(ptO, tc03, p2, tc23, p3, tc33);
			addCubicTriangle(ptO, tc03, p1, tc13, p2, tc23);
			addCubicTriangle(p1, tc13, p2, tc23, p3, tc33);
		}

		ptP = p1;
		ptO = p2;
	}

	void PathImpl::arcTo(int segment, float rx, float ry, float angle, const glm::vec2& endPt)
	{
		assert(segment==VG_SCCWARC_TO || segment==VG_LCCWARC_TO
				|| segment==VG_SCWARC_TO || segment==VG_LCWARC_TO);

		if (startNewPath)
		{
			startNewPath = false;
			addPathPoint(ptO);
		}

		bool isCCW      = segment==VG_SCCWARC_TO || segment==VG_LCCWARC_TO;
		bool isSmallArc = segment==VG_SCCWARC_TO || segment==VG_SCWARC_TO;

		//handle etraordinary(near zero) cases
		//handle case when input data is not valid

		//rotate by -angle
		glm::vec2 p1 = glm::rotateGTX(ptO, -angle);
		glm::vec2 p2 = glm::rotateGTX(endPt, -angle);

		//scale
		p1 *= glm::vec2(1/rx, 1/ry);
		p2 *= glm::vec2(1/rx, 1/ry);

		glm::vec2 middle = (p2+p1)/2.0f;
		glm::vec2 ox = glm::normalize(middle-p1);
		glm::vec2 oy = isCCW ? glm::vec2(-ox.y, ox.x) : glm::vec2(ox.y, -ox.x);

		float a = glm::length(middle - p1);
		float b = sqrt(1-a*a);
		
		glm::vec2	center = middle + oy*b;

		//standart path internal side of shape
		if (isSmallArc)
		{
			glm::vec2 up = (1-b)*(-oy);
			
			glm::vec2 pt1 = p1+up;
			glm::vec2 pt2 = middle+up;
			glm::vec2 pt3 = p2+up;

			glm::vec2	pt;
			
			addArcTriangle(glm::rotateGTX(p1*glm::vec2(rx, ry), angle),  p1-center,
						   glm::rotateGTX(pt1*glm::vec2(rx, ry), angle), pt1-center,
						   glm::rotateGTX(pt2*glm::vec2(rx, ry), angle), pt2-center);

			addArcTriangle(glm::rotateGTX(pt2*glm::vec2(rx, ry), angle), pt2-center,
						   glm::rotateGTX(pt3*glm::vec2(rx, ry), angle), pt3-center,
						   glm::rotateGTX(p2*glm::vec2(rx, ry), angle),  p2-center);

			addPathPoint(glm::rotateGTX(pt2*glm::vec2(rx, ry), angle));
		}
		else
		{
			glm::vec2 side = (1-a)*(-ox);
			
			glm::vec2 pt1 = p1+side;
			glm::vec2 pt2 = center-ox;
			glm::vec2 pt3 = center+oy-ox;
			glm::vec2 pt4 = center+oy;
			glm::vec2 pt5 = center+oy+ox;
			glm::vec2 pt6 = center+ox;
			glm::vec2 pt7 = p2-side;

			addArcTriangle(glm::rotateGTX(p1*glm::vec2(rx, ry), angle),  p1-center,
						   glm::rotateGTX(pt1*glm::vec2(rx, ry), angle), pt1-center,
						   glm::rotateGTX(pt2*glm::vec2(rx, ry), angle), pt2-center);

			addArcTriangle(glm::rotateGTX(pt2*glm::vec2(rx, ry), angle), pt2-center,
						   glm::rotateGTX(pt3*glm::vec2(rx, ry), angle), pt3-center,
						   glm::rotateGTX(pt4*glm::vec2(rx, ry), angle), pt4-center);

			addArcTriangle(glm::rotateGTX(pt4*glm::vec2(rx, ry), angle), pt4-center,
						   glm::rotateGTX(pt5*glm::vec2(rx, ry), angle), pt5-center,
						   glm::rotateGTX(pt6*glm::vec2(rx, ry), angle), pt6-center);

			addArcTriangle(glm::rotateGTX(pt6*glm::vec2(rx, ry), angle), pt6-center,
						   glm::rotateGTX(pt7*glm::vec2(rx, ry), angle), pt7-center,
						   glm::rotateGTX(p2*glm::vec2(rx, ry), angle),  p2-center);

			addPathPoint(glm::rotateGTX(pt2*glm::vec2(rx, ry), angle));
			addPathPoint(glm::rotateGTX(pt4*glm::vec2(rx, ry), angle));
			addPathPoint(glm::rotateGTX(pt6*glm::vec2(rx, ry), angle));
		}

		addPathPoint(endPt);
		ptP = ptO = endPt;
	}

	void PathImpl::preprocess(size_t segOffset, size_t dataOffset)
	{
		struct dataFeeder
		{
			dataFeeder(std::vector<float>& source, size_t offset):
				dataSource(source),
				ptO(offset)
			{}

			glm::vec2	getPoint()
			{
				size_t	idx = ptO;
				ptO += 2;
				return glm::vec2(dataSource[idx], dataSource[idx+1]);
			}

			float		getFloat()
			{
				return dataSource[ptO++];
			}

			std::vector<float>&	dataSource;
			size_t				ptO;
		};

		dataFeeder	dataSource(data, dataOffset);
		
		startPath(glm::vec2());
		for (size_t s=segOffset; s<segs.size(); ++s)
		{
			int		segment = segs[s]&0x1E;
			int		segidx = segment>>1;
			bool	relative = segs[s]&1;
			glm::vec2	pt;
			
			if (segment==VG_CLOSE_PATH)
			{
				closePath();
			}
			else if (segment==VG_MOVE_TO)
			{
				moveTo(dataSource.getPoint() + (relative?ptO:glm::vec2()));
			}
			else
			{
				switch (segment)
				{
					case VG_LINE_TO:
						{
							glm::vec2 newPos = dataSource.getPoint() + (relative?ptO:glm::vec2());
							lineTo(newPos);
						}
						break;

					case VG_HLINE_TO:
						{
							glm::vec2 newPos = glm::vec2(dataSource.getFloat() + (relative?ptO.x:0), ptO.y);
							lineTo(newPos);
						}
						break;

					case VG_VLINE_TO:
						{
							glm::vec2 newPos = glm::vec2(ptO.x, dataSource.getFloat() + (relative?ptO.y:0));
							lineTo(newPos);
						}
						break;

					case VG_QUAD_TO:
						{
							glm::vec2 p1 = dataSource.getPoint() + (relative?ptO:glm::vec2());
							glm::vec2 p2 = dataSource.getPoint() + (relative?ptO:glm::vec2());
							quadTo(p1, p2);
						}
						break;

					case VG_SQUAD_TO:
						{
							glm::vec2 p1 = 2.0f*ptO - ptP;
							glm::vec2 p2 = dataSource.getPoint() + (relative?ptO:glm::vec2());
							quadTo(p1, p2);
						}
						break;

					case VG_CUBIC_TO:
						{
							glm::vec2 p1 = dataSource.getPoint() + (relative?ptO:glm::vec2());
							glm::vec2 p2 = dataSource.getPoint() + (relative?ptO:glm::vec2());
							glm::vec2 p3 = dataSource.getPoint() + (relative?ptO:glm::vec2());
							cubicTo(p1, p2, p3);
						}
						break;

					case VG_SCUBIC_TO:
						{
							glm::vec2 p1 = 2.0f*ptO - ptP;
							glm::vec2 p2 = dataSource.getPoint() + (relative?ptO:glm::vec2());
							glm::vec2 p3 = dataSource.getPoint() + (relative?ptO:glm::vec2());
							cubicTo(p1, p2, p3);
						}
						break;

					case VG_SCWARC_TO:
					case VG_SCCWARC_TO:
					case VG_LCWARC_TO:
					case VG_LCCWARC_TO:
						{
							float		rx = dataSource.getFloat();
							float		ry = dataSource.getFloat();
							float		angle = dataSource.getFloat();
							glm::vec2	endPt = dataSource.getPoint() + (relative?ptO:glm::vec2());
							arcTo(segment, rx, ry, angle, endPt);
						}
						break;

					default:
						assert(0);
				}
			}
		}
	}

	void PathImpl::maskPathRegion()
	{
		if (vertices.empty())
			return;

		glEnableClientState(GL_VERTEX_ARRAY);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
		glStencilFunc(GL_ALWAYS, 0, 1);
		glStencilMask(0x01);
		
		glCullFace(GL_FRONT_AND_BACK);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		glUseProgram(0);

		size_t base = 0;
		glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &vertices[0]);
		//for(size_t i=0; i<subPathes.size(); ++i)
		//{
		//	size_t numPoints = subPathes[i];
		//	glDrawArrays(GL_TRIANGLE_FAN, base, numPoints);
		//	base += numPoints;
		//}

		if (!quads.empty())
		{
			glUseProgram(*maskQuad);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &quads[0]);
			glTexCoordPointer(2, GL_FLOAT, 2*sizeof(float), &quadsTC[0]);
			glDrawArrays(GL_TRIANGLES, 0, quads.size());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		if (!cubics.empty())
		{
			glUseProgram(*maskCubic);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &cubics[0]);
			glTexCoordPointer(3, GL_FLOAT, 3*sizeof(float), &cubicsTC[0]);
			glDrawArrays(GL_TRIANGLES, 0, cubics.size());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glUseProgram(0);
			glColor4f(1, 0, 0, 1);
			glColorMask(TRUE, TRUE, TRUE, TRUE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, cubics.size());
		}

		if (!arcs.empty())
		{
			glUseProgram(*maskArc);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &arcs[0]);
			glTexCoordPointer(2, GL_FLOAT, 2*sizeof(float), &arcsTC[0]);
			glDrawArrays(GL_TRIANGLES, 0, arcs.size());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glPopAttrib();
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	bool isValidCommand(int c)
	{
		return c>=(VG_CLOSE_PATH >> 1) && c<=(VG_LCWARC_TO >> 1);
	}

	size_t coordCountForData(size_t segCount, const ubyte *segs)
	{
		size_t	count = 0;

		for (size_t s=0; s<segCount; ++s)
		{
			ubyte command = ((segs[s] & 0x1E) >> 1);
			if (!isValidCommand(command))
				return 0;
			count += coordsPerCommand[command];
		}

		return count;
	}
}