#include "gtest/gtest.h"

#include <cstdlib>

#include "road_system.h"

using geometry::Vec2d;
using traffic::FilledSquare;

static Vec2d<float> random_point() {
	float r_x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float r_y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	r_x = (r_x - 0.5f) * 200.0f;
	r_y = (r_y - 0.5f) * 200.0f;
	return Vec2d<float> {r_x, r_y};
}

static float random_angle() {
	float r_a = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	r_a *= 2 * M_PI;
	return r_a;
}

static float random_side_len() {
	float r_f = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	r_f *= 10;
	return r_f;
}

TEST(TestFilledSquarePiece, random_points) {
	for (int i = 0; i < 100; i++) {
		Vec2d<float> p = random_point();
		float s = random_side_len();
		float r = random_angle();
		FilledSquare sq {p, s, r};
		for (int j = 0; j < 1000; j++) {
			Vec2d<float> r_p = sq.randomPointInside();
			bool inside = sq.inside(r_p);
			ASSERT_TRUE(inside);
		}
	}
}
