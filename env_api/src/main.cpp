#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "env_api.h"

namespace py = pybind11;

PYBIND11_MODULE(env_api, m) {
    m.def("load_traffic_environment", &env_api::load_traffic_environment);
    m.def("destination_count", &env_api::destination_count);
    m.def("car_count", &env_api::car_count);
	// m.def("state_dim_size", &env_api::state_dim_size);
	m.def("action_dim_size", &env_api::action_dim_size);
    m.def("spawn_car", &env_api::spawn_car);
	m.def("remove_car", &env_api::remove_car);
	m.def("index_to_action", &env_api::index_to_action);
	m.def("do_action", &env_api::do_action);
	m.def("update", &env_api::update);
    m.def("read_state_simple", &env_api::read_state_simple);
    m.def("read_state_sensors", &env_api::read_state_sensors);
	m.def("get_reward_simple", &env_api::get_reward_simple);
	m.def("get_reward_advanced", &env_api::get_reward_advanced);
	m.def("in_terminal_state", &env_api::in_terminal_state);
	m.def("get_car_position", &env_api::get_car_position);
	m.def("get_car_size", &env_api::get_car_size);
	m.def("get_car_rotation_degrees", &env_api::get_car_rotation_degrees);
	m.def("get_car_ids", &env_api::get_car_ids);

	m.attr("car_action_do_nothing")        = traffic::DO_NOTHING;
	m.attr("car_action_brake_light")       = traffic::BRAKE_LIGHT;
	m.attr("car_action_brake_medium")      = traffic::BRAKE_MEDIUM;
	m.attr("car_action_brake_hard")        = traffic::BRAKE_HARD;
	m.attr("car_action_gas_light")         = traffic::GAS_LIGHT;
	m.attr("car_action_gas_medium")        = traffic::GAS_MEDIUM;
	m.attr("car_action_gas_hard")          = traffic::GAS_HARD;
	m.attr("car_action_turn_left_light")   = traffic::TURN_LEFT_LIGHT;
	m.attr("car_action_turn_left_medium")  = traffic::TURN_LEFT_MEDIUM;
	m.attr("car_action_turn_left_hard")    = traffic::TURN_LEFT_HARD;
	m.attr("car_action_turn_right_light")  = traffic::TURN_RIGHT_LIGHT;
	m.attr("car_action_turn_right_medium") = traffic::TURN_RIGHT_MEDIUM;
	m.attr("car_action_turn_right_hard")   = traffic::TURN_RIGHT_HARD;


#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
