#include "env_api.h"
#include "constants.h"
#include <cmath>

namespace env_api {

// should this be global?
static traffic::TrafficEnvironment* environment = nullptr;

void load_traffic_environment(unsigned int id) {
	if (environment != nullptr) {
		delete environment;
	}
	environment = traffic::load_environment(id);
}

size_t destination_count() {
	return environment->destinationCount();
}

size_t car_count() {
	return environment->carCount();
}

size_t state_dim_size() {
	return 8;
}

size_t action_dim_size() {
	return 13;
}

unsigned long spawn_car(size_t start, size_t goal) {
	unsigned long car_id = environment->spawnCar(start, goal);
	return car_id;
}

traffic::car_action index_to_action(size_t idx) {
	return (1L << idx);
}

void do_action(unsigned long car_id, traffic::car_action action) {
	environment->doAction(car_id, action);
}

void update() {
	float dt = 1.0f / (float)FRAME_RATE;
	environment->update(dt);
}

py::array_t<float> read_state(size_t car_id) {
	traffic::CarMechanics* car = environment->getCarMechanics(car_id);

	Vec2d<float> car_pos = car->getPos();
	float speed = car->getSpeed();

	float rotation = car->getRotation();
	float rot_x = std::cos(rotation);
	float rot_y = std::sin(rotation);

	float steering_angle = car->getSteeringAngle();

	Vec2d<float> destination_pos = environment->getCarDestination(car_id);
	Vec2d<float> vec_to_dest = destination_pos - car_pos;

	auto result = py::array_t<float>(state_dim_size());
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

int get_reward_simple(unsigned long car_id) {
	traffic::car_state state = environment->getCarState(car_id);
	if (state == traffic::OFF_ROAD) {
		return -1;
	} else if (state == traffic::REACHED_GOAL) {
		return 1;
	} else {
		return 0;
	}
}

const int max_dist = std::hypot(WORLD_WIDTH, WORLD_HEIGHT);
int get_reward_advanced(unsigned long car_id) {
	traffic::car_state state = environment->getCarState(car_id);
	if (state == traffic::REACHED_GOAL) {
		return 10000;
	} else if (state == traffic::OFF_ROAD) {
		return -10000;
	}
	traffic::CarMechanics* car = environment->getCarMechanics(car_id);
	Vec2d<float> car_pos = car->getPos();
	Vec2d<float> dest = environment->getCarDestination(car_id);
	Vec2d<float> to_dest = dest - car_pos;
	int score = max_dist - to_dest.abs();
	return score;
}

bool in_terminal_state(unsigned long car_id) {
	traffic::car_state state = environment->getCarState(car_id);
	return state == traffic::OFF_ROAD || state == traffic::NOT_FOUND
			|| state == traffic::REACHED_GOAL;
}

std::tuple<float, float> get_car_position(unsigned long car_id) {
	Vec2d<float> pos = environment->getCarMechanics(car_id)->getPos();
	return std::make_tuple(pos.x, pos.y);
}

std::tuple<float, float> get_car_size(unsigned long car_id) {
	Vec2d<float> size = environment->getCarMechanics(car_id)->getSize();
	return std::make_tuple(size.x, size.y);
}

float get_car_rotation(unsigned long car_id) {
	return environment->getCarMechanics(car_id)->getRotation();
}

} // namespace env_api
