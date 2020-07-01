#include "car.h"

#define _USE_MATH_DEFINES

CarBody::CarBody(const float center_x, const float center_y)
		: position(center_x, center_y),
			velocity(0.0f, 0.0f),
			size(CAR_WIDTH, CAR_HEIGHT),
			steering_angle(0.0f),
			max_steering_angle(MAX_STEERING_ANGLE) {}

void CarBody::update(const float dt) {
	float speed = velocity.abs();
	float speed_delta = speed * dt;
	float steer_delta = steering_angle * speed_delta;

	this->velocity.rotate_left(steer_delta);
	this->position += velocity;
}

Vec2d<float> CarBody::getPos() const {
	return position;
}

Vec2d<float> CarBody::getSize() const {
	return size;
}

float CarBody::getRotation() const {
	if (velocity.x == 0) {
		return std::atan(velocity.y / (velocity.x + 0.001f));
	}
	return std::atan(velocity.y / velocity.x);
}
