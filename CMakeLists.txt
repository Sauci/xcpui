#cmake_minimum_required(VERSION 3.13)
cmake_minimum_required(VERSION 3.9)

project(xcpui C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/script/cmake")

# build directories settings.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# https://cmake.org/cmake/help/v3.9/module/FindDoxygen.html
# https://docs.oracle.com/cd/E88353_01/html/E37853/cmake-variables-7.html
find_package(Doxygen OPTIONAL_COMPONENTS dot)

if (DOXYGEN_FOUND)
    doxygen_add_docs(doxygen
        ${CMAKE_CURRENT_SOURCE_DIR}/api
        ${CMAKE_CURRENT_SOURCE_DIR}/interface
        ${CMAKE_CURRENT_SOURCE_DIR}/rpc
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        COMMENT "Generate man pages")
endif ()

include(GoogleTest)
file(MAKE_DIRECTORY test)
enable_testing()
include_directories(dependencies/gtest/googlemock/include)
include_directories(dependencies/gtest/googletest/include)

#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})
#link_directories(${GTEST_LIBRARY_DIR})

#find_package(GMock REQUIRED)
#include_directories(${GMOCK_INCLUDE_DIRS})
#link_directories(${GMOCK_LIBRARY_DIR})

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    include(CodeCoverage)
    set(COVERAGE_GCOVR_EXCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

    setup_target_for_coverage_gcovr_html(NAME peak_api_test_coverage
        EXECUTABLE peak_api_test)

    setup_target_for_coverage_gcovr_html(NAME peak_interface_test_coverage
        EXECUTABLE peak_interface_test)

    setup_target_for_coverage_gcovr_html(NAME queue_safe_test_coverage
        EXECUTABLE queue_safe_test)

    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
endif ()

add_subdirectory(api)
add_subdirectory(dependencies)
#add_subdirectory(gateway)
add_subdirectory(interface)
add_subdirectory(rpc)
add_subdirectory(utils)

target_compile_definitions(rpc
    PUBLIC _WIN32_WINNT=0x0A00
    #    PUBLIC __USE_W32_SOCKETS
    #    PUBLIC ASIO_WINDOWS
    #    PUBLIC _WIN32_WINDOWS
    #    PUBLIC _WIN32_WINDOWS
    )


