#include <gtest/gtest.h>
#include "impl/Math.h"

TEST(CubicTCGenTest, OrientSelectionBug)
{
	Array<glm::vec2> cp;
	cp.push_back(glm::vec2(12.52016f, 0.19535054f));
	cp.push_back(glm::vec2(13.76016f, 0.19535054f));
	cp.push_back(glm::vec2(14.52016f, 0.23535054f));
	cp.push_back(glm::vec2(14.52016f, 0.23535054f));

	Array<glm::vec2> p;
	Array<glm::vec3> tc;

	cubicTriVertices(cp, p, tc);
	EXPECT_TRUE(cubic::calcImplicit(glm::mix(tc[0], tc[3], 0.5f))<=0);

	float	s0 = cubic::calcImplicit(tc[0]),
			s1 = cubic::calcImplicit(tc[1]),
			s2 = cubic::calcImplicit(tc[2]),
			s3 = cubic::calcImplicit(tc[3]);

	EXPECT_TRUE(ml::greaterThenE(s1, 0) || ml::equalE(s1, 0));
	EXPECT_TRUE(ml::greaterThenE(s2, 0) || ml::equalE(s2, 0));
	EXPECT_TRUE(ml::equalE(s0, 0));
	EXPECT_TRUE(ml::equalE(s3, 0));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}