#include <core/core.h>
#include "nedmalloc.h"

namespace mem
{
    arena_t create_arena (size_t capacity, size_t numthreads)
    {
        return (arena_t)nedalloc::nedcreatepool(capacity, numthreads);
    }

    void destroy_arena(arena_t arena)
    {
        assert(arena);
        nedalloc::neddestroypool((nedalloc::nedpool*)arena);
    }

    void* alloc(arena_t arena, size_t size, size_t alignment)
    {
        assert(arena);
        return nedalloc::nedpmalloc2((nedalloc::nedpool*)arena, size, alignment, 0);
    }

    void* realloc(arena_t arena, void* ptr, size_t size, size_t alignment)
    {
        assert(arena);
        return nedalloc::nedprealloc2((nedalloc::nedpool*)arena, ptr, size, alignment, 0);
    }

    void free(arena_t arena, void* ptr)
    {
        assert(arena);
        nedalloc::nedpfree2((nedalloc::nedpool*)arena, ptr);
    }
}
