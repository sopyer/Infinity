#include "PathObject.h"
#include "Cubic.h"

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

	PathObject::PathObject(float pathScale, float pathBias):
		scale(pathScale), bias(pathBias)
	{}
	
	void PathObject::startPath(const glm::vec2& start)
	{
		startNewPath = true;
		numPathVertices = 0;
		ptP = ptO = start;
	}

	void PathObject::closePath()
	{
		if (numPathVertices > 0)
			subPathes.push_back(numPathVertices);

		startNewPath = true;
		numPathVertices = 0;
	}

	void PathObject::addPathPoint(const glm::vec2& p)
	{
		addPathPoint(vertices, p);
	}

	void PathObject::addPathPoint(std::vector<glm::vec2>& pts, const glm::vec2& p)
	{
		pts.push_back(p);
		numPathVertices += 1;
		min = glm::min(min, p);
		max = glm::max(max, p);
	}

	void PathObject::addSpecialTriangle(
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

	void PathObject::addCubicTriangle(const glm::vec2& p1, const glm::vec3& tc1,
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

	void PathObject::moveTo(const glm::vec2& newPos)
	{
		closePath();
		ptP = ptO = newPos;
	}

	void PathObject::lineTo(const glm::vec2& point)
	{
		if (startNewPath)
		{
			startNewPath = false;
			addPathPoint(ptO);
		}

		ptP = ptO = point;
		addPathPoint(ptO);
	}

	void PathObject::quadTo(const glm::vec2& p1, const glm::vec2& p2)
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

	glm::vec3 changeOrient(const glm::vec3& tc)
	{
		return glm::vec3(-tc.x, -tc.y, tc.z);
	}

	float cubicSign(const glm::vec3& tc)
	{
		return tc.x*tc.x*tc.x - tc.y*tc.z;
	}

	void PathObject::triangulateCubic(float d[4], const glm::vec2 p[4], glm::vec3 tc[4])
	{
		//regular cases
		if (d[0]<0 && d[3]<0)
		{
			if (d[2]<0)
			{
				addCubicTriangle(p[0], tc[0], p[2], tc[2], p[3], tc[3]);
			}
			else
			{
				addCubicTriangle(p[0], tc[0], p[1], tc[1], p[3], tc[3]);
				if (d[1]>0)
				{
					//addCubicTriangle(p[0], tc[0], p[1], tc[1], p[3], tc[3]);
					tc[0].x = -tc[0].x; tc[0].y = -tc[0].y;
					tc[2].x = -tc[2].x; tc[2].y = -tc[2].y;
					tc[3].x = -tc[3].x; tc[3].y = -tc[3].y;
					addCubicTriangle(p[0], tc[0], p[2], tc[2], p[3], tc[3]);
				}
				else
				{
					//addCubicTriangle(p[0], tc[0], p[1], tc[1], p[3], tc[3]);
				}
			}
		}
		else
		{
			addCubicTriangle(p[1], tc[1], p[2], tc[2], p[3], tc[3]);
			if (d[2]<0)
			{
				if (d[1]>0)
				{
					addCubicTriangle(p[0], tc[0], p[2], tc[2], p[3], tc[3]);
					//addCubicTriangle(p[1], tc[1], p[2], tc[2], p[3], tc[3]);
				}
				else
				{
					//assert(0);
					//addCubicTriangle(p[1], tc[1], p[2], tc[2], p[3], tc[3]);
				}
			}
			else
			{
				addCubicTriangle(p[0], tc[0], p[1], tc[1], p[3], tc[3]);
				//addCubicTriangle(p[1], tc[1], p[2], tc[2], p[3], tc[3]);
			}
		}

		//special cases: some of d[] == 0
	}

	void PathObject::cubicTo(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
	{
		if (startNewPath)
		{
			startNewPath = false;
			addPathPoint(ptO);
		}

		glm::vec3 cp0(ptO, 1);
		glm::vec3 cp1(p1, 1);
		glm::vec3 cp2(p2, 1);
		glm::vec3 cp3(p3, 1);

		glm::vec2 p[4] = {ptO, p1, p2, p3};
		glm::vec3 tc[4];

		size_t numTri = 0;
		size_t idxTri[2][3];

		float a[4] = {
			glm::dot(cp3, glm::cross(cp2, cp1)),	
			-glm::dot(cp3, glm::cross(cp2, cp0)),	
			glm::dot(cp3, glm::cross(cp1, cp0)),
			-glm::dot(cp2, glm::cross(cp1, cp0))
		};

		float d[4];

		d[0] = 3*(a[3]+a[2]+a[1]+a[0]);
		d[1] = 3*(3*a[3]+2*a[2]+a[1]);
		d[2] = 3*(3*a[3]+a[2]);
		d[3] = 3*(3*a[3]);

		//following expression has the same sign as d1*d1*(3*d2*d2 - 4*d1*d3)
		//and later it is reused as discriminant for quadratic equation
		float D = 3.0f*d[2]*d[2] - 4.0f*d[1]*d[3];
		
		glm::vec3	cpp[4] = {cp0, cp1, cp2, cp3};

		cubic::calcDets(cpp, a, d, D);

		//See Loop, Blinn. Rendering resolution independent curves using programmable hardware
		if (d[1] != 0)
		{
			if (D>=0)
			{
				float t1, t2;

				if (d[2] == 0)
				{
					t1 = -sqrt(D/3.0f)/2.0f/d[1];
					t2 = sqrt(D/3.0f)/2.0f/d[1];
				}
				else
				{
					float dd = 0.5f * (d[2] + glm::sign(d[2])*sqrt(D/3.0f));
					
					t1 = dd/d[1];
					t2 = d[3]/3.0f/dd;
				}

				glm::vec3	kk0(t1*t2,   t1*t1*t1,    t2*t2*t2),
							kk1(-t1-t2,  -3.0f*t1*t1, -3.0f*t2*t2),
							kk2(1.0f,    3.0f*t1,     3.0f*t2),
							kk3(0.0f,    -1.0f,       -1.0f);

				tc[0] = glm::vec3(kk0);
				tc[1] = glm::vec3(kk0+1.0f/3.0f*kk1);
				tc[2] = glm::vec3(kk0+2.0f/3.0f*kk1+1.0f/3.0f*kk2);
				tc[3] = glm::vec3(kk0+kk1+kk2+kk3);
				
				cubic::calcSerpentineCuspTC(D, d, tc);
				cubic::triangulate(a, numTri, idxTri);
			}
			else
			{
				//	loop
				float t1 = (d[2] + sqrt(-D))/d[1]/2.0f;
				float t2 = 2.0f*d[1]/(d[2] - sqrt(-D));

				glm::vec3	kk0(t1,       t1*t1,             t1),
							kk1(-t1*t2-1, -t1*t1*t2-2.0f*t1, -1-2*t1*t2),
							kk2(t2,       1+2.0f*t1*t2,      t1*t2*t2+2.0f*t2),
							kk3(0.0f,     -t2,               -t2*t2);

				tc[0] = glm::vec3(kk0);
				tc[1] = glm::vec3(kk0+1.0f/3.0f*kk1);
				tc[2] = glm::vec3(kk0+2.0f/3.0f*kk1+1.0f/3.0f*kk2);
				tc[3] = glm::vec3(kk0+kk1+kk2+kk3);
				
				//!!! TODO !!!
				//Handle subdivision case

				cubic::calcLoopTC(D, d, tc);
				cubic::triangulate(a, numTri, idxTri);
			}
		}
		else if (d[2]!=0)
		{
			//cusp with cusp at infinity
			float t = d[3]/3.0f/d[2];

			glm::vec3	kk0(t,     t*t*t,     1.0f),
						kk1(-1.0f, -3.0f*t*t, 0.0f),
						kk2(0.0f,  3.0f*t,    0.0f),
						kk3(0.0f,  -1,        0.0f);

			tc[0] = glm::vec3(kk0);
			tc[1] = glm::vec3(kk0+1.0f/3.0f*kk1);
			tc[2] = glm::vec3(kk0+2.0f/3.0f*kk1+1.0f/3.0f*kk2);
			tc[3] = glm::vec3(kk0+kk1+kk2+kk3);
			
			cubic::calcInfCuspTC(D, d, tc);
			cubic::triangulate(a, numTri, idxTri);
		}
		else if (d[3]!=0)
		{
			cubic::calcQuadraticTC(tc);
			cubic::triangulate(a, numTri, idxTri);
		}

		for (size_t i=0; i<numTri; ++i)
		{
			bool doChangeOrient = cubic::calcImplicit(tc[idxTri[i][0]])<0;
			addCubicTriangle(
				p[idxTri[i][0]], doChangeOrient?changeOrient(tc[idxTri[i][0]]):tc[idxTri[i][0]],
				p[idxTri[i][1]], doChangeOrient?changeOrient(tc[idxTri[i][1]]):tc[idxTri[i][1]],
				p[idxTri[i][2]], doChangeOrient?changeOrient(tc[idxTri[i][2]]):tc[idxTri[i][2]]
			);
		}

		addPathPoint(p3);

		ptP = p2;
		ptO = p3;
	}

	void PathObject::arcTo(int segment, float rx, float ry, float angle, const glm::vec2& endPt)
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

		//!!! TODO !!!
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

	void PathObject::preprocess(size_t segOffset, size_t dataOffset)
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

	void PathObject::maskPathRegion()
	{
		if (vertices.empty())
			return;

		glEnableClientState(GL_VERTEX_ARRAY);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		//!!! TODO !!!
		//Fix masking with incrementing stencil - different masks for different polies
		//and may be use different z-buffer to avoid collision with 3D.
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