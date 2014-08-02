#include <SDL2/SDL.h>
#include <core/core.h>

namespace mt
{
    static const uint32_t MAX_EVENTS = 128;
    
    static const uint32_t ID_TYPE_BITS  =  4;
    static const uint32_t ID_GEN_BITS   =  8;
    static const uint32_t ID_INDEX_BITS = 20;

    static const uint32_t ID_TYPE_OFFSET  = ID_INDEX_BITS + ID_GEN_BITS;
    static const uint32_t ID_GEN_OFFSET   = ID_INDEX_BITS;
    static const uint32_t ID_INDEX_OFFSET = 0;

    static const uint32_t ID_TYPE_MASK  = ((1 << ID_TYPE_BITS ) - 1) << ID_TYPE_OFFSET;
    static const uint32_t ID_GEN_NASK   = ((1 << ID_GEN_BITS  ) - 1) << ID_GEN_OFFSET;
    static const uint32_t ID_INDEX_MASK = ((1 << ID_INDEX_BITS) - 1) << ID_INDEX_OFFSET;

    static const uint32_t ID_TYPE_MT_EVENT = 1;

    static const uint32_t ID_INVALID_TYPE = 0x0F;

    uint32_t handleConstruct(uint32_t type, uint32_t gen, uint32_t index)
    {
        assert( type != ID_INVALID_TYPE );

        assert( type  < (1<<(ID_TYPE_BITS )) );
        assert( gen   < (1<<(ID_GEN_BITS  )) );
        assert( index < (1<<(ID_INDEX_BITS)) );

        return (type  << ID_TYPE_OFFSET ) |
               (gen   << ID_GEN_OFFSET  ) |
               (index << ID_INDEX_OFFSET);
    }

    uint32_t handleIncGen(uint32_t handle)
    {
        return (handle & ~ID_GEN_NASK) | ((handle + (1 << ID_GEN_OFFSET)) & ID_GEN_NASK);
    }

    struct FixedStack
    {
        uint32_t array[MAX_EVENTS];
        uint32_t pointer;
    };

    FixedStack freeEventID;

    struct event_t
    {
        uint32_t   handle;
        uint32_t   signaled;
        SDL_cond*  cond;
        SDL_mutex* mutex;
    };

    event_t eventPool[MAX_EVENTS];

    event_t* getEventByHandle(uint32_t eventID)
    {
        uint32_t index = eventID & ID_INDEX_OFFSET;
        if (index < MAX_EVENTS && eventPool[index].handle == eventID)
        {
            return &eventPool[index];
        }

        return 0;
    }

    uint32_t eventPoolAllocEvent()
    {
        assert(freeEventID.pointer < MAX_EVENTS);

        uint32_t handle = handleIncGen(freeEventID.array[freeEventID.pointer++]);
        uint32_t index  = handle & ID_INDEX_OFFSET;
        assert(index < MAX_EVENTS);

        eventPool[index].handle = handle;

        return handle;
    }

    void eventPoolReleaseEvent(uint32_t eventID)
    {
        assert(freeEventID.pointer > 0);
        
        event_t* event = getEventByHandle(eventID);
        assert(event);

        event->handle = INVALID_HANDLE;

        freeEventID.array[--freeEventID.pointer] = eventID;
    }

    void syncAndReleaseEvent(uint32_t handle)
    {
        event_t* event = getEventByHandle(handle);
        assert(event);

        SDL_LockMutex(event->mutex);
        if (!event->signaled)
        {
            SDL_CondWait(event->cond, event->mutex);
        }
        SDL_UnlockMutex(event->mutex);

        eventPoolReleaseEvent(handle);
    }

    void eventPoolCreate()
    {
        for (size_t i = 0; i < MAX_EVENTS; ++i)
        {
            freeEventID.array[i] = handleConstruct(ID_TYPE_MT_EVENT, 0, i);
        }

        freeEventID.pointer = 0;

        for (size_t i = 0; i < MAX_EVENTS; ++i)
        {
            eventPool[i].handle   = INVALID_HANDLE;
            eventPool[i].signaled = 0;
            eventPool[i].cond     = SDL_CreateCond();
            eventPool[i].mutex    = SDL_CreateMutex();
        }        
    }

    void eventPoolDestroy()
    {
        freeEventID.pointer = MAX_EVENTS;

        for (size_t i = 0; i < MAX_EVENTS; ++i)
        {
            eventPool[i].handle   = INVALID_HANDLE;
            eventPool[i].signaled = 0;
            SDL_DestroyCond(eventPool[i].cond);
            SDL_DestroyMutex(eventPool[i].mutex);
        }        
    }

    static const size_t MAX_POOL_THREADS = 8;

    typedef struct {
        void (*function)(void *);
        void*     argument;
        event_t*  event;
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

    int addAsyncTask(void (*taskFunc)(void *), void *arg, uint32_t* handle)
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

            event_t* event = 0;
            if (handle)
            {
                *handle = eventPoolAllocEvent();
                event = getEventByHandle(*handle);
            }

            /* Add task to queue */
            pool.queue[pool.tail].function = taskFunc;
            pool.queue[pool.tail].argument = arg;
            pool.queue[pool.tail].event    = event;
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
            task.event    = pool->queue[pool->head].event;

            pool->head += 1;
            pool->head = (pool->head == pool->queue_size) ? 0 : pool->head;
            pool->count -= 1;

            /* Unlock */
            SDL_UnlockMutex(pool->lock);

            /* Get to work */
            if (task.event)
            {
                SDL_LockMutex(task.event->mutex);
            }

            (*(task.function))(task.argument);

            if (task.event)
            {
                task.event->signaled = true;
                SDL_CondBroadcast(task.event->cond);
                SDL_UnlockMutex(task.event->mutex);
            }
        }

        pool->started--;

        SDL_UnlockMutex(pool->lock);

        return 0;
    }
}
