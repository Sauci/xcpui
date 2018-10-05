add_library(queue INTERFACE)
target_include_directories(queue INTERFACE include)
target_link_libraries(queue INTERFACE ${Boost_LIBRARIES})

add_subdirectory(test)
