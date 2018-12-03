add_library(isotp INTERFACE)
target_include_directories(isotp INTERFACE include)
target_link_libraries(isotp INTERFACE can_interface)
