#pragma once

#include <pybind11/pybind11.h>
#include<pybind11/numpy.h>

#include "car.h"

namespace py = pybind11;

namespace agent {

py::array_t<double> add_arrays_1d(py::array_t<double>& input1, py::array_t<double>& input2);

py::array_t<float> read_state();

} // namespace agent
