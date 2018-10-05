add_executable(peak_api_test peak_api_test.cpp)
target_link_libraries(peak_api_test
    PRIVATE peak_api
    PRIVATE gtest)

gtest_discover_tests(peak_api_test)
add_test(NAME peak_api_test
    COMMAND peak_api_test --gtest_output=xml:peak_api_test.xml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
