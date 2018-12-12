set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable (simple_sort_test simple_sort_test.cpp)
target_link_libraries (simple_sort_test LINK_PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
