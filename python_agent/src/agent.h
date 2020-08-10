#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "game_world.h"

namespace py = pybind11;

namespace agent {

void load_traffic_environment(unsigned int id);
size_t destination_count();
size_t car_count();
unsigned long spawn_car(size_t start, size_t goal);

py::array_t<float> read_state(size_t car_index);

} // namespace agent
