add_library(rpc_shared INTERFACE)
target_include_directories(rpc_shared INTERFACE include)
target_link_libraries(rpc_shared INTERFACE xcp_api_types)
