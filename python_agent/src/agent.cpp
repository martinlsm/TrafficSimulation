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

py::array_t<double> add_arrays_1d(py::array_t<double>& input1, py::array_t<double>& input2) {

	// Get info from input1, input2
	py::buffer_info buf1 = input1.request();
	py::buffer_info buf2 = input2.request();

	if (buf1.ndim !=1 || buf2.ndim !=1)
	{
		throw std::runtime_error("Number of dimensions must be one");
	}

	if (buf1.size !=buf2.size)
	{
		throw std::runtime_error("Input shape must match");
	}

	//Apply resources
	auto result = py::array_t<double>(buf1.size);
	py::buffer_info buf3 = result.request();

	//Obtain numpy.ndarray data pointer
	double* ptr1 = (double*)buf1.ptr;
	double* ptr2 = (double*)buf2.ptr;
	double* ptr3 = (double*)buf3.ptr;

	//Pointer visits numpy.ndarray
	for (int i = 0; i < buf1.shape[0]; i++)
	{
		ptr3[i] = ptr1[i] + ptr2[i];
	}

	return result;

}

} // namespace agent
