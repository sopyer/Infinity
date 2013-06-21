#include "Scheduler.h"
#include <assert.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

namespace mt
{
    #define MAX_POOL_THREADS 8

    typedef struct {
        void (*function)(void *);
        void *argument;
    } task_t;

    struct threadpool_t
    {
        SDL_mutex*  lock;
        SDL_cond*   notify;
        SDL_Thread* threads[MAX_POOL_THREADS];
        task_t*     queue;
        int         thread_count;
        int         queue_size;
        int         head;
        int         tail;
        int         count;
        int         shutdown;
        int         started;
    };

    static int SDLCALL threadpoolThread(void* poolPtr);
    static void releaseMTResources();

    threadpool_t pool;

    void init(int thread_count, int queue_size)
    {
        int  i;
        char threadName[8] = "Worker\0";

        assert(thread_count < MAX_POOL_THREADS);

        memset(&pool, 0, sizeof(threadpool_t));

        /* Initialize */
        pool.thread_count = thread_count;
        pool.queue_size   = queue_size;

        /* Allocate thread and task queue */
        pool.queue = (task_t*)malloc(sizeof(task_t) * queue_size);

        pool.lock   = SDL_CreateMutex();
        pool.notify = SDL_CreateCond();

        /* Initialize mutex and conditional variable first */
        if (pool.lock    == NULL  ||
            pool.notify  == NULL  ||
            pool.queue   == NULL)
        {
            goto err;
        }

        for(i = 0; i < thread_count; i++)
        {
            threadName[6] = 0x30+i;

            pool.threads[i] = SDL_CreateThread(threadpoolThread, threadName, &pool);
            if (pool.threads[i] == 0)
            {
                goto err;
            }

            pool.started++;
        }

        return;

    err:
        releaseMTResources();
    }

    void fini()
    {
        if (SDL_LockMutex(pool.lock) != 0)
        {
            return;
        }

        do
        {
            /* Already shutting down */
            if(pool.shutdown)
            {
                break;
            }

            pool.shutdown = 1;

            /* Wake up all worker threads */
            if (SDL_CondBroadcast(pool.notify) != 0 ||
                SDL_UnlockMutex(pool.lock) != 0)
            {
                break;
            }

            /* Join all worker thread */
            for(int i = 0; i < pool.thread_count; i++)
            {
                SDL_WaitThread(pool.threads[i], NULL);
            }
        }
        while(0);

        SDL_UnlockMutex(pool.lock);
    
        releaseMTResources();
    }

    int addAsyncTask(void (*taskFunc)(void *), void *arg)
    {
        int err = 0;
        int next;

        if(taskFunc == NULL)
        {
            return invalidValue;
        }

        if(SDL_LockMutex(pool.lock) != 0)
        {
            return lockFailure;
        }

        next = pool.tail + 1;
        next = (next == pool.queue_size) ? 0 : next;

        do
        {
            /* Are we full? */
            if(pool.count == pool.queue_size)
            {
                err = queueFull;
                break;
            }

            /* Are we shutting down? */
            if(pool.shutdown)
            {
                err = shutdown;
                break;
            }

            /* Add task to queue */
            pool.queue[pool.tail].function = taskFunc;
            pool.queue[pool.tail].argument = arg;
            pool.tail = next;
            pool.count += 1;

            /* pthread_cond_broadcast */
            if (SDL_CondSignal(pool.notify) != 0)
            {
                err = lockFailure;
                break;
            }
        }
        while(0);

        if(SDL_UnlockMutex(pool.lock) != 0)
        {
            err = lockFailure;
        }

        return err;
    }

    static void releaseMTResources()
    {
        assert(pool.started <= 0);

        free(pool.queue);
 
        SDL_LockMutex(pool.lock);
        SDL_DestroyMutex(pool.lock);
        SDL_DestroyCond(pool.notify);

        memset(&pool, 0, sizeof(threadpool_t));
    }

    static int SDLCALL threadpoolThread(void* poolPtr)
    {
        threadpool_t*   pool = (threadpool_t*)poolPtr;
        task_t          task;

        for(;;)
        {
            /* Lock must be taken to wait on conditional variable */
            SDL_LockMutex(pool->lock);

            /* Wait on condition variable, check for spurious wakeups.
               When returning from pthread_cond_wait(), we own the lock. */
            while((pool->count == 0) && (!pool->shutdown))
            {
                SDL_CondWait(pool->notify, pool->lock);
            }

            if(pool->shutdown)
            {
                break;
            }

            /* Grab our task */
            task.function = pool->queue[pool->head].function;
            task.argument = pool->queue[pool->head].argument;
            pool->head += 1;
            pool->head = (pool->head == pool->queue_size) ? 0 : pool->head;
            pool->count -= 1;

            /* Unlock */
            SDL_UnlockMutex(pool->lock);

            /* Get to work */
            (*(task.function))(task.argument);
        }

        pool->started--;

        SDL_UnlockMutex(pool->lock);

        return 0;
    }
}
