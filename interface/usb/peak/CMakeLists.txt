include(ExternalProject)

if (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
    set(PEAK_ARCHIVE_SUB_PATH Win32)
elseif (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(PEAK_ARCHIVE_SUB_PATH x64)
else ()
    message(ERROR "unable to determine platform address size")
endif ()

externalproject_add(peak_dll
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib
    URL https://www.peak-system.com/fileadmin/media/files/pcan-basic.zip
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")

add_library(usb_interface_peak source/usb_interface_peak.cpp)
target_include_directories(usb_interface_peak PUBLIC include)
target_link_libraries(usb_interface_peak PUBLIC usb_interface)

add_subdirectory(test)
