add_subdirectory("${PROJECT_SOURCE_DIR}/lib/googletest" "lib/googletest")

set(TEST_SOURCE
	../src/car.cpp
	../src/car_actions.cpp
	../src/roadway.cpp
)

add_library(test_lib STATIC ${TEST_SOURCE})
target_include_directories(test_lib PRIVATE ${PROJECT_SOURCE_DIR}/include)

macro(package_add_test TESTNAME)
    # create an executable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
	target_include_directories(${TESTNAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
	target_link_libraries(${TESTNAME} gtest gmock gtest_main test_lib)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

# package_add_test(test1 vec2d_test.cpp)
package_add_test(test2 straight_road_test.cpp)
