#include "utils.h"

//File::File(const char *path)
//{
//	hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, 
//		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	DWORD uiSize = GetFileSize(hFile, NULL);
//	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
//	//pData = (byte*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
//}
//File::~File()
//{
//	//UnmapViewOfFile(pData);
//	CloseHandle(hMapping);
//	CloseHandle(hFile);
//}
//
//MemoryView File::MapView(unsigned int offset, unsigned int size)
//{
//	return MemoryView(MapViewOfFile(hMapping, FILE_MAP_READ, 0, offset, size), UnmapViewOfFile);
//}
