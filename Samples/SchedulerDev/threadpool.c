/*
 * Copyright (c) 2011, Mathias Brossard <mathias@brossard.org>.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file threadpool.c
 * @brief Threadpool implementation file
 */

#include <assert.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "threadpool.h"

#define MAX_POOL_THREADS 8

/**
 *  @struct threadpool_task
 *  @brief the work struct
 *
 *  @var function Pointer to the function that will perform the task.
 *  @var argument Argument to be passed to the function.
 */

typedef struct {
    void (*function)(void *);
    void *argument;
} threadpool_task_t;

/**
 *  @struct threadpool
 *  @brief The threadpool struct
 *
 *  @var notify       Condition variable to notify worker threads.
 *  @var threads      Array containing worker threads ID.
 *  @var thread_count Number of threads
 *  @var queue        Array containing the task queue.
 *  @var queue_size   Size of the task queue.
 *  @var head         Index of the first element.
 *  @var tail         Index of the next element.
 *  @var shutdown     Flag indicating if the pool is shutting down
 */
struct threadpool_t {
  SDL_mutex*          lock;
  SDL_cond*           notify;
  SDL_Thread*         threads[MAX_POOL_THREADS];
  threadpool_task_t*  queue;
  int thread_count;
  int queue_size;
  int head;
  int tail;
  int count;
  int shutdown;
  int started;
};

/**
 * @function void *threadpool_thread(void *threadpool)
 * @brief the worker thread
 * @param threadpool the pool which own the thread
 */
static int SDLCALL threadpool_thread(void *threadpool);

int threadpool_free(threadpool_t *pool);

threadpool_t *threadpool_create(int thread_count, int queue_size, int flags)
{
    int           i;
    threadpool_t* pool;
    char          threadName[8] = "Worker\0";


    /* TODO: Check for negative or otherwise very big input parameters */

    if((pool = (threadpool_t*)malloc(sizeof(threadpool_t))) == NULL)
    {
        goto err;
    }

    assert(thread_count < MAX_POOL_THREADS);

    /* Initialize */
    pool->thread_count = thread_count;
    pool->queue_size   = queue_size;
    pool->head     = pool->tail = pool->count = 0;
    pool->shutdown = pool->started = 0;

    /* Allocate thread and task queue */
    pool->queue = (threadpool_task_t*)malloc(sizeof(threadpool_task_t) * queue_size);

    pool->lock   = SDL_CreateMutex();
    pool->notify = SDL_CreateCond();

    /* Initialize mutex and conditional variable first */
    if (pool->lock    == NULL  ||
        pool->notify  == NULL  ||
        pool->threads == NULL  ||
        pool->queue   == NULL)
    {
        goto err;
    }

    for(i = 0; i < thread_count; i++)
    {
        threadName[6] = 0x30+i;

        pool->threads[i] = SDL_CreateThread(threadpool_thread, threadName, pool);
        if (pool->threads[i] == 0)
        {
            goto err;
        }

        pool->started++;
    }

    return pool;

err:
    if (pool) threadpool_free(pool);

    return NULL;
}

int threadpool_add(threadpool_t *pool, void (*function)(void *), void *argument, int flags)
{
    int err = 0;
    int next;

    if(pool == NULL || function == NULL)
    {
        return threadpool_invalid;
    }

    if(SDL_LockMutex(pool->lock) != 0)
    {
        return threadpool_lock_failure;
    }

    next = pool->tail + 1;
    next = (next == pool->queue_size) ? 0 : next;

    do
    {
        /* Are we full? */
        if(pool->count == pool->queue_size)
        {
            err = threadpool_queue_full;
            break;
        }

        /* Are we shutting down? */
        if(pool->shutdown)
        {
            err = threadpool_shutdown;
            break;
        }

        /* Add task to queue */
        pool->queue[pool->tail].function = function;
        pool->queue[pool->tail].argument = argument;
        pool->tail = next;
        pool->count += 1;

        /* pthread_cond_broadcast */
        if (SDL_CondSignal(pool->notify) != 0)
        {
            err = threadpool_lock_failure;
            break;
        }
    }
    while(0);

    if(SDL_UnlockMutex(pool->lock) != 0)
    {
        err = threadpool_lock_failure;
    }

    return err;
}

int threadpool_destroy(threadpool_t *pool, int flags)
{
    int i, err = 0;

    if (pool == NULL)
    {
        return threadpool_invalid;
    }

    if (SDL_LockMutex(pool->lock) != 0)
    {
        return threadpool_lock_failure;
    }

    do
    {
        /* Already shutting down */
        if(pool->shutdown)
        {
            err = threadpool_shutdown;
            break;
        }

        pool->shutdown = 1;

        /* Wake up all worker threads */
        if (SDL_CondBroadcast(pool->notify) != 0 ||
            SDL_UnlockMutex(pool->lock) != 0)
        {
            err = threadpool_lock_failure;
            break;
        }

        /* Join all worker thread */
        for(i = 0; i < pool->thread_count; i++)
        {
            SDL_WaitThread(pool->threads[i], NULL);
        }
    }
    while(0);

    if(SDL_UnlockMutex(pool->lock) != 0)
    {
        err = threadpool_lock_failure;
    }
    
    /* Only if everything went well do we deallocate the pool */
    if(!err)
    {
        threadpool_free(pool);
    }
    return err;
}

int threadpool_free(threadpool_t *pool)
{
    if(pool == NULL || pool->started > 0)
    {
        return -1;
    }

    /* Did we manage to allocate ? */
    if (pool->threads)
    {
        free(pool->queue);
 
        /* Because we allocate pool->threads after initializing the
           mutex and condition variable, we're sure they're
           initialized. Let's lock the mutex just in case. */
        SDL_LockMutex(pool->lock);
        SDL_DestroyMutex(pool->lock);
        SDL_DestroyCond(pool->notify);
    }

    free(pool);    

    return 0;
}


static int SDLCALL threadpool_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t*)threadpool;
    threadpool_task_t task;

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
