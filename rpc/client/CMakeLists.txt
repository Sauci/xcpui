add_executable(xcp_client source/client.cpp)
target_link_libraries(xcp_client
    PRIVATE rpc
    PRIVATE rpc_shared
    PRIVATE xcp_interface
    PRIVATE $<$<PLATFORM_ID:Windows>:ws2_32>
    PRIVATE $<$<PLATFORM_ID:Windows>:wsock32>)
