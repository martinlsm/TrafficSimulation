# find Numpy include path
execute_process(
	COMMAND "python3.8" -c
		  "from __future__ import print_function\ntry: import numpy; print(numpy.get_include(), end='')\nexcept:pass\n"
	RESULT_VARIABLE NUMPY_RESULT
	OUTPUT_VARIABLE NUMPY_INCLUDE_PATH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (NOT ${NUMPY_RESULT})
	message("Numpy found at ${NUMPY_INCLUDE_PATH}")
else()
	message("Could not find Numpy")
endif()

# find Swig
find_package(SWIG 3.0 COMPONENTS python OPTIONAL_COMPONENTS cpp)
if(SWIG_FOUND)
	message("SWIG found: ${SWIG_EXECUTABLE}")
endif()
include(${SWIG_USE_FILE})

find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_SWIG_FLAGS "")

set(AGENT_SOURCES
	agent.i
)

set_source_files_properties(agent.i PROPERTY CPLUSPLUS ON)
set_source_files_properties(agent.i PROPERTIES SWIG_FLAGS "-includeall")

swig_add_library(agent
	 TYPE SHARED
	 LANGUAGE python
	 SOURCES ${AGENT_SOURCES}
)

swig_link_libraries(agent PUBLIC ${PYTHON_LIBRARIES} -shared -lc)

include_directories(agent
	PRIVATE
	${NUMPY_INCLUDE_PATH}
	${Python_INCLUDE_DIRS}
)
