#include "car.h"

#define _USE_MATH_DEFINES

CarBody::CarBody(const float center_x, const float center_y)
		: position(center_x, center_y),
			velocity(0.0f, 0.001f),
			size(CAR_WIDTH, CAR_HEIGHT),
			steering_angle(0.0f),
			max_steering_angle(MAX_STEERING_ANGLE),
			max_speed(MAX_SPEED) {}

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

float CarBody::changeSpeed(const float amount_faster) {
	float current_speed = velocity.abs();
	float new_speed = current_speed + amount_faster;

	if (new_speed > max_speed) {
		float scaling = max_speed / current_speed;
		velocity = scaling * velocity;
	} else if (new_speed < 0) {
		velocity = Vec2d<float>(0, 0);
	} else {
		float scaling = new_speed / current_speed;
		velocity = scaling * velocity;
	}
}

float CarBody::turn(const float amount_left) {
	float new_angle = steering_angle + new_angle;

	if (new_angle > max_steering_angle) {
		steering_angle = max_steering_angle;
	} else if (new_angle < -max_steering_angle) {
		steering_angle = -max_steering_angle;
	} else {
		steering_angle = new_angle;
	}
}
