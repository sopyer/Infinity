#pragma once

#include <stdint.h>
#include <stdarg.h>

#include <physfs/physfs.h>

#ifndef _NDEBUG
#   define CORE_ENABLE_ASSERT
#endif

typedef volatile long atomic_t;

#include <core/debug.h>
#include <core/bits.h>
#include <core/mt.h>
#include <core/ml.h>
#include <core/profiler.h>
#include <core/timer.h>
#include <core/memory.h>
#include <core/str.h>

#define UNUSED(var)         ((void)(var))
#define ARRAY_SIZE(arr)     sizeof(arr)/sizeof(arr[0])
#define BUFFER_OFFSET(i)    ((char*)NULL + (i))

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(a ## b)
#define PP_CAT_II(res) res

#define CORE_UNIQUE_NAME(base) PP_CAT(base, __LINE__)

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;

struct ProfilerCPUAutoTimeslice 
{
    uint16_t mid;

     ProfilerCPUAutoTimeslice(uint16_t id)
     {
         mid = id;
         profilerAddCPUEvent( mid, PROF_EVENT_PHASE_BEGIN );
     }
    ~ProfilerCPUAutoTimeslice()
    {
        profilerAddCPUEvent( mid, PROF_EVENT_PHASE_END );
    }
};

namespace core
{
    void abort();
}

inline void atomicLock  (atomic_t* lock) { while (_InterlockedExchange(lock, 1) == 1); }
inline void atomicUnlock(atomic_t* lock) { _InterlockedExchange(lock, 0); }

// TODO : remove all usages 
// NOTE: hack for fast profiling and quick integration;
//       profiler_scope_initialized and profiler_scope_initialized
//       add around 8-16Kb per 1K events + a lot of code;
#define PROFILER_CPU_TIMESLICE(name)                                \
    static uint16_t scope_id = 0;                                   \
    {                                                               \
        static volatile atomic_t profiler_scope_initialized = 0;    \
        static volatile atomic_t profiler_scope_spinlock = 0;       \
        if (profiler_scope_initialized == 0)                        \
        {                                                           \
            atomicLock(&profiler_scope_spinlock);                   \
            if (profiler_scope_initialized == 0)                    \
            {                                                       \
                scope_id = profilerGenerateId();                    \
                profilerAddDesc(scope_id, name);                    \
                profiler_scope_initialized = 1;                     \
            }                                                       \
            atomicUnlock(&profiler_scope_spinlock);                 \
        }                                                           \
    }                                                               \
    ProfilerCPUAutoTimeslice profiler_autoscope(scope_id)           \


char* cpToUTF8(int cp, char* str);

template<typename T, typename E>
size_t calcBlobSize1(size_t numElements)
{
    assert(numElements>=1);
    return sizeof(T) + (numElements - 1) * sizeof(E);
}

template<typename T, typename E>
size_t calcBlobSize0(size_t numElements)
{
    assert(numElements>=0);
    return sizeof(T) + numElements * sizeof(E);
}

struct point_t
{
    float x, y;
};

struct rect_t
{
    float x, y, w, h;
};

bool testPtInRect(const point_t& pt, const rect_t& rect);
bool testPtInRect(float xp, float yp, float xr, float yr, float wr, float hr);

//--------------------------------------------------------------------------

struct memory_t
{
    uint8_t* buffer;
    size_t   size;
    size_t   allocated;
}; 

bool mem_thread_stack_init(memory_t* mem, size_t size);

bool mem_area(memory_t* mem, size_t size);
bool mem_file(memory_t* mem, const char* name);
void mem_free(memory_t* mem);

//--------------------------------------------------------------------------

struct blob32_data_t
{
    uint32_t size;
};

typedef blob32_data_t* blob32_t;

blob32_t blob32_alloc(mspace_t arena, uint32_t size);

static inline uint8_t* blob32_data(blob32_t blob, uint32_t offset = 0)
{
    return (uint8_t*)(&blob[1]) + offset;
}

blob32_t read_file_to_blob32(mspace_t arena, const char* name, size_t name_len);

//--------------------------------------------------------------------------

static inline bool mem_space_available(size_t max_size, size_t offset, size_t size);

template <typename type>
static inline bool mem_value_available(size_t max_size, size_t offset);

template <typename type>
static inline bool mem_array_available(size_t max_size, size_t offset, size_t count);

template <typename type>
static inline type mem_as_value(uint8_t* mem, size_t offset);

template <typename type>
static inline type* mem_as_ptr(uint8_t* mem, size_t offset);

template <typename type>
static inline type& mem_as_ref(uint8_t* mem, size_t offset);

template <typename type>
static inline type mem_as_value_advance(uint8_t* mem, size_t& offset);

template <typename type>
static inline type* mem_as_ptr_advance(uint8_t* mem, size_t& offset);

template <typename type>
static inline type& mem_as_ref_advance(uint8_t* mem, size_t& offset);

template <typename type>
static inline type* mem_as_array_advance(uint8_t* mem, size_t count, size_t& offset);

void read_file_to_mem(mspace_t arena, const char* name, size_t name_len, uint8_t** mem, size_t& size);

//--------------------------------------------------------------------------

static inline bool mem_space_available(size_t max_size, size_t offset, size_t size)
{
    return offset + size <= max_size;
}

template <typename type>
static inline bool mem_value_available(size_t max_size, size_t offset)
{
    return mem_space_available(max_size, offset, sizeof(type));
}

template <typename type>
static inline bool mem_array_available(size_t max_size, size_t offset, size_t count)
{
    return mem_space_available(max_size, offset, count * sizeof(type));
}

template <typename type>
static inline type mem_as_value(uint8_t* mem, size_t offset)
{
    assert(mem);
    return *(type*)(mem + offset);
}

template <typename type>
static inline type* mem_as_ptr(uint8_t* mem, size_t offset)
{
    assert(mem);
    return (type*)(mem + offset);
}

template <typename type>
static inline type& mem_as_ref(uint8_t* mem, size_t offset)
{
    assert(mem);
    return *(type*)(mem + offset);
}

template <typename type>
static inline type mem_as_value_advance(uint8_t* mem, size_t& offset)
{
    assert(mem);
    const size_t data_offset = offset;
    const size_t size = sizeof(type);
    offset += size;
    return *(type*)(mem + data_offset);
}

template <typename type>
static inline type* mem_as_ptr_advance(uint8_t* mem, size_t& offset)
{
    assert(mem);
    const size_t data_offset = offset;
    const size_t size = sizeof(type);
    offset += size;
    return (type*)(mem + data_offset);
}

template <typename type>
static inline type& mem_as_ref_advance(uint8_t* mem, size_t& offset)
{
    assert(mem);
    const size_t data_offset = offset;
    const size_t size = sizeof(type);
    offset += size;
    return *(type*)(mem + data_offset);
}

template <typename type>
static inline type* mem_as_array_advance(uint8_t* mem, size_t count, size_t& offset)
{
    assert(mem);
    const size_t data_offset = offset;
    const size_t size = count * sizeof(type);
    offset += size;
    return (type*)(mem + data_offset);
}

//--------------------------------------------------------------------------


#define TRUE  1
#define FALSE 0

namespace core
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

namespace core
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

namespace core
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

    template<typename T>
    inline size_t align_up(size_t offset)
    {
        size_t stride = sizeof(T);
        size_t rem    = offset % stride;
        size_t adjust = (rem==0)? 0 : (stride - rem);

        return offset + adjust;
    }

    inline size_t align_up(size_t offset, size_t stride)
    {
        size_t rem    = offset % stride;
        size_t adjust = (rem==0)? 0 : (stride - rem);

        return offset + adjust;
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
        static_assert((N & (N-1)) == 0, "N is not power of 2");
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

#ifdef __cplusplus
extern "C"
{
#endif
void mem_set(void *dest, size_t len, uint8_t value);
void mem_set8(uint8_t *dp, size_t len, uint8_t value);
void mem_set16(uint16_t *dp, size_t len, uint16_t value);
void mem_set32(uint32_t *dp, size_t len, uint32_t value);

void mem_move(void *dest, const void *src, size_t len);
void mem_move8(uint8_t *dp, const uint8_t *sp, size_t len);
void mem_move16(uint16_t *dp, const uint16_t *sp, size_t len);
void mem_move32(uint32_t *dp, const uint32_t *sp, size_t len);

void mem_copy(void *dest, const void *src, size_t len);
void mem_copy8(uint8_t *dp, const uint8_t *sp, size_t len);
void mem_copy16(uint16_t *dp, const uint16_t *sp, size_t len);
void mem_copy32(uint32_t *dp, const uint32_t *sp, size_t len);
#ifdef __cplusplus
}

template<typename T>
void mem_zero(T* p)
{
    mem_set(p, sizeof(T), 0);
}

template<typename T>
void mem_zero(T* p, size_t count)
{
    mem_set(p, count*sizeof(T), 0);
}

#endif

#ifdef __cplusplus
extern "C"
{
#endif
enum LogCategory
{
    LOG_CAT_APP,
    LOG_CAT_VIDEO,
    LOG_CAT_AUDIO,
    LOG_CAT_FILEIO,
    LOG_CAT_NET,
    LOG_CAT_SYS,
    LOG_CAT_INPUT,
    LOG_CAT_CUSTOM
};

enum LogPriority
{
    LOG_PRIO_TRACE,
    LOG_PRIO_DEBUG,
    LOG_PRIO_INFO,
    LOG_PRIO_WARN,
    LOG_PRIO_ERROR,
    LOG_PRIO_CRITICAL,
    LOG_PRIO_COUNT
};

void core_log(int cat, int prio, const char* fmt, ...);
#ifdef __cplusplus
}
#endif
