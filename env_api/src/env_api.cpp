#include "env_api.h"
#include "constants.h"
#include <cmath>

namespace env_api {

static const float MAX_DIST = std::hypot(WORLD_HEIGHT, WORLD_WIDTH);

Environment::Environment(unsigned int env_id) {
	traffic_env = traffic::load_environment(env_id);
}

Environment::~Environment() {
	delete traffic_env;
}

size_t Environment::destination_count() {
	return traffic_env->destinationCount();
}

size_t Environment::car_count() {
	return traffic_env->carCount();
}

// size_t state_dim_size() {
// 	return 8;
// }

size_t Environment::action_dim_size() {
	return 13;
}

unsigned long Environment::spawn_car(size_t start, size_t goal) {
	unsigned long car_id = traffic_env->spawnCar(start, goal);
	return car_id;
}

unsigned long Environment::spawn_car_at_random_pos(size_t goal) {
	unsigned long car_id = traffic_env->spawnCarAtRandomPos(goal);
	return car_id;
}

void Environment::remove_car(unsigned long car_id) {
	traffic_env->removeCar(car_id);
}

traffic::car_action Environment::index_to_action(size_t idx) {
	return (1L << idx);
}

void Environment::do_action(unsigned long car_id, traffic::car_action action) {
	traffic_env->doAction(car_id, action);
}

void Environment::update() {
	float dt = 1.0f / (float)FRAME_RATE;
	traffic_env->update(dt);
}

py::array_t<float> Environment::read_state_simple(size_t car_id) {
	traffic::CarMechanics* car = traffic_env->getCarMechanics(car_id);

	Vec2d<float> car_pos = car->getPos();
	float speed = car->getSpeed();

	float rotation = car->getRotation();
	float rot_x = std::cos(rotation);
	float rot_y = std::sin(rotation);

	float steering_angle = car->getSteeringAngle();

	Vec2d<float> destination_pos = traffic_env->getCarDestination(car_id);
	Vec2d<float> vec_to_dest = destination_pos - car_pos;

	auto result = py::array_t<float>(8);
	py::buffer_info res_buf = result.request();
	float* res_data = (float*)res_buf.ptr;

	res_data[0] = car_pos.x;
	res_data[1] = car_pos.y;
	res_data[2] = speed;
	res_data[3] = rot_x;
	res_data[4] = rot_y;
	res_data[5] = steering_angle;
	res_data[6] = vec_to_dest.x;
	res_data[7] = vec_to_dest.y;

	return result;
}

py::array_t<float> Environment::read_state_sensors(unsigned long car_id) {
	traffic::CarMechanics* car = traffic_env->getCarMechanics(car_id);

	Vec2d<float> car_pos = car->getPos();
	float speed = car->getSpeed();
	float rotation = car->getRotation();
	float steering_angle = car->getSteeringAngle();

	float speed_x = speed * std::cos(rotation);
	float speed_y = speed * std::sin(rotation);

	Vec2d<float> destination_pos = traffic_env->getCarDestination(car_id);
	Vec2d<float> vec_to_dest = destination_pos - car_pos;

	vector<float> sensors = traffic_env->getCarSensorReadings(car_id);

	// FIXME: Remove hard coded number.
	auto result = py::array_t<float>(15);
	py::buffer_info res_buf = result.request();
	float* res_data = (float*)res_buf.ptr;

	// output normalized vector
	res_data[0] = speed_x / MAX_DIST;
	res_data[1] = speed_y / MAX_DIST;
	res_data[2] = steering_angle;
	res_data[3] = vec_to_dest.x / MAX_DIST;
	res_data[4] = vec_to_dest.y / MAX_DIST;
	for (size_t i = 0; i < sensors.size(); i++) {
		res_data[i + 5] = sensors[i] / MAX_DIST;
	}

	return result;
}

int Environment::get_reward_simple(unsigned long car_id) {
	traffic::car_state state = traffic_env->getCarState(car_id);
	if (state == traffic::OFF_ROAD) {
		return -1;
	} else if (state == traffic::REACHED_GOAL) {
		return 1;
	} else {
		return 0;
	}
}

int Environment::get_reward_advanced(unsigned long car_id) {
	traffic::car_state state = traffic_env->getCarState(car_id);
	int score;
	if (state == traffic::ON_ROAD_STANDING_STILL) {
		score = -50;
	} else if (state == traffic::REACHED_GOAL) {
		score = MAX_DIST;
	} else if (state == traffic::OFF_ROAD) {
		traffic::CarMechanics* car = traffic_env->getCarMechanics(car_id);
		Vec2d<float> car_pos = car->getPos();
		Vec2d<float> dest = traffic_env->getCarDestination(car_id);
		Vec2d<float> to_dest = dest - car_pos;
		score = -to_dest.abs();
	} else {
		score = 0;
	}
	return score;
}

bool Environment::in_terminal_state(unsigned long car_id) {
	traffic::car_state state = traffic_env->getCarState(car_id);
	return state == traffic::OFF_ROAD || state == traffic::NOT_FOUND
			|| state == traffic::REACHED_GOAL;
}

std::tuple<float, float> Environment::get_car_position(unsigned long car_id) {
	Vec2d<float> pos = traffic_env->getCarMechanics(car_id)->getPos();
	return std::make_tuple(pos.x, pos.y);
}

std::tuple<float, float>
Environment::get_car_destination(unsigned long car_id) {
	Vec2d<float> dest = traffic_env->getCarDestination(car_id);
	return std::make_tuple(dest.x, dest.y);
}

std::tuple<float, float> Environment::get_car_size(unsigned long car_id) {
	Vec2d<float> size = traffic_env->getCarMechanics(car_id)->getSize();
	return std::make_tuple(size.x, size.y);
}

float Environment::get_car_rotation_degrees(unsigned long car_id) {
	float rotation_rad = traffic_env->getCarMechanics(car_id)->getRotation();
	return 180.0f / M_PI * rotation_rad;
}

py::list Environment::get_car_ids() {
	py::list l;
	std::vector<unsigned long> v = traffic_env->get_car_ids();
	for (unsigned long car_id : v) {
		l.append(car_id);
	}
	return l;
}

int Environment::get_goal_margin() {
	return GOAL_MARGIN;
}

} // namespace env_api

