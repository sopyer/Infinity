#include <core/core.h>
#define USE_LOCKS 1
#define ONLY_MSPACES 1
#include "malloc.c.h"

mspace_t mem_create_space(size_t capacity)
{
    return (mspace_t)create_mspace(capacity, 1);
}

void mem_destroy_space(mspace_t mspace)
{
    assert(mspace);
    destroy_mspace(mspace);
}

void* mem_alloc(mspace_t mspace, size_t size, size_t alignment)
{
    assert(mspace);
    return mspace_malloc2(mspace, size, alignment, 0);
}

void* mem_realloc(mspace_t mspace, void* ptr, size_t size, size_t alignment)
{
    assert(mspace);
    return mspace_realloc2(mspace, ptr, size, alignment, 0);
}

void  mem_free(mspace_t mspace, void* ptr)
{
    assert(mspace);
    mspace_free(mspace, ptr);
}
