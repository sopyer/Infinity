#ifndef _UTILS_H_INCLUDED_
#	define _UTILS_H_INCLUDED_

#include <stdint.h>
#include <assert.h>

#define UNUSED(var)			((void)(var))
#define ARRAY_SIZE(arr)		sizeof(arr)/sizeof(arr[0])
#define BUFFER_OFFSET(i)    ((char*)NULL + (i))

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

#endif