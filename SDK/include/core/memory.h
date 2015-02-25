#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
    struct mspace_internal_t;
    typedef struct mspace_internal_t* mspace_t;

    mspace_t mem_create_space (size_t capacity);
    void     mem_destroy_space(mspace_t mspace);

    void* mem_alloc(mspace_t mspace, size_t size, size_t alignment);
    void* mem_realloc(mspace_t mspace, void* ptr, size_t size, size_t alignment);

    void  mem_free(mspace_t mspace, void* ptr);
#ifdef __cplusplus
}

namespace mem
{
    template<typename T>
    T* alloc(mspace_t mspace)
    {
        return (T*)mem_alloc(mspace, sizeof(T), _alignof(T));
    }

    template<typename T>
    T* alloc_array(mspace_t mspace, size_t count)
    {
        return (T*)mem_alloc(mspace, sizeof(T)*count, _alignof(T));
    }

    inline void free(mspace_t space, void* ptr)
    {
        mem_free(space, ptr);
    }
}
#endif
