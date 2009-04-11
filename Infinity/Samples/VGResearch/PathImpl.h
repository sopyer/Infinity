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

		glm::vec2					min, max;
		glm::vec2					surfMin, surfMax;

		PathImpl(float pathScale, float pathBias);

		void preprocess(size_t segOffset, size_t dataOffset);
		void maskPathRegion();
	};

	bool isValidCommand(int c);
	size_t coordCountForData(size_t segCount, const ubyte *segs);
}