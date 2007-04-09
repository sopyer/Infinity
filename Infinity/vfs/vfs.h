#ifndef _VFS_INCLUDED_H_
#	define _VFS_INCLUDED_H_

#include "vfs_impl\implvfs.h"
#include <boost\shared_ptr.hpp>

typedef boost::shared_ptr<void*> FileViewPtr;

void vfsInit();
void vfsTerminate();

void vfsAddRoot(const char* dir);
void vfsAddZip(const char* zip);

DataStreamPtr vfsMapFile(const char* file);

#endif