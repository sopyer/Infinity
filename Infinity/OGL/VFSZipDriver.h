#pragma once

#include <map>
#include "zlib.h"
#include "debug.h"

std::string ZipPreparePath(std::string s)
{
	std::string::size_type pos;
	while( ( pos = s.find_first_of('/') ) != std::string::npos )
	{
		s.replace(pos, 1, 1, '\\');
	}
	pos=0;
	if( s[pos]=='\\' ) s.erase(pos, 1);
	pos = s.length()-1;
	if( s[pos] == '\\' ) s.erase(pos, 1);
	return s;
}

/*class VFSZipDriver: public VFSDriver
{
	private:
		ZZIP_DIR*	dir;
	public:
		VFSZipDriver(std::string path)
		{
			zzip_error_t err;
			dir = zzip_dir_open(path.c_str(), &err);
		}
		~VFSZipDriver()
		{
			if(dir)
				zzip_dir_close(dir);
		}

		DataStreamPtr Open(std::string path)
		{
			ZZIP_FILE*	f;
			ZZIP_STAT	zs;
			int size;
			zzip_dir_stat(dir, ZipPreparePath(path).c_str(), &zs, 0);
			size = zs.st_size;
			byte* buf = new byte[size];
			f = zzip_file_open(dir, ZipPreparePath(path).c_str(), 0);
			zzip_file_read(f, (char*)buf, size);
			zzip_file_close(f);
			return DataStreamPtr(new MemoryDataStream(bytes_ptr(buf), size));
		}

		bool Exist(std::string path)
		{
			ZZIP_FILE*	f;
			f = zzip_file_open(dir, ZipPreparePath(path).c_str(), 0);
			if( f )
			{
				zzip_file_close(f);
				return true;
			}
			return false;
		}
};*/

const int16 ZIP_INFO_IN_DATA_DESCRITOR =	0x0008; // the fields crc-32, compressed size

#pragma pack( push, packing )
#pragma pack( 1 )
#define PACK_STRUCT

struct ZipFileDataDescriptor
{
	int32 CRC32;
	int32 CompressedSize;
	int32 UncompressedSize;
} PACK_STRUCT;

struct ZipFileHeader
{
	int32 Sig;
	int16 VersionToExtract;
	int16 GeneralBitFlag;
	int16 CompressionMethod;
	int16 LastModFileTime;
	int16 LastModFileDate;
	ZipFileDataDescriptor DataDescriptor;
	int16 FilenameLength;
	int16 ExtraFieldLength;
} PACK_STRUCT;

// Default alignment
#pragma pack( pop, packing )

#undef PACK_STRUCT

struct ZipEntry
{
//	std::string Path;
	int32 FileDataPosition; // position of compressed data in file
	int32 CompressedSize;
	int32 UncompressedSize;
	int16 CompressionMethod;
};

class VFSZipDriver: public VFSDriver
{
	private:
		typedef std::map<std::string, ZipEntry>	ZipEntryMap;		
		FileDataStream	ZipFile;
		ZipEntryMap		FileList;
	public:
		VFSZipDriver(std::string path):ZipFile(path)
		{
			while( ScanLocalHeader() );
		}
		~VFSZipDriver()
		{
		}

		DataStreamPtr Open(std::string path)
		{
			byte* cdata, *udata;
			uint32	csize, usize;
			debug("\tTrying to open in zip:%s", path.c_str());
			ZipEntryMap::iterator it = FileList.find(path);
			if( it!=FileList.end() )
			{
				csize = (*it).second.CompressedSize;
				usize = (*it).second.UncompressedSize;
				cdata = new byte[csize];
				ZipFile.Seek((*it).second.FileDataPosition);
				ZipFile.Read(cdata, csize);
				switch((*it).second.CompressionMethod)
				{
					case 0: // no compression
						return DataStreamPtr(new MemoryDataStream(bytes_ptr(cdata), csize));
					case 8:
					{
						z_stream stream;
						int32 err;

						udata = new byte[usize];

						stream.next_in = cdata;
						stream.avail_in = csize;
						stream.next_out = udata;
						stream.avail_out = usize;
						stream.zalloc = (alloc_func)0;
						stream.zfree = (free_func)0;

						// Perform inflation. wbits < 0 indicates no zlib header inside the data.
						err = inflateInit2(&stream, -MAX_WBITS);
						if (err == Z_OK)
						{
							err = inflate(&stream, Z_FINISH);
							inflateEnd(&stream);
							if (err == Z_STREAM_END)
								err = Z_OK;
						}

						delete [] cdata;

						if (err != Z_OK)
						{
							debug("-failed\n");
							return DataStreamPtr();
						}
						else
						{
							debug("-success\n");
							return DataStreamPtr(new MemoryDataStream(bytes_ptr(udata), usize));
						}
					}
					break;
				};
			}
			debug("-failed\n");
			return DataStreamPtr();
		}

		bool Exist(std::string path)
		{
			ZipEntryMap::iterator it = FileList.find(path);
			if( it!=FileList.end() )
				return true;
			return false;
		}
	private:
		bool ScanLocalHeader()
		{
			char tmp[1024];

			ZipEntry	entry;
			ZipFileHeader	header;
			entry.FileDataPosition = 0;
			memset(&header, 0, sizeof(ZipFileHeader));

			ZipFile.Read(&header, sizeof(ZipFileHeader));

			if (header.Sig != 0x04034b50)
				return false; // local file headers end here.

			// read filename
			ZipFile.Read(tmp, header.FilenameLength);
			tmp[header.FilenameLength] = 0x0;

			// move forward length of extra field.

			if (header.ExtraFieldLength)
				ZipFile.Seek(header.ExtraFieldLength, true);

			// if bit 3 was set, read DataDescriptor, following after the compressed data
			if (header.GeneralBitFlag & ZIP_INFO_IN_DATA_DESCRITOR)
			{
				// read data descriptor
				ZipFile.Read(&header.DataDescriptor, sizeof(header.DataDescriptor));
			}
			// store position in file
			entry.FileDataPosition = ZipFile.GetPos();
			entry.CompressedSize = header.DataDescriptor.CompressedSize;
			entry.UncompressedSize = header.DataDescriptor.UncompressedSize;
			entry.CompressionMethod = header.CompressionMethod;
			// move forward length of data
			ZipFile.Seek(entry.CompressedSize, true);
			FileList[ZipPreparePath(tmp)]=entry;
			return true;
		}
};