cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0063 NEW)

# Set the project name
project (traffic_sim CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC TRUE)

set(APP_DIR "${CMAKE_CURRENT_LIST_DIR}"
    CACHE PATH "Application directory")
set(BLEND2D_DIR "${APP_DIR}/../blend2d"
    CACHE PATH "Location of 'blend2d'")

# Enable Blend2D static build.
set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

find_package(Qt5Widgets REQUIRED)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
	src/qblcanvas.cpp
	src/main.cpp
	src/car.cpp
)

# Add an executable with the above sources
add_executable(traffic_sim ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(traffic_sim
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)
set_property(TARGET traffic_sim PROPERTY CXX_VISIBILITY_PRESET hidden)
target_link_libraries(traffic_sim Qt5::Widgets Blend2D::Blend2D)
qt5_use_modules(traffic_sim Widgets)
