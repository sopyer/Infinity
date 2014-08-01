#pragma once

namespace mem
{
    struct arena_data_t;
    typedef struct arena_data_t* arena_t;

    arena_t create_arena (size_t capacity, size_t numthreads);
    void    destroy_arena(arena_t arena);

    void* alloc  (arena_t arena, size_t size, size_t alignment = 0);
    void* realloc(arena_t arena, void* ptr, size_t size, size_t alignment = 0);
    void  free   (arena_t arena, void* ptr);

    template<typename T>
    T* alloc(arena_t arena)
    {
        return (T*)alloc(arena, sizeof(T), _alignof(T));
    }

    template<typename T>
    T* alloc_array(arena_t arena, size_t count)
    {
        return (T*)alloc(arena, sizeof(T)*count, _alignof(T));
    }
}
