include(ExternalProject)

if (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
    set(PEAK_ARCHIVE_SUB_PATH Win32)
elseif (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(PEAK_ARCHIVE_SUB_PATH x64)
else ()
    message(ERROR "unable to determine platform address size")
endif ()

externalproject_add(peak_basic_dll
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/peak_basic
    URL https://www.peak-system.com/fileadmin/media/files/pcan-basic.zip
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")

externalproject_add(peak_isotp_dll
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/peak_isotp
    URL https://www.peak-system.com/fileadmin/media/files/PCAN-ISO-TP.ZIP
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")

externalproject_add(peak_xcp_dll
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/peak_xcp
    URL http://www.peak-system.com/fileadmin/media/files/PCAN-XCP.ZIP
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")
