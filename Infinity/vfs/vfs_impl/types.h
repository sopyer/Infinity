#pragma once

#include <boost\smart_ptr.hpp>

typedef long				int32;
typedef unsigned long		uint32;
typedef short				int16;
typedef unsigned short		uint16;
typedef char				int8;
typedef unsigned char		uint8;
typedef __int64				int64;
typedef unsigned __int64	uint64;
typedef unsigned char		byte;

typedef boost::shared_array<byte> bytes_ptr; 
