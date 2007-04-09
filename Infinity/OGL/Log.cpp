#include "Log.h"
#include "ConsoleSink.h"
#include "FileSink.h"

FileSink::FileSink(std::string name)
{
	hFile = NULL;
	hFile=CreateFile(name.c_str(), GENERIC_WRITE, NULL, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

void FileSink::Print(std::string message)
{
	uint32 written;
	WriteFile(hFile, message.c_str(), message.length(), &written, NULL);
}

FileSink::~FileSink()
{
	CloseHandle(hFile);
}


ConsoleSink::ConsoleSink()
{
	AllocConsole();
	StdOut=GetStdHandle(STD_OUTPUT_HANDLE);
}

void ConsoleSink::Print(std::string message)
{
	uint32 written;
	WriteConsole(StdOut, message.c_str(), message.length(), &written, NULL);
}

ConsoleSink::~ConsoleSink()
{
	FreeConsole();
}

Log::Log():OutputLevel(Debug)
{
}

Log::~Log()
{
}

void Log::SetOutputLevel(LogOutputLevel level)
{
	OutputLevel=level;
}

void Log::AttachSink(LogSinkPtr sink)
{
	Sinks.push_back(sink);
}

void Log::Print(LogOutputLevel level, std::string message)
{
	if(level>OutputLevel)
		return;
	std::for_each(Sinks.begin(), Sinks.end(), boost::bind(boost::mem_fn(&LogSink::Print), _1, message));
}
