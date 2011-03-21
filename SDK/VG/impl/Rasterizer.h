#pragma once

#include <glm/glm.h>
#include <vg/openvg.h>
#include <gl/glee.h>

#include "Math.h"
#include <cassert>

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
	
	typedef unsigned short u16;

	struct Bezier3Vertex
	{
		float x, y;
		float k, l, m;
		
		static void construct(Bezier3Vertex* v, const glm::vec2& pos, const glm::vec3& klm)
		{
			v->x = pos.x; v->y = pos.y;
			v->k = klm.x; v->l= klm.y; v->m = klm.z;
		}
	};

	struct Geometry
	{
		float xmin, ymin, xmax, ymax;

		Array<glm::vec2>	shapeVertices;
		Array<u16>			shapeIndices;

		u16	shapeAddVertex(const glm::vec2& v)
		{
			size_t idx = shapeVertices.size();
			shapeVertices.pushBack(v);
			assert(idx<=USHRT_MAX);
			return (u16)idx;
		}

		void shapeAddTri(u16 i0, u16 i1, u16 i2)
		{
			shapeIndices.push_back(i0);
			shapeIndices.push_back(i1);
			shapeIndices.push_back(i2);
		}

		Array<Bezier3Vertex>	bezier3Vertices;
		Array<u16>				bezier3Indices;

		void bezier3AddVertices(glm::vec2 pos[4], glm::vec3 klm[4])
		{
			Bezier3Vertex* v = bezier3Vertices.expand(4);

			Bezier3Vertex::construct(v++, pos[0], klm[0]);
			Bezier3Vertex::construct(v++, pos[1], klm[1]);
			Bezier3Vertex::construct(v++, pos[2], klm[2]);
			Bezier3Vertex::construct(v++, pos[3], klm[3]);
		}
		
		//TODO: change this properly later
		void bezier3GenIndices()
		{
			assert(bezier3Vertices.size()%4 == 0);
			assert(bezier3Vertices.size()<=USHRT_MAX);

			for (size_t i = 0; i<bezier3Vertices.size(); i+=4)
			{
				bezier3Indices.push_back((u16)i+0);
				bezier3Indices.push_back((u16)i+1);
				bezier3Indices.push_back((u16)i+3);

				bezier3Indices.push_back((u16)i+1);
				bezier3Indices.push_back((u16)i+2);
				bezier3Indices.push_back((u16)i+3);
			}
		}
	};

	void buildFillGeometry(Geometry&      geom,
						   const VGint    numSegments,
						   const VGubyte* pathSegments,
						   const VGfloat* pathData);

	template<class VertexType, VGuint PRIM_COUNT>
	struct GeometryOld
	{
		Array<VertexType>	vertices;
		IndexVector			indices;
		
		GLsizei	offset[PRIM_COUNT], count[PRIM_COUNT];
		
		glm::vec2	mMin, mMax;
		
		static const VGuint VertexSize = sizeof(VertexType);

		////////////////////////////////////////////////
		Array<glm::vec2>	shapeVertices;
		Array<u16>			shapeIndices;

		u16	shapeAddVertex(const glm::vec2& v)
		{
			size_t idx = shapeVertices.size();
			shapeVertices.pushBack(v);
			assert(idx<=USHRT_MAX);
			return (u16)idx;
		}

		void shapeAddTri(u16 i0, u16 i1, u16 i2)
		{
			shapeIndices.push_back(i0);
			shapeIndices.push_back(i1);
			shapeIndices.push_back(i2);
		}

		Array<Bezier3Vertex>	bezier3Vertices;
		Array<u16>				bezier3Indices;

		void bezier3AddVertices(glm::vec2 pos[4], glm::vec3 klm[4])
		{
			Bezier3Vertex* v = bezier3Vertices.expand(4);

			Bezier3Vertex::construct(v++, pos[0], klm[0]);
			Bezier3Vertex::construct(v++, pos[1], klm[1]);
			Bezier3Vertex::construct(v++, pos[2], klm[2]);
			Bezier3Vertex::construct(v++, pos[3], klm[3]);
		}
		
		//TODO: change this properly later
		void bezier3GenIndices()
		{
			assert(bezier3Vertices.size()%4 == 0);
			assert(bezier3Vertices.size()<=USHRT_MAX);

			for (size_t i = 0; i<bezier3Vertices.size(); i+=4)
			{
				bezier3Indices.push_back((u16)i+0);
				bezier3Indices.push_back((u16)i+1);
				bezier3Indices.push_back((u16)i+3);

				bezier3Indices.push_back((u16)i+1);
				bezier3Indices.push_back((u16)i+2);
				bezier3Indices.push_back((u16)i+3);
			}
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

	enum StrokePrimitiveTypes
	{
		STROKE_PRIM_TYPE_TRI,
		STROKE_PRIM_TYPE_ARC,
		STROKE_PRIM_TYPE_QUAD,
		STROKE_PRIM_TYPE_CUBIC,
		STROKE_PRIM_TYPE_JOINT_BEVEL,
		STROKE_PRIM_TYPE_JOINT_MITER,
		STROKE_PRIM_TYPE_JOINT_ROUND,
		STROKE_PRIM_TYPE_CAP,
		STROKE_PRIM_TYPE_COUNT,
	};

	struct FillGeometry: public GeometryOld<FillVertex, FILL_PRIM_TYPE_COUNT>
	{
	};

	void rasterizeEvenOddA2C(Geometry& geom);
	void rasterizeNonZeroA2C(Geometry& geom);
	void rasterizeEvenOdd(Geometry& geom);
	void rasterizeEvenOddAA(Geometry& geom);
	void rasterizeNonZero(Geometry& geom);

	void RasterizeEvenOdd(GeometryOld<FillVertex, FILL_PRIM_TYPE_COUNT>& geom);
	void RasterizeEvenOddAA(GeometryOld<FillVertex, FILL_PRIM_TYPE_COUNT>& geom);

	//Safely supports up to 127 self intersections
	void RasterizeNonZero(GeometryOld<FillVertex, FILL_PRIM_TYPE_COUNT>& geom);

	//Strokes for quads and cubics essentially are approximations and not a good ones - they would not work in some cases
	struct StrokeGeometry: public GeometryOld<StrokeVertex, STROKE_PRIM_TYPE_COUNT>
	{
		void RasterizePrimitives(float halfWidth, bool jointMiter = false, bool jointRound = true);

		//Safely supports up to 127 self intersections
		//We rasterize strokes in 2 passes, which behave differently for different stroke primitives
		void Rasterize(float halfWidth=5, bool capRound = false, bool capButt = false);
	};

	void buildFillGeometry(FillGeometry&  geom,
						   const VGint    numSegments,
						   const VGubyte* pathSegments,
						   const VGfloat* pathData);

	void buildStrokeGeometry(const VGint      numSegments,
							 const VGubyte*   pathSegments,
							 const VGfloat*   pathData,
							 StrokeGeometry&  geom);
}
