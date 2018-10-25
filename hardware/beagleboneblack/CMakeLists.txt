cmake_minimum_required(VERSION 2.8)
project(beagleboneblack)

enable_language( C ASM )

add_library(
    ${PROJECT_NAME} STATIC
    src/resources/index.s
    src/resources/main.js.s
    src/resources/resources.cpp
    src/hardware_beagleboneblack.cpp
    )

include_directories( ${CMAKE_SOURCE_DIR}/common/include )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/resources
    ${PROJECT_SOURCE_DIR}/include
    )

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CONTROL_LIB_DIR}
    ARCHIVE DESTINATION ${CONTROL_LIB_DIR}
    )
