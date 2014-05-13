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
    //TODO: Handle resize!!!!!!!!!!!!!!!!
    // maps uint32_t -> [0..N]
    // should be used only for small indices N<256
    struct _vindex32_t
    {
        uint32_t used;
        uint32_t allocated;
        uint32_t data[1];
    };

    typedef _vindex32_t* vindex32_t;

    inline void vindex_create(vindex32_t* ptrIndexData, size_t keysPrealloc);
    inline void vindex_destroy(vindex32_t indexData);
    inline void vindex_get(vindex32_t* ptrIndexData, uint32_t key, uint32_t* retIndex);


    inline void vindex_create(vindex32_t* ptrIndexData, size_t keysPrealloc)
    {
        size_t   dataSize;
        vindex32_t indexData;

        dataSize  = sizeof(_vindex32_t)+sizeof(uint32_t)*(keysPrealloc-1);
        indexData = (vindex32_t)malloc(dataSize);

        indexData->used      = 0;
        indexData->allocated = keysPrealloc;

        *ptrIndexData = indexData;
    }

    inline void vindex_destroy(vindex32_t indexData)
    {
        free(indexData);
    }

    // adds key to index
    inline void vindex_get(vindex32_t* ptrIndexData, uint32_t key, uint32_t* retIndex)
    {
        size_t    idx, count;
        uint32_t* keys;
        vindex32_t  indexData;

        indexData = *ptrIndexData;
        idx   = 0;
        count = indexData->used;
        keys  = indexData->data;

        for (; idx < count; ++idx)
        {
            if (keys[idx]==key)
            {
                goto result;
            }
        }

        //if (idx>=indexData->allocated)
        //{
        //    assert(indexData->allocated < (1<<30)); // avoid overflow!!!
        //    size_t newSize = sizeof(_vindex_t)+sizeof(uint32_t)*(indexData->allocated*2-1);
        //    indexData = (vindex_t)realloc(indexData, newSize);
        //    *ptrIndexData = indexData;
        //}

        assert(idx < indexData->allocated);
        indexData->data[idx] = key;
        ++indexData->used;
    
    result:
        if (retIndex) *retIndex = idx;
    }
}

#endif