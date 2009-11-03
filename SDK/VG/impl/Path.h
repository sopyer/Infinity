#ifndef __IMPL_PATH_H_INCLUDED__
#	define	__IMPL_PATH_H_INCLUDED__

#include <vg/openvg.h>
#include <vector>

namespace impl
{

	class Path
	{
		public:
			void appendData(const VGint    numSegments,
							const VGubyte* pathSegments,
							const VGfloat* pathData);

		private:
			void rebuild();

		private:
			/* Path data */
			std::vector<VGubyte>	mSegments;
			std::vector<float>		mData;
			
			//Internal behaviour flags
			bool	mRebuildGeometry:1;
	};

}

#endif
