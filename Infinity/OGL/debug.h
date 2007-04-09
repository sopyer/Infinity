#pragma once

#ifdef _DEBUG

#	include "Log.h"
#	include <cstdarg>
#	include <cstdlib>
//#	include
//#	define debug(arg) {char* str=new char[256];sprintf(str, arg);Log::Ref().Print(Log::Debug, str);delete []str;}

inline void debug(const char* str, ...)
{
	va_list	arg;
	va_start(arg, str);
	char s[256];
	vsprintf(s, str, arg);
	Log::getRef().Print(Log::Debug, s);
	va_end(arg);
}

#else
//#	define debug(arg)
inline void debug(const char* str, ...)
{
}
#endif