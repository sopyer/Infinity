#ifndef _UTILS_H_INCLUDED_
#	define _UTILS_H_INCLUDED_

#	include <windows.h>
#	include <boost\smart_ptr.hpp>
#	include <string>
//#	include "..\Utils\iniparser\iniparser.h"

#include <vector>
#include <map>
#include <string>

#define UNUSED(var)			((void)(var))
#define ELEMENT_COUNT(arr)	sizeof(arr)/sizeof(arr[0])
#define ARRAY_SIZE(arr)		sizeof(arr)/sizeof(arr[0])

namespace utils
{
	using std::vector;
	using std::map;
	using std::string;
}

//typedef boost::shared_ptr<void> MemoryView;

typedef unsigned __int64 u64;
typedef signed   __int64 s64;

typedef unsigned int u32;
typedef signed int   s32;

typedef unsigned short u16;
typedef signed   short s16;

typedef unsigned char u8;
typedef signed   char s8;

template<typename T, T val0>
struct CArray1
{
	static T	ptr[];
};
template<typename T, T val0>
T CArray1<T, val0>::ptr[] = {val0};

template<typename T, T val0, T val1>
struct CArray2
{
	static T	ptr[];
};
template<typename T, T val0, T val1>
T CArray2<T, val0, val1>::ptr[] = {val0, val1};

template<typename T, T val0, T val1, T val2>
struct CArray3
{
	static T	ptr[];
};
template<typename T, T val0, T val1, T val2>
T CArray3<T, val0, val1, val2>::ptr[] = {val0, val1, val2};

//template<size_t val0>
//struct CArray1
//{
//	static const size_t	ptr[];
//};
//template<size_t val0>
//const size_t CArray1<val0>::ptr[] = {val0};
//
//template<size_t val0, size_t val1>
//struct CArray2
//{
//	static const size_t	ptr[];
//};
//template<size_t val0, size_t val1>
//const size_t CArray2<val0, val1>::ptr[] = {val0, val1};

//template<size_t val0, size_t val1, size_t val2>
//struct CArray3
//{
//	static const size_t	ptr[];
//};
//template<size_t val0, size_t val1, size_t val2>
//const size_t CArray3<val0, val1, val2>::ptr[] = {val0, val1, val2};

template<size_t val0, size_t val1, size_t val2, size_t val3>
struct CArray4
{
	static const size_t	ptr[];
};
template<size_t val0, size_t val1, size_t val2, size_t val3>
const size_t CArray4<val0, val1, val2, val3>::ptr[] = {val0, val1, val2, val3};

template<size_t val0, size_t val1, size_t val2, size_t val3, size_t val4>
struct CArray5
{
	static const size_t	ptr[];
};
template<size_t val0, size_t val1, size_t val2, size_t val3, size_t val4>
const size_t CArray5<val0, val1, val2, val3, val4>::ptr[] = {val0, val1, val2, val3, val4};

template<size_t val0, size_t val1, size_t val2, size_t val3, size_t val4, size_t val5>
struct CArray6
{
	static const size_t	ptr[];
};
template<size_t val0, size_t val1, size_t val2, size_t val3, size_t val4, size_t val5>
const size_t CArray6<val0, val1, val2, val3, val4, val5>::ptr[] = {val0, val1, val2, val3, val4, val5};

template<size_t val0, size_t val1, size_t val2, size_t val3, size_t val4, size_t val5, size_t val6>
struct CArray7
{
	static const size_t	ptr[];
};
template<size_t val0, size_t val1, size_t val2, size_t val3, size_t val4, size_t val5, size_t val6>
const size_t CArray7<val0, val1, val2, val3, val4, val5, val6>::ptr[] = {val0, val1, val2, val3, val4, val5, val6};

//class File
//{
//	private:
//		HANDLE	hFile;
//		HANDLE	hMapping;
//	public:
//		File(const char *path);
//		~File();
//
//		MemoryView MapView(unsigned int offset=0, unsigned int size=0);
//};

#endif