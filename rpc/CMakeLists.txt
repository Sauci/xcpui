add_subdirectory(client)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_subdirectory(server)
endif ()

add_subdirectory(shared)
