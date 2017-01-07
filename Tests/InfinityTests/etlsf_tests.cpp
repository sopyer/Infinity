#include <sput.h>

#include <core/core.h>
#include <etlsf.h>

enum test_private
{
    ARENA_SYSMEM_SIZE = 64*1024,
    ARENA_EXTMEM_SIZE = 20*1024,
};


void test_basic_alloc_free()
{
    etlsf_t  arena;

    etlsf_alloc_t id0, id1, id2, id3, id4, id5;

    arena = etlsf_create(ARENA_EXTMEM_SIZE, 128);

    id0 = etlsf_alloc_range(arena, ARENA_EXTMEM_SIZE);
    sput_fail_unless(id0.value, "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_offset(arena, id0) == 0, "Single allocation offset test");
    sput_fail_unless(etlsf_alloc_size(arena, id0) == ARENA_EXTMEM_SIZE, "Single allocation size test");
    etlsf_free_range(arena, id0);

    id0 = etlsf_alloc_range(arena, 2 * 1024);
    id1 = etlsf_alloc_range(arena, 2 * 1024);
    id2 = etlsf_alloc_range(arena, 4 * 1024);
    id3 = etlsf_alloc_range(arena, 3 * 1024);
    id4 = etlsf_alloc_range(arena, 5 * 1024);
    id5 = etlsf_alloc_range(arena, 4 * 1024);

    sput_fail_unless(etlsf_alloc_is_valid(arena, id0), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_is_valid(arena, id1), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_is_valid(arena, id2), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_is_valid(arena, id3), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_is_valid(arena, id4), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_is_valid(arena, id5), "Allocation succeeded");

    sput_fail_unless(etlsf_alloc_offset(arena, id0) ==  0 * 1024, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_offset(arena, id1) ==  2 * 1024, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_offset(arena, id2) ==  4 * 1024, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_offset(arena, id3) ==  8 * 1024, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_offset(arena, id4) == 11 * 1024, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_offset(arena, id5) == 16 * 1024, "Allocation offset test");

    sput_fail_unless(etlsf_alloc_size(arena, id0) == 2 * 1024, "Allocation size test");
    sput_fail_unless(etlsf_alloc_size(arena, id1) == 2 * 1024, "Allocation size test");
    sput_fail_unless(etlsf_alloc_size(arena, id2) == 4 * 1024, "Allocation size test");
    sput_fail_unless(etlsf_alloc_size(arena, id3) == 3 * 1024, "Allocation size test");
    sput_fail_unless(etlsf_alloc_size(arena, id4) == 5 * 1024, "Allocation size test");
    sput_fail_unless(etlsf_alloc_size(arena, id5) == 4 * 1024, "Allocation size test");

    etlsf_free_range(arena, id2);
    etlsf_free_range(arena, id4);

    id2 = etlsf_alloc_range(arena, 4*1024+512);
    id4 = etlsf_alloc_range(arena, 256);

    sput_fail_unless(etlsf_alloc_is_valid(arena, id2), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_offset(arena, id2) == 11*1024, "Single allocation offset test");
    sput_fail_unless(etlsf_alloc_size(arena, id2) == 4*1024+512, "Single allocation size test");

    sput_fail_unless(etlsf_alloc_is_valid(arena, id4), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_offset(arena, id4) == 11*1024 + 4*1024+512, "Single allocation offset test");
    sput_fail_unless(etlsf_alloc_size(arena, id4) == 256, "Single allocation size test");

    etlsf_destroy(arena);
}

void test_merge_prev()
{
    etlsf_t  arena;

    etlsf_alloc_t id0, id1, id2;

    arena = etlsf_create(1024, 128);

    id0 = etlsf_alloc_range(arena, 256);
    id1 = etlsf_alloc_range(arena, 256);
    id2 = etlsf_alloc_range(arena, 256);

    sput_fail_unless(etlsf_alloc_is_valid(arena, id0), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_is_valid(arena, id1), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_is_valid(arena, id2), "Allocation succeeded");

    sput_fail_unless(etlsf_alloc_offset(arena, id0) ==    0, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_offset(arena, id1) ==  256, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_offset(arena, id2) ==  512, "Allocation offset test");

    sput_fail_unless(etlsf_alloc_size(arena, id0) == 256, "Allocation size test");
    sput_fail_unless(etlsf_alloc_size(arena, id1) == 256, "Allocation size test");
    sput_fail_unless(etlsf_alloc_size(arena, id2) == 256, "Allocation size test");

    etlsf_free_range(arena, id0);
    etlsf_free_range(arena, id1);

    id0 = etlsf_alloc_range(arena, 512);
    id1 = etlsf_alloc_range(arena, 256);

    sput_fail_unless(etlsf_alloc_is_valid(arena, id0), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_offset(arena, id0) == 0, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_size(arena, id0) == 512, "Allocation size test");

    sput_fail_unless(etlsf_alloc_is_valid(arena, id1), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_offset(arena, id1) == 768, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_size(arena, id1) == 256, "Allocation size test");

    etlsf_destroy(arena);
}

void test_merge_next()
{
    etlsf_t  arena;

    etlsf_alloc_t id0, id1, id2;

    arena = etlsf_create(1024, 128);

    id0 = etlsf_alloc_range(arena, 256);
    id1 = etlsf_alloc_range(arena, 256);
    id2 = etlsf_alloc_range(arena, 256);

    sput_fail_unless(etlsf_alloc_is_valid(arena, id0), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_is_valid(arena, id1), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_is_valid(arena, id2), "Allocation succeeded");

    sput_fail_unless(etlsf_alloc_offset(arena, id0) ==    0, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_offset(arena, id1) ==  256, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_offset(arena, id2) ==  512, "Allocation offset test");

    sput_fail_unless(etlsf_alloc_size(arena, id0) == 256, "Allocation size test");
    sput_fail_unless(etlsf_alloc_size(arena, id1) == 256, "Allocation size test");
    sput_fail_unless(etlsf_alloc_size(arena, id2) == 256, "Allocation size test");

    etlsf_free_range(arena, id1);
    etlsf_free_range(arena, id0);

    id0 = etlsf_alloc_range(arena, 512);
    id1 = etlsf_alloc_range(arena, 256);

    sput_fail_unless(etlsf_alloc_is_valid(arena, id0), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_offset(arena, id0) == 0, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_size(arena, id0) == 512, "Allocation size test");

    sput_fail_unless(etlsf_alloc_is_valid(arena, id1), "Allocation succeeded");
    sput_fail_unless(etlsf_alloc_offset(arena, id1) == 768, "Allocation offset test");
    sput_fail_unless(etlsf_alloc_size(arena, id1) == 256, "Allocation size test");

    etlsf_destroy(arena);
}


void test_max_allocs()
{
    const uint16_t  MAX_ALLOCS = 0xFFFF;
    const uint32_t  MIN_ALLOC  = 0x100;
    const uint32_t  MEM_SIZE   = MIN_ALLOC * MAX_ALLOCS;

    static etlsf_alloc_t allocs[MAX_ALLOCS];

    etlsf_t  arena = etlsf_create(MEM_SIZE, MAX_ALLOCS);

    bool tests_passed;

    tests_passed = true;
    for (size_t i=0; i < MAX_ALLOCS; ++i)
    {
        allocs[i] = etlsf_alloc_range(arena, MIN_ALLOC);

        tests_passed &= (allocs[i].value != 0);
        tests_passed &= (etlsf_alloc_offset(arena, allocs[i]) == i*MIN_ALLOC);
        tests_passed &= (etlsf_alloc_size(arena, allocs[i]) == MIN_ALLOC);
    }
    sput_fail_unless(tests_passed, "All allocation succeeded, sizes and offsets are correct");

    tests_passed = true;
    for (size_t i=0; i < 0x10001; ++i)
    {
        etlsf_alloc_t id = etlsf_alloc_range(arena, MIN_ALLOC);
        tests_passed &= (id.value == 0);
    }
    sput_fail_unless(tests_passed, "All additional 0x10001 allocation predictably failed");


    tests_passed = true;
    for (size_t i=0; i < MAX_ALLOCS; i+=2)
    {
        etlsf_free_range(arena, allocs[i]);
        allocs[i] = { 0 };
    }

    uint32_t offset_sum = 0;
    for (size_t i=0; i < MAX_ALLOCS; i+=2)
    {
        allocs[i] = etlsf_alloc_range(arena, MIN_ALLOC);

        offset_sum += etlsf_alloc_offset(arena, allocs[i]) / MIN_ALLOC / 2;

        tests_passed &= (allocs[i].value != 0);
        tests_passed &= (etlsf_alloc_offset(arena, allocs[i]) % MIN_ALLOC == 0);
        tests_passed &= (etlsf_alloc_size(arena, allocs[i]) == MIN_ALLOC);
    }
    // All offsets after sorting should form an arithmetic progression sequence
    // So calculate their sum divided by MIN_ALLOC*2
    // count = (MAX_ALLOC + 1) / 2 //round count up
    // max_offset = MIN_ALLOC * (MAX_ALLOC - 1)
    // sum  = (0 + max_offset) / 2 * count
    // sum /= MIN_ALLOC*2
    const uint32_t EXPECTED_SUM = (MAX_ALLOCS-1) / 2 * (MAX_ALLOCS + 1) / 2 / 2;
    sput_fail_unless(tests_passed && offset_sum == EXPECTED_SUM, "All allocation after every second allocation was freed(starting from first), succeeded, sizes and offsets are correct");

    etlsf_destroy(arena);
}

int run_etlsf_tests()
{
    core::init();

    sput_start_testing();

    sput_enter_suite("ETLSF: basic alloc/free");
    sput_run_test(test_basic_alloc_free);
    sput_enter_suite("ETLSF: merge prev");
    sput_run_test(test_merge_prev);
    sput_enter_suite("ETLSF: merge next");
    sput_run_test(test_merge_next);
    sput_enter_suite("ETLSF: max allocs");
    sput_run_test(test_max_allocs);

    sput_finish_testing();

    core::fini();

    return sput_get_return_value();
}
