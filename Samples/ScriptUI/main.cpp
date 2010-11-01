#include <stdarg.h> 
#include <stdio.h> 
#include <cstring>
#include <cassert>
#include <squirrel.h> 
#include <sqrat.h>

//#include <sqstdio.h> 
//#include <sqstdaux.h> 

//#ifdef _MSC_VER
//#pragma comment (lib ,"squirrel.lib")
//#pragma comment (lib ,"sqstdlib.lib")
//#endif

//#ifdef SQUNICODE 
//
//#define scvprintf vfwprintf
//#else 

#define scvprintf vfprintf
//#endif 

void printfunc(HSQUIRRELVM v,const SQChar *s,...)
{
	va_list vl;
	va_start(vl, s);
	scvprintf(stdout, s, vl);
	va_end(vl);
}

void errorfunc(HSQUIRRELVM v,const SQChar *s,...)
{
	va_list vl;
	va_start(vl, s);
	scvprintf(stderr, s, vl);
	va_end(vl);
}

void call_foo(HSQUIRRELVM v, int n,float f,const SQChar *s)
{
	SQInteger top = sq_gettop(v); //saves the stack size before the call
	sq_pushroottable(v); //pushes the global table
	sq_pushstring(v,_SC("foo"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { //gets the field 'foo' from the global table
		sq_pushroottable(v); //push the 'this' (in this case is the global table)
		sq_pushinteger(v,n); 
		sq_pushfloat(v,f);
		sq_pushstring(v,s,-1);
		sq_call(v,4,SQFalse,SQTrue); //calls the function 
	}
	sq_settop(v,top); //restores the original stack size
}

const char* script = "printHello";//"print(\"Hello World\");";
		//"function foo(i, f, s)												\n"
		//"{																	\n"
		//"	print(\"Called foo(), i=\"+i+\", f=\"+f+\", s='\"+s+\"'\n\");	\n" 
		//"}																	\n"; 


void printHello()
{
	printf("Hello world using Sqrat!\n");
}

int sum(int a, int b)
{
	return a+b;
}

const char* config = 
	"config <- {													\n"
	"	resources = [\"../../AppData\", \"packedResources.pkg\"]	\n"
	"	width = 1280												\n"
	"	height = 720												\n"
	"	fullscreen = false											\n"
	"	vsync = false												\n"
	"	startScript = \"vgUI.nut\"									\n"
	"};																\n";

#include "Common.h"
#include "Object.h"
#include "Function.h"

namespace sqlink
{
	//goal: parity with sqrat
	//TODO:
	//sqlink::Var +/-
	//sqlink::Object +/-
	//sqlink::Function +/- 
	//sqlink::Function sqlink::compile(const char*) +/-
	//sqlink::BinderIface

	class Table: public Object
	{
		public:
			Table()
			{}

			Table(const sqrat::Object& object):Object(object)
			{
				assert(sq_istable(obj));
			}

			Table(HSQUIRRELVM vm, HSQOBJECT object):Object(vm, object)
			{
				assert(sq_istable(obj));
			}

			template<typename T>
			T get(const SQChar* slot, T defaultValue = T())
			{
				sq_pushobject(vm, GetObject());
				sq_pushstring(vm, slot, -1);

				if(SQ_FAILED(sq_get(vm, -2)))
				{
					sq_pop(vm, 1);
					return defaultValue;
				}
				else
				{
					T ret = sqlink::Type<T>::read(vm, -1);
					sq_pop(vm, 2);
					return ret;
				}
			}

			bool exist(const SQChar* slot)
			{
				bool succeeded;

				sq_pushobject(vm, GetObject());
				sq_pushstring(vm, slot, -1);
				succeeded = SQ_SUCCEEDED(sq_get(vm, -2));
				sq_pop(vm, succeeded?2:1);

				return succeeded;
			}
	};

	Table getRootTable(HSQUIRRELVM vm)
	{
		HSQOBJECT obj;

		sq_pushroottable(vm);
		sq_getstackobj(vm, -1, &obj);
		sq_pop(vm, 1);

		return Table(vm, obj);
	}
}

namespace sqlink
{
	class Array: public Object
	{
		public:
			Array()
			{}

			Array(const sqrat::Object& object):Object(object)
			{
				assert(sq_isarray(obj));
			}

			Array(HSQUIRRELVM vm, HSQOBJECT object):Object(vm, object)
			{
				assert(sq_isarray(obj));
			}

			Object at(size_t i)
			{
				HSQOBJECT slotObj;
				sq_pushobject(vm, GetObject());
				sq_pushinteger(vm, i);
				if(SQ_FAILED(sq_get(vm, -2))) {
					sq_pop(vm, 1);
					return sqrat::Object(); // Return a NULL object
				} else {
					sq_getstackobj(vm, -1, &slotObj);
					sq_pop(vm, 2);
					return Object(vm, slotObj);
				}
			}

			Object operator[] (size_t i)
			{
				return at(i);
			}

			size_t size()
			{
				size_t result;

				sq_pushobject(vm, GetObject());
				result = sq_getsize(vm, -1);
				sq_pop(vm, 1);

				return result;
			}
	};
}

namespace sqlink
{
	template <typename T>
	struct Type
	{
		static T read(HSQUIRRELVM vm, SQInteger idx)
		{
			return sqrat::Var<T>(vm, -1).value;
		}

		static void push(HSQUIRRELVM vm, T value)
		{
			sqrat::Var::push(vm, value);
		}
	};
}

namespace sqlink
{
	template<>
	struct Type<Table>
	{
		static Table read(HSQUIRRELVM vm, SQInteger idx)
		{
			HSQOBJECT obj;
			sq_getstackobj(vm, idx, &obj);
			return Table(vm, obj);
		}

		static void push(HSQUIRRELVM vm, Table& value)
		{
			sq_pushobject(vm, value.GetObject());
		}
	};

	template<>
	struct Type<Array>
	{
		static Array read(HSQUIRRELVM vm, SQInteger idx)
		{
			HSQOBJECT obj;
			sq_getstackobj(vm, idx, &obj);
			return Array(vm, obj);
		}

		static void push(HSQUIRRELVM vm, Table& value)
		{
			sq_pushobject(vm, value.GetObject());
		}
	};
}

const char* testClass = 
	"cls <- class {													\n"
	"	height = 720												\n"
	"};																\n"
	"inst <- cls()													\n";

int main(int argc, char* argv[]) 
{ 
	HSQUIRRELVM v; 
	v = sq_open(1024); // creates a VM with initial stack size 1024 
	
	{
		sqlink::Table root = sqlink::getRootTable(v);
		sqlink::Function script = sqlink::compileString(v, testClass);
	}

	//REGISTRATION OF STDLIB
	//sq_pushroottable(v); //push the root table where the std function will be registered
	//sqstd_register_iolib(v);  //registers a library
	// ... call here other stdlibs string,math etc...
	//sq_pop(v,1); //pops the root table
	//END REGISTRATION OF STDLIB
	
	//sqstd_seterrorhandlers(v); //registers the default error handlers

	sq_setprintfunc(v, printfunc,errorfunc); //sets the print function

	sqrat::RootTable(v).Func("printHello", &printHello);
	sqrat::RootTable(v).Func("sum", &sum);

	sqlink::Table root = sqlink::getRootTable(v);
	sqlink::Function script = sqlink::compileString(v, config);

	if (script.isNull())
	{
		const SQChar* msg = sqlink::getLastErrorDesc(v);
		printf("Error compiling script:\n\t%s", msg);
	}

	script(root);
	if (const SQChar* msg = sqlink::getLastErrorDesc(v))
	{
		printf("Error executing script:\n\t%s", msg);
	}

	{
		sqlink::Table conf = sqlink::getRootTable(v).get<sqlink::Table>("config");
		sqlink::Array resList = conf.get<sqlink::Array>("resources");
		size_t sz = resList.size();
		assert(sz==2);
		const char* dir;
		dir = resList[0].as<const char*>();
		dir = resList[1].as<const char*>();

		int width = conf.get<int>("width", 1);
		int height = conf.get<int>("height", 1);
		bool fullscreen = conf.get<bool>("fullscreen", true);
		bool vsync = conf.get<bool>("vsync", true);
		const char* startScript = conf.get<const char*>("startScript", "default.nut");
	}

	//sq_pushroottable(v); //push the root table(were the globals of the script will be stored)
	//if(SQ_SUCCEEDED(sqstd_dofile(v, _SC("test.nut"), SQFalse, SQTrue))) // also prints syntax errors if any 
	//if (SQ_SUCCEEDED(sq_compilebuffer(v, script, strlen(script), "test.nut", SQTrue)))
	//{
	//	sq_pushroottable(v); //push the root table(were the globals of the script will be stored)
	//	//call_foo(v,1,2.5,_SC("teststring"));
	//	//assert(SQ_SUCCEEDED(sq_call(v,1,SQFalse,SQTrue))); //calls the function 
	//}	

	//sq_pop(v,1); //pops the root table
	sq_close(v); 

	return 0; 
} 
