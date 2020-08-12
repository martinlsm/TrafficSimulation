#include <pybind11/pybind11.h>
#include<pybind11/numpy.h>

#include "agent.h"

namespace py = pybind11;

PYBIND11_MODULE(python_agent, m) {
    m.def("load_traffic_environment", &agent::load_traffic_environment);
    m.def("destination_count", &agent::destination_count);
    m.def("car_count", &agent::car_count);
    m.def("spawn_car", &agent::spawn_car);
	m.def("index_to_action", &agent::index_to_action);
	m.def("do_action", &agent::do_action);
	m.def("update", &agent::update);
    m.def("read_state", &agent::read_state);
	m.def("get_reward", &agent::get_reward);
	m.def("in_terminal_state", &agent::in_terminal_state);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
