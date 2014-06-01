#ifndef _UTILS_H_INCLUDED_
#	define _UTILS_H_INCLUDED_

#include <stdint.h>
#include <assert.h>
// switch to dlalloc and use mspaces
#include <stdlib.h> 

#define UNUSED(var)			((void)(var))
#define ARRAY_SIZE(arr)		sizeof(arr)/sizeof(arr[0])
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

bool marea(memory_t* mem, size_t size);
bool mopen(memory_t* mem, const char* name);
void mfree(memory_t* mem);

template <typename type>
type* madvance(memory_t* mem, size_t count)
{
    uint8_t* var = mem->buffer + mem->allocated;

    mem->allocated += sizeof(type)*count;
    assert(mem->allocated<=mem->size);

    return (type*)var;
}

template <typename type>
type* madvance(memory_t* mem)
{
    uint8_t* var = mem->buffer + mem->allocated;

    mem->allocated += sizeof(type);
    assert(mem->allocated<=mem->size);

    return (type*)var;
}

namespace ml
{
    static const float cf_pi          = 3.141592653589793f;
    static const float cf_2_pi        = cf_pi * 2.0f;
    static const float cf_1_over_pi   = 1.0f / cf_pi;
    static const float cf_1_over_2_pi = 1.0f / cf_2_pi;

    typedef union {float f; uint32_t u32;} f_u_conv_t;

    inline float abs(float v)
    {
        f_u_conv_t conv = {v};

        conv.u32 &= 0x7FFFFFFF;

        return conv.f;
    }
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
        return  1.0f / (1.0f + ml::cf_1_over_2_pi / (cutoffFreq * dt));
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

#endif