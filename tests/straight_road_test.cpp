#include "gtest/gtest.h"

#include "road_system.h"

using geometry::Vec2d;
using traffic::StraightRoad;
using traffic::CarMechanics;

static Vec2d<float> random_point() {
	float r_x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float r_y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	r_x = (r_x - 0.5f) * 200.0f;
	r_y = (r_y - 0.5f) * 200.0f;
	return Vec2d<float> {r_x, r_y};
}

static float random_width() {
	float r_f = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	r_f *= 20.0f;
	return r_f;
}

TEST(TestStraightRoad, inside_1) {
	Vec2d<float> pos {200, 200};
	Vec2d<float> a {100, 100};
	Vec2d<float> b {300, 300};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(pos));
}

TEST(TestStraightRoad, inside_2) {
	Vec2d<float> pos {50, 0};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {100, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(pos));
}

TEST(TestStraightRoad, inside_3) {
	Vec2d<float> pos {50, 38};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {100, 0};
	StraightRoad road {a, b, 75};
	EXPECT_FALSE(road.inside(pos));
}

TEST(TestStraightRoad, inside_4) {
	Vec2d<float> pos {50, 10};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {100, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(pos));
}

TEST(TestStraightRoad, inside_5) {
	Vec2d<float> pos {0, 50};
	Vec2d<float> a {0, 100};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(pos));
}

TEST(TestStraightRoad, Inside_6) {
	Vec2d<float> pos {0, 105};
	Vec2d<float> a {0, 100};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 75};
	EXPECT_FALSE(road.inside(pos));
}

TEST(TestStraightRoad, inside_7) {
	Vec2d<float> pos {50, 0};
	Vec2d<float> a {100, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(pos));
}

TEST(TestStraightRoad, inside_8) {
	Vec2d<float> pos {100 + 0.5f * 300, 100 + 0.5f * 400};
	Vec2d<float> a {100, 100};
	Vec2d<float> b {100 + 300, 100 + 400};
	StraightRoad road {a, b, 50};
	EXPECT_TRUE(road.inside(pos));
}

TEST(TestStraightRoad, inside_9) {
	Vec2d<float> pos {100 + 0.5f * 300, 100 + 0.5f * 400};
	Vec2d<float> a {100 + 300, 100 + 400};
	Vec2d<float> b {100, 100};
	StraightRoad road {a, b, 50};
	EXPECT_TRUE(road.inside(pos));
}

TEST(TestStraightRoad, inside_10) {
	Vec2d<float> pos {100 + 0.5f * 300, 500 - 0.5f * 400};
	Vec2d<float> a {100, 500};
	Vec2d<float> b {100 + 300, 500 - 400};
	StraightRoad road {a, b, 50};
	EXPECT_TRUE(road.inside(pos));
}

TEST(TestStraightRoad, sensor_reading_1) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = 0.0f;
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(pos, sensor_angle), 100.0f, 1e-2);
}

TEST(TestStraightRoad, sensor_reading_2) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = M_PI;
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(pos, sensor_angle), 100.0f, 1e-2);
}

TEST(TestStraightRoad, sensor_reading_3) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = M_PI / 2;
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(pos, sensor_angle), 25.0f, 1e-5);
}

TEST(TestStraightRoad, sensor_reading_4) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = -M_PI / 2;
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(pos, sensor_angle), 25.0f, 1e-5);
}

TEST(TestStraightRoad, sensor_reading_5) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = 0.0f;
	Vec2d<float> a {200, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(pos, sensor_angle), 100.0f, 1e-5);
}

TEST(TestStraightRoad, sensor_reading_6) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = M_PI;
	Vec2d<float> a {200, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(pos, sensor_angle), 100.0f, 1e-5);
}

TEST(TestStraightRoad, sensor_reading_7) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = 3 * M_PI / 2;
	Vec2d<float> a {200, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(pos, sensor_angle), 25.0f, 1e-5);
}

TEST(TestStraightRoad, sensor_reading_8) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = M_PI / 2;
	Vec2d<float> a {200, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(pos, sensor_angle), 25.0f, 1e-5);
}

TEST(TestStraightRoad, sensor_reading_9) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = M_PI / 4;
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(pos, sensor_angle), 25.0f / std::sin(M_PI / 4), 1e-5);
}

TEST(TestStraightRoad, sensor_reading_10) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = 5 * M_PI / 4;
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(pos, sensor_angle), 25.0f / std::sin(M_PI / 4), 1e-5);
}

TEST(TestStraightRoad, sensor_reading_11) {
	Vec2d<float> pos {100, 100};
	float sensor_angle = 5 * M_PI / 4;
	Vec2d<float> a {0, 200};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(pos, sensor_angle), 25.0f, 1e-5);
}

TEST(TestStraightRoad, random_points) {
	for (int i = 0; i < 100; i++) {
		Vec2d<float> a = random_point();
		Vec2d<float> b = random_point();
		float w = random_width();
		StraightRoad road {a, b, w};
		for (int j = 0; j < 1000; j++) {
			Vec2d<float> r_p = road.randomPointInside();
			bool inside = road.inside(r_p);
			ASSERT_TRUE(inside);
		}
	}
}
