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
)

# Library headers
set(AISLIB_HEADERS
    include/aislib/bit_vector.h
    include/aislib/ais_message.h
    include/aislib/nmea_utils.h
    include/aislib/multipart_message_manager.h
    include/aislib/ais_parser.h
    include/aislib/message_factory.h
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
    add_subdirectory(tests)
endif()

# Examples
if(AISLIB_BUILD_EXAMPLES)
    add_subdirectory(examples)
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