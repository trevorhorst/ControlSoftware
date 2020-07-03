cmake_minimum_required(VERSION 2.8)
project(
    mlb
    DESCRIPTION "MLB Transaction Library"
    )

enable_language( C ASM )

# Make use of the SMTP library
option( USE_SMTP "Use optional SMTP library" ON )
option( USE_HTTP "Use optional HTTP library" ON )

# Configure a SOURCE variable that contains all the source files for the library
set(
    SOURCE ${SOURCE}
    src/resources/index.html.s
    src/resources/bundle.js.s
    src/hardware.cpp
    )

# Configure a HEADERS variable that contains all the source files for the library
set(
    HEADERS ${HEADERS}
    include/hardware/hardware.h
    include/hardware/resources/resources.h
    )

# Creates a library. Libraries can be of the type STATIC, SHARED or MODULE
# STATIC - Archives of object files for use when linking other targets
# SHARED - Linked dynamically and loaded at run time
# MODULE - Not linked in to other targets buy may be laoded dynamically at runtime
#
# In some cases you'll need a fictional target, where nothing needs to be compiled.
# An example of this is a header only library. This is an INTERFACE library.
add_library(
    ${PROJECT_NAME} STATIC
    ${SOURCE}
    ${HEADERS}
    )

# Adds a dependency if a target is given. If no target of that name is found, it
# then it adds a link to a library of the target name on your PATH.
# Therefore if curl is not a target to be built, then cmake will look for that
# library in the local PATH
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        common
        smtp
        http
    )

# Specifies include directories to use when compiling a given target
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/resources>
    $<INSTALL_INTERFACE:include>
    )

install(
    TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CONTROL_LIB_DIR}
    ARCHIVE DESTINATION ${CONTROL_LIB_DIR}
    )
