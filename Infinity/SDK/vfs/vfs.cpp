#include "vfs.h"
#include <boost\bind.hpp>
#include <map>

VFS* vfs;

void vfsInit()
{
	vfs = new VFS();
}

void vfsTerminate()
{
	delete vfs;
}

void vfsAddRoot(const char* dir)
{
	if( vfs )
		vfs->AddRootPath(dir);
}

void vfsAddZip(const char* zip)
{
	if( vfs )
		vfs->AddArchive(zip);
}

DataStreamPtr vfsMapFile(const char* file)
{
	if( vfs )
		return vfs->Open(file);
	return DataStreamPtr();
}