cmake_minimum_required(VERSION 2.8)

project(ControlSoftware)

# add_compile_options(-std=c++11)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic")

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

add_subdirectory(app)
add_subdirectory(remote)
add_subdirectory(common)
add_subdirectory(hardware/beagleboneblack)

# install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CONTROL_BIN_DIR})
