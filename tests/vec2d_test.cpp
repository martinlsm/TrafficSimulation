#include "gtest/gtest.h"

#include "vec2d.h"

using geometry::Vec2d;

TEST(TestVec2d, Constructor) {
	float a = 12.34f;
	float b = 56.789f;
	Vec2d<float> v{a, b};
	EXPECT_EQ(a, v.x);
	EXPECT_EQ(b, v.y);
}

TEST(TestVec2d, Abs) {
	int a = 3;
	int b = -4;
	Vec2d<int> v{a, b};
	EXPECT_EQ(5, v.abs());

	double ad = 0.0;
	double bd = 10.0;
	Vec2d<double> vd{ad, bd};
	EXPECT_EQ(10.0, vd.abs());
}

TEST(TestVec2d, Normalize) {
	float a = 12.34f;
	float b = -43.21f;
	Vec2d<float> v{a, b};
	v.normalize();
	EXPECT_FLOAT_EQ(1.0f, v.abs());
}

TEST(TestVec2d, RotateLeft) {
	float a = 1.0f;
	float b = 0.0f;
	Vec2d<float> v{a, b};

	v.rotate_left(M_PI / 2);
	ASSERT_NEAR(0.0f, v.x, 2.0e-7f);
	ASSERT_NEAR(1.0f, v.y, 2.0e-7f);
	EXPECT_FLOAT_EQ(1.0f, v.abs());

	v.rotate_left(M_PI / 2);
	ASSERT_NEAR(-1.0f, v.x, 2.0e-7f);
	ASSERT_NEAR(0.0f, v.y, 2.0e-7f);
	EXPECT_FLOAT_EQ(1.0f, v.abs());

	v.rotate_left(M_PI / 2);
	ASSERT_NEAR(0.0f, v.x, 2.0e-7f);
	ASSERT_NEAR(-1.0f, v.y, 2.0e-7f);
	EXPECT_FLOAT_EQ(1.0f, v.abs());

	v.rotate_left(M_PI / 2);
	ASSERT_NEAR(1.0f, v.x, 2.0e-7f);
	ASSERT_NEAR(0.0f, v.y, 2.0e-7f);
	EXPECT_FLOAT_EQ(1.0f, v.abs());

	float c = 1.0f;
	float d = 1.0f;
	v = Vec2d<float>{c, d};
	v.rotate_left(-3.0f / 8.0f * (2.0f * M_PI));
	ASSERT_NEAR(0.0f, v.x, 2.0e-7f);
	ASSERT_NEAR(-std::sqrt(2.0f), v.y, 2.0e-7f);
	EXPECT_FLOAT_EQ(std::sqrt(2.0f), v.abs());
}

TEST(Vec2dTest, IncrementOp) {
	float a = 123.45f;
	float b = 54.321f;
	float c = 99.88f;
	float d = 77.66f;
	Vec2d<float> v1{a, b};
	Vec2d<float> v2{c, d};
	v1 += v2;
	EXPECT_EQ(a + c, v1.x);
	EXPECT_EQ(b + d, v1.y);
}

TEST(Vec2dTest, AddOp) {
	float a = -12.34f;
	float b = 43.21f;
	float c = 44.0f;
	float d = 63.0f;
	Vec2d<float> v1{a, b};
	Vec2d<float> v2{c, d};
	Vec2d<float> v3 = v1 + v2;
	EXPECT_EQ(a + c, v3.x);
	EXPECT_EQ(b + d, v3.y);
}

TEST(Vec2dTest, MulOp) {
	float a = 10.0f;
	float b = -5.25f;
	float m = 2.0f;
	Vec2d<float> v{a, b};
	v = m * v;
	EXPECT_EQ(m * a, v.x);
	EXPECT_EQ(m * b, v.y);
}
