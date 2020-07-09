#include <algorithm>

#include "car.h"

#define _USE_MATH_DEFINES

namespace traffic {

CarBody::CarBody(const float center_x, const float center_y,
		const float direction)
			: position(center_x, center_y),
			velocity(0.0f, direction),
			size(CAR_WIDTH, CAR_HEIGHT),
			steering_angle(0.0f),
			max_steering_angle(MAX_STEERING_ANGLE),
			max_speed(MAX_SPEED) {}

void CarBody::update(const float dt) {
	float speed = velocity.abs();
	float speed_delta = speed * dt;
	float steer_delta = steering_angle * speed_delta;

	this->velocity.rotate(steer_delta);
	this->position += velocity;
}

Vec2d<float> CarBody::getPos() const {
	return position;
}

Vec2d<float> CarBody::getSize() const {
	return size;
}

float CarBody::getRotation() const {
	return velocity.v;
}

float CarBody::changeSpeed(const float amount_faster) {
	float current_speed = velocity.abs();
	float new_speed = current_speed + amount_faster;
	new_speed = std::max(0.0f, std::min(max_speed, new_speed));
	velocity = PolarVec2d<float>(new_speed, velocity.v);
	return new_speed;
}

float CarBody::turn(const float amount_right) {
	float new_angle = steering_angle + amount_right;

	if (new_angle > max_steering_angle) {
		steering_angle = max_steering_angle;
	} else if (new_angle < -max_steering_angle) {
		steering_angle = -max_steering_angle;
	} else {
		steering_angle = new_angle;
	}
	return steering_angle;
}

} // traffic
