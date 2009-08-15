#pragma once

#include <vector>
#include <glm/glm.h>
#include <Framework.h>
#include <vg/openvg.h>
#include ".\types.h"
#include "VertexTypes.h"

namespace vg
{
	extern const int coordsPerCommand[];

	//!!!! FIX MIN,MAX !!!!//
	struct PathObject
	{
		/* Properties */
		int		format;
		float	scale;
		float	bias;
		int		segHint;
		int		dataHint;

		/* Raw data */
		std::vector<u8>		segs;
		std::vector<float>	data;

		/* Rendering Data */
		std::vector<Vertex>		geomArea;
		std::vector<GLuint>		idxArea;
		std::vector<VertexTex2>	geomArcs;
		std::vector<VertexTex2>	geomQuads;
		std::vector<VertexTex3>	geomCubics;

		//area geometry of current segment
		std::vector<Vertex>		geomSegArea;
		std::vector<GLuint>		idxSegArea;
		

		std::vector<glm::vec2>		vertices;
		std::vector<GLsizei>		subPathes;
		std::vector<glm::vec2>		quads;
		std::vector<glm::vec2>		quadsTC;
		std::vector<glm::vec2>		arcs;
		std::vector<glm::vec2>		arcsTC;
		std::vector<glm::vec2>		cubics;
		std::vector<glm::vec3>		cubicsTC;

		glm::vec2					min, max;
		glm::vec2					surfMin, surfMax;

		PathObject(float pathScale, float pathBias);

		void preprocess(size_t segOffset, size_t dataOffset);
		
		//Path builder data
		glm::vec2	ptO;
		bool		startNewPath;
		GLsizei		numPathVertices;

		void lineTo(const glm::vec2& point);
		void quadTo(const glm::vec2& p1, const glm::vec2& p2);
		void cubicTo(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
		void arcTo(int segment, float rx, float ry, float angle, const glm::vec2& endPt);

		//Helper methods for path building
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
	
		/**********************************************************/
		/*	Simple is cubic curve such that implicit equation at
		/*	control points p2 and p3 has the same sign
		/**********************************************************/
		template<bool reverse>
		void addSimpleCubic(const glm::vec2& p1, const glm::vec3& tc1,
							const glm::vec2& p2, const glm::vec3& tc2,
							const glm::vec2& p3, const glm::vec3& tc3,
							const glm::vec2& p4, const glm::vec3& tc4);

		void startSegment(float x, float y);
		void endSegment(bool closePath);

		void lineTo(float x, float y);
		void quadTo(float x1, float y1, float x2, float y2);
		void cubicTo(float x1, float y1, float x2, float y2, float x3, float y3);
		void arcTo(u8 segment, float rx, float ry, float angle, float xe, float ye);

		void clear();

		void addSegGeom(GLuint num, glm::vec2 pts[]);
		void recalcSegmentBBox();
		void recalcBBox();
	};

	bool isValidCommand(int c);
	size_t coordCountForData(size_t segCount, const ubyte *segs);
}