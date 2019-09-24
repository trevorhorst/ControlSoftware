cmake_minimum_required(VERSION 2.8)

project(ControlSoftware)

# Creates a truncated filename macro for use with the logger
# https://stackoverflow.com/questions/8487986/file-macro-shows-full-path
set( CMAKE_CXX_FLAGS 
    "-D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'" 
    )
    
# Add compiler options
set( CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic"
    )

if(DEFINED ENV{CONTROL_PROD})
    # In a production build, install the libraries to the system
    message("Production Build")
    set(CONTROL_LIB_DIR "lib")
    set(CONTROL_BIN_DIR "bin")
else()
    # In a development build, keep the libraries and binaries local
    message("Development Build")
    set(CONTROL_LIB_DIR "$ENV{PWD}/lib")
    set(CONTROL_BIN_DIR "$ENV{PWD}/bin")
endif()

if(DEFINED ENV{CONTROL_PROJECT})
    set( CONTROL_PROJECT "$ENV{CONTROL_PROJECT}" )
else()
    message("No project set, building example project")
    set( CONTROL_PROJECT "example" )
endif()
message("Building ${CONTROL_PROJECT}")

add_subdirectory(app)
add_subdirectory(remote)
add_subdirectory(common)
add_subdirectory(hardware/${CONTROL_PROJECT})

# install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CONTROL_BIN_DIR})
