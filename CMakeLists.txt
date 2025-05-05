cmake_minimum_required(VERSION 3.14)
project(aislib VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(AISLIB_BUILD_TESTS "Build tests" ON)
option(AISLIB_BUILD_EXAMPLES "Build examples" ON)
option(AISLIB_BUILD_DOCS "Build documentation" OFF)

# Library sources
set(AISLIB_SOURCES
    src/bit_vector.cpp
    src/ais_message.cpp
    src/nmea_utils.cpp
    src/multipart_message_manager.cpp
    src/ais_parser.cpp
    src/message_factory.cpp
    src/position_report_class_b.cpp
    src/static_and_voyage_data.cpp
    src/binary_message.cpp
    src/binary_addressed_message.cpp
    src/binary_broadcast_message.cpp
    # Application-specific message types
    src/application/meteorological_data.cpp
    src/application/area_notice.cpp
)

# Library headers
set(AISLIB_HEADERS
    include/aislib/bit_vector.h
    include/aislib/ais_message.h
    include/aislib/nmea_utils.h
    include/aislib/multipart_message_manager.h
    include/aislib/ais_parser.h
    include/aislib/message_factory.h
    include/aislib/position_report_class_b.h
    include/aislib/static_data.h
    include/aislib/binary_message.h
    include/aislib/binary_addressed_message.h
    include/aislib/binary_broadcast_message.h
    # Application-specific message types
    include/aislib/application/binary_application_ids.h
    include/aislib/application/meteorological_data.h
    include/aislib/application/area_notice.h
)

# Create the library
add_library(aislib ${AISLIB_SOURCES})
target_include_directories(aislib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add compile options
target_compile_options(aislib PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

# Tests
if(AISLIB_BUILD_TESTS)
    enable_testing()
    
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
    
    # Add test executables
    add_executable(
        bit_vector_test
        tests/bit_vector_test.cpp
    )
    target_link_libraries(
        bit_vector_test
        aislib
        gtest_main
    )
    
    add_executable(
        nmea_utils_test
        tests/nmea_utils_test.cpp
    )
    target_link_libraries(
        nmea_utils_test
        aislib
        gtest_main
    )
    
    add_executable(
        multipart_message_manager_test
        tests/multipart_message_manager_test.cpp
    )
    target_link_libraries(
        multipart_message_manager_test
        aislib
        gtest_main
    )
    
    add_executable(
        position_report_class_b_test
        tests/position_report_class_b_test.cpp
    )
    target_link_libraries(
        position_report_class_b_test
        aislib
        gtest_main
    )
    
    # Binary message test
    add_executable(
        binary_message_test
        tests/binary_message_test.cpp
    )
    target_link_libraries(
        binary_message_test
        aislib
        gtest_main
    )
    
    # Register tests with CTest
    include(GoogleTest)
    gtest_discover_tests(bit_vector_test)
    gtest_discover_tests(nmea_utils_test)
    gtest_discover_tests(multipart_message_manager_test)
    gtest_discover_tests(position_report_class_b_test)
    gtest_discover_tests(binary_message_test)
endif()

# Examples
if(AISLIB_BUILD_EXAMPLES)
    add_executable(nmea_parser examples/nmea_parser.cpp)
    target_link_libraries(nmea_parser aislib)
    
    add_executable(binary_message_example examples/binary_message_example.cpp)
    target_link_libraries(binary_message_example aislib)
endif()

# Documentation
if(AISLIB_BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        # Configure Doxygen input file
        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            @ONLY
        )
        
        # Add documentation target
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )
    else()
        message(WARNING "Doxygen not found, documentation will not be built")
    endif()
endif()

# Installation
include(GNUInstallDirs)

# Install library
install(
    TARGETS aislib
    EXPORT aislibTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

# Export targets
install(
    EXPORT aislibTargets
    FILE aislibTargets.cmake
    NAMESPACE aislib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/aislib
)

# Create and install package configuration files
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/aislibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/aislibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/aislibConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/aislib"
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/aislibConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/aislibConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/aislib"
)