#ifndef __SQLINK_COMMON_H_INCLUDED__
#	define __SQLINK_COMMON_H_INCLUDED__

namespace sqlink
{
	template <typename T>
	struct Type;

	class ErrorHandling
	{
		private:
			static bool& errorHandling()
			{
				static bool eh = true;
				return eh;
			}

		public:
			static bool isEnabled()
			{
				return errorHandling();
			}

			static void enable(bool enable)
			{
				errorHandling() = enable;
			}
	};

	inline const SQChar* getLastErrorDesc( HSQUIRRELVM vm )
	{
		const SQChar* sqErr;
		
		sq_getlasterror(vm);
		
		if(sq_gettype(vm, -1) == OT_NULL)
		{
			return 0;
		}
		
		sq_tostring(vm, -1);
		sq_getstring(vm, -1, &sqErr);
		
		return sqErr;
	}

}

#endif