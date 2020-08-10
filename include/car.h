#pragma once

#include "vec2d.h"
#include "polar_vec2d.h"

using geometry::Vec2d;
using geometry::PolarVec2d;

namespace traffic {

static const float CAR_HEIGHT = 32.0f;
static const float CAR_WIDTH = 16.0f;
static const float MAX_STEERING_ANGLE = 0.08f * (2.0f*M_PI);
static const float MAX_SPEED = 30.0f;

class CarMechanics {
private:
	Vec2d<float> position;
	PolarVec2d<float> velocity;
	Vec2d<float> size;
	float steering_angle;  // [-max_steering_angle, max_steering_angle]

	const float max_steering_angle;
	const float max_speed;
public:
	CarMechanics(const float center_x, const float center_y, const float direction);
	void update(const float dt);

	Vec2d<float> getPos() const;
	Vec2d<float> getSize() const;
	float getSpeed() const;
	float getRotation() const;
	float getSteeringAngle() const;

	float changeSpeed(const float amount_faster);
	float turn(const float amount_right);
};

} // traffic
