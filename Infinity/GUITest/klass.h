#pragma once

namespace exttype
{
	template<typename T>
	class klass;

	// GObject wrapper?????????
	// yes indeed

	class base
	{
	public:
		virtual void  destroy(void* instance) = 0;
		virtual void* clone(void* other) = 0;
	};

	template<typename T>
	class klass: base
	{
		void  destroy(void* instance)
		{delete (T*)instance}

		void* clone(void* other)
		{new T((T*)other);}
	};

	template<typename T>
	class declare
	{
	public:
		static klass<T>	mInstance;
	};

	template<typename T>
	klass<T> declare<T>::mInstance;

	class any
	{
		template<typename T> friend T as(any& value);
		template<typename T> friend bool is(const any& value);

	public:
		template<typename T>
		any(const T& value): 
			mKlass(&declare<T>::mInstance),
			mInstace(&value)
		{}

	private:
		base* mKlass;
		void* mInstance;
	};

	template<typename T>
	bool is(const any& value)
	{
		return &declare<T>::mInstance == any.mKlass;
	}

	template<typename T> 
	T& as(any& value)
	{
		assert(is<T>(value));
		return *static_cast<T*>(value.mInstance);
	}


}