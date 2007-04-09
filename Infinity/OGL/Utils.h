#pragma once

#include <windows.h>
#include "types.h"

class DataStream
{
	public:
		virtual				~DataStream(){}
		virtual uint32		Read(void* buffer, uint32 size_to_read) = 0;
		virtual bool		Seek(uint32 finalPos, bool relative_movement = false) = 0;
		virtual uint32		GetSize() = 0;
		virtual uint32		GetPos() = 0;
		virtual byte*		MapStream() = 0;
};

template< typename T >
bool IsEmpty(boost::shared_ptr<T>& boost_ptr)
{
	return boost_ptr.get()==0;
}

/*class FileDataStream: public DataStream
{
	private:
		FILE*	f;
	public:
		FileDataStream(std::string& path)
		{
			f = fopen(path.c_str(), "rb");
		}
		~FileDataStream()
		{
			fclose(f);
		}
		int32 GetSize()
		{
			int p = ftell(f);
			fseek(f, 0, SEEK_END);
			int res = ftell(f);
			fseek(f, p, SEEK_SET);
			return res;
		}
		int32 Read(void* buffer, int32 size_to_read)
		{
			return (int32)fread(buffer, size_to_read, 1, f);
		}
		bool Seek(uint32 final_pos, bool relative_movement = false)
		{
			return fseek(f, final_pos, relative_movement?SEEK_CUR:SEEK_SET)==0;
		}
		int32 GetPos()
		{
			return ftell(f);
		}
};*/

inline bool FileExist(std::string path)//Also work on templates *? - should be corrected
{
/*	bool result=false;
	WIN32_FIND_DATAA ss;
	HANDLE			search;
	ZeroMemory(&ss, sizeof(WIN32_FIND_DATAA));
	search = FindFirstFileA(path.c_str(), &ss);
	if( search!=INVALID_HANDLE_VALUE)
	{
		if(ss.dwFileAttributes & ~FILE_ATTRIBUTE_DIRECTORY)
			result=true;
		FindClose(search);
	}
	return result;*/
	uint32 attr = GetFileAttributes(path.c_str());
	if( attr==INVALID_FILE_ATTRIBUTES )
		return false;
	if( attr & ~FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}

inline bool DirExist(std::string path)//Also work on templates *? - should be corrected
{
/*	bool result=false;
	WIN32_FIND_DATAA ss;
	HANDLE			search;
	ZeroMemory(&ss, sizeof(WIN32_FIND_DATAA));
	search = FindFirstFileA(path.c_str(), &ss);
	if( search!=INVALID_HANDLE_VALUE)
	{
		if(ss.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			result=true;
		FindClose(search);
	}
	return result;*/
	uint32 attr = GetFileAttributes(path.c_str());
	if( attr==INVALID_FILE_ATTRIBUTES )
		return false;
	if( attr & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}

class BaseMemoryStream: public DataStream
{
	protected:
		byte*		pData;
		uint32		uiSize, uiPos;
	public:
		BaseMemoryStream():pData(0), uiSize(0), uiPos(0){}
//		BaseMemoryStream(byte* ptr, uint32 s):pData(ptr), uiSize(s), uiPos(0){} What's the hell with this constructor???
		uint32 Read(void* buffer, uint32 size_to_read)
		{
			int size=size_to_read;
			if( uiPos+size>uiSize )
				size = uiSize-uiPos;
			memcpy(buffer, (pData+uiPos), size);
			uiPos+=size;
			return size;
		}
		uint32 GetSize()
		{
			return uiSize;
		}
		bool Seek(uint32 final_pos, bool relative_movement = false)
		{
			if( !relative_movement && final_pos<=uiSize )
			{
				uiPos=final_pos;
				return true;
			}
			if( relative_movement && final_pos+uiPos<=uiSize )
			{
				uiPos+=final_pos;
				return true;
			}
			return false;
		}
		uint32 GetPos()
		{
			return uiPos;
		}

		byte* MapStream()
		{
			return pData;
		}
};

class FileDataStream: public BaseMemoryStream
{
	private:
		HANDLE	hFile;
		HANDLE	hMapping;
	public:
		FileDataStream(std::string path)
		{
			hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			uiSize = GetFileSize(hFile, NULL);
			hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
			pData = (byte*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
		}
		~FileDataStream()
		{
			UnmapViewOfFile(pData);
			CloseHandle(hMapping);
			CloseHandle(hFile);
		}
};

//Make this class be inherited from Memory pData stream or some base class (problem with bytes_ptr)
//class FileDataStream: public DataStream
//{
//	private:
//		HANDLE	hFile;
//		HANDLE	hMapping;
//		byte*	pData;
//	public:
//		FileDataStream(std::string path)
//		{
//			hFile = CreateFile(path.c_str(), GENERIC_READ, NULL, NULL, 
//				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//			hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
//			pData = (byte*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
//
//		}
//		~FileDataStream()
//		{
//			UnmapViewOfFile(pData);
//			CloseHandle(hMapping);
//			CloseHandle(hFile);
//		}
//		uint32 GetSize()
//		{
//			return GetFileSize(hFile, NULL);
//		}
//		uint32 Read(void* buffer, uint32 size_to_read)
//		{
//			uint32	bytes_read;
//			ReadFile(hFile, buffer, size_to_read, &bytes_read, NULL);
//			return bytes_read;
//		}
//		bool Seek(uint32 final_pos, bool relative_movement = false)
//		{
//			return SetFilePointer(hFile, final_pos, NULL, (relative_movement)?FILE_CURRENT:FILE_BEGIN)!=INVALID_SET_FILE_POINTER;
//		}
//		uint32 GetPos()
//		{
//			return SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
//		}
//		byte* MapStream()
//		{
//			return pData;
//		}
//};

class MemoryDataStream: public BaseMemoryStream
{
	private:
		bytes_ptr	bytes;
	public:
		MemoryDataStream(bytes_ptr ptr, int32 s):bytes(ptr)//, BaseMemoryStream(bytes.get(),s)
		{
			pData = bytes.get();
			uiSize = s;
		}
		~MemoryDataStream(){}
};

//class MemoryDataStream: public DataStream
//{
//	private:
//		bytes_ptr	bytes;
//		byte*		pData;
//		uint32		uiSize, uiPos;
//	public:
//		MemoryDataStream(bytes_ptr ptr, int32 s):bytes(ptr), pData(bytes.get()), uiSize(s), uiPos(0){}//its not an option
//		~MemoryDataStream()
//		{
//		}
//		uint32 Read(void* buffer, uint32 size_to_read)
//		{
//			int size=size_to_read;
//			if( uiPos+size>uiSize )
//				size = uiSize-uiPos;
//			memcpy(buffer, (pData+uiPos), size);
//			uiPos+=size;
//			return size;
//		}
//		uint32 GetSize()
//		{
//			return uiSize;
//		}
//		bool Seek(uint32 final_pos, bool relative_movement = false)
//		{
//			if( !relative_movement && final_pos<=uiSize )
//			{
//				uiPos=final_pos;
//				return true;
//			}
//			if( relative_movement && final_pos+uiPos<=uiSize )
//			{
//				uiPos+=final_pos;
//				return true;
//			}
//			return false;
//		}
//		uint32 GetPos()
//		{
//			return uiPos;
//		}
//
//		byte* MapStream()
//		{
//			return pData;
//		}
//};