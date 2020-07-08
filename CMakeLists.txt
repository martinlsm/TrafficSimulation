# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (traffic_sim)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/main.cpp
	src/car.cpp
	src/car_actions.cpp
	src/roadway.cpp
	src/rendering.cpp
	src/game_world.cpp
)

# Add an executable with the above sources
add_executable(traffic_sim ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(traffic_sim
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(traffic_sim -lsfml-graphics -lsfml-window -lsfml-system)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
