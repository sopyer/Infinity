#include <stdlib.h>

#include "utils.h"
#include "physfs/physfs.h"

bool marea(memory_t* mem, size_t size)
{
    mem->allocated = 0;
    mem->size      = size;
    mem->buffer    = (uint8_t*)malloc(size);

    return true;
}

bool mopen(memory_t* mem, const char* name)
{
    PHYSFS_File* src = PHYSFS_openRead(name);

    if (!src) return false;

    //explicit conversion to avoid warning on 32-bit system
    assert(PHYSFS_fileLength(src)<SIZE_MAX);

    mem->size      = (size_t)PHYSFS_fileLength(src);
    mem->allocated = mem->size;
    mem->buffer    = (uint8_t*)malloc(mem->size+1);

    PHYSFS_read(src, mem->buffer, mem->size, 1);

    PHYSFS_close(src);

    return true;
}

void mfree(memory_t* mem)
{
    if (mem->buffer) free(mem->buffer);

    mem->buffer    = 0;
    mem->size      = 0;
    mem->allocated = 0;
}
