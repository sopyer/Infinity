#ifndef _UTILS_H_INCLUDED_
#	define _UTILS_H_INCLUDED_

#	include <windows.h>
#	include <boost\smart_ptr.hpp>
#	include <string>
//#	include "..\Utils\iniparser\iniparser.h"

#include <vector>
#include <map>
#include <string>

#define UNUSED(var) ((void)var)

namespace utils
{
	using std::vector;
	using std::map;
	using std::string;
}

//typedef boost::shared_ptr<void> MemoryView;

typedef unsigned __int64 u64;
typedef signed   __int64 s64;

typedef unsigned int u32;
typedef signed int   s32;

typedef unsigned short u16;
typedef signed   short s16;

typedef unsigned char u8;
typedef signed   char s8;


//class File
//{
//	private:
//		HANDLE	hFile;
//		HANDLE	hMapping;
//	public:
//		File(const char *path);
//		~File();
//
//		MemoryView MapView(unsigned int offset=0, unsigned int size=0);
//};

#endif