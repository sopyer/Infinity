#pragma once

#include <glm/glm.h>
#include <vg/openvg.h>
#include <gl/glee.h>

#include "Math.h"

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
	
	template<class VertexType, VGuint PRIM_COUNT>
	struct Geometry
	{
		Array<VertexType>	vertices;
		IndexVector			indices;
		
		GLsizei	offset[PRIM_COUNT], count[PRIM_COUNT];
		
		glm::vec2	mMin, mMax;
		
		static const VGuint VertexSize = sizeof(VertexType);

		void drawElements(VGuint primType) const
		{
			assert(primType<PRIM_COUNT);
			if (count[primType])
				glDrawElements(GL_TRIANGLES, count[primType], GL_UNSIGNED_INT, &indices[0]+offset[primType]);
		}

		void drawElementsNZ(VGuint primType)
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glCullFace(GL_FRONT);
			drawElements(primType);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glCullFace(GL_BACK);
			drawElements(primType);
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

	struct FillGeometry: public Geometry<FillVertex, FILL_PRIM_TYPE_COUNT>
	{
		void RasterizeEvenOdd(VGuint prims);

		//Safetly supports up to 127 self intersections
		void RasterizeNonZero();
	};

	//Strokes for quads and cubics essentially are approximations and not a good ones - they would not work in some cases
	struct StrokeGeometry: public Geometry<StrokeVertex, STROKE_PRIM_TYPE_COUNT>
	{
		void RasterizePrimitives(float halfWidth, bool jointMiter = false, bool jointRound = true);

		//Safely supports up to 127 self intersections
		//We rasterize strokes in 2 passes, which behave differently for different stroke primitives
		void Rasterize(float halfWidth=5, bool capRound = false, bool capButt = false);
	};

	void buildFillGeometry(const VGint    numSegments,
						   const VGubyte* pathSegments,
						   const VGfloat* pathData,
						   FillGeometry&  geom);

	void buildStrokeGeometry(const VGint      numSegments,
							 const VGubyte*   pathSegments,
							 const VGfloat*   pathData,
							 StrokeGeometry&  geom);
}
