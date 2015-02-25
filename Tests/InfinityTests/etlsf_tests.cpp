#include <sput.h>

#include <core/etlsf.h>

enum test_private
{
    ARENA_SYSMEM_SIZE = 64*1024,
    ARENA_EXTMEM_SIZE = 20*1024,
};


void test_alloc_free()
{
    etlsf_arena_t  arena;
    mspace_t  marena;

    uint16_t id0, id1, id2, id3, id4, id5;

    marena = mem_create_space(ARENA_SYSMEM_SIZE);
    arena = etlsf_create(marena, 0, ARENA_EXTMEM_SIZE, 128);

    id0 = etlsf_alloc(arena, ARENA_EXTMEM_SIZE);
    sput_fail_unless(id0, "Allocation succeeded");
    sput_fail_unless(etlsf_block_offset(arena, id0) == 0, "Single allocation offset test");
    sput_fail_unless(etlsf_block_size(arena, id0) == ARENA_EXTMEM_SIZE, "Single allocation size test");
    etlsf_free(arena, id0);

    id0 = etlsf_alloc(arena, 2 * 1024);
    id1 = etlsf_alloc(arena, 2 * 1024);
    id2 = etlsf_alloc(arena, 4 * 1024);
    id3 = etlsf_alloc(arena, 3 * 1024);
    id4 = etlsf_alloc(arena, 5 * 1024);
    id5 = etlsf_alloc(arena, 4 * 1024);

    sput_fail_unless(id0, "Allocation succeeded");
    sput_fail_unless(id1, "Allocation succeeded");
    sput_fail_unless(id2, "Allocation succeeded");
    sput_fail_unless(id3, "Allocation succeeded");
    sput_fail_unless(id4, "Allocation succeeded");
    sput_fail_unless(id5, "Allocation succeeded");

    sput_fail_unless(etlsf_block_offset(arena, id0) ==  0 * 1024, "Allocation offset test");
    sput_fail_unless(etlsf_block_offset(arena, id1) ==  2 * 1024, "Allocation offset test");
    sput_fail_unless(etlsf_block_offset(arena, id2) ==  4 * 1024, "Allocation offset test");
    sput_fail_unless(etlsf_block_offset(arena, id3) ==  8 * 1024, "Allocation offset test");
    sput_fail_unless(etlsf_block_offset(arena, id4) == 11 * 1024, "Allocation offset test");
    sput_fail_unless(etlsf_block_offset(arena, id5) == 16 * 1024, "Allocation offset test");

    sput_fail_unless(etlsf_block_size(arena, id0) == 2 * 1024, "Allocation size test");
    sput_fail_unless(etlsf_block_size(arena, id1) == 2 * 1024, "Allocation size test");
    sput_fail_unless(etlsf_block_size(arena, id2) == 4 * 1024, "Allocation size test");
    sput_fail_unless(etlsf_block_size(arena, id3) == 3 * 1024, "Allocation size test");
    sput_fail_unless(etlsf_block_size(arena, id4) == 5 * 1024, "Allocation size test");
    sput_fail_unless(etlsf_block_size(arena, id5) == 4 * 1024, "Allocation size test");

    etlsf_free(arena, id2);
    etlsf_free(arena, id4);

    id2 = etlsf_alloc(arena, 4*1024+512);
    id4 = etlsf_alloc(arena, 256);

    sput_fail_unless(id2, "Allocation succeeded");
    sput_fail_unless(etlsf_block_offset(arena, id2) == 11*1024, "Single allocation offset test");
    sput_fail_unless(etlsf_block_size(arena, id2) == 4*1024+512, "Single allocation size test");

    sput_fail_unless(id4, "Allocation succeeded");
    sput_fail_unless(etlsf_block_offset(arena, id4) == 11*1024 + 4*1024+512, "Single allocation offset test");
    sput_fail_unless(etlsf_block_size(arena, id4) == 256, "Single allocation size test");

    etlsf_destroy(arena);
    mem_destroy_space(marena);
}

void test_merge_prev()
{
    etlsf_arena_t  arena;
    mspace_t  marena;

    uint16_t id0, id1, id2;

    marena = mem_create_space(ARENA_SYSMEM_SIZE);
    arena = etlsf_create(marena, 0, 1024, 128);

    id0 = etlsf_alloc(arena, 256);
    id1 = etlsf_alloc(arena, 256);
    id2 = etlsf_alloc(arena, 256);

    sput_fail_unless(id0, "Allocation succeeded");
    sput_fail_unless(id1, "Allocation succeeded");
    sput_fail_unless(id2, "Allocation succeeded");

    sput_fail_unless(etlsf_block_offset(arena, id0) ==    0, "Allocation offset test");
    sput_fail_unless(etlsf_block_offset(arena, id1) ==  256, "Allocation offset test");
    sput_fail_unless(etlsf_block_offset(arena, id2) ==  512, "Allocation offset test");

    sput_fail_unless(etlsf_block_size(arena, id0) == 256, "Allocation size test");
    sput_fail_unless(etlsf_block_size(arena, id1) == 256, "Allocation size test");
    sput_fail_unless(etlsf_block_size(arena, id2) == 256, "Allocation size test");

    etlsf_free(arena, id0);
    etlsf_free(arena, id1);

    id0 = etlsf_alloc(arena, 512);
    id1 = etlsf_alloc(arena, 256);

    sput_fail_unless(id0, "Allocation succeeded");
    sput_fail_unless(etlsf_block_offset(arena, id0) == 0, "Allocation offset test");
    sput_fail_unless(etlsf_block_size(arena, id0) == 512, "Allocation size test");

    sput_fail_unless(id1, "Allocation succeeded");
    sput_fail_unless(etlsf_block_offset(arena, id1) == 768, "Allocation offset test");
    sput_fail_unless(etlsf_block_size(arena, id1) == 256, "Allocation size test");

    etlsf_destroy(arena);
    mem_destroy_space(marena);
}

void test_merge_next()
{
    etlsf_arena_t  arena;
    mspace_t  marena;

    uint16_t id0, id1, id2;

    marena = mem_create_space(ARENA_SYSMEM_SIZE);
    arena = etlsf_create(marena, 0, 1024, 128);

    id0 = etlsf_alloc(arena, 256);
    id1 = etlsf_alloc(arena, 256);
    id2 = etlsf_alloc(arena, 256);

    sput_fail_unless(id0, "Allocation succeeded");
    sput_fail_unless(id1, "Allocation succeeded");
    sput_fail_unless(id2, "Allocation succeeded");

    sput_fail_unless(etlsf_block_offset(arena, id0) ==    0, "Allocation offset test");
    sput_fail_unless(etlsf_block_offset(arena, id1) ==  256, "Allocation offset test");
    sput_fail_unless(etlsf_block_offset(arena, id2) ==  512, "Allocation offset test");

    sput_fail_unless(etlsf_block_size(arena, id0) == 256, "Allocation size test");
    sput_fail_unless(etlsf_block_size(arena, id1) == 256, "Allocation size test");
    sput_fail_unless(etlsf_block_size(arena, id2) == 256, "Allocation size test");

    etlsf_free(arena, id1);
    etlsf_free(arena, id0);

    id0 = etlsf_alloc(arena, 512);
    id1 = etlsf_alloc(arena, 256);

    sput_fail_unless(id0, "Allocation succeeded");
    sput_fail_unless(etlsf_block_offset(arena, id0) == 0, "Allocation offset test");
    sput_fail_unless(etlsf_block_size(arena, id0) == 512, "Allocation size test");

    sput_fail_unless(id1, "Allocation succeeded");
    sput_fail_unless(etlsf_block_offset(arena, id1) == 768, "Allocation offset test");
    sput_fail_unless(etlsf_block_size(arena, id1) == 256, "Allocation size test");

    etlsf_destroy(arena);
    mem_destroy_space(marena);
}

int run_etlsf_tests()
{
    sput_start_testing();

    sput_enter_suite("ETLSF: alloc/free");
    sput_run_test(test_alloc_free);
    sput_enter_suite("ETLSF: merge prev");
    sput_run_test(test_merge_prev);
    sput_enter_suite("ETLSF: merge next");
    sput_run_test(test_merge_next);

    sput_finish_testing();

    return sput_get_return_value();
}
