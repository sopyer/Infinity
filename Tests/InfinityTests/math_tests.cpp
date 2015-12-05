#include <sput.h>

#include <core/core.h>

void test_vector_math()
{
    {
        v128 r = vi_set(0.0f, 1.0f, 2.0f, 3.0f);
        float expected_vi_set[4] = {0.0f, 1.0f, 2.0f, 3.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_set, 4*sizeof(float))==0,
            "check vi_set"
        );
    }

    {
        v128 r = vi_set_x(10.0f);
        float expected_vi_set_x[4] = {10.0f, 0.0f, 0.0f, 0.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_set_x, 4*sizeof(float))==0,
            "check vi_set_x"
        );
    }

    {
        v128 r = vi_set_all(10.0f);
        float expected_vi_set_all[4] = {10.0f, 10.0f, 10.0f, 10.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_set_all, 4*sizeof(float))==0,
            "check vi_set_all"
        );
    }


    {
        v128 r = vi_seti_x(0xfeedf00d);
        int32_t expected_vi_seti_x[4] = {0xfeedf00d, 0, 0, 0};
        sput_fail_unless(
            memcmp(&r, expected_vi_seti_x, 4*sizeof(float))==0,
            "check vi_seti_x"
        );
    }

    {
        v128 r = vi_seti_all(0xfeedf00d);
        int32_t expected_vi_seti_all[4] = {
            0xfeedf00d, 0xfeedf00d, 0xfeedf00d, 0xfeedf00d
        };
        sput_fail_unless(
            memcmp(&r, expected_vi_seti_all, 4*sizeof(float))==0,
            "check vi_seti_all"
        );
    }

    {
        v128 r = vi_set_zero();
        float expected_vi_set_zero[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_set_zero, 4*sizeof(float))==0,
            "check vi_set_zero"
        );
    }

    {
        v128 a = vi_set(1.0f, 2.0f, 3.0f, 4.0f);
        v128 b = vi_set(3.0f, 1.0f, 4.0f, 2.0f);
        sput_fail_unless(
            vi_dot4(a, b) == 25.0f,
            "check vi_dot4"
        );
    }

    {
        v128 a = vi_set(1.0f, 2.0f, 3.0f, 4.0f);
        v128 b = vi_set(3.0f, 1.0f, 4.0f, 2.0f);
        sput_fail_unless(
            vi_dot3(a, b) == 17.0f,
            "check vi_dot3"
        );
    }

    {
        v128 a = vi_set(1.0f, 2.0f, 3.0f, 4.0f);
        v128 b = vi_set(3.0f, 1.0f, 4.0f, 2.0f);
        sput_fail_unless(
            vi_dot2(a, b) == 5.0f,
            "check vi_dot2"
        );
    }

    {
        v128 a = vi_set(0.0f, 1.0f, 0.0f, 0.0f);
        v128 b = vi_set(0.0f, 0.0f, 1.0f, 0.0f);
        v128 r = vi_cross3(a, b);
        float expected_vi_cross3[4] = {1.0f, 0.0f, 0.0f, 0.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_cross3, 4*sizeof(float))==0,
            "check vi_cross3"
        );
    }


    {
        v128 a = vi_set(0.0f, 1.0f, 0.0f, 1.0f);
        v128 b = vi_set(5.0f, 5.0f, 5.0f, 5.0f);
        v128 c = vi_set(1.0f, 1.0f, 0.0f, 0.0f);
        v128 r = vi_mad(a, b, c);
        float expected_vi_mad[4] = {1.0f, 6.0f, 0.0f, 5.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_mad, 4*sizeof(float))==0,
            "check vi_mad"
        );
    }

    {
        v128 a = vi_set(0.0f, 1.0f, 0.0f, 1.0f);
        v128 b = vi_set(5.0f, 5.0f, 5.0f, 5.0f);
        v128 r = vi_mul(a, b);
        float expected_vi_mul[4] = {0.0f, 5.0f, 0.0f, 5.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_mul, 4*sizeof(float))==0,
            "check vi_mul"
        );
    }

    {
        v128 a = vi_set(10.0f, 5.0f, 0.0f, 1.0f);
        v128 b = vi_set(5.0f, 5.0f, 5.0f, 2.0f);
        v128 r = vi_div(a, b);
        float expected_vi_div[4] = {2.0f, 1.0f, 0.0f, 0.5f};
        sput_fail_unless(
            memcmp(&r, expected_vi_div, 4*sizeof(float))==0,
            "check vi_div"
        );
    }

    {
        v128 a = vi_set(10.0f, 5.0f, 0.0f, 1.0f);
        v128 b = vi_set(5.0f, 5.0f, 5.0f, 2.0f);
        v128 r = vi_add(a, b);
        float expected_vi_add[4] = {15.0f, 10.0f, 5.0f, 3.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_add, 4*sizeof(float))==0,
            "check vi_add"
        );
    }

    {
        v128 a = vi_set(10.0f, 5.0f, 0.0f, 1.0f);
        v128 b = vi_set(5.0f, 5.0f, 5.0f, 2.0f);
        v128 r = vi_sub(a, b);
        float expected_vi_sub[4] = {5.0f, 0.0f, -5.0f, -1.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_sub, 4*sizeof(float))==0,
            "check vi_sub"
        );
    }

    {
        v128 a = vi_set(16.0f, 4.0f, 0.25f, 0.0625f);
        v128 r = vi_sqrt(a);
        float expected_vi_sqrt[4] = {4.0f, 2.0f, 0.5f, 0.25f};
        sput_fail_unless(
            memcmp(&r, expected_vi_sqrt, 4*sizeof(float))==0,
            "check vi_sqrt"
        );
    }

    {
        v128 a = vi_set(2.0f, 4.0f, -0.5f, -0.25f);
        v128 r = vi_neg(a);
        float expected_vi_neg[4] = {-2.0f, -4.0f, 0.5f, 0.25f};
        sput_fail_unless(
            memcmp(&r, expected_vi_neg, 4*sizeof(float))==0,
            "check vi_neg"
        );
    }

    {
        v128 a = vi_set(2.0f, 0.0f, -0.5f, -0.25f);
        v128 r = vi_abs(a);
        float expected_vi_abs[4] = {2.0f, 0.0f, 0.5f, 0.25f};
        sput_fail_unless(
            memcmp(&r, expected_vi_abs, 4*sizeof(float))==0,
            "check vi_abs"
        );
    }

    {
        v128 a = vi_set(2.0f, 0.0f, -0.5f, -0.0f);
        v128 r = vi_sign(a);
        float expected_vi_sign[4] = {1.0f, 0.0f, -1.0f, -0.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_sign, 4*sizeof(float))==0,
            "check vi_sign"
        );
    }

    v128  vi_max   (v128 a, v128 b);
    {
        v128 a = vi_set(10.0f, 5.0f, 0.0f, 1.0f);
        v128 b = vi_set(5.0f, 5.0f, 5.0f, 2.0f);
        v128 r = vi_max(a, b);
        float expected_vi_max[4] = {10.0f, 5.0f, 5.0f, 2.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_max, 4*sizeof(float))==0,
            "check vi_max"
        );
    }

    v128  vi_min   (v128 a, v128 b);
    {
        v128 a = vi_set(10.0f, 5.0f, 0.0f, 1.0f);
        v128 b = vi_set(5.0f, 5.0f, 5.0f, 2.0f);
        v128 r = vi_min(a, b);
        float expected_vi_min[4] = {5.0f, 5.0f, 0.0f, 1.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_min, 4*sizeof(float))==0,
            "check vi_min"
        );
    }

    {
        v128 a = vi_set(10.0f, 5.0f, 0.0f, 1.0f);
        v128 b = vi_set(5.0f, 5.0f, 5.0f, 2.0f);
        v128 r = vi_lerp(a, b, 0.5f);
        float expected_vi_lerp[4] = {7.5f, 5.0f, 2.5f, 1.5f};
        sput_fail_unless(
            memcmp(&r, expected_vi_lerp, 4*sizeof(float))==0,
            "check vi_lerp"
        );
    }

    {
        v128 a = vi_set(10.0f, 5.0f, 9.0f, 1.0f);
        v128 b = vi_set(5.0f, 6.0f, 7.0f, 8.0f);
        v128 c = vi_set(15.0f, 7.0f, 7.0f, 10.0f);
        v128 r = vi_clamp(a, b, c);
        float expected_vi_clamp[4] = {10.0f, 6.0f, 7.0f, 8.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_clamp, 4*sizeof(float))==0,
            "check vi_clamp"
        );
    }

    {
        v128 a = vi_set(10.0f, 5.0f, 0.5f, -1.0f);
        v128 r = vi_sat(a);
        float expected_vi_sat[4] = {1.0f, 1.0f, 0.5f, 0.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_sat, 4*sizeof(float))==0,
            "check vi_sat"
        );
    }

    {
        v128 test_fail = {-1.0f, -1.0f, 1.0f, -1.0f};
        v128 test_succeed = {-1.0f, -1.0f, -1.0f, -1.0f};
        sput_fail_if(
            vi_all(test_fail),
            "check vi_all false"
        );
        sput_fail_unless(
            vi_all(test_succeed),
            "check vi_all true"
        );
    }

    {
        v128 test_fail = {1.0f, 1.0f, 1.0f, 1.0f};
        v128 test_succeed = {-1.0f, -1.0f, 1.0f, 1.0f};
        sput_fail_if(
            vi_any(test_fail),
            "check vi_all false"
        );
        sput_fail_unless(
            vi_any(test_succeed),
            "check vi_all true"
        );
    }


    {
        v128 r = vi_cvt_u8x4_to_v128(0xFF00FF00);
        float expected_vi_cvt_u8x4_to_v128[4] = {0.0f, 1.0f, 0.0f, 1.0f};
        sput_fail_unless(
            memcmp(&r, expected_vi_cvt_u8x4_to_v128, 4*sizeof(float))==0,
            "check vi_cvt_u8x4_to_v128"
        );
    }
}

int run_math_tests()
{
    sput_start_testing();

    sput_enter_suite("Math: test vector code");
    sput_run_test(test_vector_math);

    sput_finish_testing();

    return sput_get_return_value();
}
