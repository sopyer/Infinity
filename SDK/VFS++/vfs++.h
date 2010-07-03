#ifndef __VFSPP_H_INCLUDED__
#	define __VFSPP_H_INCLUDED__

#	include <physfs/physfs.h>

#include <Singleton.h>

class File
{
public:
	File(PHYSFS_file* handle): mHandle(handle){}
	~File(){}

	PHYSFS_sint64 size() {return PHYSFS_fileLength(mHandle);}

	bool eof() {return PHYSFS_eof(mHandle)!=0;}

	PHYSFS_sint64 tell() {return PHYSFS_tell(mHandle);}

	bool seek(PHYSFS_uint64 pos) {return PHYSFS_seek(mHandle, pos)!=0;}

	PHYSFS_sint64 read(void *buffer, PHYSFS_uint32 objSize, PHYSFS_uint32 objCount)
	{return PHYSFS_read(mHandle, buffer, objSize, objCount);}

	void close() {PHYSFS_close(mHandle);}

	operator PHYSFS_file* () {return mHandle;}

	operator bool() {return mHandle!=0;}

public:
	PHYSFS_file*	mHandle;
};

class VFS: public Singleton<VFS>
{
public:
	VFS()  {PHYSFS_init(0);}
	~VFS() {PHYSFS_deinit();}

	static bool mount(const char* newDir, const char* mountPoint = 0, int appendToPath = 1)
	{return PHYSFS_mount(newDir, mountPoint, appendToPath) != 0;}

	static bool exist(const char* fname) {return PHYSFS_exists(fname) != 0;}

	static File openRead(const char* fname) {return PHYSFS_openRead(fname);}
	static void close(File fileObj) {PHYSFS_close(fileObj);}
};

#endif