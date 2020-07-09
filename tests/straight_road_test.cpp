#include "gtest/gtest.h"

#include "roadway.h"

using geometry::Vec2d;
using traffic::StraightRoad;
using traffic::CarBody;

TEST(TestStraightRoad, Inside_1) {
	CarBody car {200, 200, 0.0f};
	Vec2d<float> a {100, 100};
	Vec2d<float> b {300, 300};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, Inside_2) {
	CarBody car {50, 0, 0.0f};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {100, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, Inside_3) {
	CarBody car {50, 38, 0.0f};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {100, 0};
	StraightRoad road {a, b, 75};
	EXPECT_FALSE(road.inside(car));
}

TEST(TestStraightRoad, Inside_4) {
	CarBody car {50, 10, 0.0f};
	Vec2d<float> a {0, 0};
	Vec2d<float> b {100, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, Inside_5) {
	CarBody car {0, 50, 0.0f};
	Vec2d<float> a {0, 100};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, Inside_6) {
	CarBody car {0, 105, 0.0f};
	Vec2d<float> a {0, 100};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 75};
	EXPECT_FALSE(road.inside(car));
}

TEST(TestStraightRoad, Inside_7) {
	CarBody car {50, 0, 0.0f};
	Vec2d<float> a {100, 0};
	Vec2d<float> b {0, 0};
	StraightRoad road {a, b, 75};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, Inside_8) {
	CarBody car {100 + 0.5f * 300, 100 + 0.5f * 400, 0.0f};
	Vec2d<float> a {100, 100};
	Vec2d<float> b {100 + 300, 100 + 400};
	StraightRoad road {a, b, 50};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, Inside_9) {
	CarBody car {100 + 0.5f * 300, 100 + 0.5f * 400, 0.0f};
	Vec2d<float> a {100 + 300, 100 + 400};
	Vec2d<float> b {100, 100};
	StraightRoad road {a, b, 50};
	EXPECT_TRUE(road.inside(car));
}

TEST(TestStraightRoad, Inside_10) {
	CarBody car {100 + 0.5f * 300, 500 - 0.5f * 400, 0.0f};
	Vec2d<float> a {100, 500};
	Vec2d<float> b {100 + 300, 500 - 400};
	StraightRoad road {a, b, 50};
	EXPECT_TRUE(road.inside(car));
}
