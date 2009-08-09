#include <VFS++.h>
#include <cstdio>

int main()
{
	VFS vfs;

	vfs.mount("myfile.zip");
	vfs.exist("myfile.txt");

	File myfile = vfs.openRead("myfile.txt");
	PHYSFS_sint64 file_size = myfile.size();
	char *myBuf;
	myBuf = new char[myfile.size()];
	int length_read = myfile.read(myBuf, 1, myfile.size());
	delete [] myBuf;
	vfs.close(myfile);

	return 0;
}