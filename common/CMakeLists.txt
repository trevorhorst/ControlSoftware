cmake_minimum_required(VERSION 2.8)
project(common)

set( SOURCE
    ${SOURCE}
    # cJSON Library
    src/cjson/cJSON.c

    # Commands
    src/command/command.cpp
    src/command/command_console.cpp
    src/command/command_datetime.cpp
    src/command/command_handler.cpp
    src/command/command_help.cpp
    src/command/command_server.cpp
    src/command/command_system.cpp

    # Console
    src/console/console.cpp

    # Control
    src/control/control.cpp

    # Error
    src/error/error.cpp

    # HTTP
    src/http/http.cpp
    # src/http/client.cpp
    src/http/client/client.cpp
    src/http/server/request.cpp
    src/http/server/server.cpp

    # Resources
    src/resources/resources.cpp

    # System
    src/system/datetime.cpp
    src/system/system.cpp
    )

set( HEADERS
    ${HEADERS}
    # cJSON Library
    include/common/cjson/cJSON.h

    # Commands
    include/common/command/command.h
    include/common/command/command_console.h
    include/common/command/command_datetime.h
    include/common/command/command_handler.h
    include/common/command/command_help.h
    include/common/command/command_led.h
    include/common/command/command_server.h
    include/common/command/command_system.h
    include/common/command/command_template.h

    # Control
    include/common/control/control.h
    include/common/control/control_template.h

    # Error
    include/common/error/error.h

    # Hardware
    include/common/hardware/hardware.h

    # HTTP
    include/common/http/http.h
    # include/common/http/client.h
    include/common/http/client/client.h
    include/common/http/server/request.h
    include/common/http/server/server.h

    # Resources
    include/common/resources/resources.h

    # System
    include/common/system/datetime.h
    include/common/system/system.h

    # Console
    include/common/console/console.h

    # Miscellaneous
    include/common/common_types.h
    include/common/singleton.h
    )

add_library(
    ${PROJECT_NAME} STATIC
    ${SOURCE}
    ${HEADERS}
    )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    )

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CONTROL_LIB_DIR}
    ARCHIVE DESTINATION ${CONTROL_LIB_DIR}
    )
