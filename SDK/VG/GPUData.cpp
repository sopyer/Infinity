#include <cassert>
#include <gl\glee.h>
#include "GPUData.h"
#include "Cubic.h"
#include "SharedResources.h"

namespace vg
{
	enum
	{
		CW  = 0,
		CCW = 1
	};

	GPUData::GPUData(): mBase(-1)
	{}

	void GPUData::begin(float x, float y)
	{
		assert(mBase==-1);
		Vertex v(glm::vec2(x, y), glm::vec2());
		mCursor = mBase = addVertex(v);
	}

	void GPUData::end(bool closePath)
	{
		if (closePath)
		{
			//TODO: do not store fill????
			lineTo(data.vertices[mBase].p.x, data.vertices[mBase].p.y);
			addJointBevel(mCursor-2, mBase+1);

			data.offsetFillTri[CCW] = 0;
			data.countFillTri[CCW] = (GLsizei)data.fill.indTri[CCW].size();
			data.offsetFillTri[CW] = 0;
			data.countFillTri[CW] = (GLsizei)data.fill.indTri[CW].size();
			data.offsetFillQuad[CCW] = 0;
			data.countFillQuad[CCW] = (GLsizei)data.fill.indQuad[CCW].size();
			data.offsetFillQuad[CW] = 0;
			data.countFillQuad[CW] = (GLsizei)data.fill.indQuad[CW].size();
		}

		data.offsetStrokeTri[CCW] = 0;
		data.countStrokeTri[CCW] = (GLsizei)data.stroke.indTri[CCW].size();
		data.offsetStrokeTri[CW] = 0;
		data.countStrokeTri[CW] = (GLsizei)data.stroke.indTri[CW].size();
		data.offsetStrokeQuad[CCW] = 0;
		data.countStrokeQuad[CCW] = (GLsizei)data.stroke.indQuad[CCW].size();
		data.offsetStrokeQuad[CW] = 0;
		data.countStrokeQuad[CW] = (GLsizei)data.stroke.indQuad[CW].size();
		data.offsetJointBevel[CCW] = 0;
		data.countJointBevel[CCW] = (GLsizei)data.indJointBevel[CCW].size();
		data.offsetJointBevel[CW] = 0;
		data.countJointBevel[CW] = (GLsizei)data.indJointBevel[CW].size();

		mBase=-1;
	}

	glm::vec2 rotate90CW(glm::vec2 vec)
	{
		return glm::vec2(vec.y, -vec.x);
	}

	glm::vec2 rotate90CCW(glm::vec2 vec)
	{
		return glm::vec2(-vec.y, vec.x);
	}
	
	glm::vec2 makeNormal(glm::vec2 p1, glm::vec2 p2)
	{
		return glm::normalize(rotate90CCW(p2-p1));
	}

	void GPUData::lineTo(float x1, float y1)
	{
		glm::vec2	p0(getCursor()), p1(x1, y1), n(makeNormal(p0, p1)), zero;

		Vertex v[] = {
			Vertex(p0, n), Vertex(p0, -n),
			Vertex(p1, n), Vertex(p1, -n),
			Vertex(p1, zero)
		};
		
		u32 startIdx  = addVertices(ELEMENT_COUNT(v), v);
		u32 end       = startIdx+4;

		//addTri<false>(data.fill, mBase, mCursor, end);
		addPrim(data.fill.indTri, mBase, mCursor, end);
		addStroke(startIdx, startIdx+2);
		if (!isFirstPrim(ELEMENT_COUNT(v)))
			addJointBevel(mCursor-2, startIdx);

		mCursor = end;
	}

	void GPUData::quadTo(float x1, float y1, float x2, float y2)
	{
		glm::vec2	p0(getCursor()), p1(x1, y1), p2(x2, y2);
		glm::vec2	n01(makeNormal(p0, p1)), n12(makeNormal(p1, p2));
		float		det(n01.y*n12.x-n01.x*n12.y);
		glm::vec2	zero, offset(det==0?zero:makeNormal(n01, n12)/det);

		Vertex v[] = {
			Vertex(p0, n01,    glm::vec3(0.0f, 0.0f, 0.0f)),	Vertex(p0, -n01,    glm::vec3(0.0f, 0.0f, 0.0f)),
			Vertex(p1, offset, glm::vec3(0.5f, 0.0f, 0.0f)),	Vertex(p1, -offset, glm::vec3(0.5f, 0.0f, 0.0f)),
			Vertex(p2, n12,    glm::vec3(1.0f, 1.0f, 0.0f)),	Vertex(p2, -n12,    glm::vec3(1.0f, 1.0f, 0.0f)),
			Vertex(p2, zero)
		};

		u32 startIdx  = addVertices(ELEMENT_COUNT(v), v);
		u32 end       = startIdx+6;

		//addTri<false>(data.fill, mBase, startIdx, startIdx+4);
		//addQuad(data.fill, startIdx, startIdx+2, startIdx+4);
		addPrim(data.fill.indTri, mBase, startIdx, startIdx+4);
		addPrim(data.fill.indQuad, startIdx, startIdx+2, startIdx+4);

		//addQuad(data.stroke, startIdx, startIdx+2, startIdx+4);
		//addQuad(data.stroke, startIdx+5, startIdx+3, startIdx+1);
		addPrim(data.stroke.indQuad, startIdx, startIdx+2, startIdx+4);
		addPrim(data.stroke.indQuad, startIdx+5, startIdx+3, startIdx+1);
		addStroke(startIdx, startIdx+4);
		if (!isFirstPrim(ELEMENT_COUNT(v)))
			addJointBevel(mCursor-2, startIdx);

		mCursor = end;
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
			{
				//addTri<false>(data.fill, i0, i1, i2);
				addPrim(data.fill.indTri, i0, i1, i2);
			}
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
		{
			//addTri<false>(data.fill, i0, i1, i2);
			addPrim(data.fill.indTri, i0, i1, i2);
		}
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
		//vertices.insert(vertices.end(), other.vertices.begin(), other.vertices.end());

		//std::transform(	other.regIndices.begin(), other.regIndices.end(),
		//				regIndices.end(),
		//				AddFunctor((u32)regIndices.size()));

		//std::transform(	other.arcIndices.begin(), other.arcIndices.end(),
		//				arcIndices.end(),
		//				AddFunctor((u32)arcIndices.size()));

		//std::transform(	other.quadIndices.begin(), other.quadIndices.end(),
		//				quadIndices.end(),
		//				AddFunctor((u32)quadIndices.size()));

		//std::transform(	other.cubicIndices.begin(), other.cubicIndices.end(),
		//				cubicIndices.end(),
		//				AddFunctor((u32)cubicIndices.size()));
	}

	u32 GPUData::addVertex(const Vertex& v)
	{
		u32 idx = (u32)data.vertices.size();
		data.vertices.push_back(v);
		return idx;
	}

	u32 GPUData::addVertices(u32 count, Vertex v[])
	{
		u32 idx = (u32)data.vertices.size();
		data.vertices.insert(data.vertices.end(), v, v+count);
		return idx;
	}

	void GPUData::addStroke(u32 start, u32 end)
	{
		//Order of indices is very important!!!
		addPrim(data.stroke.indTri, start, end,     end+1, true);
		addPrim(data.stroke.indTri, end+1, start+1, start, true);
	}

	void GPUData::addJointBevel(u32 end, u32 start)
	{
		glm::vec2	v1 = data.vertices[end].n,
					v2 = data.vertices[start].n;
		
		float		det = v1.x*v2.y - v1.y*v2.x;
		
		if (det<0)
			addPrim(data.indJointBevel, end,   start,   end+2, true);
		else
			addPrim(data.indJointBevel, end+1, start+1, end+2, true);
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

		//addCubic(data.fill, i1, i2, i3);
		//addCubic(data.fill, i3, i4, i1);
		addPrim(data.fill.indCubic, i1, i2, i3);
		addPrim(data.fill.indCubic, i3, i4, i1);

		firstIdx = i1;
		lastIdx  = i4;
	}

	//template<>
	//float GPUData::calcTriOrient<false>(u32 i0, u32 i1, u32 i2)
	//{
	//	glm::vec2	v1 = data.vertices[i1].p-data.vertices[i0].p,
	//				v2 = data.vertices[i2].p-data.vertices[i0].p;
	//	
	//	float		det = v1.x*v2.y - v1.y*v2.x;

	//	return det;
	//}

	//template<>
	//float GPUData::calcTriOrient<true>(u32 i0, u32 i1, u32 i2)
	//{
	//	glm::vec2	v1 = (data.vertices[i1].p+data.vertices[i1].n)-(data.vertices[i0].p+data.vertices[i0].n),
	//				v2 = (data.vertices[i2].p+data.vertices[i2].n)-(data.vertices[i0].p+data.vertices[i0].n);
	//	
	//	float		det = v1.x*v2.y - v1.y*v2.x;

	//	return det;
	//}

	void GPUData::addPrim(IndexVector idx[2], u32 i0, u32 i1, u32 i2, bool displaced)
	{
		glm::vec2	v1, v2;
		Vertex*		vtx = &data.vertices[0];

		if (!displaced)
		{
			v1 = vtx[i1].p-vtx[i0].p;
			v2 = vtx[i2].p-vtx[i0].p;
		}
		else
		{
			v1 = (vtx[i1].p+vtx[i1].n)-(vtx[i0].p+vtx[i0].n);
			v2 = (vtx[i2].p+vtx[i2].n)-(vtx[i0].p+vtx[i0].n);
		}
		
		float		det = v1.x*v2.y - v1.y*v2.x;

		//float det = calcTriOrient<displaced>(i0, i1, i2);

		if (det==0)
			return;

		std::vector<u32>& indices = idx[det>0?CCW:CW];

		indices.push_back(i0);
		indices.push_back(i1);
		indices.push_back(i2);
	}

	//template<bool displaced>
	//void GPUData::addTri(Region& reg, u32 i0, u32 i1, u32 i2)
	//{
	//	float det = calcTriOrient<displaced>(i0, i1, i2);

	//	if (det==0)
	//		return;

	//	std::vector<u32>& indices = reg.indTri[det>0?CCW:CW];

	//	indices.push_back(i0);
	//	indices.push_back(i1);
	//	indices.push_back(i2);
	//}

	//void GPUData::addQuad(Region& reg, u32 i0, u32 i1, u32 i2)
	//{
	//	float det = calcTriOrient<false>(i0, i1, i2);

	//	if (det==0)
	//		return;

	//	std::vector<u32>& indices = reg.indQuad[det>0?CCW:CW];

	//	indices.push_back(i0);
	//	indices.push_back(i1);
	//	indices.push_back(i2);
	//}

	//void GPUData::addArc(Region& reg, u32 i0, u32 i1, u32 i2)
	//{
	//	float det = calcTriOrient<false>(i0, i1, i2);

	//	if (det==0)
	//		return;

	//	std::vector<u32>& indices = reg.indArc[det>0?CCW:CW];

	//	indices.push_back(i0);
	//	indices.push_back(i1);
	//	indices.push_back(i2);
	//}

	//void GPUData::addCubic(Region& reg, u32 i0, u32 i1, u32 i2)
	//{
	//	float det = calcTriOrient<false>(i0, i1, i2);

	//	if (det==0)
	//		return;

	//	std::vector<u32>& indices = reg.indCubic[det>0?CCW:CW];

	//	indices.push_back(i0);
	//	indices.push_back(i1);
	//	indices.push_back(i2);
	//}

	void RasterizeFillEvenOdd(const GData& data)
	{
		if (data.vertices.empty())
			return;

		const Vertex& vtx = data.vertices[0];

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
		glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &vtx.p);

		glUseProgram(0);
		if (data.countFillTri[CCW])
			glDrawElements(GL_TRIANGLES, data.countFillTri[CCW], GL_UNSIGNED_INT, &data.fill.indTri[CCW][0]);
		if (data.countFillTri[CW])
			glDrawElements(GL_TRIANGLES, data.countFillTri[CW], GL_UNSIGNED_INT, &data.fill.indTri[CW][0]);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), &vtx.tc);

		glUseProgram(vg::shared::prgMaskQuad);
		if (data.countFillQuad[CCW])
			glDrawElements(GL_TRIANGLES, data.countFillQuad[CCW], GL_UNSIGNED_INT, &data.fill.indQuad[CCW][0]);
		if (data.countFillQuad[CW])
			glDrawElements(GL_TRIANGLES, data.countFillQuad[CW], GL_UNSIGNED_INT, &data.fill.indQuad[CW][0]);

		//if (!data.cubicIndices.empty())
		//{
		//	glUseProgram(vg::shared::prgMaskCubic);
		//	glDrawElements(GL_TRIANGLES, (GLsizei)data.cubicIndices.size(), GL_UNSIGNED_INT, &data.cubicIndices[0]);
		//}

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		
		glPopAttrib();
	}

	void RasterizeFillNonZero(const GData& data)
	{
		if (data.vertices.empty())
			return;

		//// Clear stencil
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glBegin(GL_QUADS);
		//glVertex2f(path.mObject->min.x, path.mObject->min.y);
		//glVertex2f(path.mObject->max.x, path.mObject->min.y);
		//glVertex2f(path.mObject->max.x, path.mObject->max.y);
		//glVertex2f(path.mObject->min.x, path.mObject->max.y);
		//glEnd();

		const Vertex& vtx = data.vertices[0];

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT_AND_BACK);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
		glStencilMask(0xFF);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &vtx.p);

		glUseProgram(0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		if (data.countFillTri[CCW])
			glDrawElements(GL_TRIANGLES, data.countFillTri[CCW], GL_UNSIGNED_INT, &data.fill.indTri[CCW][0]);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		if (data.countFillTri[CW])
			glDrawElements(GL_TRIANGLES, data.countFillTri[CW], GL_UNSIGNED_INT, &data.fill.indTri[CW][0]);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), &vtx.tc);

		glUseProgram(vg::shared::prgMaskQuad);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		if (data.countFillQuad[CCW])
			glDrawElements(GL_TRIANGLES, data.countFillQuad[CCW], GL_UNSIGNED_INT, &data.fill.indQuad[CCW][0]);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		if (data.countFillQuad[CW])
			glDrawElements(GL_TRIANGLES, data.countFillQuad[CW], GL_UNSIGNED_INT, &data.fill.indQuad[CW][0]);

		//if (!data.cubicIndices.empty())
		//{
		//	glUseProgram(vg::shared::prgMaskCubic);
		//	glDrawElements(GL_TRIANGLES, (GLsizei)data.cubicIndices.size(), GL_UNSIGNED_INT, &data.cubicIndices[0]);
		//}

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glPopAttrib();
	}

	void RasterizeStroke(const GData& data)
	{
		if (data.vertices.empty())
			return;

		const Vertex& vtx = data.vertices[0];

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
		glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
		glStencilMask(0xFF);

		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

 		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &vtx.p);

		glUseProgram(shared::prgMaskStrokeSeg);
		glEnableVertexAttribArray(shared::locOffsetAttrib);
		glVertexAttribPointer(shared::locOffsetAttrib, 2, GL_FLOAT, false, sizeof(Vertex), &vtx.n);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		if (data.countStrokeTri[CCW])
			glDrawElements(GL_TRIANGLES, data.countStrokeTri[CCW], GL_UNSIGNED_INT, &data.stroke.indTri[CCW][0]);
		if (data.countJointBevel[CCW])
			glDrawElements(GL_TRIANGLES, data.countJointBevel[CCW], GL_UNSIGNED_INT, &data.indJointBevel[CCW][0]);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		if (data.countStrokeTri[CW])
			glDrawElements(GL_TRIANGLES, data.countStrokeTri[CW], GL_UNSIGNED_INT, &data.stroke.indTri[CW][0]);
		if (data.countJointBevel[CW])
			glDrawElements(GL_TRIANGLES, data.countJointBevel[CW], GL_UNSIGNED_INT, &data.indJointBevel[CW][0]);
		glDisableVertexAttribArray(shared::locOffsetAttrib);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), &vtx.tc);

		glUseProgram(vg::shared::prgStrokeMaskQuad);
		glEnableVertexAttribArray(shared::locOffsetAttribQuad);
		glVertexAttribPointer(shared::locOffsetAttribQuad, 2, GL_FLOAT, false, sizeof(Vertex), &vtx.n);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		if (data.countStrokeQuad[CCW])
			glDrawElements(GL_TRIANGLES, data.countStrokeQuad[CCW], GL_UNSIGNED_INT, &data.stroke.indQuad[CCW][0]);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		if (data.countStrokeQuad[CW])
			glDrawElements(GL_TRIANGLES, data.countStrokeQuad[CW], GL_UNSIGNED_INT, &data.stroke.indQuad[CW][0]);
		glDisableVertexAttribArray(shared::locOffsetAttribQuad);

		//if (!data.cubicIndices.empty())
		//{
		//	glUseProgram(vg::shared::prgMaskCubic);
		//	glDrawElements(GL_TRIANGLES, (GLsizei)data.cubicIndices.size(), GL_UNSIGNED_INT, &data.cubicIndices[0]);
		//}

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glPopAttrib();
	}
}