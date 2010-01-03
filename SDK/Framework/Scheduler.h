#ifndef __SCHEDULER_H_INCLUDED__
#	define __SCHEDULER_H_INCLUDED__

namespace scheduler
{
	struct SchedEntry;
	typedef SchedEntry*	Task;

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
}

#endif