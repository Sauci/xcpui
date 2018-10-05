add_executable(peak_interface_test peak_interface_test.cpp)
target_link_libraries(peak_interface_test
    PRIVATE gtest
    PRIVATE peak_api_mock
    PRIVATE xcp_peak_interface)

gtest_discover_tests(peak_interface_test)
add_test(NAME peak_interface_test
    COMMAND peak_interface_test --gtest_output=xml:peak_interface_test.xml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
