#ifndef __VGEXP_H_INCLUDED__
#	define __VGEXP_H_INCLUDED__

#include <VG/impl/Math.h>

struct CubicVertex
{
	glm::vec2	pos;
	glm::vec4	klmn;
};

template<typename VertexType>
struct Geometry
{
	Array<VertexType>		vertices;
	Array<unsigned short>	indices;
};

void initVGExp();
void terminateVGExp();
void clearStencil();
void drawQuad();
void rasterizeEvenOdd(Geometry<glm::vec2>& geom);
void rasterizeEvenOdd(Geometry<CubicVertex>& geom);
void drawCubicAA(Geometry<CubicVertex>& geom);
void addCubic(Geometry<CubicVertex>& cubics, Geometry<glm::vec2>& tri, const Array<glm::vec2>& v);

#endif