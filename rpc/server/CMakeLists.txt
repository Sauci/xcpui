add_executable(xcp_server source/server.cpp)
target_link_libraries(xcp_server
    PRIVATE rpc
    PRIVATE rpc_shared
    PRIVATE xcp_peak_interface
    PRIVATE $<$<PLATFORM_ID:Windows>:ws2_32>
    PRIVATE $<$<PLATFORM_ID:Windows>:wsock32>)
