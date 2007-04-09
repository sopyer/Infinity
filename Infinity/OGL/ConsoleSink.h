#pragma once

#include "Log.h"
#include <windows.h>
#include "types.h"

class ConsoleSink: public LogSink
{
	private:
		HANDLE	StdOut;
	public:
		ConsoleSink();
		void Print(std::string message);
		~ConsoleSink();
};