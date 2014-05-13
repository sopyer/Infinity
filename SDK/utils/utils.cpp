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

char* cpToUTF8(int cp, char* str)
{
    int n = 0;
    if (cp < 0x80) n = 1;
    else if (cp < 0x800) n = 2;
    else if (cp < 0x10000) n = 3;
    else if (cp < 0x200000) n = 4;
    else if (cp < 0x4000000) n = 5;
    else if (cp <= 0x7fffffff) n = 6;
    str[n] = '\0';
    switch (n) {
    case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
    case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
    case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
    case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
    case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
    case 1: str[0] = cp;
    }
    return str;
}
