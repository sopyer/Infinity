#include <windows.h>
#include <cstdlib>
#include "Framework.h"

namespace logging
{
	void message(const char *s,...)
	{
		char str[4096];
		DWORD written=0;
		va_list vl;
		va_start(vl, s);
		vsnprintf(str, 4096, s, vl);
		WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), str, strlen(str), &written, 0);
		OutputDebugString(str);
		va_end(vl);
	}
}
