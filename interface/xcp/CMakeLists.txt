add_library(xcp_interface INTERFACE)
target_include_directories(xcp_interface INTERFACE include)
target_link_libraries(xcp_interface
    INTERFACE interface_interface
    INTERFACE peak_api
    INTERFACE xcp_api_types)

add_library(xcp_interface_mock INTERFACE)
target_include_directories(xcp_interface_mock INTERFACE include)
target_link_libraries(xcp_interface_mock
    INTERFACE gmock
    INTERFACE xcp_interface)

add_subdirectory(peak)
