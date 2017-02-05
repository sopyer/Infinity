#include <core/core.h>

bool testPtInRect(const point_t& pt, const rect_t& rect)
{
    return (rect.x<=pt.x) && (pt.x<rect.x+rect.w) && (rect.y<=pt.y) && (pt.y<rect.y+rect.h);
}

bool testPtInRect(float xp, float yp, float xr, float yr, float wr, float hr)
{
    return (xr<=xp) && (xp<xr+wr) && (yr<=yp) && (yp<yr+hr);
}

blob32_t blob32_alloc(mspace_t arena, uint32_t size)
{
    blob32_t blob = (blob32_t)mem_alloc(arena, sizeof(blob32_data_t) + size, 0);
    blob->size = size;
    return blob;
}

blob32_t read_file_to_blob32(mspace_t arena, const char* name, size_t)
{
    PHYSFS_File* src = PHYSFS_openRead(name);

    blob32_t blob = nullptr;

    if (src)
    {
        uint64_t size = PHYSFS_fileLength(src);

        if (size <= UINT32_MAX)
        {
            blob = blob32_alloc(arena, (uint32_t)size);

            if (blob)
            {
                PHYSFS_read(src, blob32_data(blob), (uint32_t)size, 1);
            }
        }

        PHYSFS_close(src);
    }

    return blob;
}

void read_file_to_mem(mspace_t arena, const char* name, size_t name_len, uint8_t*& mem, size_t& size)
{
    PHYSFS_File* src = PHYSFS_openRead(name);

    mem = nullptr;
    size = 0;

    if (src)
    {
        uint64_t file_size = PHYSFS_fileLength(src);

        if (file_size <= UINT32_MAX)
        {
            size = (size_t)file_size;
            mem = (uint8_t*)mem_alloc(arena, size, 0);
        }

        if (mem)
        {
            PHYSFS_read(src, mem, (uint32_t)file_size, 1);
        }

        PHYSFS_close(src);
    }
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
    mem->buffer    = (uint8_t*)core::thread_stack_alloc(size);

    return true;
}

bool mem_file(memory_t* mem, const char* name)
{
    PHYSFS_File* src = PHYSFS_openRead(name);

    if (!src) return false;

    //explicit conversion to avoid warning on 32-bit system
    assert(PHYSFS_fileLength(src)<SIZE_MAX);

    mem->size      = (size_t)PHYSFS_fileLength(src);
    mem->allocated = 0;
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

namespace core
{
    static const size_t THREAD_DATA_STACK_SIZE = 256 * (1<<10);

    static stack_mem_t mainThreadDataStack = 0;
    static uint8_t* threadDataStackMem;

    static const size_t MSPACE_CORE_SIZE = 1*(1<<20);
    static mspace_t mspace_core;

    void init()
    {
        threadDataStackMem = (uint8_t*)malloc(THREAD_DATA_STACK_SIZE);
        stack_mem_init(&mainThreadDataStack, threadDataStackMem, THREAD_DATA_STACK_SIZE);

        profilerInit();
        mt::init(1, 128);

        mspace_core = mem_create_space(MSPACE_CORE_SIZE);
    }

    void fini()
    {
        mem_destroy_space(mspace_core);
        mspace_core = 0;

        mt::fini();

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

extern "C"
{
#define ETLSF_assert assert
#define ETLSF_memset(ptr, size, value) mem_set(ptr, size, 0)
#define ETLSF_alloc(size) mem_alloc(core::mspace_core, size, 0)
#define ETLSF_free(ptr) mem_free(core::mspace_core, ptr)
#define ETLSF_fls bit_fls
#define ETLSF_ffs bit_ffs
#define ETLSF_align2 bit_align_up
#include "etlsf.c"

#include "str.c"
#include "mem_utils.c"
#include "mem_alloc.c"
#include "mjson.c"
#include "Remotery.c"
}

#include "ml.cpp"
#include "mt.cpp"
#include "timer.cpp"
#include "profiler.cpp"

extern "C"
{
    void core_log(int cat, int prio, const char* fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);
        SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, ap);
        va_end(ap);
    }
}

#ifdef __WIN32__
#include "windows.h"
#endif

namespace core
{
    void abort()
    {
#ifdef __WIN32__
        ExitProcess(1);
#else
        _exit(1);
#endif
    }
}