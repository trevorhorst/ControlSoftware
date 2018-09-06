cmake_minimum_required(VERSION 2.8)
project(controld)
# enable_language( C ASM )

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(
    ${PROJECT_NAME}
    beagleboneblack
    common
    pthread
    readline
    )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CONTROL_BIN_DIR})
