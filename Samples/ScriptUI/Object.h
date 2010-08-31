#ifndef __SQLINK_OBJECT_H_INCLUDED__
#	define __SQLINK_OBJECT_H_INCLUDED__

namespace sqlink
{
	class Object: public sqrat::Object
	{
		public:
			Object()
			{}

			Object(const sqrat::Object& object): sqrat::Object(object)
			{}

			Object(HSQUIRRELVM vm, HSQOBJECT object): sqrat::Object(object, vm)
			{}

			template <typename T>
			T as()
			{
				sq_pushobject(vm, obj);
				T ret = sqlink::Type<T>::read(vm, -1);
				sq_pop(vm, 1);
				return ret;
			}

			void addRef()
			{
				sq_addref(vm, &obj);
			}

			SQBool decRef()
			{
				return sq_release(vm, &obj);
			}

			bool isNull()
			{
				return sq_isnull(obj);
			}
	};
}

#endif