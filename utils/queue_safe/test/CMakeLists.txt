add_executable(queue_safe_test queue_safe_test.cpp)
target_link_libraries(queue_safe_test
    PRIVATE gtest
    PRIVATE queue_safe)

gtest_discover_tests(queue_safe_test)
add_test(NAME queue_safe_test
    COMMAND queue_safe_test --gtest_output=xml:queue_safe_test.xml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
