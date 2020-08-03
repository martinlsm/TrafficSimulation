# set(AGENT_CPP_SOURCES
# 	agent.cpp
# )

# add_library(agent_cpp_lib SHARED ${AGENT_CPP_SOURCES})
# target_include_directories(agent_cpp_lib PUBLIC)

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
	"/home/martin/.local/lib/python3.8/site-packages/numpy/core/include"
	"/usr/include/python3.8"
	${Python_INCLUDE_DIRS}
)

execute_process(COMMAND echo python3.8 "get_numpy_path.py"
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
	OUTPUT_VARIABLE NUMPY_PATH)

message(${NUMPY_PATH})
