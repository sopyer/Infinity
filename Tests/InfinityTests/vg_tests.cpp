#include <sput.h>

#include <gfx/Path.h>
#include <glm/glm.h>
#include <core/core.h>

const float relaxedMaxDiff = 0.001f;
const float relaxedRelDiff = 0.0001f;
const float stricterMaxDif = 0.00001f;

bool equalRelAbs(float A, float B, float maxDiff = relaxedMaxDiff, float maxRelDiff = relaxedRelDiff)
{
    // Check if the numbers are really close -- needed
    // when comparing numbers near zero.
    float diff = fabs(A - B);
    if (diff <= maxDiff)
        return true;

    A = fabs(A);
    B = fabs(B);
    float largest = (B > A) ? B : A;

    if (diff <= largest * maxRelDiff)
        return true;
    return false;
}

float calcImplicit(const glm::vec3& tc)
{
    return tc.x*tc.x*tc.x - tc.y*tc.z;
}

namespace vg
{
    void bezier3MakeImplicit(v128 pos[4], v128 klm[4], int& subdPtCount, float* subdPts);
}

void test_orientation_selection_bug()
{
    v128 cp[4] = {
        {12.52016f, 0.19535054f, 0.0f, 0.0f},
        {13.76016f, 0.19535054f, 0.0f, 0.0f},
        {14.52016f, 0.23535054f, 0.0f, 0.0f},
        {14.52016f, 0.23535054f, 0.0f, 0.0f},
    };
    v128 klm[4];
    glm::vec3 tc[4];
    int count;
    float subdPt[2];

    vg::bezier3MakeImplicit(cp, klm, count, subdPt);

    vi_store_v3(&tc[0], klm[0]);
    vi_store_v3(&tc[1], klm[1]);
    vi_store_v3(&tc[2], klm[2]);
    vi_store_v3(&tc[3], klm[3]);

    sput_fail_unless(calcImplicit(glm::mix(tc[0], tc[3], 0.5f)) <= 0.0f, "Check midpoint explicit cubic function sign");

    float s0 = calcImplicit(tc[0]),
          s1 = calcImplicit(tc[1]),
          s2 = calcImplicit(tc[2]),
          s3 = calcImplicit(tc[3]);

    sput_fail_unless(s1>=stricterMaxDif || fabs(s1) < stricterMaxDif, "Check hull point 1 explicit cubic function sign");
    sput_fail_unless(s2>=stricterMaxDif || fabs(s2) < stricterMaxDif, "Check hull point 2 explicit cubic function sign");
    sput_fail_unless(fabs(s0) < stricterMaxDif, "Check hull point 0 explicit cubic function sign");
    sput_fail_unless(fabs(s3) < stricterMaxDif, "Check hull point 3 explicit cubic function sign");
}

int run_vg_tests()
{
    sput_start_testing();

    sput_enter_suite("VG: cubic geometry generation tests");
    sput_run_test(test_orientation_selection_bug);

    sput_finish_testing();

    return sput_get_return_value();
}
