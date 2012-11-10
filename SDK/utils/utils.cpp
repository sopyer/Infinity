#include <stdlib.h>

#include "utils.h"
#include "VFS++.h"

bool marea(memory_t* mem, size_t size)
{
    mem->allocated = 0;
    mem->size      = size;
    mem->buffer    = (uint8_t*)malloc(size);

    return true;
}

bool mopen(memory_t* mem, const char* name)
{
    File	src = VFS::openRead(name);

    if (!src) return false;

    //explicit conversion to avoid warning on 32-bit system
    assert(src.size()<SIZE_MAX);

    mem->size      = (size_t)src.size();
    mem->allocated = mem->size;
    mem->buffer    = (uint8_t*)malloc(mem->size+1);

    src.read(mem->buffer, mem->size, 1);

    src.close();

    return true;
}

void mfree(memory_t* mem)
{
    if (mem->buffer) free(mem->buffer);

    mem->buffer    = 0;
    mem->size      = 0;
    mem->allocated = 0;
}
