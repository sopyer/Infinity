#include <etlsf.h>
#include <stddef.h>


#ifndef ETLSF_assert
    #include <assert.h>
    #define ETLSF_assert assert
#endif

#ifndef ETLSF_memset
    #include <memory.h>
    #define ETLSF_memset(ptr, size, value) memset(ptr, value, size)
#endif

#ifndef ETLSF_alloc
    #include <stdlib.h>
    #define ETLSF_alloc(size) malloc(size)
#endif

#ifndef ETLSF_free
    #include <stdlib.h>
    #define ETLSF_free(ptr) free(ptr)
#endif

#ifndef ETLSF_fls
    #if defined (__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)) && defined (__GNUC_PATCHLEVEL__)

        static int ETLSF_fls(uint32_t word)
        {
            const int bit = word ? 32 - __builtin_clz(word) : 0;
            return bit - 1;
        }

    #elif defined (_MSC_VER) && (_MSC_VER >= 1400)

        #include <intrin.h>

        #pragma intrinsic(_BitScanReverse)
        #pragma intrinsic(_BitScanForward)

        static int ETLSF_fls(uint32_t word)
        {
            unsigned long index;
            return _BitScanReverse(&index, word) ? index : -1;
        }

    #elif defined (__ARMCC_VERSION)

        static int ETLSF_fls(uint32_t word)
        {
            const int bit = word ? 32 - __clz(word) : 0;
            return bit - 1;
        }

    #else

        static int ETLSF_fls_generic_fls(uint32_t word)
        {
            int bit = 32;

            if (!word) bit -= 1;
            if (!(word & 0xffff0000)) { word <<= 16; bit -= 16; }
            if (!(word & 0xff000000)) { word <<= 8; bit -= 8; }
            if (!(word & 0xf0000000)) { word <<= 4; bit -= 4; }
            if (!(word & 0xc0000000)) { word <<= 2; bit -= 2; }
            if (!(word & 0x80000000)) { word <<= 1; bit -= 1; }

            return bit;
        }

        static int ETLSF_fls(uint32_t word)
        {
            return bit_fls_generic_fls(word) - 1;
        }

    #endif
#endif

#ifndef ETLSF_ffs
    #if defined (__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)) && defined (__GNUC_PATCHLEVEL__)

        static int ETLSF_ffs(uint32_t word)
        {
            return __builtin_ffs(word) - 1;
        }

    #elif defined (_MSC_VER) && (_MSC_VER >= 1400)

        #include <intrin.h>

        #pragma intrinsic(_BitScanReverse)
        #pragma intrinsic(_BitScanForward)

        static int ETLSF_ffs(uint32_t word)
        {
            unsigned long index;
            return _BitScanForward(&index, word) ? index : -1;
        }

    #elif defined (__ARMCC_VERSION)

        static int ETLSF_ffs(uint32_t word)
        {
            const unsigned int reverse = word & (~word + 1);
            const int bit = 32 - __clz(reverse);
            return bit - 1;
        }

    #else

        static int ETLSF_fls_generic_ffs(uint32_t word)
        {
            int bit = 32;

            if (!word) bit -= 1;
            if (!(word & 0xffff0000)) { word <<= 16; bit -= 16; }
            if (!(word & 0xff000000)) { word <<= 8; bit -= 8; }
            if (!(word & 0xf0000000)) { word <<= 4; bit -= 4; }
            if (!(word & 0xc0000000)) { word <<= 2; bit -= 2; }
            if (!(word & 0x80000000)) { word <<= 1; bit -= 1; }

            return bit;
        }

        static int ETLSF_ffs(uint32_t word)
        {
            return bit_fls_generic_ffs(word & (~word + 1)) - 1;
        }

    #endif
#endif

#ifndef ETLSF_align2
    static uint32_t ETLSF_align2(uint32_t size, uint32_t align)
    {
        return (size + (align - 1)) & ~(align - 1);
    }
#endif


#define BLOCK_MEM(block, mem) (arena->storage[block].mem)

enum etlsf_private
{
    /* All allocation sizes and addresses are aligned to 256 bytes. */
    ALIGN_SIZE_LOG2 = 8,
    ALIGN_SIZE = (1 << ALIGN_SIZE_LOG2),

    /* log2 of number of linear subdivisions of block sizes. */
    SL_INDEX_COUNT_LOG2 = 4,
    SL_INDEX_COUNT      = (1 << SL_INDEX_COUNT_LOG2),

    /*
    ** We support allocations of sizes up to (1 << FL_INDEX_MAX) bits.
    ** However, because we linearly subdivide the second-level lists, and
    ** our minimum size granularity is 4 bytes, it doesn't make sense to
    ** create first-level lists for sizes smaller than SL_INDEX_COUNT * 4,
    ** or (1 << (SL_INDEX_COUNT_LOG2 + 2)) bytes, as there we will be
    ** trying to split size ranges into more slots than we have available.
    ** Instead, we calculate the minimum threshold size, and place all
    ** blocks below that size into the 0th first-level list.
    */
    FL_INDEX_MAX   = 30,
    FL_INDEX_SHIFT = (SL_INDEX_COUNT_LOG2 + ALIGN_SIZE_LOG2),
    FL_INDEX_COUNT = (FL_INDEX_MAX - FL_INDEX_SHIFT + 1),

    SMALL_BLOCK_SIZE = (1 << FL_INDEX_SHIFT),

    BLOCK_SIZE_MIN = (size_t)1 << ALIGN_SIZE_LOG2,
    BLOCK_SIZE_MAX = (size_t)1 << FL_INDEX_MAX,

    BLOCK_COUNT_MAX = 0xFFFE,
};

struct used_block_data_t
{
    uint32_t  size;
};

struct free_block_data_t
{
    uint32_t  sl : 8;
    uint32_t  fl : 8;
    uint16_t  next_free;
    uint16_t  prev_free;
};

struct block_t
{
    uint16_t  next_phys;
    uint16_t  prev_phys;

    uint32_t  offset  : 31;
    uint32_t  is_free :  1;

    union
    {
        struct used_block_data_t used_data;
        struct free_block_data_t free_data;
    };
};

struct etlsf_private_t
{
    /* Bitmaps for free lists. */
    unsigned int fl_bitmap;
    unsigned int sl_bitmap[FL_INDEX_COUNT];

    /* Head of free lists. */
    uint16_t blocks[FL_INDEX_COUNT][SL_INDEX_COUNT];

    uint32_t size;
    uint16_t total_blocks;
    uint16_t unused_blocks;
    uint16_t first_free_block;

    struct block_t  storage[1];
};

static uint32_t   arena_data_size();
static uint32_t   arena_total_size(uint16_t max_allocs);

static uint16_t   storage_new_block (etlsf_t arena);
static void       storage_free_block(etlsf_t arena, uint16_t id);

static  void      block_register_free  (etlsf_t arena, uint16_t id);
static  void      block_unregister_free(etlsf_t arena, uint16_t id);
static  uint16_t  block_find_free      (etlsf_t arena, uint32_t size);
static  void      block_trim           (etlsf_t arena, uint16_t id);

//-------------------------  API implementation  ----------------------------//
etlsf_t etlsf_create(uint32_t size, uint16_t max_allocs)
{
    ETLSF_assert(max_allocs < BLOCK_COUNT_MAX);
    ETLSF_assert(size % ALIGN_SIZE == 0);

    etlsf_t arena = (etlsf_t)ETLSF_alloc(arena_total_size(max_allocs + 1));

    ETLSF_memset(arena, sizeof(struct etlsf_private_t), 0); // sets also all lists point to zero block

    arena->size             = size;
    arena->total_blocks     = max_allocs + 1;
    arena->unused_blocks    = max_allocs;
    arena->first_free_block = 0;

    ETLSF_assert(BLOCK_SIZE_MIN <= size && size <= BLOCK_SIZE_MAX);

    /*
    ** Create the main free block. Offset the start of the block slightly
    ** so that the prev_phys_block field falls outside of the pool -
    ** it will never be used.
    */
    uint16_t id = storage_new_block(arena);
    BLOCK_MEM(id, prev_phys) = 0;
    BLOCK_MEM(id, next_phys) = 0;
    BLOCK_MEM(id, offset)    = 0;
    block_register_free(arena, id);

    return arena;
}

void etlsf_destroy(etlsf_t arena)
{
    ETLSF_assert(arena);

    ETLSF_free(arena);
}

uint16_t etlsf_alloc(etlsf_t arena, uint32_t size)
{
    ETLSF_assert(size);
    ETLSF_assert(size <= BLOCK_SIZE_MAX);

    const size_t adjust = ETLSF_align2(size, BLOCK_SIZE_MIN);

    //!!!!!TODO: add support for arbitrary size and alignment
    uint16_t id = block_find_free(arena, adjust);
    BLOCK_MEM(id, used_data.size) = adjust;
    block_trim(arena, id);

    ETLSF_assert(BLOCK_MEM(id, offset) % BLOCK_SIZE_MIN == 0);

    return id;
}

void etlsf_free(etlsf_t arena, uint16_t block)
{
    if (block)
    {
        ETLSF_assert(etlsf_is_block_valid(arena, block) && "block already marked as free");
        block_register_free(arena, block);
    }
}

uint32_t etlsf_block_size(etlsf_t arena, uint16_t block)
{
    ETLSF_assert(etlsf_is_block_valid(arena, block));
    return BLOCK_MEM(block, used_data.size);
}

uint32_t etlsf_block_offset(etlsf_t arena, uint16_t block)
{
    ETLSF_assert(etlsf_is_block_valid(arena, block));
    return BLOCK_MEM(block, offset);
}

int etlsf_is_block_valid(etlsf_t arena, uint16_t block)
{
    return !BLOCK_MEM(block, is_free);
}


//------------------------------  Arena utils  --------------------------------//
static uint32_t arena_data_size()
{
    return offsetof(struct etlsf_private_t, storage);
}

static uint32_t arena_total_size(uint16_t max_allocs)
{
    return arena_data_size() + max_allocs * sizeof(struct block_t);
}


//----------------------------  Storage utils  --------------------------------//
static uint16_t storage_new_block(etlsf_t arena)
{
    if (arena->first_free_block)
    {
        ETLSF_assert(arena->first_free_block < arena->total_blocks);

        uint16_t id = arena->first_free_block;
        arena->first_free_block = BLOCK_MEM(id, next_phys);

        return id;
    }

    if (arena->unused_blocks > 0)
    {
        uint16_t id = arena->total_blocks - arena->unused_blocks;
        --arena->unused_blocks;

        return id;
    }

    return 0;
}

static void storage_free_block(etlsf_t arena, uint16_t id)
{
    ETLSF_assert(id > 0);
    ETLSF_assert(id < arena->total_blocks);
    ETLSF_assert(id+arena->unused_blocks < arena->total_blocks);

    if (id + 1 + arena->unused_blocks == arena->total_blocks)
    {
        ++arena->unused_blocks;
    }
    else
    {
        BLOCK_MEM(id, next_phys) = arena->first_free_block;
        arena->first_free_block = id;
    }
}


//------------------------------  Size utils  -------------------------------//
#define size_to_fl_sl(size, fl, sl)                                                      \
{                                                                                        \
    if (size < SMALL_BLOCK_SIZE)                                                         \
    {                                                                                    \
        /* Store small blocks in first list. */                                          \
        fl = 0;                                                                          \
        sl = size / (SMALL_BLOCK_SIZE / SL_INDEX_COUNT);                                 \
    }                                                                                    \
    else                                                                                 \
    {                                                                                    \
        fl = ETLSF_fls(size);                                                            \
        sl = (size >> (fl - SL_INDEX_COUNT_LOG2)) ^ (1 << SL_INDEX_COUNT_LOG2);          \
        fl -= (FL_INDEX_SHIFT - 1);                                                      \
    }                                                                                    \
}                                                                                        \


//------------------------------  Block operations  ------------------------------//
static uint32_t block_get_actual_size(etlsf_t arena, uint16_t id)
{
    uint16_t next = BLOCK_MEM(id, next_phys);
    return (next ? BLOCK_MEM(next, offset) : arena->size) - BLOCK_MEM(id, offset);
}

static void block_trim(etlsf_t arena, uint16_t id)
{
    uint32_t bsize = block_get_actual_size(arena, id);
    uint32_t size  = BLOCK_MEM(id, used_data.size);

    ETLSF_assert(!BLOCK_MEM(id, is_free));
    ETLSF_assert(size>=BLOCK_SIZE_MIN);

    if (bsize >= size + BLOCK_SIZE_MIN)
    {
        uint16_t next   = BLOCK_MEM(id, next_phys);
        uint32_t offset = BLOCK_MEM(id, offset);

        uint16_t new_block = storage_new_block(arena);

        if (!new_block)
        {
            ETLSF_assert(0);
            return;
        }

        BLOCK_MEM(new_block, offset) = offset+size;

        BLOCK_MEM(id, next_phys) = new_block;

        BLOCK_MEM(new_block, next_phys) = next;
        BLOCK_MEM(new_block, prev_phys) = id;

        BLOCK_MEM(next, prev_phys) = new_block;

        block_register_free(arena, new_block);
    }
}

//Probably a bug when prev block is free
static void block_register_free(etlsf_t arena, uint16_t id)
{
    ETLSF_assert(id);

    uint16_t prev = BLOCK_MEM(id, prev_phys);
    uint16_t next = BLOCK_MEM(id, next_phys);
    //Merge prev block if free
    if (BLOCK_MEM(prev, is_free))
    {
        uint16_t prev_prev = BLOCK_MEM(prev, prev_phys);

        ETLSF_assert(prev != 0 && "block must not be first");

        BLOCK_MEM(id, offset) = BLOCK_MEM(prev, offset);

        BLOCK_MEM(id,        prev_phys)      = prev_prev;
        BLOCK_MEM(prev_prev, next_phys)      = id;

        block_unregister_free(arena, prev);
        storage_free_block(arena, prev);
    }
    //Merge next block if free
    if (BLOCK_MEM(next, is_free))
    {
        uint16_t next_next = BLOCK_MEM(next, next_phys);

        ETLSF_assert(next != 0 && "block must not be last");

        BLOCK_MEM(id,        next_phys)      = next_next;
        BLOCK_MEM(next_next, prev_phys)      = id;

        block_unregister_free(arena, next);
        storage_free_block(arena, next);
    }

    uint32_t fl = 0, sl = 0;
    uint32_t size = block_get_actual_size(arena, id);
    size_to_fl_sl(size, fl, sl);

    uint16_t next_free = arena->blocks[fl][sl];
    ETLSF_assert(id < arena->total_blocks);
    ETLSF_assert(next_free < arena->total_blocks);

    BLOCK_MEM(id,        free_data.prev_free)   = 0;
    BLOCK_MEM(id,        free_data.next_free)   = next_free;
    BLOCK_MEM(next_free, free_data.prev_free) = id;

    BLOCK_MEM(id, free_data.fl) = fl;
    BLOCK_MEM(id, free_data.sl) = sl;
    BLOCK_MEM(id, is_free)      = 1;

    /*
    ** Insert the new block at the head of the list, and mark the first-
    ** and second-level bitmaps appropriately.
    */
    arena->blocks[fl][sl] = id;
    arena->fl_bitmap     |= (1 << fl);
    arena->sl_bitmap[fl] |= (1 << sl);
}

static void block_unregister_free(etlsf_t arena, uint16_t id)
{
    uint32_t fl = BLOCK_MEM(id, free_data.fl);
    uint32_t sl = BLOCK_MEM(id, free_data.sl);

    uint16_t prev = BLOCK_MEM(id, free_data.prev_free);
    uint16_t next = BLOCK_MEM(id, free_data.next_free);

    ETLSF_assert(prev < arena->total_blocks);
    ETLSF_assert(next < arena->total_blocks);

    BLOCK_MEM(next, free_data.prev_free) = prev;
    BLOCK_MEM(prev, free_data.next_free) = next;
    BLOCK_MEM(id, is_free) = 0;

    /* If this block is the head of the free list, set new head. */
    if (arena->blocks[fl][sl] == id)
    {
        arena->blocks[fl][sl] = next;

        /* If the new head is null, clear the bitmap. */
        if (next == 0)
        {
            arena->sl_bitmap[fl] &= ~(1 << sl);

            /* If the second bitmap is now empty, clear the fl bitmap. */
            if (!arena->sl_bitmap[fl])
            {
                arena->fl_bitmap &= ~(1 << fl);
            }
        }
    }
}

static uint16_t block_find_free(etlsf_t arena, uint32_t size)
{
    uint32_t  fl = 0, sl = 0;
    uint16_t  id = 0;

    if (size)
    {
        size_to_fl_sl(size, fl, sl);

        uint32_t sl_map = arena->sl_bitmap[fl] & (~0 << sl);
        if (!sl_map)
        {
            /* No block exists. Search in the next largest first-level list. */
            const unsigned int fl_map = arena->fl_bitmap & (~0 << (fl + 1));
            if (!fl_map)
            {
                /* No free blocks available, memory has been exhausted. */
                return 0;
            }

            fl = ETLSF_ffs(fl_map);
            sl_map = arena->sl_bitmap[fl];
        }
        ETLSF_assert(sl_map && "internal error - second level bitmap is null");
        sl = ETLSF_ffs(sl_map);

        /* Return the first block in the free list. */
        id = arena->blocks[fl][sl];
    }

    if (id)
    {
        ETLSF_assert(block_get_actual_size(arena, id) >= size);
        block_unregister_free(arena, id);
    }

    return id;
}
