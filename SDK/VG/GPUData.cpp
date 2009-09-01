#include <cassert>
#include <gl\glee.h>
#include "GPUData.h"
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

		addTri(mBase, i1, i2);
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

	void cubic(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
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

		std::transform(	other.triIndices.begin(), other.triIndices.end(),
						triIndices.end(),
						AddFunctor((u32)triIndices.size()));

		std::transform(	other.arcIndices.begin(), other.arcIndices.end(),
						arcIndices.end(),
						AddFunctor((u32)arcIndices.size()));

		std::transform(	other.quadIndices.begin(), other.quadIndices.end(),
						quadIndices.end(),
						AddFunctor((u32)quadIndices.size()));

		std::transform(	other.cubicIndices.begin(), other.cubicIndices.end(),
						cubicIndices.end(),
						AddFunctor((u32)triIndices.size()));
	}

	u32 GPUData::addVertex(const Vertex& v)
	{
		u32 idx = (u32)vertices.size();
		vertices.push_back(v);
		return idx;
	}

	void GPUData::addTri(u32 i0, u32 i1, u32 i2)
	{
		triIndices.push_back(i0);
		triIndices.push_back(i1);
		triIndices.push_back(i2);
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

		glUseProgram(0);

		glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &data.vertices[0].p);
		glDrawElements(GL_TRIANGLES, (GLsizei)data.triIndices.size(), GL_UNSIGNED_INT, &data.triIndices[0]);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (!data.quadIndices.empty())
		{
			glUseProgram(vg::shared::prgMaskQuad);
			//glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &data.vertices[0].p);
			glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), &data.vertices[0].tc);
			glDrawElements(GL_TRIANGLES, (GLsizei)data.quadIndices.size(), GL_UNSIGNED_INT, &data.quadIndices[0]);
		}

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glPopAttrib();
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}