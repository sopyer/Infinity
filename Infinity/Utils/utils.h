#ifndef _UTILS_H_INCLUDED_
#	define _UTILS_H_INCLUDED_

#	include <windows.h>
#	include <boost\smart_ptr.hpp>
#	include <string>
//#	include "..\Utils\iniparser\iniparser.h"

typedef boost::shared_ptr<void> MemoryView;

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