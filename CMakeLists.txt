# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (traffic_sim)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -g")
# SET(GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS} -fsanitize=address,undefined")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(MAIN_FILE src/main.cpp)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
	src/car.cpp
	src/car_actions.cpp
	src/road_system.cpp
	src/rendering.cpp
	src/traffic_env.cpp
	src/texture_manager.cpp
)

add_library(traffic_framework SHARED ${SOURCES})
target_include_directories(traffic_framework PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(traffic_framework -lsfml-graphics -lsfml-window -lsfml-system)

# Add an executable with the above sources
add_executable(traffic_sim ${MAIN_FILE} ${SOURCES})
target_link_libraries(traffic_sim traffic_framework)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
