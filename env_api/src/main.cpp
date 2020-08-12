#include <pybind11/pybind11.h>
#include<pybind11/numpy.h>

#include "env_api.h"

namespace py = pybind11;

PYBIND11_MODULE(env_api, m) {
    m.def("load_traffic_environment", &env_api::load_traffic_environment);
    m.def("destination_count", &env_api::destination_count);
    m.def("car_count", &env_api::car_count);
    m.def("spawn_car", &env_api::spawn_car);
	m.def("index_to_action", &env_api::index_to_action);
	m.def("do_action", &env_api::do_action);
	m.def("update", &env_api::update);
    m.def("read_state", &env_api::read_state);
	m.def("get_reward", &env_api::get_reward);
	m.def("in_terminal_state", &env_api::in_terminal_state);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
