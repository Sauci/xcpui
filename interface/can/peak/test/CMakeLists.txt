add_executable(can_interface_peak_test can_interface_peak_test.cpp)
target_link_libraries(can_interface_peak_test
    PRIVATE can_interface_peak
    PRIVATE gtest)

gtest_discover_tests(can_interface_peak_test)
add_test(NAME can_interface_peak_test
    COMMAND can_interface_peak_test --gtest_output=xml:can_interface_peak_test.xml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
