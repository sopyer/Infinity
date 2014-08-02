#pragma once

#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <physfs/physfs.h>

#include <core/mt.h>
#include <core/ml.h>
#include <core/profiler.h>
#include <core/timer.h>

#define UNUSED(var)         ((void)(var))
#define ARRAY_SIZE(arr)     sizeof(arr)/sizeof(arr[0])
#define BUFFER_OFFSET(i)    ((char*)NULL + (i))
#define MEMORY_T_INITIALIZER  {0, 0, 0}

char* cpToUTF8(int cp, char* str);

struct point_t
{
    float x, y;
};

struct rect_t
{
    float x, y;
    float w, h;
};

bool testPtInRect(const point_t& pt, const rect_t& rect);
bool testPtInRect(float xp, float yp, float xr, float yr, float wr, float hr);

struct memory_t
{
    uint8_t* buffer;
    size_t   size;
    size_t   allocated;
};

bool mem_area(memory_t* mem, size_t size);
bool mem_file(memory_t* mem, const char* name);
void mem_free(memory_t* mem);

bool mem_thread_stack_init(memory_t* mem, size_t size);

template <typename type>
type mem_read(memory_t* mem)
{
    type* value = (type*)(mem->buffer + mem->allocated);

    mem->allocated += sizeof(type);
    assert(mem->allocated<=mem->size);

    return *value;
}

template <typename type>
type* mem_raw_array(memory_t* mem, size_t count)
{
    uint8_t* var = mem->buffer + mem->allocated;

    mem->allocated += sizeof(type)*count;
    assert(mem->allocated<=mem->size);

    return (type*)var;
}

template <typename type>
type* mem_raw_data(memory_t* mem)
{
    uint8_t* var = mem->buffer + mem->allocated;

    mem->allocated += sizeof(type);
    assert(mem->allocated<=mem->size);

    return (type*)var;
}

#define TRUE  1
#define FALSE 0

namespace ut
{
    template<size_t N>
    struct moving_avg_filter_t
    {
        size_t active;
        float  samples[N];
    };

    template<size_t N>
    inline void moving_avg_filter_reset(moving_avg_filter_t<N>* filter)
    {
        memset(filter, 0, sizeof(filter));
    }

    template<size_t N>
    inline float moving_avg_filter(moving_avg_filter_t<N>* filter, float sample)
    {
        size_t pos = filter->active++ % N;

        filter->samples[pos] = sample;

        float sum = 0.0f;
        for (size_t i = 0; i < N; ++i)
        {
            sum += sample[i];
        }

        return sum / N;
    }

    ////////////////////////////////////////////////////////////////////////////////////////

    inline float exp_avg_calcLambda(float dt, float cutoffFreq)
    {
        return  1.0f / (1.0f + FLT_1_OVER_2_PI / (cutoffFreq * dt));
    }

    inline float exp_avg_filter(float* yn_1, float lambda, float sample)
    {
        *yn_1 = lambda * (sample - *yn_1) + *yn_1;

        return *yn_1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////

    struct one_euro_filter_t
    {
        float dxn_1;
        float yn_1;
        float cutoffFreq;
        float dcutoffFreq;
        float dslope;
    };

    inline float one_euro_filter_accum(one_euro_filter_t* filter, float dt, float sample)
    {
        float lambda;

        lambda = exp_avg_calcLambda(dt, filter->dcutoffFreq);
        filter->dxn_1 = exp_avg_filter(&filter->dxn_1, lambda, sample - filter->yn_1);
        lambda = exp_avg_calcLambda(dt, filter->cutoffFreq + filter->dslope * ml::abs(filter->dxn_1));
        filter->yn_1 = exp_avg_filter(&filter->yn_1, lambda, sample);
    }
}

namespace ut
{
    template<typename type_t, size_t N>
    struct index_t
    {
        uint32_t used;
        type_t   data[N];
    };

    template<typename type_t, size_t N>
    size_t index_lookup(index_t<type_t, N>* index, type_t key, size_t defValue);


    template<typename type_t, size_t N>
    size_t index_lookup(index_t<type_t, N>* index, type_t key, size_t defValue)
    {
        size_t  idx   = 0;
        size_t  count = index->used;
        type_t* keys  = index->data;

        for (; idx < count; ++idx)
        {
            if (keys[idx]==key)
            {
                return idx;
            }
        }

        return defValue;
    }

    template<typename type_t, size_t N>
    size_t index_lookup_or_add(index_t<type_t, N>* index, type_t key)
    {

        size_t  idx   = 0;
        size_t  count = index->used;
        type_t* keys  = index->data;

        for (; idx < count; ++idx)
        {
            if (keys[idx]==key)
            {
                return idx;
            }
        }

        assert(idx < N);

        keys[idx] = key;
        
        ++index->used;

        return idx;
    }
}

struct stack_mem_data_t;
typedef stack_mem_data_t* stack_mem_t;

void  stack_mem_init(stack_mem_t* stack, void* mem, size_t size);
void* stack_mem_alloc(stack_mem_t stack, size_t size, size_t align = 0);
void  stack_mem_reset(stack_mem_t stack, void* ptr);

template<typename T>
T* stack_mem_alloc(stack_mem_t stack, size_t count, size_t align = 0)
{
    return (T*)stack_mem_alloc(stack, count*sizeof(T), align);
}

#define STATIC_ASSERT(e) typedef char __C_ASSERT__[(e)?1:-1]

namespace ut
{
    void init();
    void fini();

    //TODO: make mt!!!!!
    stack_mem_t get_thread_data_stack();
    void* thread_stack_alloc(size_t size, size_t align = 0);
    void  thread_stack_reset(void* ptr);

    template<typename T>
    inline T min(T x, T y)
    {
        return (x < y) ? x : y;
    }

    template<typename T>
    inline T max(T x, T y)
    {
        return (x > y) ? x : y;
    }

    template<typename T>
    inline void swap(T& x, T& y)
    {
        T tmp = x;
        x = y;
        y = tmp;
    }

/*------------------ Ring buffer--------------------*/
    template<typename T, size_t N>
    struct ring_buffer_t
    {
        size_t head;
        size_t tail;
        T      storage[N];
    };

    template<typename T, size_t N>
    void ring_buffer_reset(ring_buffer_t<T, N>& rb)
    {
        STATIC_ASSERT((N & (N-1)) == 0);
        rb.head = rb.tail = 0;
    }

    template<typename T, size_t N>
    size_t ring_buffer_used(ring_buffer_t<T, N>& rb)
    {
        return rb.head - rb.tail;
    }

    template<typename T, size_t N>
    T* ring_buffer_alloc(ring_buffer_t<T, N>& rb)
    {
        assert(ring_buffer_used(rb) < N);
        ++rb.head;
        return &rb.storage[rb.head & (N-1)];
    }

    template<typename T, size_t N>
    T* ring_buffer_back(ring_buffer_t<T, N>& rb)
    {
        assert(ring_buffer_used(rb) > 0);
        return &rb.storage[rb.tail & (N-1)];
    }

    template<typename T, size_t N>
    void ring_buffer_pop(ring_buffer_t<T, N>& rb)
    {
        assert(ring_buffer_used(rb) > 0);
        ++rb.tail;
    }

};
