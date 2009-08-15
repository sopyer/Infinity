#ifndef __VERTEXTYPES_H_INCLUDED__
#	define __VERTEXTYPES_H_INCLUDED__

#include <glm/glm.h>

namespace vg
{
	typedef glm::vec2	Vertex;

	struct VertexTex2
	{
		glm::vec2 p;
		glm::vec2 tc;
	};

	struct VertexTex3
	{
		glm::vec2 p;
		glm::vec3 tc;
	};
}

#endif