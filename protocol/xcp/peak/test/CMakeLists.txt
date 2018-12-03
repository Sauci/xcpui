add_executable(xcp_peak_interface_test peak_interface_test.cpp)
target_link_libraries(xcp_peak_interface_test
    PRIVATE gtest
    PRIVATE peak_api_mock
    PRIVATE xcp_peak_interface)

gtest_discover_tests(xcp_peak_interface_test)
add_test(NAME xcp_peak_interface_test
    COMMAND xcp_peak_interface_test --gtest_output=xml:xcp_peak_interface_test.xml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
