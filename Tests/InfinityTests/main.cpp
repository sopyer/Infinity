#define SPUT_MAIN
#include <sput.h>

int run_vg_tests();
int run_etlsf_tests();
int run_math_tests();
int run_bit_tests();
int run_cstr_tests();

extern "C" int assert_handler(const char* cond, const char* file, int line) { return true; }

int main(int argc, char **argv)
{
    int res = EXIT_SUCCESS;

    res |= run_bit_tests();
    res |= run_etlsf_tests();
    res |= run_math_tests();
    res |= run_vg_tests();
    res |= run_cstr_tests();

    return res;
}