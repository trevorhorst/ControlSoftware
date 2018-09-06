cmake_minimum_required(VERSION 2.8)
project(common)

add_library(
    ${PROJECT_NAME} STATIC
    src/cjson/cJSON.c
    src/command/command.cpp
    src/command/command_console.cpp
    src/console/console.cpp
    src/control/control.cpp
    src/error/error.cpp
    )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/common/include
    )

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CONTROL_LIB_DIR}
    ARCHIVE DESTINATION ${CONTROL_LIB_DIR}
    )
