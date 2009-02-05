#pragma once

std::string DeleteBackslashes(std::string s)
{
	std::string::size_type pos; 
	pos=0;
	if( s[pos]=='\\' ) s.erase(pos, 1);
	pos = s.length()-1;
	if( s[pos] == '\\' ) s.erase(pos, 1);
	return s;
}

class VFSNativeDriver: public VFSDriver
{
	private:
		std::string rel;
	public:
		VFSNativeDriver(std::string path):rel(DeleteBackslashes(path)){}
		~VFSNativeDriver(){}

		DataStreamPtr Open(std::string path)
		{
			return DataStreamPtr(new FileDataStream(rel+'\\'+DeleteBackslashes(path)));
		}

		bool Exist(std::string path)
		{
			return FileExist((rel+'\\'+DeleteBackslashes(path)).c_str());
		}
};