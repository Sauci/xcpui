add_library(can_interface_peak source/can_interface_peak.cpp)
target_include_directories(can_interface_peak PUBLIC include)
target_link_libraries(can_interface_peak
    PUBLIC can_interface
    PUBLIC usb_interface_peak)

add_subdirectory(test)
