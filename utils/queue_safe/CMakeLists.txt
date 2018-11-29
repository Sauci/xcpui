add_library(queue_safe INTERFACE)
target_include_directories(queue_safe INTERFACE include)
target_link_libraries(queue_safe INTERFACE ${Boost_LIBRARIES})

if (${enable_test})
    add_subdirectory(test)
endif ()
