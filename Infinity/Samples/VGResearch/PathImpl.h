#pragma once

#include <vector>
#include <glm/glm.h>
#include <Framework.h>
#include <vg/openvg.h>
#include "types.h"

namespace vg
{
	extern const int coordsPerCommand[];

	struct PathImpl
	{
		/* Properties */
		int		format;
		float	scale;
		float	bias;
		int		segHint;
		int		dataHint;

		/* Raw data */
		std::vector<byte>	segs;
		std::vector<float>	data;

		/* Subdivision */
		std::vector<glm::vec2>		vertices;
		std::vector<size_t>			subPathes;
		std::vector<glm::vec2>		quads;
		std::vector<glm::vec2>		quadsTC;
		std::vector<glm::vec2>		arcs;
		std::vector<glm::vec2>		arcsTC;
		std::vector<glm::vec2>		cubics;
		std::vector<glm::vec3>		cubicsTC;

		glm::vec2					min, max;
		glm::vec2					surfMin, surfMax;

		PathImpl(float pathScale, float pathBias);

		void preprocess(size_t segOffset, size_t dataOffset);
		void maskPathRegion();
		
		//Path builder data
		glm::vec2	ptO, ptP;
		bool		startNewPath;
		size_t		numPathVertices;

		//Path builder methods
		void startPath(const glm::vec2& start);
		void closePath();

		void moveTo(const glm::vec2& newPos);
		void lineTo(const glm::vec2& point);
		void quadTo(const glm::vec2& p1, const glm::vec2& p2);
		void cubicTo(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
		void arcTo(int segment, float rx, float ry, float angle, const glm::vec2& endPt);

		void addTriangle(std::vector<glm::vec2>& pts, 
						 const glm::vec2& p1,
						 const glm::vec2& p2,
						 const glm::vec2& p3);

		void addPathPoint(const glm::vec2& p);
		void addPathPoint(std::vector<glm::vec2>& pts, const glm::vec2& p);

		void addSpecialTriangle(std::vector<glm::vec2>& pts,
								std::vector<glm::vec2>& tcs,
								const glm::vec2& p1, const glm::vec2& tc1,
								const glm::vec2& p2, const glm::vec2& tc2,
								const glm::vec2& p3, const glm::vec2& tc3);

		void addQuadTriangle(const glm::vec2& p1, const glm::vec2& tc1,
							 const glm::vec2& p2, const glm::vec2& tc2,
							 const glm::vec2& p3, const glm::vec2& tc3)
		{addSpecialTriangle(quads, quadsTC, p1, tc1, p2, tc2, p3, tc3);}

		void addArcTriangle(const glm::vec2& p1, const glm::vec2& tc1,
						    const glm::vec2& p2, const glm::vec2& tc2,
						    const glm::vec2& p3, const glm::vec2& tc3)
		{addSpecialTriangle(arcs, arcsTC, p1, tc1, p2, tc2, p3, tc3);}

		void addCubicTriangle(const glm::vec2& p1, const glm::vec3& tc1,
							  const glm::vec2& p2, const glm::vec3& tc2,
							  const glm::vec2& p3, const glm::vec3& tc3);
	};

	bool isValidCommand(int c);
	size_t coordCountForData(size_t segCount, const ubyte *segs);
}