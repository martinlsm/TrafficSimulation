#include "agent.h"
#include <cmath>

namespace agent {

// should this be global?
static traffic::TrafficEnvironment environment = traffic::load_environment(1);

void load_traffic_environment(unsigned int id) {
	environment = traffic::load_environment(id);
}

size_t destination_count() {
	return environment.destinations.size();
}

size_t car_count() {
	return environment.active_cars.size();
}

void spawn_car(size_t dest_index) {
	traffic::CarBody car = environment.destinations[dest_index].placeCar();
	environment.active_cars.push_back(car);
}

py::array_t<float> read_state(size_t car_index) {
	traffic::CarBody& car = environment.active_cars[car_index];

	Vec2d<float> pos = car.getPos();
	float speed = car.getSpeed();

	float rotation = car.getRotation();
	float rot_x = std::cos(rotation);
	float rot_y = std::sin(rotation);

	float steering_angle = car.getSteeringAngle();

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

} // namespace agent
