set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable (simple_sort_test simple_sort_test.cpp)
target_link_libraries (simple_sort_test LINK_PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_executable (sort_and_replace_test sort_and_replace_test.cpp)
target_link_libraries (sort_and_replace_test LINK_PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
