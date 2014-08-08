#include <stddef.h>

#include "etlsf.h"

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

    BLOCK_FREE_BIT      = 1 << 0,

    BLOCK_COUNT_MAX = 0xFFFE,
};

struct block_t
{
    uint16_t  next_phys;
    uint16_t  prev_phys;

    uint16_t  next_free;
    uint16_t  prev_free;

    uint32_t  offset;
    uint32_t  size         : 31;
    uint32_t  is_free      : 1;
};

struct etlsf_arena_data_t
{
    mem::arena_t marena;

    /* Bitmaps for free lists. */
    unsigned int fl_bitmap;
    unsigned int sl_bitmap[FL_INDEX_COUNT];

    /* Head of free lists. */
    uint16_t blocks[FL_INDEX_COUNT][SL_INDEX_COUNT];

    uint16_t size;
    uint16_t unused;
    uint16_t first_ava;

    block_t  storage[1];
};

static uint32_t   arena_data_size();
static uint32_t   arena_total_size(uint16_t max_allocs);

static uint16_t   storage_new_block (etlsf_arena_t arena);
static void       storage_free_block(etlsf_arena_t arena, uint16_t id);

static  void      block_set_offset   (etlsf_arena_t arena, uint16_t id, uint32_t offset);
static  void      block_set_size     (etlsf_arena_t arena, uint16_t id, uint32_t size);
static  void      block_set_free     (etlsf_arena_t arena, uint16_t id);
static  void      block_set_used     (etlsf_arena_t arena, uint16_t id);
static  void      block_set_next_free(etlsf_arena_t arena, uint16_t id, uint16_t next);
static  void      block_set_prev_free(etlsf_arena_t arena, uint16_t id, uint16_t prev);
static  void      block_set_next_phys(etlsf_arena_t arena, uint16_t id, uint16_t next);
static  void      block_set_prev_phys(etlsf_arena_t arena, uint16_t id, uint16_t prev);

static  uint32_t  block_get_offset   (etlsf_arena_t arena, uint16_t id);
static  uint32_t  block_get_size     (etlsf_arena_t arena, uint16_t id);
static  int       block_is_free      (etlsf_arena_t arena, uint16_t id);
static  uint16_t  block_get_next_free(etlsf_arena_t arena, uint16_t id);
static  uint16_t  block_get_prev_free(etlsf_arena_t arena, uint16_t id);
static  uint16_t  block_get_next_phys(etlsf_arena_t arena, uint16_t id);
static  uint16_t  block_get_prev_phys(etlsf_arena_t arena, uint16_t id);

static  int       block_is_last      (etlsf_arena_t arena, uint16_t id);

static  void      block_insert_free_list  (etlsf_arena_t arena, uint16_t id);
static  void      block_remove_free_list  (etlsf_arena_t arena, uint16_t id, uint32_t fl, uint32_t sl);
static  uint16_t  block_find_suitable_free(etlsf_arena_t arena, uint32_t size);
static  uint16_t  block_merge_next        (etlsf_arena_t arena, uint16_t id);
static  void      block_trim              (etlsf_arena_t arena, uint16_t id, uint32_t size);

static  void      size_to_fl_sl(uint32_t size, uint32_t& fl, uint32_t& sl);


//-------------------------  API implementation  ----------------------------//
etlsf_arena_t etlsf_create(mem::arena_t marena, uint32_t size, uint16_t max_allocs)
{
    assert(max_allocs < BLOCK_COUNT_MAX);
    assert(size % ALIGN_SIZE == 0);

    etlsf_arena_t arena = (etlsf_arena_t)mem::alloc(marena, arena_total_size(max_allocs + 1));

    arena->marena = marena;

    memset(arena, 0, sizeof(etlsf_arena_data_t)); // sets also all lists point to zero block

    arena->size      = max_allocs + 1;
    arena->unused    = max_allocs;
    arena->first_ava = 0;

    assert(BLOCK_SIZE_MIN <= size && size <= BLOCK_SIZE_MAX);

    /*
    ** Create the main free block. Offset the start of the block slightly
    ** so that the prev_phys_block field falls outside of the pool -
    ** it will never be used.
    */
    uint16_t id = storage_new_block(arena);
    block_set_offset      (arena, id, 0);
    block_set_size        (arena, id, size);
    block_set_free        (arena, id);
    block_insert_free_list(arena, id);

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
    uint16_t id = block_find_suitable_free(arena, adjust);

    if (id)
    {
        //block should be marked as used before trim, as trim will try to merge
        //it with trimmed again
        block_set_used(arena, id);
        block_trim(arena, id, size);
    }

    return id;
}

void etlsf_free(etlsf_arena_t arena, uint16_t block)
{
    if (block)
    {
        assert(!block_is_free(arena, block) && "block already marked as free");
        block_insert_free_list(arena, block);
    }
}

uint32_t etlsf_block_size(etlsf_arena_t arena, uint16_t block)
{
    return block_get_size(arena, block);
}

uint32_t etlsf_block_offset(etlsf_arena_t arena, uint16_t block)
{
    return block_get_offset(arena, block);
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
    if (arena->first_ava)
    {
        assert(arena->first_ava < arena->size);

        uint16_t id = arena->first_ava;
        arena->first_ava = block_get_next_free(arena, id);

        return id;
    }

    if (arena->unused > 0)
    {
        uint16_t id = arena->size - arena->unused;
        --arena->unused;

        return id;
    }

    return 0;
}

static void storage_free_block(etlsf_arena_t arena, uint16_t id)
{
    assert(id > 0);
    assert(id < arena->size);
    assert(id+arena->unused < arena->size);

    if (id + 1 + arena->unused == arena->size)
    {
        ++arena->unused;
    }
    else
    {
        block_set_next_free(arena, id, arena->first_ava);
        arena->first_ava = id;
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


//------------------------------  Block accessors  ------------------------------//
static void block_set_offset(etlsf_arena_t arena, uint16_t id, uint32_t offset)
{
    assert(id < arena->size);

    arena->storage[id].offset = offset;
}

static void block_set_size(etlsf_arena_t arena, uint16_t id, uint32_t size)
{
    assert(id < arena->size);
    assert(size >= BLOCK_SIZE_MIN);
    uint32_t oldsize = arena->storage[id].size;
    arena->storage[id].size = size | (oldsize & BLOCK_FREE_BIT);
}

static void block_set_free(etlsf_arena_t arena, uint16_t id)
{
    assert(id < arena->size);
    arena->storage[id].size |= BLOCK_FREE_BIT;
}

static void block_set_used(etlsf_arena_t arena, uint16_t id)
{
    assert(id < arena->size);
    arena->storage[id].size &= ~BLOCK_FREE_BIT;
}

static void block_set_next_free(etlsf_arena_t arena, uint16_t id, uint16_t next)
{
    assert(id < arena->size);
    assert(next < arena->size);
    arena->storage[id].next_free = next;
}

static void block_set_prev_free(etlsf_arena_t arena, uint16_t id, uint16_t prev)
{
    assert(id < arena->size);
    assert(prev < arena->size);
    arena->storage[id].prev_free = prev;
}

static void block_set_next_phys(etlsf_arena_t arena, uint16_t id, uint16_t next)
{
    assert(id < arena->size);
    assert(next < arena->size);
    arena->storage[id].next_phys = next;
}

static void block_set_prev_phys(etlsf_arena_t arena, uint16_t id, uint16_t prev)
{
    assert(id < arena->size);
    assert(prev < arena->size);
    arena->storage[id].prev_phys = prev;
}

static uint32_t block_get_offset(etlsf_arena_t arena, uint16_t id)
{
    assert(id < arena->size);
    return arena->storage[id].offset;
}

static uint32_t block_get_size(etlsf_arena_t arena, uint16_t id)
{
    assert(id < arena->size);
    return arena->storage[id].size & ~BLOCK_FREE_BIT;
}

static int block_is_free(etlsf_arena_t arena, uint16_t id)
{
    assert(id < arena->size);
    return arena->storage[id].size & BLOCK_FREE_BIT;
}

static uint16_t block_get_next_free(etlsf_arena_t arena, uint16_t id)
{
    assert(id < arena->size);
    return arena->storage[id].next_free;
}

static uint16_t block_get_prev_free(etlsf_arena_t arena, uint16_t id)
{
    assert(id < arena->size);
    return arena->storage[id].prev_free;
}

static uint16_t block_get_next_phys(etlsf_arena_t arena, uint16_t id)
{
    assert(id < arena->size);
    return arena->storage[id].next_phys;
}

static uint16_t block_get_prev_phys(etlsf_arena_t arena, uint16_t id)
{
    assert(id < arena->size);
    return arena->storage[id].prev_phys;
}


//----------------------------  Block utils  ------------------------------
static int block_is_last(etlsf_arena_t arena, uint16_t id)
{
    assert(id < arena->size);
    return block_get_next_phys(arena, id) == 0;
}


//------------------------------  Block operations  ------------------------------//
static uint16_t block_merge_next(etlsf_arena_t arena, uint16_t id)
{
    assert(id && "next physical block can't be null");
    assert(!block_is_last(arena, id) && "previous block can't be last!");

    uint16_t next = block_get_next_phys(arena, id);
    uint32_t size_next = block_get_size(arena, next);
    uint32_t size      = block_get_size(arena, id);
    block_set_size(arena, id, size + size_next);
    block_set_next_phys(arena, id, next);
    block_set_prev_phys(arena, next, id);

    if (block_is_free(arena, next))
    {
        uint32_t fl, sl;
        size_to_fl_sl(size_next, fl, sl);
        block_remove_free_list(arena, next, fl, sl);
    }

    storage_free_block(arena, next);

    return id;
}

static void block_trim(etlsf_arena_t arena, uint16_t id, uint32_t size)
{
    assert(size>=BLOCK_SIZE_MIN);
    uint32_t bsize = block_get_size(arena, id);
    if (bsize >= size + BLOCK_SIZE_MIN)
    {
        uint16_t next   = block_get_next_phys(arena, id);
        uint32_t offset = block_get_offset(arena, id);

        uint16_t new_block = storage_new_block(arena);

        if (!new_block)
        {
            assert(0);
            return;
        }

        block_set_size(arena, id, size);

        block_set_offset(arena, new_block, offset+size);
        block_set_size  (arena, new_block, bsize-size);

        block_set_next_phys(arena, id, new_block);

        block_set_next_phys(arena, new_block, next);
        block_set_prev_phys(arena, new_block, id);

        block_set_prev_phys(arena, next, new_block);

        block_insert_free_list(arena, new_block);
    }
}

static void block_insert_free_list(etlsf_arena_t arena, uint16_t id)
{
    assert(id);

    uint16_t prev = block_get_prev_phys(arena, id);
    uint16_t next = block_get_next_phys(arena, id);
    if (block_is_free(arena, prev))
        id = block_merge_next(arena, prev);
    if (block_is_free(arena, next))
        id = block_merge_next(arena, id);

    block_set_free(arena, id);

    uint32_t fl = 0, sl = 0;
    uint32_t size = block_get_size(arena, id);
    size_to_fl_sl(size, fl, sl);

    next = arena->blocks[fl][sl];
    assert(id < arena->size);
    assert(next < arena->size);

    block_set_prev_free(arena, id, 0);
    block_set_next_free(arena, id, next);
    block_set_prev_free(arena, next, id);

    /*
    ** Insert the new block at the head of the list, and mark the first-
    ** and second-level bitmaps appropriately.
    */
    arena->blocks[fl][sl] = id;
    arena->fl_bitmap     |= (1 << fl);
    arena->sl_bitmap[fl] |= (1 << sl);
}

static void block_remove_free_list(etlsf_arena_t arena, uint16_t id, uint32_t fl, uint32_t sl)
{
    uint16_t prev = block_get_prev_free(arena, id);
    uint16_t next = block_get_next_free(arena, id);
    assert(prev < arena->size);
    assert(next < arena->size);
    block_set_prev_free(arena, next, prev);
    block_set_next_free(arena, prev, next);

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

static uint16_t block_find_suitable_free(etlsf_arena_t arena, uint32_t size)
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
        assert(block_get_size(arena, id) >= size);
        block_remove_free_list(arena, id, fl, sl);
    }

    return id;
}
