#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "env_api.h"

namespace py = pybind11;

struct Pet {
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};

PYBIND11_MODULE(env_api, m) {
	py::class_<Pet>(m, "Pet").def(py::init<const std::string &>()).def("setName", &Pet::setName).def("getName", &Pet::getName);

	py::class_<env_api::Environment>(m, "Environment")
		.def(py::init<unsigned int>())
    	.def("destination_count", &env_api::Environment::destination_count)
    	.def("car_count", &env_api::Environment::car_count)
		// .def("state_dim_size", &env_api::state_dim_size)
		.def("action_dim_size", &env_api::Environment::action_dim_size)
		.def("spawn_car", &env_api::Environment::spawn_car)
		.def("spawn_car_at_random_pos", &env_api::Environment::spawn_car_at_random_pos)
		.def("remove_car", &env_api::Environment::remove_car)
		.def("index_to_action", &env_api::Environment::index_to_action)
		.def("do_action", &env_api::Environment::do_action)
		.def("update", &env_api::Environment::update)
    	.def("read_state_simple", &env_api::Environment::read_state_simple)
    	.def("read_state_sensors", &env_api::Environment::read_state_sensors)
		.def("get_reward_simple", &env_api::Environment::get_reward_simple)
		.def("get_reward_advanced", &env_api::Environment::get_reward_advanced)
		.def("in_terminal_state", &env_api::Environment::in_terminal_state)
		.def("get_car_position", &env_api::Environment::get_car_position)
		.def("get_car_destination", &env_api::Environment::get_car_destination)
		.def("get_car_size", &env_api::Environment::get_car_size)
		.def("get_car_rotation_degrees", &env_api::Environment::get_car_rotation_degrees)
		.def("get_car_ids", &env_api::Environment::get_car_ids)
		.def("get_goal_margin", &env_api::Environment::get_goal_margin);

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
