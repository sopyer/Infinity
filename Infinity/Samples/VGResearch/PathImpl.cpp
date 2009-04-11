#include "PathImpl.h"

#pragma once

namespace vg
{
	ProgramRef	maskQuad;
	ProgramRef	maskArc;

	void init(ProgramRef maskQuad, ProgramRef maskArc)
	{
		vg::maskQuad = maskQuad;
		vg::maskArc = maskArc;
	}

	void deinit()
	{
		vg::maskQuad = 0;
		vg::maskArc = 0;
	}
}

namespace vg
{
	const int coordsPerCommand[] =
	{
	  0, /* VG_CLOSE_PATH */
	  2, /* VG_MOTE_TO */
	  2, /* VG_LINE_TO */
	  1, /* VG_HLINE_TO */
	  1, /* VG_VLINE_TO */
	  4, /* VG_QUAD_TO */
	  6, /* VG_CUBIC_TO */
	  2, /* VG_SQUAD_TO */
	  4, /* VG_SCUBIC_TO */
	  5, /* VG_SCCWARC_TO */
	  5, /* VG_SCWARC_TO */
	  5, /* VG_LCCWARC_TO */
	  5  /* VG_LCWARC_TO */
	};

	PathImpl::PathImpl(float pathScale, float pathBias):
		scale(pathScale), bias(pathBias)
	{}
	
	void PathImpl::preprocess(size_t segOffset, size_t dataOffset)
	{
		struct dataFeeder
		{
			dataFeeder(std::vector<float>& source, size_t offset):
				dataSource(source),
				cursor(offset)
			{}

			glm::vec2	getPoint()
			{
				size_t	idx = cursor;
				cursor += 2;
				return glm::vec2(dataSource[idx], dataSource[idx+1]);
			}

			float		getFloat()
			{
				return dataSource[cursor++];
			}

			std::vector<float>&	dataSource;
			size_t				cursor;
		};

		glm::vec2	cursor;
		bool		startNewPath = true;
		size_t		subPathVertCount = 0;
		//size_t	data_idx = dataOffset;
		dataFeeder	dataSource(data, dataOffset);

		for (size_t s=segOffset; s<segs.size(); ++s)
		{
			int		segment = segs[s]&0x1E;
			int		segidx = segment>>1;
			bool	relative = segs[s]&1;
			glm::vec2	pt;
			
			if (segment==VG_CLOSE_PATH && subPathVertCount>0)
			{
				subPathes.push_back(subPathVertCount);
				startNewPath = true;
				subPathVertCount = 0;
			}
			else if (segment==VG_MOVE_TO)
			{
				if (subPathVertCount)
					subPathes.push_back(subPathVertCount);

				startNewPath = true;
				
				glm::vec2 newPos(dataSource.getPoint());
				
				if (relative)
					cursor += newPos;
				else
					cursor = newPos;
			}
			else
			{
				if (startNewPath)
				{
					startNewPath = false;
					vertices.push_back(cursor);
					subPathVertCount += 1;
				}
				
				glm::vec2	newPos;
				float		coord;

				switch (segment)
				{
					case VG_LINE_TO:
						newPos = dataSource.getPoint();
						
						if (relative)
							cursor += newPos;
						else
							cursor = newPos;

						vertices.push_back(cursor);
						subPathVertCount += 1;

						break;

					case VG_HLINE_TO:
						coord = dataSource.getFloat();
						
						if (relative)
							cursor.x += coord;
						else
							cursor.x = coord;

						vertices.push_back(cursor);
						subPathVertCount += 1;
						break;

					case VG_VLINE_TO:
						coord = dataSource.getFloat();
						
						if (relative)
							cursor.y += coord;
						else
							cursor.y = coord;

						vertices.push_back(cursor);
						subPathVertCount += 1;
						break;

						//use external part of triangle
						//all features will be additions

					case VG_QUAD_TO:
						quads.push_back(cursor);
						quadsTC.push_back(glm::vec2(0,0));
						newPos = dataSource.getPoint();
						newPos = relative?cursor+newPos:newPos;
						quadsTC.push_back(glm::vec2(0.5f,0));
						quads.push_back(newPos);
						min = glm::min(min, newPos);
						max = glm::max(max, newPos);

						newPos = dataSource.getPoint();
						newPos = relative?cursor+newPos:newPos;
						vertices.push_back(newPos);
						quads.push_back(newPos);
						quadsTC.push_back(glm::vec2(1,1));
						
						if (relative)
							cursor += newPos;
						else
							cursor = newPos;

						subPathVertCount += 2;
						break;
					//case VG_SQUAD_TO:
					//	assert(0);
					//	break;
					//case VG_SCUBIC_TO:
					//	break;
					case VG_SCWARC_TO:
					case VG_SCCWARC_TO:
					case VG_LCWARC_TO:
					case VG_LCCWARC_TO:
						{
							bool CCW = segment==VG_SCCWARC_TO || segment==VG_LCCWARC_TO;
							bool smallArc = segment==VG_SCCWARC_TO || segment==VG_SCWARC_TO;

							float		rx = dataSource.getFloat();
							float		ry = dataSource.getFloat();
							float		angle = dataSource.getFloat();
							glm::vec2	endPt = dataSource.getPoint();

							//handle etraordinary(near zero) cases

							//rotate by -angle
							glm::vec2 p1 = glm::rotateGTX(cursor, -angle);
							glm::vec2 p2 = glm::rotateGTX(relative?endPt+cursor:endPt, -angle);

							//scale
							p1 *= glm::vec2(1/rx, 1/ry);
							p2 *= glm::vec2(1/rx, 1/ry);

							glm::vec2 middle = (p2+p1)/2.0f;
							glm::vec2 ox = glm::normalize(middle);
							glm::vec2 oy = CCW ? glm::vec2(-ox.y, ox.x) : glm::vec2(ox.y, -ox.x);

							float a = glm::length(middle - p1);
							float b = sqrt(1-a*a);
							
							glm::vec2	center = middle + oy*b;

							//float scale1 = rx-a;
							//float scale2 = rx-b;

							//standart path internal side of shape
						
							//!!!FIX THIS
							if (smallArc)
							{
								glm::vec2 up = (1-b)*(-oy);
								
								glm::vec2 pt1 = p1+up;
								glm::vec2 pt2 = middle+up;
								glm::vec2 pt3 = p2+up;

								arcsTC.push_back(p1-center);
								arcsTC.push_back(pt1-center);
								arcsTC.push_back(pt2-center);
								
								arcs.push_back(glm::rotateGTX(p1*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(pt1*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(pt2*glm::vec2(rx, ry), angle));

								arcsTC.push_back(pt2-center);
								arcsTC.push_back(pt3-center);
								arcsTC.push_back(p2-center);
								
								arcs.push_back(glm::rotateGTX(pt2*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(pt3*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(p2*glm::vec2(rx, ry), angle));

								vertices.push_back(glm::rotateGTX(pt2*glm::vec2(rx, ry), angle));
								min = glm::min(min, vertices.back());
								max = glm::max(max, vertices.back());
								subPathVertCount += 1;
							}
							else
							{
								glm::vec2 side = (1-a)*(-ox);
								
								glm::vec2 pt1 = p1+side;
								glm::vec2 pt2 = center-ox;
								glm::vec2 pt3 = center-oy-oy;
								glm::vec2 pt4 = center-oy;
								glm::vec2 pt5 = center-oy+ox;
								glm::vec2 pt6 = center-ox;
								glm::vec2 pt7 = p2-side;

								arcsTC.push_back(p1-center);
								arcsTC.push_back(pt1-center);
								arcsTC.push_back(pt2-center);
								
								arcs.push_back(glm::rotateGTX(p1*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(pt1*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(pt2*glm::vec2(rx, ry), angle));

								arcsTC.push_back(pt2-center);
								arcsTC.push_back(pt3-center);
								arcsTC.push_back(pt4-center);
								
								arcs.push_back(glm::rotateGTX(pt2*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(pt3*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(pt4*glm::vec2(rx, ry), angle));

								arcsTC.push_back(pt4-center);
								arcsTC.push_back(pt5-center);
								arcsTC.push_back(pt6-center);
								
								arcs.push_back(glm::rotateGTX(pt4*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(pt5*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(pt6*glm::vec2(rx, ry), angle));

								arcsTC.push_back(pt6-center);
								arcsTC.push_back(pt7-center);
								arcsTC.push_back(p2-center);
								
								arcs.push_back(glm::rotateGTX(pt6*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(pt7*glm::vec2(rx, ry), angle));
								arcs.push_back(glm::rotateGTX(p2*glm::vec2(rx, ry), angle));

								vertices.push_back(glm::rotateGTX(pt2*glm::vec2(rx, ry), angle));
								min = glm::min(min, vertices.back());
								max = glm::max(max, vertices.back());
								vertices.push_back(glm::rotateGTX(pt4*glm::vec2(rx, ry), angle));
								min = glm::min(min, vertices.back());
								max = glm::max(max, vertices.back());
								vertices.push_back(glm::rotateGTX(pt6*glm::vec2(rx, ry), angle));
								min = glm::min(min, vertices.back());
								max = glm::max(max, vertices.back());
								subPathVertCount += 3;
							}
							vertices.push_back(endPt);
							cursor = endPt;
							subPathVertCount += 1;
						}
						break;

					default:
						assert(0);
				}
			}
			
			min = glm::min(min, cursor);
			max = glm::max(max, cursor);
			//data_idx += coordsPerCommand[segidx];
		}
	}

	void PathImpl::maskPathRegion()
	{
		if (vertices.empty())
			return;

		glEnableClientState(GL_VERTEX_ARRAY);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
		glStencilFunc(GL_ALWAYS, 0, 1);
		glStencilMask(0x01);
		
		glCullFace(GL_FRONT_AND_BACK);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		glUseProgram(0);

		size_t base = 0;
		glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &vertices[0]);
		for(size_t i=0; i<subPathes.size(); ++i)
		{
			size_t numPoints = subPathes[i];
			glDrawArrays(GL_TRIANGLE_FAN, base, numPoints);
			base += numPoints;
		}

		if (!quads.empty())
		{
			glUseProgram(*maskQuad);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &quads[0]);
			glTexCoordPointer(2, GL_FLOAT, 2*sizeof(float), &quadsTC[0]);
			glDrawArrays(GL_TRIANGLES, 0, quads.size());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		if (!arcs.empty())
		{
			glUseProgram(*maskArc);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &arcs[0]);
			glTexCoordPointer(2, GL_FLOAT, 2*sizeof(float), &arcsTC[0]);
			glDrawArrays(GL_TRIANGLES, 0, arcs.size());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glPopAttrib();
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	//void drawPath()
	//{
	//	glColor3f(1, 1, 1);
	//	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//	glEnableClientState(GL_VERTEX_ARRAY);
	//	glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &segData[0]);

	//	glEnable(GL_STENCIL_TEST);
	//	//glDepthFunc(GL_LEQUAL); //Should be set!!!!!!
	//	
	//	glUseProgram(0);
	//	glDisable(GL_DEPTH_TEST);
	//	glDepthMask(GL_FALSE);
	//	glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
	//	glStencilFunc(GL_ALWAYS, 0, 1);
	//	glStencilMask(0x01);
	//	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//	drawShape();
	//	glEnable(GL_DEPTH_TEST);
	//	
	//	glUseProgram(*program);
	//	glUniform4f(uFillColor, 0.45, 0.33, 0.89, 1.0f);
	//	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//	glStencilFunc(GL_EQUAL, 1, 1);
	//	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//	drawShape();
	//	glDisable(GL_STENCIL_TEST);
	//	
	//	glDisableClientState(GL_VERTEX_ARRAY);
	//	glPopAttrib();
	//}

	bool isValidCommand(int c)
	{
		return c>=(VG_CLOSE_PATH >> 1) && c<=(VG_LCWARC_TO >> 1);
	}

	size_t coordCountForData(size_t segCount, const ubyte *segs)
	{
		size_t	count = 0;

		for (size_t s=0; s<segCount; ++s)
		{
			ubyte command = ((segs[s] & 0x1E) >> 1);
			if (!isValidCommand(command))
				return 0;
			count += coordsPerCommand[command];
		}

		return count;
	}
}