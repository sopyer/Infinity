#include "Path.h"

#include <climits>
#include <cassert>
#include <glm/glm.h>

namespace impl
{
	VGint pathDataSize(const VGint segCount, const VGubyte *segs);

	////////////////////////////////////////////////////
	//			Public methods
	////////////////////////////////////////////////////

	void Path::appendData(const VGint    numSegments,
						  const VGubyte* pathSegments,
						  const VGfloat* pathData)
	{
		mSegments.insert(mSegments.end(), pathSegments, pathSegments+numSegments);
		mData.insert(mData.end(), pathData, pathData+numSegments);

		mRebuildGeometry = true;
	}

	////////////////////////////////////////////////////
	//			Private methods
	////////////////////////////////////////////////////

	class DataReader
	{
		public:
			DataReader(std::vector<VGfloat>	data)
			{
				assert(!data.empty());
				assert(data.size()<=(size_t)INT_MAX);

				mCurrent = &data[0];
				mEnd     = mCurrent+data.size();
			}
			
			void readFloat(float& value)
			{
				assert(mCurrent<mEnd);
				value = *mCurrent;
				mCurrent++;
			}

			void readPoint(glm::vec2& point)
			{
				readFloat(point.x);
				readFloat(point.y);
			}

		private:
			float*	mCurrent;
			float*	mEnd;
	};

	void Path::rebuild()
	{
		DataReader	reader(mData);
		bool		segmentStarted(false);

		//for (size_t s=segOffset; s<segs.size(); ++s)
		//{
		//	int		segment  = segs[s]&0x1E;
		//	int		segidx   = segment>>1;
		//	bool	rel      = segs[s]&1;
		//	
		//	if (segment==VG_CLOSE_PATH)
		//	{
		//		if (segmentStarted)
		//		{
		//			endSegment(true);
		//			segmentStarted = false;
		//		}
		//	}
		//	else if (segment==VG_MOVE_TO)
		//	{
		//		if (segmentStarted)
		//		{
		//			endSegment(false);
		//			segmentStarted = false;
		//		}

		//		xo = xp = iterFloat(data) + rel?xo:0;
		//		yo = yp = iterFloat(data) + rel?yo:0;
		//	}
		//	else
		//	{
		//		//Here starts non control commands
		//		//So we can handle start path case
		//		if (!segmentStarted)
		//		{
		//			segmentStarted = true;
		//			startSegment(xo, yo);
		//		}

		//		switch (segment)
		//		{
		//			case VG_LINE_TO:
		//				{
		//					float x = iterFloat(data) + (rel?xo:0);
		//					float y = iterFloat(data) + (rel?yo:0);
		//					lineTo(x, y);
		//					xo = xp = x;
		//					yo = yp = y;
		//				}
		//				break;

		//			case VG_HLINE_TO:
		//				{
		//					float x = iterFloat(data) + (rel?xo:0);
		//					float y = yo;
		//					lineTo(x, y);
		//					xo = xp = x;
		//					yo = yp = y;
		//				}
		//				break;

		//			case VG_VLINE_TO:
		//				{
		//					float x = xo;
		//					float y = iterFloat(data) + (rel?yo:0);
		//					lineTo(x, y);
		//					xo = xp = x;
		//					yo = yp = y;
		//				}
		//				break;

		//			case VG_QUAD_TO:
		//				{
		//					float x1 = iterFloat(data) + (rel?xo:0);
		//					float y1 = iterFloat(data) + (rel?yo:0);
		//					float x2 = iterFloat(data) + (rel?xo:0);
		//					float y2 = iterFloat(data) + (rel?yo:0);
		//					quadTo(x1, y1, x2, y2);
		//					xp = x1; yp = y1;
		//					xo = x2; yo = y2;
		//				}
		//				break;

		//			case VG_SQUAD_TO:
		//				{
		//					float x1 = 2*xo - xp;
		//					float y1 = 2*yo - yp;
		//					float x2 = iterFloat(data) + (rel?xo:0);
		//					float y2 = iterFloat(data) + (rel?yo:0);
		//					quadTo(x1, y1, x2, y2);
		//					xp = x1; yp = y1;
		//					xo = x2; yo = y2;
		//				}
		//				break;

		//			case VG_CUBIC_TO:
		//				{
		//					float x1 = iterFloat(data) + (rel?xo:0);
		//					float y1 = iterFloat(data) + (rel?yo:0);
		//					float x2 = iterFloat(data) + (rel?xo:0);
		//					float y2 = iterFloat(data) + (rel?yo:0);
		//					float x3 = iterFloat(data) + (rel?xo:0);
		//					float y3 = iterFloat(data) + (rel?yo:0);
		//					cubicTo(x1, y1, x2, y2, x3, y3);
		//					xp = x2; yp = y2;
		//					xo = x3; yo = y3;
		//				}
		//				break;

		//			case VG_SCUBIC_TO:
		//				{
		//					float x1 = 2*xo - xp;
		//					float y1 = 2*yo - yp;
		//					float x2 = iterFloat(data) + (rel?xo:0);
		//					float y2 = iterFloat(data) + (rel?yo:0);
		//					float x3 = iterFloat(data) + (rel?xo:0);
		//					float y3 = iterFloat(data) + (rel?yo:0);
		//					cubicTo(x1, y1, x2, y2, x3, y3);
		//					xp = x2; yp = y2;
		//					xo = x3; yo = y3;
		//				}
		//				break;

		//			case VG_SCWARC_TO:
		//			case VG_SCCWARC_TO:
		//			case VG_LCWARC_TO:
		//			case VG_LCCWARC_TO:
		//				{
		//					float rx = iterFloat(data);
		//					float ry = iterFloat(data);
		//					float angle = iterFloat(data);
		//					float xe = iterFloat(data) + (rel?xo:0);
		//					float ye = iterFloat(data) + (rel?yo:0);
		//					arcTo(segment, rx, ry, angle, xe, ye);
		//					xp = xo = xe;
		//					yp = yo = ye;
		//				}
		//				break;

		//			default:
		//				assert(0);
		//		}
		//	}
		//}

		//if (segmentStarted)
		//	endSegment(false);

		mRebuildGeometry = false;
	}

	////////////////////////////////////////////////////
	//			Helper functions
	////////////////////////////////////////////////////
	VGint pathDataSize(const VGint segCount, const VGubyte *segs)
	{
		return 0;
	}
}

