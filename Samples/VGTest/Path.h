#ifndef __VGTEST_PATH_H_INCLUDED__
#	define __VGTEST_PATH_H_INCLUDED__

struct PathGeometry
{
};

//struct Path
//{
//	u8*		segments;
//	float*	data;
//};

#include <vg/vg.h>
#include <cassert>

VGfloat hourHandData[] = {
	14.52016f, -0.22339926f, 14.52016f, -0.22339926f, 13.76016f, -0.18339926f, 12.52016f, 
	-0.18339926f, 12.407921f, -0.37046566f, 12.504577f, -0.73236826f, 12.51766f, -0.77839926f, 12.463759f, 
	-0.72153926f, 11.846763f, -0.11453206f, 10.34141f, -0.34339946f, 9.8192854f, -0.65667406f, 10.149819f, 
	-1.5673425f, 10.09391f, -1.6483993f, 10.093583f, -1.6487295f, 10.092843f, -1.6494491f, 10.09266f, 
	-1.6496495f, 10.092518f, -1.6497711f, 10.091561f, -1.6507801f, 10.09141f, -1.6508993f, 10.090753f, 
	-1.6512401f, 10.089275f, -1.6519685f, 10.08891f, -1.6521493f, 10.088487f, -1.6521521f, 10.086846f, 
	-1.6521697f, 10.08641f, -1.6521493f, 10.085775f, -1.6521621f, 10.084402f, -1.6521705f, 10.08391f, 
	-1.6521493f, 10.083296f, -1.6520901f, 10.082063f, -1.6522709f, 10.08141f, -1.6521493f, 10.071661f, 
	-1.6499309f, 10.057457f, -1.6405787f, 10.04016f, -1.6233993f, 9.8599574f, -1.4444145f, 8.4401604f, 
	-1.2033995f, 8.4401604f, -1.2033995f, 8.4401604f, -1.2033995f, 7.5201605f, -1.5836341f, 7.2801605f, 
	-2.4846493f, 6.9201605f, -1.7662741f, 6.4002657f, -2.5833995f, 5.3364105f, -1.1033995f, 4.8984389f, 
	-1.1833993f, 4.9601605f, -1.8233993f, 4.5201605f, -1.7433995f, 4.5201605f, -1.3433993f, 4.5601605f, 
	-0.86339926f, 4.5601605f, -0.66339946f, 4.1492141f, -0.40188766f, 3.0588833f, -1.2568071f, 1.9389105f, 
	-0.45839926f, 1.7189257f, -1.3239197f, 0.93381675f, -1.9646493f, 0.00016045078f, -1.9646493f, -1.1038396f, 
	-1.9646493f, -1.9998396f, -1.0686493f, -1.9998396f, 0.035350739f, -1.9998396f, 1.1393507f, -1.1038396f, 
	2.0353504f, 0.00016045078f, 2.0353504f, 0.95178565f, 2.0353504f, 1.7495117f, 1.3699361f, 1.9514105f, 
	0.47910074f, 3.0669893f, 1.2622983f, 4.1507449f, 0.41481354f, 4.5601605f, 0.67535074f, 4.5601605f, 
	0.87535054f, 4.5201605f, 1.3553507f, 4.5201605f, 1.7553504f, 4.9601605f, 1.8353504f, 4.8984389f, 
	1.1953505f, 5.3364105f, 1.1153505f, 6.4002657f, 2.5953504f, 6.9201605f, 1.7782254f, 7.2801605f, 
	2.4966004f, 7.5201605f, 1.5955851f, 8.4401604f, 1.2153507f, 8.4401604f, 1.2153507f, 8.4401604f, 
	1.2153503f, 9.8599574f, 1.4563659f, 10.04016f, 1.6353507f, 10.05798f, 1.6530497f, 10.072795f, 
	1.6620423f, 10.08266f, 1.6641005f, 10.083093f, 1.6643383f, 10.084736f, 1.6651299f, 10.08516f, 
	1.6653507f, 10.085364f, 1.6653597f, 10.08621f, 1.6653487f, 10.08641f, 1.6653507f, 10.086606f, 
	1.6653451f, 10.087468f, 1.6653633f, 10.08766f, 1.6653507f, 10.087884f, 1.6651189f, 10.088688f, 
	1.6643339f, 10.08891f, 1.6641005f, 10.089022f, 1.6641025f, 10.090026f, 1.6640951f, 10.09016f, 
	1.6641005f, 10.090273f, 1.6641029f, 10.091275f, 1.6640931f, 10.09141f, 1.6641005f, 10.091635f, 
	1.6638723f, 10.092436f, 1.6630807f, 10.09266f, 1.6628505f, 10.093101f, 1.6624309f, 10.094727f, 
	1.6608027f, 10.09516f, 1.6603507f, 10.095384f, 1.6599065f, 10.096218f, 1.6582635f, 10.09641f, 
	1.6578507f, 10.145745f, 1.5644635f, 9.8234414f, 0.66613194f, 10.34141f, 0.35535074f, 11.858903f, 
	0.12463734f, 12.473674f, 0.74456774f, 12.51891f, 0.79285074f, 12.509595f, 0.76074854f, 12.405363f, 
	0.38667874f, 12.52016f, 0.19535054f, 13.76016f, 0.19535054f, 14.52016f, 0.23535054f, 14.52016f, 
	0.23535054f, 14.52016f, -0.22339926f, 9.8176604f, 0.0041007388f, 9.8178934f, 0.0043203388f, 9.8174304f, 
	0.0051429388f, 9.8176604f, 0.0053507388f, 9.8167414f, 0.0061243388f, 9.8161184f, 0.0081303388f, 9.8151604f, 
	0.0091007388f, 9.7547194f, 0.072799539f, 9.6219194f, 0.51684454f, 9.6226604f, 0.95535074f, 8.7850974f, 
	0.59535074f, 8.1965054f, 0.77535074f, 8.0201604f, 0.89535074f, 7.8924909f, 0.44033934f, 7.5606733f, 
	0.19535054f, 7.3976605f, 0.0053507388f, 7.5606733f, -0.20464946f, 7.8924909f, -0.42963806f, 8.0201604f, 
	-0.88464946f, 8.1965054f, -0.76464946f, 8.7850974f, -0.58464926f, 9.6226604f, -0.94464946f, 9.6219134f, 
	-0.50263546f, 9.7566594f, -0.058605861f, 9.8164104f, 0.0028507388f, 9.8166454f, 0.0030825388f, 9.8174284f, 
	0.0038811388f, 9.8176604f, 0.0041007388f, 7.8626605f, -1.1646495f, 7.6012393f, -0.024040261f, 5.5601605f, 
	-0.38464946f, 5.5601605f, -0.38464946f, 5.5601605f, -0.38464946f, 5.5013789f, -0.54464926f, 5.3401605f, 
	-0.78464926f, 5.7801605f, -0.78464926f, 6.7781937f, -2.5634311f, 7.8626605f, -1.1646495f, 7.8626605f, 
	1.1953505f, 6.7781937f, 2.5941324f, 5.7801605f, 0.81410074f, 5.3401605f, 0.81410074f, 5.5013789f, 
	0.57410074f, 5.5601605f, 0.41410054f, 5.5601605f, 0.41410054f, 5.5601605f, 0.41410054f, 7.6012393f, 
	0.054741339f, 7.8626605f, 1.1953505f, 4.0776605f, -0.028399261f, 4.0777981f, -0.027576061f, 4.0775349f, 
	-0.026723661f, 4.0776605f, -0.025899261f, 4.0776545f, -0.025482461f, 4.0776697f, -0.023816461f, 4.0776605f, 
	-0.023399461f, 4.0776749f, -0.023294861f, 4.0776653f, -0.022409661f, 4.0776605f, -0.022149261f, 4.0776317f, 
	-0.021525661f, 4.0776749f, -0.020274661f, 4.0776605f, -0.019649261f, 4.0776569f, -0.019388861f, 4.0776493f, 
	-0.018504261f, 4.0776605f, -0.018399261f, 4.0783093f, -0.012367661f, 4.0776605f, -0.0069966612f, 4.0776605f, 
	-0.00089946115f, 4.0776609f, 0.22208534f, 3.6391417f, 0.40285074f, 3.0989105f, 0.40285074f, 2.5586789f, 
	0.40285074f, 2.1201605f, 0.22208534f, 2.1201605f, -0.00089946115f, 2.1201593f, -0.22388426f, 2.5586793f, 
	-0.40464926f, 3.0989105f, -0.40464926f, 3.6161997f, -0.40464926f, 4.0425397f, -0.23827286f, 4.0776605f, 
	-0.028399261f, 
};

VGubyte hourHandSegs[] = {
	VG_MOVE_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, 
	VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, 
	VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, 
	VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, 
	VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, 
	VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_LINE_TO_ABS, VG_CLOSE_PATH, VG_MOVE_TO_ABS, VG_CUBIC_TO_ABS, 
	VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, 
	VG_CLOSE_PATH, VG_MOVE_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CLOSE_PATH, VG_MOVE_TO_ABS, VG_CUBIC_TO_ABS, 
	VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CLOSE_PATH, VG_MOVE_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, 
	VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CUBIC_TO_ABS, VG_CLOSE_PATH, 
};

inline void analyzePath(const VGint numSegments, const VGubyte* segments, const VGfloat* data)
{
	glm::vec2			o, p;
	bool				segmentStarted = false;

	for (VGint s=0; s<numSegments; ++s)
	{
		int		segment  = segments[s]&0x1E;
		bool	rel      = segments[s]&1;
			
		//points.clear();
		//points.push_back(o);
		if (segment==VG_CLOSE_PATH)
		{
			if (segmentStarted)
			{
				//builder.end();
				segmentStarted = false;
			}
		}
		else if (segment==VG_MOVE_TO)
		{
			if (segmentStarted)
			{
				//builder.end();
				segmentStarted = false;
			}

			//o = p = readVec2(pathData) + (rel?o:glm::vec2(0));
		}
		else
		{
			//Here starts non control commands
			//So we can handle start path case
			if (!segmentStarted)
			{
				segmentStarted = true;
				//builder.begin(o.x, o.y);
			}

			switch (segment)
			{
				case VG_LINE_TO:
					{
						//o = p = readVec2(pathData) + (rel?o:glm::vec2(0));
					}
					break;

				case VG_HLINE_TO:
					{
						//float x = readFloat(pathData) + (rel?o.x:0);
						//o.x = p.x = x;
					}
					break;

				case VG_VLINE_TO:
					{
					}
					break;

				case VG_QUAD_TO:
					{
					}
					break;

				case VG_SQUAD_TO:
					{
					}
					break;

				case VG_CUBIC_TO:
					{
					}
					break;

				case VG_SCUBIC_TO:
					{
					}
					break;

				case VG_SCWARC_TO:
				case VG_SCCWARC_TO:
				case VG_LCWARC_TO:
				case VG_LCCWARC_TO:
					{
					}
					break;

				default:
					assert(0);
			}

			//builder.lineTo(o.x, o.y);
		}
	}

	//if (segmentStarted)
	//	builder.end();

	//builder.copyDataTo(geom);

}

//#include <cstdlib>

const size_t MIN_BUF_SIZE = 1024;

class WriteBuffer
{
	public:
		WriteBuffer()
		{
			pos = start = (char*)malloc(MIN_BUF_SIZE);
			end = start + MIN_BUF_SIZE;
		}

		~WriteBuffer()
		{
			free(start);
		}

		void expand()
		{
			ptrdiff_t curSize = end - start;
			ptrdiff_t newSize = curSize * 2;
			ptrdiff_t curOffset = pos - start;

			start = (char*)realloc(start, newSize);
			pos = start + curOffset;
			end = start + newSize;
		}

		size_t available() {return end - pos;}
		size_t written() {return pos - start;}

		template< typename T >
		void write(const T& value)
		{
			assert((pos+sizeof(value)) <= end);
			T& dest = *(T*)pos;
			dest = value;
			pos += sizeof(value);
		}
		
		void write(void* src, size_t size)
		{
			assert((pos+size) <= end);
			memcpy(pos, src, size);
			pos += size;
		}

		template< typename T >
		void write(T* src, size_t count)
		{
			assert((pos + count*sizeof(T)) <= end);
			std::copy(src, src+count, (T*)pos);
			pos += count*sizeof(T);
		}

	private:
		char*	start;
		char*	end;
		char*	pos;
};

void testWriteBuffer()
{
	WriteBuffer buf;

	glm::vec2 vecs[] = {glm::vec2(0, 0), glm::vec2(1, 1), glm::vec2(2, 2)};

	buf.write(vecs, 2);
}

#endif