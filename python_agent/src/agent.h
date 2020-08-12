#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "traffic_env.h"

namespace py = pybind11;

namespace agent {

void load_traffic_environment(unsigned int id);
size_t destination_count();
size_t car_count();
unsigned long spawn_car(size_t start, size_t goal);
traffic::car_action index_to_action(size_t idx);
void update();
void do_action(unsigned long car_id, traffic::car_action action);
py::array_t<float> read_state(unsigned long car_id);
int get_reward(unsigned long car_id);
bool in_terminal_state(unsigned long car_id);

} // namespace agent
