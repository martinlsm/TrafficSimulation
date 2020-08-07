#include <pybind11/pybind11.h>
#include<pybind11/numpy.h>

#include "agent.h"

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(python_agent, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: python_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

	 m.def("add_arrays_1d", &agent::add_arrays_1d);

	m.def("load_traffic_environment", &agent::load_traffic_environment);
	m.def("destination_count", &agent::destination_count);
	m.def("car_count", &agent::car_count);
	m.def("spawn_car", &agent::spawn_car);

	m.def("read_state", &agent::read_state);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
