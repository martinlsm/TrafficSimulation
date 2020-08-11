#include "agent.h"
#include <cmath>

namespace agent {

// should this be global?
static traffic::TrafficEnvironment* environment = nullptr;

void load_traffic_environment(unsigned int id) {
	if (environment != nullptr) {
		delete environment;
	}
	environment = traffic::load_environment(id);
}

size_t destination_count() {
	return environment->destination_count();
}

size_t car_count() {
	return environment->car_count();
}

unsigned long spawn_car(size_t start, size_t goal) {
	unsigned long car_id = environment->spawnCar(start, goal);
	return car_id;
}

traffic::car_action index_to_action(size_t idx) {
	return (1L << idx);
}

void do_action(unsigned long car_index, traffic::car_action action) {
	environment->doAction(car_index, action);
}

py::array_t<float> read_state(size_t car_index) {
	traffic::CarMechanics* car = environment->getCarMechanics(car_index);

	Vec2d<float> pos = car->getPos();
	float speed = car->getSpeed();

	float rotation = car->getRotation();
	float rot_x = std::cos(rotation);
	float rot_y = std::sin(rotation);

	float steering_angle = car->getSteeringAngle();

	auto result = py::array_t<float>(6);
	py::buffer_info res_buf = result.request();
	float* res_data = (float*)res_buf.ptr;

	res_data[0] = pos.x;
	res_data[1] = pos.y;
	res_data[2] = speed;
	res_data[3] = rot_x;
	res_data[4] = rot_y;
	res_data[5] = steering_angle;
	
	return result;
}

int get_reward(size_t car_index) {
	traffic::car_state state = environment->getCarState(car_index);
	switch (state) {
		case traffic::OFF_ROAD:
			return -1;
		case traffic::REACHED_GOAL:
			return 1;
	}
	return 0;
}

} // namespace agent
