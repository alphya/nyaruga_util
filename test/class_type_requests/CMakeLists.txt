message(STATUS "Building test")

set(target class_type_requires-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})

if(MSVC)
  target_link_libraries(${target} ${Boost_LIBRARIES}) # libboost_unit_test_framework-vc141-mt-gd-x64-1_69.lib
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(${target} ${Boost_LIBRARIES}) # libboost_unit_test_framework.a
endif()

# target_link_libraries(${target} boost_unit_test_framework-mgw63-mt-d-x32-1_69)
# target_link_libraries(${target} libboost_unit_test_framework-vc141-mt-gd-x64-1_69.lib)
add_test(${target} "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(${target} PROPERTIES LABELS "all;class_type_requires")

clang_format(${target})
clang_tidy(${target})