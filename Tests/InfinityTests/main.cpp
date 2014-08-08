#include <stdlib.h>

int run_vg_tests();
int run_etlsf_tests();

int main(int argc, char **argv)
{
    int res = EXIT_SUCCESS;
    
    res = (run_vg_tests()==EXIT_FAILURE)    ? EXIT_FAILURE : res;
    res = (run_etlsf_tests()==EXIT_FAILURE) ? EXIT_FAILURE : res;

    return res;
}