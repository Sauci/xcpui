#cmake_minimum_required(VERSION 3.13)
cmake_minimum_required(VERSION 3.9)

project(xcpui C CXX)

option(enable_test "enable/disable test executable generation" ON)
option(enable_documentation "enable/disable documentation generation" OFF)

add_compile_definitions(

    XCP_DISABLE_PROGRAM_VERIFY_COMMAND
    XCP_DISABLE_PROGRAM_MAX_COMMAND
    XCP_DISABLE_PROGRAM_NEXT_COMMAND
    XCP_DISABLE_PROGRAM_FORMAT_COMMAND
    XCP_DISABLE_PROGRAM_PREPARE_COMMAND
    XCP_DISABLE_GET_SECTOR_INFO_COMMAND
    XCP_DISABLE_GET_PGM_PROCESSOR_INFO_COMMAND
    XCP_DISABLE_PROGRAM_RESET_COMMAND
    XCP_DISABLE_PROGRAM_COMMAND
    XCP_DISABLE_PROGRAM_CLEAR_COMMAND
    XCP_DISABLE_PROGRAM_START_COMMAND
    XCP_DISABLE_ALLOC_ODT_ENTRY_COMMAND
    XCP_DISABLE_ALLOC_ODT_COMMAND
    XCP_DISABLE_ALLOC_DAQ_COMMAND
    XCP_DISABLE_FREE_DAQ_COMMAND
    XCP_DISABLE_GET_DAQ_EVENT_INFO_COMMAND
    XCP_DISABLE_GET_DAQ_LIST_INFO_COMMAND
    XCP_DISABLE_GET_DAQ_RESOLUTION_INFO_COMMAND
    XCP_DISABLE_GET_DAQ_PROCESSOR_INFO_COMMAND
    XCP_DISABLE_READ_DAQ_COMMAND
    XCP_DISABLE_GET_DAQ_CLOCK_COMMAND
    XCP_DISABLE_START_STOP_SYNCH_COMMAND
    XCP_DISABLE_START_STOP_DAQ_LIST_COMMAND
    XCP_DISABLE_GET_DAQ_LIST_MODE_COMMAND
    XCP_DISABLE_SET_DAQ_LIST_MODE_COMMAND
    XCP_DISABLE_WRITE_DAQ_COMMAND
    XCP_DISABLE_SET_DAQ_PTR_COMMAND
    XCP_DISABLE_CLEAR_DAQ_LIST_COMMAND
    XCP_DISABLE_COPY_CAL_PAGE_COMMAND
    XCP_DISABLE_GET_SEGMENT_MODE_COMMAND
    XCP_DISABLE_SET_SEGMENT_MODE_COMMAND
    XCP_DISABLE_GET_PAGE_INFO_COMMAND
    XCP_DISABLE_GET_SEGMENT_INFO_COMMAND
    XCP_DISABLE_GET_PAG_PROCESSOR_INFO_COMMAND
    XCP_DISABLE_GET_CAL_PAGE_COMMAND
    XCP_DISABLE_SET_CAL_PAGE_COMMAND
    XCP_DISABLE_MODIFY_BITS_COMMAND
    XCP_DISABLE_SHORT_DOWNLOAD_COMMAND
    XCP_DISABLE_DOWNLOAD_MAX_COMMAND
    XCP_DISABLE_DOWNLOAD_NEXT_COMMAND
    XCP_DISABLE_DOWNLOAD_COMMAND
    XCP_DISABLE_USER_CMD_COMMAND
    XCP_DISABLE_TRANSPORT_LAYER_CMD_COMMAND
    XCP_DISABLE_BUILD_CHECKSUM_COMMAND
    XCP_DISABLE_SHORT_UPLOAD_COMMAND
    XCP_DISABLE_UPLOAD_COMMAND
    XCP_DISABLE_SET_MTA_COMMAND
    XCP_DISABLE_UNLOCK_COMMAND
    XCP_DISABLE_GET_SEED_COMMAND
    XCP_DISABLE_SET_REQUEST_COMMAND
    XCP_DISABLE_GET_ID_COMMAND
    XCP_DISABLE_GET_COMM_MODE_INFO_COMMAND
    XCP_DISABLE_SYNCH_COMMAND
    XCP_DISABLE_GET_STATUS_COMMAND
    XCP_DISABLE_DISCONNECT_COMMAND
#    XCP_DISABLE_CONNECT_COMMAND
)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/script/cmake")

# build directories settings.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if (${enable_documentation})
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
endif ()

if (${enable_test})

    enable_testing()
    include(GoogleTest)
    file(MAKE_DIRECTORY test)
    include_directories(dependencies/gtest/googlemock/include)
    include_directories(dependencies/gtest/googletest/include)

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

endif ()

add_subdirectory(api)
add_subdirectory(dependencies)
add_subdirectory(interface)
add_subdirectory(protocol)
add_subdirectory(transport)
add_subdirectory(rpc)
add_subdirectory(utils)

target_compile_definitions(rpc
    PUBLIC _WIN32_WINNT=0x0A00)
