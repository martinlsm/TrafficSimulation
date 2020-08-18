#include "gtest/gtest.h"

#include "road_system.h"

using geometry::Vec2d;
using traffic::StraightRoad;
using traffic::CarMechanics;

TEST(TestStraightRoad, inside_1) {
	CarMechanics car {200, 200, 0.0f};
	Vec2d<float> a {100, 100};
	Vec2d<float> b {300, 300};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, inside_2) {
	CarMechanics car {50, 0, 0.0f};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {100, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, inside_3) {
	CarMechanics car {50, 38, 0.0f};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {100, 0};
	StraightRoad road {a, b, 75};
	EXPECT_FALSE(road.inside(car));
}

TEST(TestStraightRoad, inside_4) {
	CarMechanics car {50, 10, 0.0f};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {100, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, inside_5) {
	CarMechanics car {0, 50, 0.0f};
	Vec2d<float> a {0, 100};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, Inside_6) {
	CarMechanics car {0, 105, 0.0f};
	Vec2d<float> a {0, 100};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 75};
	EXPECT_FALSE(road.inside(car));
}

TEST(TestStraightRoad, inside_7) {
	CarMechanics car {50, 0, 0.0f};
	Vec2d<float> a {100, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, inside_8) {
	CarMechanics car {100 + 0.5f * 300, 100 + 0.5f * 400, 0.0f};
	Vec2d<float> a {100, 100};
	Vec2d<float> b {100 + 300, 100 + 400};
	StraightRoad road {a, b, 50};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, inside_9) {
	CarMechanics car {100 + 0.5f * 300, 100 + 0.5f * 400, 0.0f};
	Vec2d<float> a {100 + 300, 100 + 400};
	Vec2d<float> b {100, 100};
	StraightRoad road {a, b, 50};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, inside_10) {
	CarMechanics car {100 + 0.5f * 300, 500 - 0.5f * 400, 0.0f};
	Vec2d<float> a {100, 500};
	Vec2d<float> b {100 + 300, 500 - 400};
	StraightRoad road {a, b, 50};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, sensor_reading_1) {
	CarMechanics car {100, 0, 0.0f};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	EXPECT_EQ(road.sensor_reading(car, 0.0f), std::numeric_limits<float>::max());
}

TEST(TestStraightRoad, sensor_reading_2) {
	CarMechanics car {100, 0, 0.0f};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	EXPECT_EQ(road.sensor_reading(car, M_PI), std::numeric_limits<float>::max());
}

TEST(TestStraightRoad, sensor_reading_3) {
	CarMechanics car {100, 0, 0.0f};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(car, M_PI / 2), 25.0f, 1e-5);
}

TEST(TestStraightRoad, sensor_reading_4) {
	CarMechanics car {100, 0, 0.0f};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(car, -M_PI / 2), 25.0f, 1e-5);
}

TEST(TestStraightRoad, sensor_reading_5) {
	CarMechanics car {100, 0, 0.0f};
	Vec2d<float> a {200, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 50};
	EXPECT_EQ(road.sensor_reading(car, 0.0f), std::numeric_limits<float>::max());
}

TEST(TestStraightRoad, sensor_reading_6) {
	CarMechanics car {100, 0, M_PI};
	Vec2d<float> a {200, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 50};
	EXPECT_EQ(road.sensor_reading(car, 0.0f), std::numeric_limits<float>::max());
}

TEST(TestStraightRoad, sensor_reading_7) {
	CarMechanics car {100, 0, M_PI};
	Vec2d<float> a {200, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(car, M_PI / 2), 25.0f, 1e-5);
}

TEST(TestStraightRoad, sensor_reading_8) {
	CarMechanics car {100, 0, M_PI};
	Vec2d<float> a {200, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(car, -M_PI / 2), 25.0f, 1e-5);
}

TEST(TestStraightRoad, sensor_reading_9) {
	CarMechanics car {100, 0, M_PI / 4};
	float sensor_angle = 0;
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(car, sensor_angle), 25.0f / std::sin(M_PI / 4), 1e-5);
}

TEST(TestStraightRoad, sensor_reading_10) {
	CarMechanics car {100, 0, M_PI / 4};
	float sensor_angle = M_PI;
	Vec2d<float> a {0, 0};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(car, sensor_angle), 25.0f / std::sin(M_PI / 4), 1e-5);
}

TEST(TestStraightRoad, sensor_reading_11) {
	CarMechanics car {100, 100, M_PI / 4};
	float sensor_angle = M_PI;
	Vec2d<float> a {0, 200};
	Vec2d<float> b {200, 0};
	StraightRoad road {a, b, 50};
	ASSERT_NEAR(road.sensor_reading(car, sensor_angle), 25.0f, 1e-5);
}
