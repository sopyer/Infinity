#pragma once

#include <boost\bind.hpp>
#include <boost\mem_fn.hpp>
#include <boost\smart_ptr.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include "Singleton.h"

class LogSink
{
	public:
		virtual void Print(std::string message)=0;
		~LogSink(){};
};

typedef boost::shared_ptr< LogSink > LogSinkPtr;
typedef std::vector< LogSinkPtr > LogSinkList;

class Log: public Singleton<Log>
{
	public:
		enum LogOutputLevel {Error, Warning, Info, Debug};

	private:
		LogOutputLevel	OutputLevel;
		LogSinkList		Sinks;

	public:
		Log();
		
		~Log();

		void SetOutputLevel(LogOutputLevel level);
		void AttachSink(LogSinkPtr sink);

		void Print(LogOutputLevel level, std::string message);
};