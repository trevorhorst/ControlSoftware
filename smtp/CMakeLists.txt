cmake_minimum_required(VERSION 2.8)
project(
    smtp
    DESCRIPTION "SMTP Library"
    )

# List of source files
set( SOURCE
    ${SOURCE}
    src/smtp.cpp
    src/client.cpp
    src/command.cpp
    )   

# List of header files
set( HEADERS
    ${HEADERS}
    include/smtp/smtp.h
    include/smtp/client.h
    include/smtp/command.h
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
    )

# Specifies include directories to use when compiling a given target
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CONTROL_LIB_DIR}
    ARCHIVE DESTINATION ${CONTROL_LIB_DIR}
    ) 
