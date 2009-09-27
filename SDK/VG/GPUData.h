#pragma once

#include <vector>
#include <glm/glm.h>
#include <vg/openvg.h>
#include <utils.h>
#include ".\types.h"

namespace vg
{
	struct Vertex
	{
		glm::vec2	p;
		glm::vec2	n;
		glm::vec3	tc;

		Vertex(const glm::vec2& _p, const glm::vec2& _n, const glm::vec3& _tc):
			p(_p), n(_n), tc(_tc)
		{}

		Vertex(const glm::vec2& _p, const glm::vec2& _n):
			p(_p), n(_n)
		{}

		Vertex(const glm::vec2& _p, const glm::vec3& _tc):
			p(_p), tc(_tc)
		{}

		Vertex(const glm::vec2& _p): p(_p)
		{}

		Vertex()
		{
		}
	};

	typedef std::vector<u32>	IndexVector;
	
	struct Region
	{
		std::vector<u32>	indTri[2];
		std::vector<u32>	indArc[2];
		std::vector<u32>	indQuad[2];
		std::vector<u32>	indCubic[2];
	};

#define DECLARE_PRIM_TYPE(name)	PT_##name,							\
								PT_##name##_CW  = PT_##name,		\
								PT_##name##_CCW

	enum PrimitiveTypes
	{
		DECLARE_PRIM_TYPE(FILL_TRI),
		DECLARE_PRIM_TYPE(FILL_ARC),
		DECLARE_PRIM_TYPE(FILL_QUAD),
		DECLARE_PRIM_TYPE(FILL_CUBIC),
		DECLARE_PRIM_TYPE(STROKE_TRI),
		DECLARE_PRIM_TYPE(STROKE_ARC),
		DECLARE_PRIM_TYPE(STROKE_QUAD),
		DECLARE_PRIM_TYPE(STROKE_CUBIC),
		DECLARE_PRIM_TYPE(JOINT_BEVEL),
		DECLARE_PRIM_TYPE(JOINT_MITER),
		DECLARE_PRIM_TYPE(JOINT_ROUND),
		DECLARE_PRIM_TYPE(CAP_ROUND),
		DECLARE_PRIM_TYPE(CAP_BEVEL),
		PT_COUNT,
	};

	struct GData
	{
		std::vector<Vertex>	vertices;
		IndexVector		indices[PT_COUNT];
		
		GLsizei	offset[PT_COUNT], count[PT_COUNT];

		void drawElements(PrimitiveTypes prim) const
		{
			if (count[prim])
				glDrawElements(GL_TRIANGLES, count[prim], GL_UNSIGNED_INT, &indices[prim][0]);
		}
	};

	//Uses fixed vertex layout
	//We share first, last non-displaced vertices
	//Order of vertices and indices is very important
	struct GPUData
	{
		public:
			enum
			{
				REGION = 1,	//(1<<0)
				STROKE = 2,	//(1<<1)
				ALL = REGION|STROKE,
			};

		public:
			GPUData();

			void begin(float x, float y);
			void end(bool closePath = false);
			
			void lineTo(float x1, float y1);
			void quadTo(float x1, float y1, float x2, float y2);
			void cubic(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
			void arc(VGPathSegment type, float rx, float ry, float angle, float x0, float y0, float x1, float y1);

		public:
			GData	data;

		private:
			u32 addVertex(const Vertex& v);
			u32 addVertices(u32 count, Vertex v[]);
			
			const glm::vec2& getCursor()
			{return data.vertices[mCursor].p;}

			bool isFirstPrim(u32 primVtx)
			{return data.vertices.size()-primVtx<2;}
			
			
			void addStroke(u32 first, u32 last);
			void addJointBevel(u32 end, u32 start);
			void addPrim(PrimitiveTypes prim, u32 i0, u32 i1, u32 i2, bool displaced=false);
			void addSimpleCubic(u32& firstIdx, u32& lastIdx,
								const glm::vec2& p1, const glm::vec3& tc1,
								const glm::vec2& p2, const glm::vec3& tc2,
								const glm::vec2& p3, const glm::vec3& tc3,
								const glm::vec2& p4, const glm::vec3& tc4);

		private:
			u32 mBase, mCursor;
	};

	void RasterizeFillNonZero(const GData& data);
	void RasterizeFillEvenOdd(const GData& data);
	void RasterizeStroke(const GData& data);
}
