#pragma once

#include "rectangle.h"

using geometry::Rectangle;

static const float CAR_HEIGHT = 2.0f;
static const float CAR_WIDTH = 1.0f;
static const float MAX_STEERING_ANGLE = 0.05f * (2.0f*M_PI);

class Car {
private:
	Rectangle rect;
	float velocity;
	float steering_angle;
	const float max_steering_angle;
public:
	Car(const float center_x, const float center_y);
	void update(const float dt);
};
