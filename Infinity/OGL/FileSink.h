#pragma once

#include "Log.h"
#include <windows.h>
#include "types.h"

class FileSink: public LogSink
{
	private:
		HANDLE	hFile;
	public:
		FileSink(std::string name);
		void Print(std::string message);
		~FileSink();
};