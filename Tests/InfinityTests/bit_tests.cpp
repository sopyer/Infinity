#include <sput.h>

#include <core/core.h>
#include <etlsf.h>

void test_ffs32()
{
    sput_fail_unless(bit_ffs(0u)==-1, "Edge case: arg=0");
    sput_fail_unless(bit_ffs(1u)==0,  "arg=1");
    sput_fail_unless(bit_ffs(0xFFFFFFFFu)==0,  "arg=0xFFFFFFFF");
    sput_fail_unless(bit_ffs(0x80000000u)==31,  "arg=0x80000000");

    for (uint32_t i=0; i<32; ++i)
    {
        sput_fail_unless(bit_ffs(0xFFFFFFFFu<<i)==i, "General test0");
        sput_fail_unless(bit_ffs(1u<<i)==i, "General test1");
        sput_fail_unless(bit_ffs(0xFFFFFFFFu>>i)==0, "General test2");
    }
}

void test_fls32()
{
    sput_fail_unless(bit_fls(0u)==-1, "Edge case: arg=0");
    sput_fail_unless(bit_fls(1u)==0,  "arg=1");
    sput_fail_unless(bit_fls(0xFFFFFFFFu)==31,  "arg=0xFFFFFFFF");
    sput_fail_unless(bit_fls(0x80000000u)==31,  "arg=0x80000000");

    for (uint32_t i=0; i<32; ++i)
    {
        sput_fail_unless(bit_fls(1u<<i)==i, "General Test0");
        sput_fail_unless(bit_fls(0xFFFFFFFFu>>i)==31-i, "General test1");
    }
}

void test_ffs64()
{
    sput_fail_unless(bit_ffs(0ull)==-1, "Edge case: arg=0");
    sput_fail_unless(bit_ffs(1ull)==0,  "arg=1");
    sput_fail_unless(bit_ffs(0xFFFFFFFFFFFFFFFFull)==0,  "arg=0xFFFFFFFFFFFFFFFF");
    sput_fail_unless(bit_ffs(0x8000000000000000ull)==63,  "arg=0x8000000000000000");

    for (uint64_t i=0; i<64; ++i)
    {
        sput_fail_unless(bit_ffs(0xFFFFFFFFFFFFFFFFull<<i)==i, "General test0");
        sput_fail_unless(bit_ffs(1ull<<i)==i, "General test1");
        sput_fail_unless(bit_ffs(0xFFFFFFFFFFFFFFFFull>>i)==0, "General test2");
    }
}

void test_fls64()
{
    sput_fail_unless(bit_fls(0ull)==-1, "Edge case: arg=0");
    sput_fail_unless(bit_fls(1ull)==0,  "arg=1");
    sput_fail_unless(bit_fls(0xFFFFFFFFFFFFFFFFull)==63,  "arg=0xFFFFFFFF");
    sput_fail_unless(bit_ffs(0x8000000000000000ull)==63,  "arg=0x8000000000000000");

    for (uint64_t i=0; i<64; ++i)
    {
        sput_fail_unless(bit_fls(1ull<<i)==i, "General Test");
        sput_fail_unless(bit_fls(0xFFFFFFFFFFFFFFFFull>>i)==63-i, "General test1");
    }
}

int run_bit_tests()
{
    sput_start_testing();

    sput_enter_suite("CORE bits: test bit_ffs32");
    sput_run_test(test_ffs32);
    sput_enter_suite("CORE bits: test bit_fls32");
    sput_run_test(test_fls32);
    sput_enter_suite("CORE bits: test bit_ffs64");
    sput_run_test(test_ffs64);
    sput_enter_suite("CORE bits: test bit_fls64");
    sput_run_test(test_fls64);

    sput_finish_testing();

    return sput_get_return_value();
}