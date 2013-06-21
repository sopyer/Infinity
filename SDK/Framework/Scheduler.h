#ifndef __FRAMEWORK_SCHEDULER_H_INCLUDED__
#	define __FRAMEWORK_SCHEDULER_H_INCLUDED__

// Implementation is based on Mathias Brossard threadpool (https://github.com/mbrossard/threadpool).

namespace mt
{
    typedef enum {
        noError       =  0,
        invalidValue  = -1,
        lockFailure   = -2,
        queueFull     = -3,
        shutdown      = -4,
        threadFailure = -5
    } error_t;

    /**
     * @param thread_count Number of worker threads.
     * @param queue_size   Size of the queue.
     * @param flags        Unused parameter.
     */
    void init(int threadCount, int queueSize);
    void fini();

    /**
     * @brief add a new task in the queue of a thread pool
     * @param taskFunc Pointer to the function that will perform the task.
     * @param arg      Argument to be passed to the function.
     * @param flags    Unused parameter.
     * @return 0 if all goes well, negative values in case of error (@see error_t for codes).
     */
    int addAsyncTask(void (*taskFunc)(void *), void *arg);

    template <class T, void (T::*callback)()>
    void taskFuncStub(void* objPtr)
    {
        T* p = static_cast<T*>(objPtr);
        (p->*callback)();
    }

    template<class T, void (T::*callback)()>
    int addAsyncTask(T* obj)
    {
        return addFrameTask(&taskFuncStub<T, callback>, obj);
    }
}

#endif