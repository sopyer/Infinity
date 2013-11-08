#include <sput.h>
#include "impl/Math.h"

void test_orientation_selection_bug()
{
	Array<glm::vec2> cp;
	cp.push_back(glm::vec2(12.52016f, 0.19535054f));
	cp.push_back(glm::vec2(13.76016f, 0.19535054f));
	cp.push_back(glm::vec2(14.52016f, 0.23535054f));
	cp.push_back(glm::vec2(14.52016f, 0.23535054f));

	Array<glm::vec2> p;
	Array<glm::vec3> tc;

	cubicTriVertices(cp, p, tc);
	sput_fail_unless(cubic::calcImplicit(glm::mix(tc[0], tc[3], 0.5f))<=0, "Check midpoint explicit cubic function sign");

	float	s0 = cubic::calcImplicit(tc[0]),
			s1 = cubic::calcImplicit(tc[1]),
			s2 = cubic::calcImplicit(tc[2]),
			s3 = cubic::calcImplicit(tc[3]);

	sput_fail_unless(ml::greaterThenE(s1, 0) || ml::equalE(s1, 0), "Check hull point 1 explicit cubic function sign");
	sput_fail_unless(ml::greaterThenE(s2, 0) || ml::equalE(s2, 0), "Check hull point 2 explicit cubic function sign");
	sput_fail_unless(ml::equalE(s0, 0), "Check hull point 0 explicit cubic function sign");
	sput_fail_unless(ml::equalE(s3, 0), "Check hull point 3 explicit cubic function sign");
}

int main(int argc, char **argv) {
    sput_start_testing();

    sput_enter_suite("VG: cubic geometry generation tests");
    sput_run_test(test_orientation_selection_bug);

    sput_finish_testing();

    return sput_get_return_value();
}