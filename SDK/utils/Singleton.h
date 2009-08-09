#ifndef _Singleton_h_
#	define _Singleton_h_

#	include <cassert>

template <typename T>
class Singleton
{
	protected:
		static T* ms_Singleton;
	
	public:
		Singleton( void ){
			assert( !ms_Singleton );
			ms_Singleton = static_cast<T*>(this);
		}
		~Singleton( void )
			{ assert( ms_Singleton );  ms_Singleton = 0; }
	    
		static T& getRef( void )
			{ assert( ms_Singleton );  return ( *ms_Singleton ); }
		static T* getPtr( void )
			{ return ( ms_Singleton ); }
};

template <typename T>
T* Singleton<T>::ms_Singleton = 0;

#endif
