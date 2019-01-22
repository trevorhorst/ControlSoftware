cmake_minimum_required(VERSION 2.8)
project(beagleboneblack)

enable_language( C ASM )

# List of source files
set( SOURCE
    ${SOURCE}
    src/resources/index.html.s
    src/resources/main.js.s
    src/hardware_beagleboneblack.cpp
    )

# List of header files
set( HEADERS
    ${HEADERS}
    include/beagleboneblack/hardware_beagleboneblack.h
    include/beagleboneblack/resources/resources.h
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
