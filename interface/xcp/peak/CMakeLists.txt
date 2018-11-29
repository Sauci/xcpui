add_library(xcp_peak_interface INTERFACE)
target_include_directories(xcp_peak_interface INTERFACE include)
target_link_libraries(xcp_peak_interface
    INTERFACE peak_api
    INTERFACE peak_api_types
    INTERFACE queue_safe
    INTERFACE xcp_interface)

if (${enable_test})
    add_subdirectory(test)
endif ()
