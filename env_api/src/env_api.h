#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "traffic_env.h"

namespace py = pybind11;

namespace env_api {

void load_traffic_environment(unsigned int id);
size_t destination_count();
size_t car_count();
size_t state_dim_size();
size_t action_dim_size();
unsigned long spawn_car(size_t start, size_t goal);
void remove_car(unsigned long car_id);
traffic::car_action index_to_action(size_t idx);
void update();
void do_action(unsigned long car_id, traffic::car_action action);
py::array_t<float> read_state(unsigned long car_id);
int get_reward_simple(unsigned long car_id);
int get_reward_advanced(unsigned long car_id);
bool in_terminal_state(unsigned long car_id);
std::tuple<float, float> get_car_position(unsigned long car_id);
std::tuple<float, float> get_car_size(unsigned long car_id);
float get_car_rotation(unsigned long car_id);
py::list get_car_ids();

} // namespace env_api
