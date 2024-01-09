
set(FUTIL_EXAMPLE_SRC main.c)

# add the executable

link_directories(${PROJECT_BINARY_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(example ${FUTIL_EXAMPLE_SRC})
target_link_libraries(example futil)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# add the install targets
install (TARGETS example DESTINATION bin)

