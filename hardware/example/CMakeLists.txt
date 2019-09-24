cmake_minimum_required(VERSION 2.8)
project(example)

enable_language( C ASM )

# List of source files
set( SOURCE
    ${SOURCE}
    src/resources/index.html.s
    src/resources/bundle.js.s
    src/hardware.cpp
    )

# List of header files
set( HEADERS
    ${HEADERS}
    include/hardware/hardware.h
    include/hardware/resources/resources.h
    )

add_library(
    ${PROJECT_NAME} STATIC
    ${SOURCE}
    ${HEADERS}
    )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    # Include the project directories
    ${PROJECT_SOURCE_DIR}/resources
    ${PROJECT_SOURCE_DIR}/include
    # Include the common directory
    ${CMAKE_SOURCE_DIR}/common/include
    )

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CONTROL_LIB_DIR}
    ARCHIVE DESTINATION ${CONTROL_LIB_DIR}
    )
