add_executable(usb_interface_peak_test usb_interface_peak_test.cpp)
target_link_libraries(usb_interface_peak_test
    PRIVATE usb_interface_peak
    PRIVATE gtest)

gtest_discover_tests(usb_interface_peak_test)
add_test(NAME usb_interface_peak_test
    COMMAND usb_interface_peak_test --gtest_output=xml:usb_interface_peak_test.xml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
