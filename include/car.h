#pragma once

#include "vec2d.h"

using geometry::Vec2d;

static const float CAR_HEIGHT = 2.0f;
static const float CAR_WIDTH = 1.0f;
static const float MAX_STEERING_ANGLE = 0.05f * (2.0f*M_PI);

class CarBody {
private:
	Vec2d<float> position;
	Vec2d<float> velocity;
	Vec2d<float> size;
	float steering_angle;  // [-max_steering_angle, max_steering_angle]
	const float max_steering_angle;
public:
	CarBody(const float center_x, const float center_y);
	void update(const float dt);
	Vec2d<float> getPos() const;
	Vec2d<float> getSize() const;
	float getRotation() const;
};
