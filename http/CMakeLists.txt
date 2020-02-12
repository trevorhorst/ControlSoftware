cmake_minimum_required(VERSION 2.8)
project(
    http
    DESCRIPTION "HTTP Services Library"
    )

set( HEADERS ${HEADERS}
    include/http/http.h
    include/http/client.h
    )

set( SOURCE ${SOURCE}
    src/http.cpp
    src/client.cpp
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
        curl
    )

# Specifies include directories to use when compiling a given target
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

install(
    TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CONTROL_LIB_DIR}
    ARCHIVE DESTINATION ${CONTROL_LIB_DIR}
    )
