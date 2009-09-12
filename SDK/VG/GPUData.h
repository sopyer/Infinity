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
			void end();
			
			void line(float x0, float y0, float x1, float y1);
			void quad(float x0, float y0, float x1, float y1, float x2, float y2);
			void cubic(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
			void arc(VGPathSegment type, float rx, float ry, float angle, float x0, float y0, float x1, float y1);

			void append(u32 mode, const GPUData& other);

		public:
			std::vector<Vertex>	vertices;
			std::vector<u32>	regIndices;
			std::vector<u32>	arcIndices;
			std::vector<u32>	quadIndices;
			std::vector<u32>	cubicIndices;
			std::vector<u32>	strokeIndices;

		private:
			u32 addVertex(const Vertex& v);
			u32 addVertices(u32 count, Vertex v[]);

			void addRegTri(u32 i0, u32 i1, u32 i2);
			void addArcTri(u32 i0, u32 i1, u32 i2);
			void addQuadTri(u32 i0, u32 i1, u32 i2);
			void addCubicTri(u32 i0, u32 i1, u32 i2);

			void addSimpleCubic(u32& firstIdx, u32& lastIdx,
								const glm::vec2& p1, const glm::vec3& tc1,
								const glm::vec2& p2, const glm::vec3& tc2,
								const glm::vec2& p3, const glm::vec3& tc3,
								const glm::vec2& p4, const glm::vec3& tc4);

		private:
			u32 mBase;
	};

	void Rasterize(GPUData& data);
	void RasterizeStroke(GPUData& data);
}