#include <stdlib.h>

#include "utils.h"
#include "physfs/physfs.h"

bool testPtInRect(const point_t& pt, const rect_t& rect)
{
    return (rect.x<=pt.x) && (pt.x<rect.x+rect.w) && (rect.y<=pt.y) && (pt.y<rect.y+rect.h);
}

bool testPtInRect(float xp, float yp, float xr, float yr, float wr, float hr)
{
    return (xr<=xp) && (xp<xr+wr) && (yr<=yp) && (yp<yr+hr);
}

bool mem_area(memory_t* mem, size_t size)
{
    mem->allocated = 0;
    mem->size      = size;
    mem->buffer    = (uint8_t*)malloc(size);

    return true;
}

bool mem_thread_stack_init(memory_t* mem, size_t size)
{
    mem->allocated = 0;
    mem->size      = size;
    mem->buffer    = (uint8_t*)ut::thread_stack_alloc(size);

    return true;
}

bool mem_file(memory_t* mem, const char* name)
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

void mem_free(memory_t* mem)
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

struct stack_mem_data_t
{
    size_t  size;
    size_t  allocated;
    uint8_t data[1];
};

void  stack_mem_init(stack_mem_t* stack, void* mem, size_t size)
{
    *stack = (stack_mem_t)mem;

    (*stack)->size = size;
    (*stack)->allocated = 0;
}

void* stack_mem_alloc(stack_mem_t stack, size_t size, size_t align)
{
    if (align)
    {
        size_t rem = ((size_t)stack->data + stack->allocated) % align;
        size_t off = rem ? align - rem : 0;
        assert(stack->allocated+off<=stack->size);
        stack->allocated += off;
    }

    assert(stack->allocated+size<=stack->size);

    void* p = stack->data + stack->allocated;

    stack->allocated += size;

    return p;
}

void  stack_mem_reset(stack_mem_t stack, void* ptr)
{
    assert(ptr>=stack->data);
    assert(ptr<stack->data+stack->allocated);

    stack->allocated = (uint8_t*)ptr - stack->data;
}

namespace ut
{
    static const size_t THREAD_DATA_STACK_SIZE = 256 * (1<<10);

    static stack_mem_t mainThreadDataStack = 0;
    static uint8_t* threadDataStackMem;

    void init()
    {
        threadDataStackMem = (uint8_t*)malloc(THREAD_DATA_STACK_SIZE);

        stack_mem_init(&mainThreadDataStack, threadDataStackMem, THREAD_DATA_STACK_SIZE);
    }

    void fini()
    {
        free(threadDataStackMem);
    }

    //TODO: make mt!!!!!
    stack_mem_t get_thread_data_stack()
    {
        assert(mainThreadDataStack);

        return mainThreadDataStack;
    }

    void* thread_stack_alloc(size_t size, size_t align)
    {
        return stack_mem_alloc(get_thread_data_stack(), size, align);
    }

    void thread_stack_reset(void* ptr)
    {
        stack_mem_reset(get_thread_data_stack(), ptr);
    }
};

