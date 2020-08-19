#include "gtest/gtest.h"

#include "road_system.h"

using geometry::Vec2d;
using traffic::StraightRoad;
using traffic::CarMechanics;

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
	EXPECT_EQ(road.sensor_reading(pos, sensor_angle), std::numeric_limits<float>::max());
}

TEST(TestStraightRoad, sensor_reading_2) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = M_PI;
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	EXPECT_EQ(road.sensor_reading(pos, sensor_angle), std::numeric_limits<float>::max());
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
	EXPECT_EQ(road.sensor_reading(pos, sensor_angle), std::numeric_limits<float>::max());
}

TEST(TestStraightRoad, sensor_reading_6) {
	Vec2d<float> pos {100, 0};
	float sensor_angle = M_PI;
	Vec2d<float> a {200, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 50};
	EXPECT_EQ(road.sensor_reading(pos, sensor_angle), std::numeric_limits<float>::max());
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
