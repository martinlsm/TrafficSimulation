# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.18)

# Set the project name
project(traffic_sim)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
	src/car.cpp
	src/car_actions.cpp
	src/roadway.cpp
	src/rendering.cpp
	src/game_world.cpp
)
set(MAIN_FILE
	src/main.cpp
)
set(PROJECT_INCLUDE_DIR
	${PROJECT_SOURCE_DIR}/include
)

add_library(traffic_framework SHARED ${SOURCES})
target_include_directories(traffic_framework PUBLIC ${PROJECT_INCLUDE_DIR})
target_link_libraries(traffic_framework -lsfml-graphics -lsfml-window -lsfml-system)

# Add an executable with the above sources
add_executable(traffic_sim ${MAIN_FILE})

target_link_libraries(traffic_sim PUBLIC traffic_framework)

add_subdirectory(python_agent)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
