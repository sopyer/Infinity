#include <stddef.h>

#include "etlsf.h"

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
        used_block_data_t used_data;
        free_block_data_t free_data;
    };
};

struct etlsf_arena_data_t
{
    mem::arena_t marena;

    /* Bitmaps for free lists. */
    unsigned int fl_bitmap;
    unsigned int sl_bitmap[FL_INDEX_COUNT];

    /* Head of free lists. */
    uint16_t blocks[FL_INDEX_COUNT][SL_INDEX_COUNT];

    uint32_t offset;
    uint32_t size;
    uint16_t total_blocks;
    uint16_t unused_blocks;
    uint16_t first_free_block;

    block_t  storage[1];
};

static uint32_t   arena_data_size();
static uint32_t   arena_total_size(uint16_t max_allocs);

static uint16_t   storage_new_block (etlsf_arena_t arena);
static void       storage_free_block(etlsf_arena_t arena, uint16_t id);

static  void      block_register_free  (etlsf_arena_t arena, uint16_t id);
static  void      block_unregister_free(etlsf_arena_t arena, uint16_t id);
static  uint16_t  block_find_free      (etlsf_arena_t arena, uint32_t size);
static  void      block_trim           (etlsf_arena_t arena, uint16_t id);

//-------------------------  API implementation  ----------------------------//
etlsf_arena_t etlsf_create(mem::arena_t marena, uint32_t offset, uint32_t size, uint16_t max_allocs)
{
    assert(max_allocs < BLOCK_COUNT_MAX);
    assert(size % ALIGN_SIZE == 0);
    assert(offset % ALIGN_SIZE == 0);

    etlsf_arena_t arena = (etlsf_arena_t)mem::alloc(marena, arena_total_size(max_allocs + 1));

    memset(arena, 0, sizeof(etlsf_arena_data_t)); // sets also all lists point to zero block

    arena->marena           = marena;
    arena->offset           = offset;
    arena->size             = size;
    arena->total_blocks     = max_allocs + 1;
    arena->unused_blocks    = max_allocs;
    arena->first_free_block = 0;

    assert(BLOCK_SIZE_MIN <= size && size <= BLOCK_SIZE_MAX);

    /*
    ** Create the main free block. Offset the start of the block slightly
    ** so that the prev_phys_block field falls outside of the pool -
    ** it will never be used.
    */
    uint16_t id = storage_new_block(arena);
    block_register_free(arena, id);

    return arena;
}

void etlsf_destroy(etlsf_arena_t arena)
{
    assert(arena);

    mem::arena_t marena = arena->marena;

    mem::free(marena, arena);
}

uint16_t etlsf_alloc(etlsf_arena_t arena, uint32_t size)
{
    assert(size);
    assert(size <= BLOCK_SIZE_MAX);

    const size_t adjust = bit_align_up(size, BLOCK_SIZE_MIN);

    //!!!!!TODO: add support for arbitrary size and alignment
    uint16_t id = block_find_free(arena, adjust);
    BLOCK_MEM(id, used_data.size) = adjust;
    block_trim(arena, id);

    assert(BLOCK_MEM(id, offset) % BLOCK_SIZE_MIN == 0);

    return id;
}

void etlsf_free(etlsf_arena_t arena, uint16_t block)
{
    if (block)
    {
        assert(etlsf_is_block_valid(arena, block) && "block already marked as free");
        block_register_free(arena, block);
    }
}

uint32_t etlsf_block_size(etlsf_arena_t arena, uint16_t block)
{
    assert(etlsf_is_block_valid(arena, block));
    return BLOCK_MEM(block, used_data.size);
}

uint32_t etlsf_block_offset(etlsf_arena_t arena, uint16_t block)
{
    assert(etlsf_is_block_valid(arena, block));
    return arena->offset + BLOCK_MEM(block, offset);
}

int etlsf_is_block_valid(etlsf_arena_t arena, uint16_t block)
{
    return !BLOCK_MEM(block, is_free);
}


//------------------------------  Arena utils  --------------------------------//
static uint32_t arena_data_size()
{
    return offsetof(etlsf_arena_data_t, storage);
}

static uint32_t arena_total_size(uint16_t max_allocs)
{
    return arena_data_size() + max_allocs * sizeof(block_t);
}


//----------------------------  Storage utils  --------------------------------//
static uint16_t storage_new_block(etlsf_arena_t arena)
{
    if (arena->first_free_block)
    {
        assert(arena->first_free_block < arena->total_blocks);

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

static void storage_free_block(etlsf_arena_t arena, uint16_t id)
{
    assert(id > 0);
    assert(id < arena->total_blocks);
    assert(id+arena->unused_blocks < arena->total_blocks);

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
static void size_to_fl_sl(uint32_t size, uint32_t& fl, uint32_t& sl)
{
    if (size < SMALL_BLOCK_SIZE)
    {
        /* Store small blocks in first list. */
        fl = 0;
        sl = size / (SMALL_BLOCK_SIZE / SL_INDEX_COUNT);
    }
    else
    {
        fl = bit_fls(size);
        sl = (size >> (fl - SL_INDEX_COUNT_LOG2)) ^ (1 << SL_INDEX_COUNT_LOG2);
        fl -= (FL_INDEX_SHIFT - 1);
    }
}

//------------------------------  Block operations  ------------------------------//
static uint32_t block_get_actual_size(etlsf_arena_t arena, uint16_t id)
{
    uint16_t next = BLOCK_MEM(id, next_phys);
    return (next ? BLOCK_MEM(next, offset) : arena->size) - BLOCK_MEM(id, offset);
}

static void block_trim(etlsf_arena_t arena, uint16_t id)
{
    uint32_t bsize = block_get_actual_size(arena, id);
    uint32_t size  = BLOCK_MEM(id, used_data.size);

    assert(!BLOCK_MEM(id, is_free));
    assert(size>=BLOCK_SIZE_MIN);

    if (bsize >= size + BLOCK_SIZE_MIN)
    {
        uint16_t next   = BLOCK_MEM(id, next_phys);
        uint32_t offset = BLOCK_MEM(id, offset);

        uint16_t new_block = storage_new_block(arena);

        if (!new_block)
        {
            assert(0);
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
static void block_register_free(etlsf_arena_t arena, uint16_t id)
{
    assert(id);

    uint16_t prev = BLOCK_MEM(id, prev_phys);
    uint16_t next = BLOCK_MEM(id, next_phys);
    //Merge prev block if free
    if (BLOCK_MEM(prev, is_free))
    {
        uint16_t prev_prev = BLOCK_MEM(prev, prev_phys);

        assert(prev != 0 && "block must not be first");

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

        assert(next != 0 && "block must not be last");

        BLOCK_MEM(id,        next_phys)      = next_next;
        BLOCK_MEM(next_next, prev_phys)      = id;

        block_unregister_free(arena, next);
        storage_free_block(arena, next);
    }

    uint32_t fl = 0, sl = 0;
    uint32_t size = block_get_actual_size(arena, id);
    size_to_fl_sl(size, fl, sl);

    uint16_t next_free = arena->blocks[fl][sl];
    assert(id < arena->total_blocks);
    assert(next_free < arena->total_blocks);

    BLOCK_MEM(id,        free_data.prev_free)   = 0;
    BLOCK_MEM(id,        free_data.next_free)   = next_free;
    BLOCK_MEM(next_free, free_data.prev_free) = id;

    BLOCK_MEM(id, free_data.fl) = fl;
    BLOCK_MEM(id, free_data.sl) = sl;
    BLOCK_MEM(id, is_free)      = TRUE;

    /*
    ** Insert the new block at the head of the list, and mark the first-
    ** and second-level bitmaps appropriately.
    */
    arena->blocks[fl][sl] = id;
    arena->fl_bitmap     |= (1 << fl);
    arena->sl_bitmap[fl] |= (1 << sl);
}

static void block_unregister_free(etlsf_arena_t arena, uint16_t id)
{
    uint32_t fl = BLOCK_MEM(id, free_data.fl);
    uint32_t sl = BLOCK_MEM(id, free_data.sl);

    uint16_t prev = BLOCK_MEM(id, free_data.prev_free);
    uint16_t next = BLOCK_MEM(id, free_data.next_free);

    assert(prev < arena->total_blocks);
    assert(next < arena->total_blocks);

    BLOCK_MEM(next, free_data.prev_free) = prev;
    BLOCK_MEM(prev, free_data.next_free) = next;
    BLOCK_MEM(id, is_free) = FALSE;

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

static uint16_t block_find_free(etlsf_arena_t arena, uint32_t size)
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

            fl = bit_ffs(fl_map);
            sl_map = arena->sl_bitmap[fl];
        }
        assert(sl_map && "internal error - second level bitmap is null");
        sl = bit_ffs(sl_map);

        /* Return the first block in the free list. */
        id = arena->blocks[fl][sl];
    }

    if (id)
    {
        assert(block_get_actual_size(arena, id) >= size);
        block_unregister_free(arena, id);
    }

    return id;
}
