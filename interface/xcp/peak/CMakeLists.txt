add_library(xcp_peak_interface INTERFACE)
target_include_directories(xcp_peak_interface INTERFACE include)
target_link_libraries(xcp_peak_interface
    INTERFACE queue
    INTERFACE peak_api
    INTERFACE peak_api_types
    INTERFACE xcp_interface)

add_subdirectory(test)
