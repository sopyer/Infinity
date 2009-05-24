#ifndef _TYPES_H_INCLUDED_
#	define	_TYPES_H_INCLUDED_

typedef unsigned char	ubyte;
//typedef char			byte;

namespace vg
{
	template<typename T>
	struct PtrToImpl
	{
		public:
			T*	mObject;
	};
}

#endif