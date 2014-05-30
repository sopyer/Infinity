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