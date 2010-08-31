#ifndef __SQLINK_FUNCTION_H_INCLUDED__
#	define __SQLINK_FUNCTION_H_INCLUDED__

namespace sqlink
{
	class Function: public Object
	{
		public:
			Function()
			{}

			Function(HSQUIRRELVM vm, HSQOBJECT object): Object(vm, object)
			{
				assert(sq_isclosure(obj) || sq_isnativeclosure(obj));
			}

			template <class R, class A1>
			R evaluate(A1 a1)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);

				sq_call(vm, 1, true, ErrorHandling::isEnabled());
				R ret = Type<R>::read(vm, -1);
				sq_pop(vm, 2);

				return ret;
			}

			template <class R, class A1, class A2>
			R evaluate(A1 a1, A2 a2)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);

				sq_call(vm, 2, true, ErrorHandling::isEnabled());
				R ret = Type<R>::read(vm, -1);
				sq_pop(vm, 2);
				return ret;
			}

			template <class R, class A1, class A2, class A3>
			R evaluate(A1 a1, A2 a2, A3 a3)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);

				sq_call(vm, 3, true, ErrorHandling::isEnabled());
				R ret = Type<R>::read(vm, -1);
				sq_pop(vm, 2);
				return ret;
			}

			template <class R, class A1, class A2, class A3, class A4>
			R evaluate(A1 a1, A2 a2, A3 a3, A4 a4)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);

				sq_call(vm, 4, true, ErrorHandling::isEnabled());
				R ret = Type<R>::read(vm, -1);
				sq_pop(vm, 2);
				return ret;
			}

		
			template <class R, class A1, class A2, class A3, class A4, class A5>
			R evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);
				Type<A5>::push(vm, a5);

				sq_call(vm, 5, true, ErrorHandling::isEnabled());
				R ret = Type<R>::read(vm, -1);
				sq_pop(vm, 2);
				return ret;
			}

			template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
			R evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);
				Type<A5>::push(vm, a5);
				Type<A6>::push(vm, a6);

				sq_call(vm, 6, true, ErrorHandling::isEnabled());
				R ret = Type<R>::read(vm, -1);
				sq_pop(vm, 2);
				return ret;
			}

			template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
			R evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);
				Type<A5>::push(vm, a5);
				Type<A6>::push(vm, a6);
				Type<A7>::push(vm, a7);

				sq_call(vm, 7, true, ErrorHandling::isEnabled());
				R ret = Type<R>::read(vm, -1);
				sq_pop(vm, 2);
				return ret;
			}

			template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
			R evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);
				Type<A5>::push(vm, a5);
				Type<A6>::push(vm, a6);
				Type<A7>::push(vm, a7);
				Type<A8>::push(vm, a8);

				sq_call(vm, 8, true, ErrorHandling::isEnabled());
				R ret = Type<R>::read(vm, -1);
				sq_pop(vm, 2);
				return ret;
			}

			template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
			R evaluate(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);
				Type<A5>::push(vm, a5);
				Type<A6>::push(vm, a6);
				Type<A7>::push(vm, a7);
				Type<A8>::push(vm, a8);
				Type<A9>::push(vm, a9);

				sq_call(vm, 9, true, ErrorHandling::isEnabled());
				R ret = Type<R>::read(vm, -1);
				sq_pop(vm, 2);
				return ret;
			}

			template <class A1>
			void execute(A1 a1)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);

				sq_call(vm, 1, false, ErrorHandling::isEnabled());
				sq_pop(vm, 1);
			}

			template <class A1, class A2>
			void execute(A1 a1, A2 a2)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);

				sq_call(vm, 2, false, ErrorHandling::isEnabled());
				sq_pop(vm, 1);
			}

			template <class A1, class A2, class A3>
			void execute(A1 a1, A2 a2, A3 a3)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);

				sq_call(vm, 3, false, ErrorHandling::isEnabled());
				sq_pop(vm, 1);
			}

			template <class A1, class A2, class A3, class A4>
			void execute(A1 a1, A2 a2, A3 a3, A4 a4)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);

				sq_call(vm, 4, false, ErrorHandling::isEnabled());
				sq_pop(vm, 1);
			}

		
			template <class A1, class A2, class A3, class A4, class A5>
			void execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);
				Type<A5>::push(vm, a5);

				sq_call(vm, 5, false, ErrorHandling::isEnabled());
				sq_pop(vm, 1);
			}

			template <class A1, class A2, class A3, class A4, class A5, class A6>
			void execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);
				Type<A5>::push(vm, a5);
				Type<A6>::push(vm, a6);

				sq_call(vm, 6, false, ErrorHandling::isEnabled());
				sq_pop(vm, 1);
			}

			template <class A1, class A2, class A3, class A4, class A5, class A6, class A7>
			void execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);
				Type<A5>::push(vm, a5);
				Type<A6>::push(vm, a6);
				Type<A7>::push(vm, a7);

				sq_call(vm, 7, false, ErrorHandling::isEnabled());
				sq_pop(vm, 1);
			}

			template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
			void execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);
				Type<A5>::push(vm, a5);
				Type<A6>::push(vm, a6);
				Type<A7>::push(vm, a7);
				Type<A8>::push(vm, a8);

				sq_call(vm, 8, false, ErrorHandling::isEnabled());
				sq_pop(vm, 1);
			}

			template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
			void execute(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
			{
				sq_pushobject(vm, obj);

				Type<A1>::push(vm, a1);
				Type<A2>::push(vm, a2);
				Type<A3>::push(vm, a3);
				Type<A4>::push(vm, a4);
				Type<A5>::push(vm, a5);
				Type<A6>::push(vm, a6);
				Type<A7>::push(vm, a7);
				Type<A8>::push(vm, a8);
				Type<A9>::push(vm, a9);

				sq_call(vm, 9, false, ErrorHandling::isEnabled());
				sq_pop(vm, 1);
			}
		
			template <class A1>
			void operator()(A1 a1)
			{
				execute(a1);
			}

			template <class A1, class A2>
			void operator()(A1 a1, A2 a2)
			{
				execute(a1, a2);
			}

			template <class A1, class A2, class A3>
			void operator()(A1 a1, A2 a2, A3 a3)
			{
				execute(a1, a2, a3);
			}

			template <class A1, class A2, class A3, class A4>
			void operator()(A1 a1, A2 a2, A3 a3, A4 a4)
			{
				execute(a1, a2, a3, a4);
			}

		
			template <class A1, class A2, class A3, class A4, class A5>
			void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
			{
				execute(a1, a2, a3, a4, a5);
			}

			template <class A1, class A2, class A3, class A4, class A5, class A6>
			void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
			{
				execute(a1, a2, a3, a4, a5, a6);
			}

			template <class A1, class A2, class A3, class A4, class A5, class A6, class A7>
			void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
			{
				execute(a1, a2, a3, a4, a5, a6, a7);
			}

			template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
			void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
			{
				execute(a1, a2, a3, a4, a5, a6, a7, a8);
			}

			template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
			void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
			{
				execute(a1, a2, a3, a4, a5, a6, a7, a8, a9);
			}
	};

	Function compileString(HSQUIRRELVM vm, const SQChar* scriptSource, const SQChar* scriptName = _SC(""))
	{
		if(SQ_FAILED(sq_compilebuffer(vm, scriptSource, strlen(scriptSource) * sizeof(SQChar), scriptName, true)))
			return Function();

		HSQOBJECT obj;
		sq_getstackobj(vm, -1, &obj);

		return Function(vm, obj);
	}

}

#endif
