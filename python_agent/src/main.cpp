#include <pybind11/pybind11.h>
#include<pybind11/numpy.h>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

py::array_t<double> add_arrays_1d(py::array_t<double>& input1, py::array_t<double>& input2) {

	// Get info from input1, input2
	py::buffer_info buf1 = input1.request();
	py::buffer_info buf2 = input2.request();

	if (buf1.ndim !=1 || buf2.ndim !=1)
	{
		throw std::runtime_error("Number of dimensions must be one");
	}

	if (buf1.size !=buf2.size)
	{
		throw std::runtime_error("Input shape must match");
	}

	//Apply resources
	auto result = py::array_t<double>(buf1.size);
	py::buffer_info buf3 = result.request();

	//Obtain numpy.ndarray data pointer
	double* ptr1 = (double*)buf1.ptr;
	double* ptr2 = (double*)buf2.ptr;
	double* ptr3 = (double*)buf3.ptr;

	//Pointer visits numpy.ndarray
	for (int i = 0; i < buf1.shape[0]; i++)
	{
		ptr3[i] = ptr1[i] + ptr2[i];
	}

	return result;

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

	 m.def("add_arrays_1d", &add_arrays_1d);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
