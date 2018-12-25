message(STATUS "Building test")

set(target singleton-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(singleton "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(singleton PROPERTIES LABELS "all;singleton")