include(peak_dll.cmake)

add_library(peak_api source/peak_api.cpp)
target_include_directories(peak_api PUBLIC include)
add_dependencies(peak_api peak_basic_dll peak_isotp_dll peak_xcp_dll)

add_library(peak_api_types INTERFACE)
target_include_directories(peak_api_types INTERFACE include)

if (${enable_test})
    add_library(peak_api_mock INTERFACE)
    target_include_directories(peak_api_mock INTERFACE include)
    target_link_libraries(peak_api_mock
        INTERFACE gmock
        INTERFACE peak_api)

    add_subdirectory(test)
endif ()
