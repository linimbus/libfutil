cmake_minimum_required (VERSION 2.6)

project (futil)

# The version number.
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(CMAKE_VERBOSE_MAKEFILE ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
    )

# add the binary tree to the search path for include files
# so that we will find config.h
include_directories("${PROJECT_BINARY_DIR}")

add_subdirectory(src)
add_subdirectory(example)

