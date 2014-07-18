#include <sput.h>

#include "impl\Rasterizer.h"

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

namespace impl
{
    void bezier3MakeImplicit(glm::vec2 pos[4], glm::vec3 klm[4], int& subdPtCount, float* subdPts);
}

void test_orientation_selection_bug()
{
    glm::vec2 cp[4] = {
        glm::vec2(12.52016f, 0.19535054f),
        glm::vec2(13.76016f, 0.19535054f),
        glm::vec2(14.52016f, 0.23535054f),
        glm::vec2(14.52016f, 0.23535054f)
    };
    glm::vec3 tc[4];
    int count;
    float subdPt[2];

    impl::bezier3MakeImplicit(cp, tc, count, subdPt);
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

int main(int argc, char **argv) {
    sput_start_testing();

    sput_enter_suite("VG: cubic geometry generation tests");
    sput_run_test(test_orientation_selection_bug);

    sput_finish_testing();

    return sput_get_return_value();
}