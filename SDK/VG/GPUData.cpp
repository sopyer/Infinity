#include <cassert>
#include <gl\glee.h>
#include "GPUData.h"
#include "Cubic.h"
#include "SharedResources.h"

namespace vg
{
	GPUData::GPUData(): mBase(-1)
	{}

	void GPUData::begin(float x, float y)
	{
		assert(mBase==-1);
		
		Vertex v;
		v.p = glm::vec2(x, y);
		
		mBase = addVertex(v);
	}

	void GPUData::end()
	{
		//Is this needed? Seems not if we will create stroke using special methods
		//u32 last = (u32)triVertice.size();
		//if (last>mBase)
		//	addTri(mBase, mBase+1, last);
		mBase=-1;
	}
	
	void GPUData::line(float x0, float y0, float x1, float y1)
	{
		Vertex v;
		
		v.p = glm::vec2(x0, y0);
		v.n = normalize(glm::vec2(-(y1-y0), x1-x0));  //perpendicular to the line segment
		u32 i1 = addVertex(v);

		v.p = glm::vec2(x1, y1);
		//normal is the same:)
		u32 i2 = addVertex(v);

		addRegTri(mBase, i1, i2);
	}

	void GPUData::quad(float x0, float y0, float x1, float y1, float x2, float y2)
	{
		Vertex v;
		
		v.p = glm::vec2(x0, y0);
		v.n = normalize(glm::vec2(-(y1-y0), x1-x0));  //perpendicular to the line segment
		v.tc = glm::vec3(0.0f, 0.0f, 0.0f);
		u32 i0 = addVertex(v);

		v.p = glm::vec2(x1, y1);
		v.n = normalize(glm::vec2(0, 0));  //to calc!!!!!
		v.tc = glm::vec3(0.5f, 0.0f, 0.0f);
		u32 i1 = addVertex(v);

		v.p = glm::vec2(x2, y2);
		v.n = normalize(glm::vec2(-(y2-y1), x2-x1));  //perpendicular tu the line segment
		v.tc = glm::vec3(1.0f, 1.0f, 0.0f);
		u32 i2 = addVertex(v);
		
		addQuadTri(i0, i1, i2);
	}

	void GPUData::cubic(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
	{
		cubic::Determinants	det;
		float	roots[2];
		size_t	numRoots = 0;

		//Vertices reversed on purpose! So we changed curve parametrization
		//We subdivide from in order 1 to 0
		//So we can use simple t2/t1 instead of (t2-t1)/(1-t2)
		glm::vec2 cp1[4] = {glm::vec2(x3, y3), glm::vec2(x2, y2), glm::vec2(x1, y1), glm::vec2(x0, y0)};
		glm::vec2 cp2[4];
		glm::vec3 tc1[4], tc2[4];

		// Calc determinant
		cubic::calcDets(cp1, det);
		
		// First calculate t values at which we should subdivide our curve
		if (det.d[1] != 0)
		{
			float t1, t2;

			if (det.D>=0)
			{
				// Handle serpentine and cusp case
				cubic::calcSerpentineCuspTC(det, tc1, t1, t2);
			}
			else
			{
				// Handle loop case
				cubic::calcLoopTC(det, tc1, t1, t2);
			}
			
			//From greater value to lower - due to changed parametrization
			if (t1<t2)
				std::swap(t1, t2);

			if (0<t1 && t1<1)
				roots[numRoots++] = t1;

			if (t1!=t2 && 0<t2 && t2<1)
				roots[numRoots++] = t2;
		}
		else if (det.d[2]!=0)
		{
			//Handle cusp at infinity case
			float t;

			cubic::calcInfCuspTC(det, tc1, t);
			roots[numRoots++] = t;
		}
		else if (det.d[3]!=0)
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

		u32 i0, i1, i2;

		for(size_t i=0; i<numRoots; ++i)
		{
			float t = i==0?roots[i]:roots[i]/roots[i-1];

			cubic::subdivide(cp1, t, cp1, cp2);
			cubic::subdivide(tc1, t, tc1, tc2);

			addSimpleCubic(
				i1, i2,
				cp2[0], tc2[0],
				cp2[1], tc2[1],
				cp2[2], tc2[2],
				cp2[3], tc2[3]
			);
			
			if (i==0)
				i0=i2;
			else
				addRegTri(i0, i1, i2);
		}
		
		// The last or the only part
		addSimpleCubic(
			i1, i2,
			cp1[0], tc1[0],
			cp1[1], tc1[1],
			cp1[2], tc1[2],
			cp1[3], tc1[3]
		);

		if (numRoots>0)
			addRegTri(i0, i1, i2);
	}

	void arc(VGPathSegment type, float rx, float ry, float angle, float x0, float y0, float x1, float y1);

	struct AddFunctor
	{
		AddFunctor(u32 num) : value(num) {}

		u32 operator()(u32 num) {return value+num;}

		u32 value;
	};

	void GPUData::append(u32 mode, const GPUData& other)
	{
		vertices.insert(vertices.end(), other.vertices.begin(), other.vertices.end());

		std::transform(	other.regIndices.begin(), other.regIndices.end(),
						regIndices.end(),
						AddFunctor((u32)regIndices.size()));

		std::transform(	other.arcIndices.begin(), other.arcIndices.end(),
						arcIndices.end(),
						AddFunctor((u32)arcIndices.size()));

		std::transform(	other.quadIndices.begin(), other.quadIndices.end(),
						quadIndices.end(),
						AddFunctor((u32)quadIndices.size()));

		std::transform(	other.cubicIndices.begin(), other.cubicIndices.end(),
						cubicIndices.end(),
						AddFunctor((u32)cubicIndices.size()));
	}

	u32 GPUData::addVertex(const Vertex& v)
	{
		u32 idx = (u32)vertices.size();
		vertices.push_back(v);
		return idx;
	}

	void GPUData::addSimpleCubic(u32& firstIdx, u32& lastIdx,
						const glm::vec2& p1, const glm::vec3& tc1,
						const glm::vec2& p2, const glm::vec3& tc2,
						const glm::vec2& p3, const glm::vec3& tc3,
						const glm::vec2& p4, const glm::vec3& tc4)
	{
		Vertex v1, v2, v3, v4;
		//TODO calc normals!!!!
		v1.p  = p1;	v1.tc = tc1;
		v2.p  = p2;	v2.tc = tc2;
		v3.p  = p3;	v3.tc = tc3;
		v4.p  = p4;	v4.tc = tc4;

		if (cubic::calcImplicit(tc2)<0 || cubic::calcImplicit(tc3)<0)
		{
			cubic::changeOrient(v1.tc);
			cubic::changeOrient(v2.tc);
			cubic::changeOrient(v3.tc);
			cubic::changeOrient(v4.tc);
		}
		u32 i1 = addVertex(v1);
		u32 i2 = addVertex(v2);
		u32 i3 = addVertex(v3);
		u32 i4 = addVertex(v4);

		addCubicTri(i1, i2, i3);
		addCubicTri(i3, i4, i1);

		firstIdx = i1;
		lastIdx  = i4;
	}

	void GPUData::addRegTri(u32 i0, u32 i1, u32 i2)
	{
		regIndices.push_back(i0);
		regIndices.push_back(i1);
		regIndices.push_back(i2);
	}

	void GPUData::addArcTri(u32 i0, u32 i1, u32 i2)
	{
		arcIndices.push_back(i0);
		arcIndices.push_back(i1);
		arcIndices.push_back(i2);
	}

	void GPUData::addQuadTri(u32 i0, u32 i1, u32 i2)
	{
		quadIndices.push_back(i0);
		quadIndices.push_back(i1);
		quadIndices.push_back(i2);
	}

	void GPUData::addCubicTri(u32 i0, u32 i1, u32 i2)
	{
		cubicIndices.push_back(i0);
		cubicIndices.push_back(i1);
		cubicIndices.push_back(i2);
	}

	void Rasterize(GPUData& data)
	{
		if (data.vertices.empty())
			return;

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnableClientState(GL_VERTEX_ARRAY);

		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT_AND_BACK);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0, 1);
		glStencilMask(0x01);

		//// Clear stencil
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glBegin(GL_QUADS);
		//glVertex2f(path.mObject->min.x, path.mObject->min.y);
		//glVertex2f(path.mObject->max.x, path.mObject->min.y);
		//glVertex2f(path.mObject->max.x, path.mObject->max.y);
		//glVertex2f(path.mObject->min.x, path.mObject->max.y);
		//glEnd();

		glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
		glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &data.vertices[0].p);

		if (!data.regIndices.empty())
		{
			glUseProgram(0);
			glDrawElements(GL_TRIANGLES, (GLsizei)data.regIndices.size(), GL_UNSIGNED_INT, &data.regIndices[0]);
		}

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (!data.quadIndices.empty())
		{
			glUseProgram(vg::shared::prgMaskQuad);
			glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), &data.vertices[0].tc);
			glDrawElements(GL_TRIANGLES, (GLsizei)data.quadIndices.size(), GL_UNSIGNED_INT, &data.quadIndices[0]);
		}

		if (!data.cubicIndices.empty())
		{
			glUseProgram(vg::shared::prgMaskCubic);
			glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), &data.vertices[0].tc);
			glDrawElements(GL_TRIANGLES, (GLsizei)data.cubicIndices.size(), GL_UNSIGNED_INT, &data.cubicIndices[0]);
		}

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glPopAttrib();
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}