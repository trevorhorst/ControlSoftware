cmake_minimum_required(VERSION 2.8)
project(
    example
    DESCRIPTION "Example Library"
    )

enable_language( C ASM )

option( USE_HTTP "Use optional HTTP library" ON )

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

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        common
        http
        )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    # Include the project directories
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/resources>
    $<INSTALL_INTERFACE:include>
    )

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CONTROL_LIB_DIR}
    ARCHIVE DESTINATION ${CONTROL_LIB_DIR}
    )
