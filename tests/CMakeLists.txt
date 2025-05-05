# Add test for application-specific message types
add_executable(
    meteorological_data_test
    application/meteorological_data_test.cpp
)
target_link_libraries(
    meteorological_data_test
    aislib
    gtest_main
)

# Add test for multi-part message handling (Phase 4)
add_executable(
    multipart_message_integration_test
    multipart_message_integration_test.cpp
)
target_link_libraries(
    multipart_message_integration_test
    aislib
    gtest_main
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(meteorological_data_test)
gtest_discover_tests(multipart_message_integration_test)