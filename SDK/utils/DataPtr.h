#include <cstring>
#include <cassert>

class DataPtr
{
	private:
		unsigned char* ptr_;
		unsigned long size_, off_;
	public:
		DataPtr(void* ptr, unsigned long size):ptr_(reinterpret_cast<unsigned char*>(ptr)), size_(size), off_(0){}

		unsigned long tell()
		{
			return off_;
		}

		void* move(long offset)
		{
			assert(off_+offset<=size_);
			void* ptr = ptr_+off_;
			off_ += offset;
			return ptr;
		} 

		void copy(void* ptr, unsigned long size)
		{
			assert(off_+size<=size_);
			memcpy(ptr, ptr_+off_, size);
			move(size);
		}
		
		template<typename type>
		void read(type &var)
		{
			assert(off_+sizeof(type)<=size_);
			var = *reinterpret_cast<type*>(ptr_+off_);
			move(sizeof(type));
		}

		void readString(char* str, unsigned long size, char delim=0)
		{
			const unsigned char* s = ptr_+off_;
			unsigned long len=0;
			while(len<=size-1 && s[len]!=delim && off_+len<size_) ++len;
			copy(str, len);
			str[len]=0;
			move(1);
		}

		bool end()
		{
			return off_>=size_;
		}
};