#include "agent.h"

namespace agent {

// TODO: replace this
static traffic::CarBody temp_car (42, 666, 0);

py::array_t<float> read_state() {
	Vec2d<float> pos = temp_car.getPos();
	float rotation = temp_car.getRotation();
	auto result = py::array_t<float>(3);
	py::buffer_info res_buf = result.request();
	float* res_data = (float*)res_buf.ptr;

	res_data[0] = pos.x;
	res_data[1] = pos.y;
	res_data[2] = rotation;
	
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
