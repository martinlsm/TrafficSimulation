#include "car.h"

#define _USE_MATH_DEFINES

Car::Car(const float center_x, const float center_y)
		: rect(center_x, center_y, CAR_HEIGHT, CAR_WIDTH),
			velocity(0.0f),
			steering_angle(0.0f),
			max_steering_angle(MAX_STEERING_ANGLE) {}

void Car::update(const float dt) {
	
}
