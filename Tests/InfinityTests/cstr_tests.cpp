#include <sput.h>

#include <core/core.h>
#include <etlsf.h>

#include <math.h>

static const char test_str1[] = "0";
static const char test_str2[] = "0x00";
static const char test_str3[] = "0xfF";
static const char test_str4[] = "07";
static const char test_str5[] = "0b101";
static const char test_str6[] = "789";
static const char test_str7[] = "0xabcdef45";
static const char test_str8[] = "-789";
static const char test_str9[] = "-10922789";

static const char test_str10[] = "-1000.0001";
static const char test_str11[] = "-12.";
static const char test_str12[] = "-.677";
static const char test_str13[] = "67.00e45";
static const char test_str14[] = ".4E45";
static const char test_str15[] = "+1.4e5";
static const char test_str16[] = "+.4E-5";
static const char test_str17[] = "  +.4E-5";

void test_cstr_todouble()
{
#define NUM_FLOAT_TESTS 17
    static const char* finput0 = "11.\0\11";
    static const char* finput1 = ".\0\11";
    static const char* finput2 = "11\0\11";
    static const char* finput3 = "1111";
    static const char* finput4 = "e1111";
    {
        struct test_data_t{
            const char* str;
            size_t      size;
            ptrdiff_t   exp_len;
            errno_t     exp_err;
            double      exp_val;
        } testData[NUM_FLOAT_TESTS] = {
            {  test_str1, sizeof(test_str1),  -1, EOK,    0          },
            {  test_str6, sizeof(test_str6),  -1, EOK,    789        },
            {  test_str8, sizeof(test_str8),  -1, EOK,    -789       },
            {  test_str9, sizeof(test_str9),  -1, EOK,    -10922789  },
            { test_str10, sizeof(test_str10), -1, EOK,    -1000.0001 },
            { test_str11, sizeof(test_str11), -1, EOK,    -12.       },
            { test_str12, sizeof(test_str12), -1, EOK,    -.677      },
            { test_str13, sizeof(test_str13), -1, EOK,    67.00e45   },
            { test_str14, sizeof(test_str14), -1, EOK,    .4e45      },
            { test_str15, sizeof(test_str15), -1, EOK,    +1.4e5     },
            { test_str16, sizeof(test_str16), -1, EOK,    +.4E-5     },
            { test_str17, sizeof(test_str17), -1, EOK,    +.4E-5     },
            {    finput0,    sizeof(finput0),  3, EOK,    11         },
            {    finput1,    sizeof(finput1),  0, EINVAL, 0.0        },
            {    finput2,    sizeof(finput2),  2, EOK,    11         },
            {    finput3,                  2,  2, EOK,    11         },
            {    finput4,                  2,  0, EINVAL, 0.0        },
        };

        for (int i = 0; i < NUM_FLOAT_TESTS; ++i)
        {
            double res;
            const char* end;
            errno_t err = cstr_todouble(testData[i].str, testData[i].size, &end, &res);
            sput_fail_unless(err == testData[i].exp_err, "Check error");
            sput_fail_unless(res == testData[i].exp_val, "Check return value");
            if (testData[i].exp_len == -1)
                sput_fail_unless(end - testData[i].str == testData[i].size - 1, "Check len");
            else
                sput_fail_unless(end - testData[i].str == testData[i].exp_len, "Check len");

        }
    }
}

void test_cstr_toimax()
{
#define NUM_IMAX_TESTS 9
    struct test_data_t{
        const char* str;
        size_t      size;
        int         base;
        uintmax_t   exp_err;
        uintmax_t   exp_len;
        uintmax_t   exp_val;
    } testData[NUM_IMAX_TESTS] = {
        { test_str1, sizeof(test_str1), 0, EOK, -1, 0 },
        { test_str2, sizeof(test_str2), 0, EOK, -1, 0 },
        { test_str3, sizeof(test_str3), 0, EOK, -1, 0xFF },
        { test_str4, sizeof(test_str4), 0, EOK, -1, 07 },
        { test_str5, sizeof(test_str5), 0, EOK, -1, 5 },
        { test_str6, sizeof(test_str6), 0, EOK, -1, 789 },
        { test_str7, sizeof(test_str7), 0, EOK, -1, 0xabcdef45 },
        { test_str8, sizeof(test_str8), 0, EOK, -1, -789 },
        { test_str9, sizeof(test_str9), 0, EOK, -1, -10922789 },
    };

    for (int i = 0; i < NUM_IMAX_TESTS; ++i)
    {
        intmax_t res;
        const char* end;
        errno_t err = cstr_toimax(testData[i].str, testData[i].size, testData[i].base, &end, &res);
        sput_fail_unless(err == testData[i].exp_err, "Check error");
        sput_fail_unless(res == testData[i].exp_val, "Check return value");
        if (testData[i].exp_len == -1)
            sput_fail_unless(end - testData[i].str == testData[i].size - 1, "Check len");
        else
            sput_fail_unless(end - testData[i].str == testData[i].exp_len, "Check len");
    }
}

void test_cstr_toumax()
{
#define NUM_UMAX_TESTS 7
        struct test_data_t{
            const char* str;
            size_t      size;
            int         base;
            uintmax_t   exp_err;
            uintmax_t   exp_len;
            uintmax_t   exp_val;
        } testData[NUM_UMAX_TESTS] = {
            { test_str1, sizeof(test_str1), 0, EOK, -1, 0 },
            { test_str2, sizeof(test_str2), 0, EOK, -1, 0 },
            { test_str3, sizeof(test_str3), 0, EOK, -1, 0xFF },
            { test_str4, sizeof(test_str4), 0, EOK, -1, 07 },
            { test_str5, sizeof(test_str5), 0, EOK, -1, 5 },
            { test_str6, sizeof(test_str6), 0, EOK, -1, 789 },
            { test_str7, sizeof(test_str7), 0, EOK, -1, 0xabcdef45 },
        };


        for (int i = 0; i < NUM_UMAX_TESTS; ++i)
        {
            uintmax_t res;
            const char* end;
            errno_t err = cstr_toumax(testData[i].str, testData[i].size, testData[i].base, &end, &res);
            sput_fail_unless(err == testData[i].exp_err, "Check error");
            sput_fail_unless(res == testData[i].exp_val, "Check return value");
            if (testData[i].exp_len == -1)
                sput_fail_unless(end - testData[i].str == testData[i].size - 1, "Check len");
            else
                sput_fail_unless(end - testData[i].str == testData[i].exp_len, "Check len");
        }
}

void test_cstr_scanf()
{
    {
        const char str[] = "1.0 0.0 0.3";
        const char fmt[] = "%f %f %f";
        float x, y, z;
        size_t len = sizeof(str);
        int parsed = cstr_scanf(str, len, fmt, &x, &y, &z);
        sput_fail_unless(parsed == 3, "Check parsed count");
        sput_fail_unless(x==1.0f, "Check return value");
        sput_fail_unless(y==0.0f, "Check return value");
        sput_fail_unless(z==0.3f, "Check return value");
    }

    {
        int res;
        const char str[] = "hello world!";
        const char fmt[] = "%s %s";
        char w1[100];
        char w2[100];
        int parsed = cstr_scanf(str, sizeof(str), fmt, w1, sizeof(w1), w2, sizeof(w2));
        sput_fail_unless(parsed == 2, "Check parsed count");
        sput_fail_unless((cstr_compare(w1, sizeof(w1), "hello", &res)==EOK, res==0), "Check return value");
        sput_fail_unless((cstr_compare(w2, sizeof(w2), "world!", &res)==EOK, res==0), "Check return value");
    }

    {
        const char str[] = "103";
        const char fmt[] = "%d";
        int x;
        size_t len = sizeof(str);
        int parsed = cstr_scanf(str, len, fmt, &x);
        sput_fail_unless(parsed == 1, "Check parsed count");
        sput_fail_unless(x==103, "Check return value");
    }
}

void test_cstr_tokenize()
{
#define TOKEN0 "sda"
#define TOKEN1 "fasd"
#define TOKEN2 "fasfas"
#define TOKEN3 "fadsf"

#define SEP0 ";"
#define SEP1 "."
#define SEP2 "^"
#define SEP3 ","

    {
        const char input[] = TOKEN0 SEP0 TOKEN1 SEP1 TOKEN2 SEP2 TOKEN3 SEP3;
        const char toklist[] = SEP0 SEP1 SEP2 SEP3;
        const char* str = input;
        rsize_t size = sizeof(input)-1;
        const char* token;
        rsize_t token_len;

        int res;
        errno_t err;
        err = cstr_tokenize(toklist, 4, &str, &size, &token, &token_len);
        sput_fail_unless(err == EOK, "Check error");
        sput_fail_unless((cstr_compare(token, token_len, TOKEN0, &res)==EOK, res==0), "Check return value");
        err = cstr_tokenize(toklist, 4, &str, &size, &token, &token_len);
        sput_fail_unless(err == EOK, "Check error");
        sput_fail_unless((cstr_compare(token, token_len, TOKEN1, &res)==EOK, res==0), "Check return value");
        err = cstr_tokenize(toklist, 4, &str, &size, &token, &token_len);
        sput_fail_unless(err == EOK, "Check error");
        sput_fail_unless((cstr_compare(token, token_len, TOKEN2, &res)==EOK, res==0), "Check return value");
        err = cstr_tokenize(toklist, 4, &str, &size, &token, &token_len);
        sput_fail_unless(err == EOK, "Check error");
        sput_fail_unless((cstr_compare(token, token_len, TOKEN3, &res)==EOK, res==0), "Check return value");
        err = cstr_tokenize(toklist, 4, &str, &size, &token, &token_len);
        sput_fail_unless(err == EOK, "Check error");
        sput_fail_unless((cstr_compare(token, token_len, "", &res)==EOK, res==0), "Check return value");
    }
}

void test_cstr_printf()
{
#include "cstr_printf_test_cases.inc"

    char str[1024];
    int res;

#define TEST(type)                                                          \
    do                                                                      \
    {                                                                       \
        for (size_t i = 0; i < type##_fmt_count; i++)                       \
        {                                                                   \
            for (size_t j = 0; j < type##_val_count; j++)                   \
            {                                                               \
                int r1 = cstr_printf(str, type##_fmt[i], type##_val[j]);    \
                sput_fail_unless(                                           \
                    (!str[0] && !string_results[i][j].r[0]) ||              \
                    ((cstr_compare(                                         \
                        type##_results[i][j].r,                             \
                        type##_results[i][j].l,                             \
                        str, &res                                           \
                    )==EOK) && (res==0)),                                   \
                    "Test case "#type                                       \
                );                                                          \
            }                                                               \
        }                                                                   \
    } while (0)

    TEST(float);
    TEST(long);
    TEST(ulong);
    TEST(llong);
    TEST(string);
    TEST(pointer);

    //for (size_t i = 0; i < string_fmt_count; i++)
    //{
    //    for (size_t j = 0; j < string_val_count; j++)
    //    {
    //        int r1 = cstr_printf(str, string_fmt[i], string_val[j]);
    //        bool test = (!str[0] && !string_results[i][j].r[0]) || ( (cstr_compare(
    //            string_results[i][j].r,
    //            string_results[i][j].l,
    //            str, &res
    //        ) == EOK) && (res == 0));
    //        sput_fail_unless(
    //            test,
    //            "Test case string"
    //        );
    //    }
    //}
}

int run_cstr_tests()
{
    sput_start_testing();

    sput_enter_suite("CSTR: test cstr_todouble");
    sput_run_test(test_cstr_todouble);
    sput_enter_suite("CSTR: test cstr_toimax");
    sput_run_test(test_cstr_toimax);
    sput_enter_suite("CSTR: test cstr_toumax");
    sput_run_test(test_cstr_toumax);
    sput_enter_suite("CSTR: test cstr_scanf");
    sput_run_test(test_cstr_scanf);
    sput_enter_suite("CSTR: test cstr_tokenize");
    sput_run_test(test_cstr_tokenize);
    sput_enter_suite("CSTR: test cstr_printf");
    sput_run_test(test_cstr_printf);

    sput_finish_testing();

    return sput_get_return_value();
}