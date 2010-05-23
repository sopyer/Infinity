#ifndef __SCHEDULER_H_INCLUDED__
#	define __SCHEDULER_H_INCLUDED__

struct SDL_Thread;
struct SDL_mutex;

namespace mt
{
	struct SchedEntry;
}

namespace mt
{
	typedef SchedEntry*	Task;
	typedef SDL_Thread*	Thread;
	typedef SDL_mutex*	Mutex;

	typedef void (*TaskFunc)(void*);
	typedef void (*TimedTaskFunc)(void*);

	//Initializes scheduler subsystem
	void init();

	//Shuts down scheduler subsystem
	void cleanup();

	//Adds task to execution queue
	//void addTask(TaskFunc func, void* userData/*, size_t flags*/);

	//Adds timed task to execution queue
	Task addTimedTask(TimedTaskFunc func, void* userData, size_t msTimeout/*, size_t flags*/);

	//Dispatches tasks for execution
	void mainLoop();

	void terminateLoop();
	void terminateTask(Task handle);

    template <class T, void (T::*callback)()>
    void taskFuncStub(void* objPtr)
    {
        T* p = static_cast<T*>(objPtr);
        (p->*callback)();
	}

	template<class T, void (T::*callback)()>
	Task addTimedTask(T* obj, size_t msTimeout)
	{
		return addTimedTask(&taskFuncStub<T, callback>, obj, msTimeout);
	}

	Thread createThread(int (__cdecl *fn)(void *), void *data);
	void waitThread(Thread thread, int *status);

	template <class T, void (T::*callback)()>
	int __cdecl threadFuncStub(void* objPtr)
	{
		T* p = static_cast<T*>(objPtr);
		(p->*callback)();
		return 0;
	}

	template<class T, void (T::*callback)()>
	Thread createThread(T* obj)
	{
		return createThread(&threadFuncStub<T, callback>, obj);
	}

	Mutex createMutex();
	int lockMutex(Mutex mtx);
	int unlockMutex(Mutex mtx);
	void destroyMutex(Mutex mtx);

	class Lock
	{
		public:
			Lock(Mutex mtx):mMutex(mtx)
			{
				lockMutex(mMutex);
			}
			~Lock()
			{
				unlockMutex(mMutex);
			}

		private:
			Mutex	mMutex;
	};
}

#endif