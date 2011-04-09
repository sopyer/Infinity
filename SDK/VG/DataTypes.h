#ifndef __DATATYPES_H_INCLUDED__
#	define __DATATYPES_H_INCLUDED__

#include <vector>

//Unoptimized version - uses dynamic allocations,
//Can be improved using custom implementation with stack cache
template<typename T>
class Array: public std::vector<T>
{
	public:
		T* begin() {return empty()?0:&*std::vector<T>::begin();}
		T* end  () {return empty()?0:begin()+size();}

		const T* begin() const {return empty()?0:&*std::vector<T>::begin();}
		const T* end  () const {return empty()?0:begin()+size();}

		void pushBack(const T& value) {push_back(value);}

		template<typename I>
		void pushBack(I itBegin, I itEnd) {insert(std::vector<T>::end(), itBegin, itEnd);}

		T* expand(size_t count)
		{
			resize(size()+count);
			return end()-count;
		}
};

#endif