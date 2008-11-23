#include "vfs_impl\implvfs.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
//#include <zzip\zzip.h>
#include "VFSNativeDriver.h"
#include "VFSZipDriver.h"

VFS::VFS()
{
}

VFS::~VFS()
{
}

bool VFS::AddRootPath(std::string path)
{
	if( DirExist(path.c_str()) )
	{
		m_Drivers.push_back(VFSDriverPtr(new VFSNativeDriver(path)));
		return true;
	}

	return false;
}

bool VFS::AddArchive(std::string path)
{
	if( FileExist(path.c_str()) ) 
	{
		m_Drivers.push_back(VFSDriverPtr(new VFSZipDriver(path)));
		return true;
	}
	
	return false;
}

DataStreamPtr VFS::Open(std::string path)
{
	VFSDriversList::iterator	it=m_Drivers.begin(), end=m_Drivers.end();
	for(; it!=end; it++)
		if( (*it)->Exist(path) )
		{
			return (*it)->Open(path);
		}
	return DataStreamPtr();
}

bool VFS::Exist(std::string path)
{
	VFSDriversList::iterator	it=m_Drivers.begin(), end=m_Drivers.end();
	for(; it!=end; it++)
		if( (*it)->Exist(path) )
		{
			return true;
		}
	return false;
}