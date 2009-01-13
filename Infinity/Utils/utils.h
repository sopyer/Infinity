#ifndef _UTILS_H_INCLUDED_
#	define _UTILS_H_INCLUDED_

#	include <windows.h>
#	include <boost\smart_ptr.hpp>
#	include <string>
//#	include "..\Utils\iniparser\iniparser.h"

#include <vector>
#include <map>
#include <string>

namespace utils
{
	using std::vector;
	using std::map;
	using std::string;
}

typedef boost::shared_ptr<void> MemoryView;

//typedef unsigned __int64 uint64;
//typedef signed   __int64 sint64;
//
//typedef unsigned int uint32;
//typedef signed int   sint32;
//
//typedef unsigned short uint16;
//typedef signed   short sint16;
//
//typedef unsigned char uint8;
//typedef signed   char sint8;


class File
{
	private:
		HANDLE	hFile;
		HANDLE	hMapping;
	public:
		File(const char *path);
		~File();

		MemoryView MapView(unsigned int offset=0, unsigned int size=0);
};

#endif