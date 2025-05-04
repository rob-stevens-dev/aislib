# Find or fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing for the project
enable_testing()

# Add test executables
add_executable(
    bit_vector_test
    bit_vector_test.cpp
)
target_link_libraries(
    bit_vector_test
    aislib
    gtest_main
)

add_executable(
    nmea_utils_test
    nmea_utils_test.cpp
)
target_link_libraries(
    nmea_utils_test
    aislib
    gtest_main
)

add_executable(
    multipart_message_manager_test
    multipart_message_manager_test.cpp
)
target_link_libraries(
    multipart_message_manager_test
    aislib
    gtest_main
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(bit_vector_test)
gtest_discover_tests(nmea_utils_test)
gtest_discover_tests(multipart_message_manager_test)