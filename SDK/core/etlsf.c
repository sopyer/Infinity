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

    SMALL_ALLOC_SIZE = (1 << FL_INDEX_SHIFT),

    RANGE_SIZE_MIN = (size_t)1 << ALIGN_SIZE_LOG2,
    RANGE_SIZE_MAX = (size_t)1 << FL_INDEX_MAX,
};

struct range_data_t
{
    uint16_t  next_phys_index;
    uint16_t  prev_phys_index;

    uint32_t  offset  : 31;
    uint32_t  is_free :  1;

    uint16_t  next_free_index;
    uint16_t  prev_free_index;
};

struct etlsf_private_t
{
    uint32_t size;

    /* Bitmaps for free lists. */
    uint32_t fl_bitmap;
    uint32_t sl_bitmap[FL_INDEX_COUNT];
    /* Head of free lists. */
    uint16_t blocks[FL_INDEX_COUNT][SL_INDEX_COUNT];

    uint16_t num_ranges;
    uint16_t unused_range_count;
    uint16_t first_free_storage_index;
    struct range_data_t storage[1];
};

#define RANGE_DATA(id) (arena->storage[id])

static size_t   arena_total_size(size_t max_allocs);

static uint16_t storage_alloc_range_data(etlsf_t arena);
static void     storage_free_range_data (etlsf_t arena, uint16_t index);

static uint32_t calc_range_size     (etlsf_t arena, uint16_t index);
static void     create_initial_range(etlsf_t arena);
static uint32_t split_range         (etlsf_t arena, uint16_t id, uint32_t size);
static void     merge_ranges        (etlsf_t arena, uint16_t target_index, uint16_t source_index);

static void     freelist_insert_range(etlsf_t arena, uint16_t index);
static void     freelist_remove_range(etlsf_t arena, uint16_t index);
static uint16_t freelist_find_suitable(etlsf_t arena, uint32_t size);

//-------------------------  API implementation  ----------------------------//

etlsf_t etlsf_create(uint32_t size, uint16_t max_allocs)
{
    ETLSF_assert(max_allocs > 0);
    ETLSF_assert(size % ALIGN_SIZE == 0);
    ETLSF_assert(RANGE_SIZE_MIN <= size && size <= RANGE_SIZE_MAX);

    etlsf_t arena = (etlsf_t)ETLSF_alloc(arena_total_size(max_allocs));

    ETLSF_memset(arena, sizeof(struct etlsf_private_t), 0); // sets also all lists point to zero block

    arena->size = size;
    arena->num_ranges = max_allocs;
    arena->unused_range_count = max_allocs;
    arena->first_free_storage_index = 0;


    create_initial_range(arena);

    return arena;
}

void etlsf_destroy(etlsf_t arena)
{
    ETLSF_assert(arena);

    ETLSF_free(arena);
}

etlsf_alloc_t etlsf_alloc_range(etlsf_t arena, uint32_t size)
{
    ETLSF_assert(size);
    ETLSF_assert(size <= RANGE_SIZE_MAX);

    const size_t adjust = ETLSF_align2(size, RANGE_SIZE_MIN);

    uint16_t index = freelist_find_suitable(arena, adjust);

    if (index)
    {
        ETLSF_assert(RANGE_DATA(index).is_free);

        freelist_remove_range(arena, index);

        ETLSF_assert(RANGE_DATA(index).offset % RANGE_SIZE_MIN == 0);

        uint16_t remainder_index = split_range(arena, index, adjust);

        if (remainder_index)
        {
            freelist_insert_range(arena, remainder_index);
        }
    }

    return { index };
}

void etlsf_free_range(etlsf_t arena, etlsf_alloc_t id)
{
    uint16_t index = id.value;
    if (index)
    {
        ETLSF_assert(etlsf_alloc_is_valid(arena, id) && "block already marked as free");

        //Merge prev block if free
        uint16_t prev_index = RANGE_DATA(index).prev_phys_index;
        if (prev_index && RANGE_DATA(prev_index).is_free)
        {
            freelist_remove_range(arena, prev_index);
            merge_ranges(arena, prev_index, index);

            index = prev_index;
        }

        //Merge next block if free
        uint16_t next_index = RANGE_DATA(index).next_phys_index;
        if (next_index && RANGE_DATA(next_index).is_free)
        {
            freelist_remove_range(arena, next_index);
            merge_ranges(arena, index, next_index);
        }

        freelist_insert_range(arena, index);
    }
}

uint32_t etlsf_alloc_size(etlsf_t arena, etlsf_alloc_t id)
{
    return etlsf_alloc_is_valid(arena, id) ? calc_range_size(arena, id.value) : 0;
}

uint32_t etlsf_alloc_offset(etlsf_t arena, etlsf_alloc_t id)
{
    return etlsf_alloc_is_valid(arena, id) ? RANGE_DATA(id.value).offset : 0;
}

int etlsf_alloc_is_valid(etlsf_t arena, etlsf_alloc_t id)
{
    return (id.value != 0) && !RANGE_DATA(id.value).is_free;
}

//------------------------------  Arena utils  --------------------------------//

static size_t arena_total_size(size_t max_allocs)
{
    return sizeof(etlsf_private_t) + max_allocs * sizeof(struct range_data_t);
}

//----------------------------  Storage utils  --------------------------------//
static uint16_t storage_alloc_range_data(etlsf_t arena)
{
    if (arena->first_free_storage_index)
    {
        ETLSF_assert(arena->first_free_storage_index <= arena->num_ranges);

        uint16_t id = arena->first_free_storage_index;
        arena->first_free_storage_index = RANGE_DATA(id).next_phys_index;

        return id;
    }

    if (arena->unused_range_count > 0)
    {
        uint16_t id = arena->num_ranges + 1 - arena->unused_range_count;
        --arena->unused_range_count;

        return id;
    }

    return 0;
}

static void storage_free_range_data(etlsf_t arena, uint16_t index)
{
    ETLSF_assert(index > 0);
    ETLSF_assert(index <= arena->num_ranges);
    ETLSF_assert(index + arena->unused_range_count <= arena->num_ranges);

    if (index + arena->unused_range_count == arena->num_ranges)
    {
        ++arena->unused_range_count;
    }
    else
    {
        RANGE_DATA(index).next_phys_index = arena->first_free_storage_index;
        arena->first_free_storage_index = index;
    }
}

//---------------------------  Physical ranges operations  --------------------------//

static uint32_t calc_range_size(etlsf_t arena, uint16_t index)
{
    assert(arena);
    assert(index);

    uint16_t next = RANGE_DATA(index).next_phys_index;
    return (next ? RANGE_DATA(next).offset : arena->size) - RANGE_DATA(index).offset;
}

static void create_initial_range(etlsf_t arena)
{
    ETLSF_assert(arena);
    /*
    ** Create the main free block. Offset the start of the block slightly
    ** so that the prev_phys_block field falls outside of the pool -
    ** it will never be used.
    */
    uint16_t index = storage_alloc_range_data(arena);
    RANGE_DATA(index).prev_phys_index = 0;
    RANGE_DATA(index).next_phys_index = 0;
    RANGE_DATA(index).offset = 0;
    freelist_insert_range(arena, index);
}

// returns block created after split
static uint32_t split_range(etlsf_t arena, uint16_t id, uint32_t size)
{
    ETLSF_assert(arena);
    ETLSF_assert(id);
    ETLSF_assert(size >= RANGE_SIZE_MIN);

    uint32_t bsize = calc_range_size(arena, id);

    uint16_t new_index = 0;
    bool can_split = bsize >= size + RANGE_SIZE_MIN;

    if (can_split && (new_index = storage_alloc_range_data(arena)))
    {
        uint16_t next = RANGE_DATA(id).next_phys_index;
        uint32_t offset = RANGE_DATA(id).offset;

        RANGE_DATA(id).next_phys_index = new_index;

        RANGE_DATA(new_index).offset = offset + size;
        RANGE_DATA(new_index).next_phys_index = next;
        RANGE_DATA(new_index).prev_phys_index = id;

        RANGE_DATA(next).prev_phys_index = new_index;
    }

    return new_index;
}

static void merge_ranges(etlsf_t arena, uint16_t target_index, uint16_t source_index)
{
    ETLSF_assert(arena);
    ETLSF_assert(target_index);
    ETLSF_assert(source_index);
    ETLSF_assert(RANGE_DATA(target_index).next_phys_index == source_index);

    RANGE_DATA(target_index).next_phys_index = RANGE_DATA(source_index).next_phys_index;

    storage_free_range_data(arena, source_index);
}


//------------------------------  Size utils  -------------------------------//

#define size_to_fl_sl(size, fl, sl)                                                      \
{                                                                                        \
    if (size < SMALL_ALLOC_SIZE)                                                         \
    {                                                                                    \
        /* Store small blocks in first list. */                                          \
        fl = 0;                                                                          \
        sl = size / (SMALL_ALLOC_SIZE / SL_INDEX_COUNT);                                 \
    }                                                                                    \
    else                                                                                 \
    {                                                                                    \
        fl = ETLSF_fls(size);                                                            \
        sl = (size >> (fl - SL_INDEX_COUNT_LOG2)) ^ (1 << SL_INDEX_COUNT_LOG2);          \
        fl -= (FL_INDEX_SHIFT - 1);                                                      \
    }                                                                                    \
}                                                                                        \

//------------------------------  Free list operations  ------------------------------//

//It is a bug when prev phys block is free
static void freelist_insert_range(etlsf_t arena, uint16_t index)
{
    ETLSF_assert(arena);
    ETLSF_assert(index);

    uint32_t fl = 0, sl = 0;
    uint32_t size = calc_range_size(arena, index);
    size_to_fl_sl(size, fl, sl);

    uint16_t next_free_index = arena->blocks[fl][sl];
    ETLSF_assert(index <= arena->num_ranges);
    ETLSF_assert(next_free_index <= arena->num_ranges);

    RANGE_DATA(index).prev_free_index = 0;
    RANGE_DATA(index).next_free_index = next_free_index;
    RANGE_DATA(index).is_free = 1;

    RANGE_DATA(next_free_index).prev_free_index = index;

    /*
    ** Insert the new block at the head of the list, and mark the first-
    ** and second-level bitmaps appropriately.
    */
    arena->blocks[fl][sl] = index;
    arena->fl_bitmap     |= (1 << fl);
    arena->sl_bitmap[fl] |= (1 << sl);
}

static void freelist_remove_range(etlsf_t arena, uint16_t index)
{
    assert(arena);
    assert(index);

    uint32_t size = calc_range_size(arena, index);
    uint32_t  fl = 0, sl = 0;
    size_to_fl_sl(size, fl, sl);

    uint16_t prev_index = RANGE_DATA(index).prev_free_index;
    uint16_t next_index = RANGE_DATA(index).next_free_index;

    ETLSF_assert(prev_index <= arena->num_ranges);
    ETLSF_assert(next_index <= arena->num_ranges);

    RANGE_DATA(next_index).prev_free_index = prev_index;
    RANGE_DATA(prev_index).next_free_index = next_index;
    RANGE_DATA(index).is_free = 0;

    /* If this block is the head of the free list, set new head. */
    if (arena->blocks[fl][sl] == index)
    {
        assert(prev_index == 0);

        arena->blocks[fl][sl] = next_index;

        /* If the new head is null, clear the bitmap. */
        if (next_index == 0)
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

static uint16_t freelist_find_suitable(etlsf_t arena, uint32_t size)
{
    uint16_t index = 0;

    if (size)
    {
        uint32_t fl_req = 0, sl_req = 0;
        size_to_fl_sl(size, fl_req, sl_req);

        const uint32_t fl_mask = 0xFFFFFFFF << fl_req;
        const uint32_t fl_map = arena->fl_bitmap & fl_mask;

        if (!fl_map)
        {
            return 0; // Out of memory
        }

        const uint32_t fl = ETLSF_ffs(fl_map);
        const uint32_t sl_map = arena->sl_bitmap[fl];

        ETLSF_assert(sl_map && "internal error - second level bitmap is null");

        const uint32_t sl_mask = 0xFFFFFFFF << ((fl > fl_req) ? 0 : sl_req);
        const uint32_t sl = ETLSF_ffs(sl_map & sl_mask);

        /* Return the first block in the free list. */
        index = arena->blocks[fl][sl];
        ETLSF_assert(calc_range_size(arena, index) >= size);
    }

    return index;
}
