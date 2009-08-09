#include "PathObject.h"
#include "Cubic.h"

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
		numPathVertices += 1;
		addPathPoint(vertices, p);
	}

	void PathObject::addPathPoint(std::vector<glm::vec2>& pts, const glm::vec2& p)
	{
		pts.push_back(p);
		if (pts.size()==1)
		{
			max = min = p;
		}
		else
		{
			min = glm::min(min, p);
			max = glm::max(max, p);
		}
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
		ptP = ptO = point;
		addPathPoint(ptO);
	}

	void PathObject::quadTo(const glm::vec2& p1, const glm::vec2& p2)
	{
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

	template<bool reverse>
	void PathObject::addSimpleCubic(const glm::vec2& p1, const glm::vec3& tc1,
									const glm::vec2& p2, const glm::vec3& tc2,
									const glm::vec2& p3, const glm::vec3& tc3,
									const glm::vec2& p4, const glm::vec3& tc4)
	{
		//Think about some check for "simplicity"

		//Inequality changed to avoid visual artefacts when value is near zero
		bool doChangeOrient = cubic::calcImplicit(tc2)<-0.0000001;
		
		//To handle case when second control point is near zero
		doChangeOrient |= cubic::calcImplicit(tc3)<-0.0000001;
		
		addCubicTriangle(p1, doChangeOrient?changeOrient(tc1):tc1, p2, doChangeOrient?changeOrient(tc2):tc2, p3, doChangeOrient?changeOrient(tc3):tc3);
		addCubicTriangle(p3, doChangeOrient?changeOrient(tc3):tc3, p4, doChangeOrient?changeOrient(tc4):tc4, p1, doChangeOrient?changeOrient(tc1):tc1);
		addPathPoint(reverse?p1:p4);
	}

	//For details of implementation see:
	//Loop, Blinn. Rendering resolution independent curves using programmable hardware.
	void PathObject::cubicTo(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
	{
		float	d[4], D;
		float	roots[2];
		size_t	numRoots = 0;

		//Vertices reversed on purpose! So we changed curve parametrization
		//We subdivide from in order 1 to 0
		//So we can use simple t2/t1 instead of (t2-t1)/(1-t2)
		glm::vec2 cp1[4] = {p3, p2, p1, ptO}, cp2[4];
		glm::vec3 tc1[4], tc2[4];

		//Handle this as soon as possible - to enable possibility of early return
		ptP = p2;
		ptO = p3;
		
		// Calc determinant
		cubic::calcDets(cp1, d, D);
		
		// First calculate t values at which we should subdivide our curve
		if (d[1] != 0)
		{
			float t1, t2;

			if (D>=0)
			{
				// Handle serpentine and cusp case
				cubic::calcSerpentineCuspTC(D, d, tc1, t1, t2);
			}
			else
			{
				// Handle loop case
				cubic::calcLoopTC(D, d, tc1, t1, t2);
			}
			
			//From greater value to lower - due to changed parametrization
			if (t1<t2)
				std::swap(t1, t2);

			if (0<t1 && t1<1)
				roots[numRoots++] = t1;

			if (t1!=t2 && 0<t2 && t2<1)
				roots[numRoots++] = t2;
		}
		else if (d[2]!=0)
		{
			//Handle cusp at infinity case
			float t;

			cubic::calcInfCuspTC(D, d, tc1, t);
			roots[numRoots++] = t;
		}
		else if (d[3]!=0)
		{
			//Handle quadratic curve case
			cubic::calcQuadraticTC(tc1);
		}
		else
		{
			//This is degenarate cases
			return;
		}

		// Subdivide curve from in direction from 1 to 0,
		// which represent normal direction in curve space
		for(size_t i=0; i<numRoots; ++i)
		{
			float t = i==0?roots[i]:roots[i]/roots[i-1];

			cubic::subdivide(cp1, t, cp1, cp2);
			cubic::subdivide(tc1, t, tc1, tc2);

			addSimpleCubic<true>(
				cp2[0], tc2[0],
				cp2[1], tc2[1],
				cp2[2], tc2[2],
				cp2[3], tc2[3]
			);
		}
		
		// The last or the only part
		addSimpleCubic<true>(
			cp1[0], tc1[0],
			cp1[1], tc1[1],
			cp1[2], tc1[2],
			cp1[3], tc1[3]
		);
	}

	void PathObject::arcTo(int segment, float rx, float ry, float angle, const glm::vec2& endPt)
	{
		assert(segment==VG_SCCWARC_TO || segment==VG_LCCWARC_TO
				|| segment==VG_SCWARC_TO || segment==VG_LCWARC_TO);

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
				//Here starts non control commands
				//So we can handle start path case
				if (startNewPath)
				{
					startNewPath = false;
					addPathPoint(ptO);
				}

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
