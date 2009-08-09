#pragma once

#include <string>
#include <vector>
#include <boost\smart_ptr.hpp>
#include <vfs_impl\utils.h>

typedef boost::shared_ptr< DataStream > DataStreamPtr;

class VFSDriver
{
	public:
		virtual DataStreamPtr Open(std::string path) = 0;
		virtual bool Exist(std::string path) = 0;
		virtual ~VFSDriver(){};
};

typedef boost::shared_ptr< VFSDriver >	VFSDriverPtr;
typedef std::vector< VFSDriverPtr >		VFSDriversList;

class VFS
{
	private:
		VFSDriversList	m_Drivers;
	public:
		VFS();
		~VFS();

		bool AddRootPath(std::string path);
		bool AddArchive(std::string path);
		bool Exist(std::string path);
		DataStreamPtr Open(std::string path);
};