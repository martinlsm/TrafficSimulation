add_subdirectory("${PROJECT_SOURCE_DIR}/lib/googletest" "lib/googletest")

macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
	target_link_libraries(${TESTNAME} PRIVATE gtest gmock gtest_main traffic_framework)
    gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

package_add_test(test1 vec2d_test.cpp)
package_add_test(test2 straight_road_test.cpp)
