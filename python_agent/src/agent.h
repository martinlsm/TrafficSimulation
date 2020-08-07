#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "game_world.h"

namespace py = pybind11;

namespace agent {

void load_traffic_environment(unsigned int id);
size_t destination_count();
size_t car_count();
void spawn_car(size_t dest_index);

py::array_t<double> add_arrays_1d(py::array_t<double>& input1, py::array_t<double>& input2);

py::array_t<float> read_state(size_t car_index);

} // namespace agent
