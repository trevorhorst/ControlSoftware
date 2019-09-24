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
    src/command/command_gpio.cpp
    src/command/command_handler.cpp
    src/command/command_help.cpp
    src/command/command_led.cpp
    src/command/command_server.cpp
    src/command/command_system.cpp
    src/command/command_heartbeat.cpp
    src/command/command_venus638flpx.cpp

    # Console
    src/console/console.cpp

    # Control
    src/control/control.cpp

    # Drivers
    src/drivers/am335x/gpio.cpp
    src/drivers/am335x/clock_module.cpp
    src/drivers/am335x/control_module.cpp
    src/drivers/devices/displays/ssd1306.cpp
    src/drivers/devices/gps/gps.cpp
    src/drivers/devices/gps/venus638flpx.cpp
    src/drivers/i2c.cpp
    src/drivers/led.cpp
    src/drivers/serial.cpp

    # Error
    src/error/error.cpp

    # Hardware library
    src/hardware/hardware_base.cpp

    # HTTP
    src/http/http.cpp
    # src/http/client.cpp
    src/http/client/client.cpp
    src/http/server/request.cpp
    src/http/server/server.cpp

    # Logger
    src/logger/log.cpp

    # Resources
    src/resources/resources.cpp
    src/resources/font/glcd.cpp

    # System
    src/system/datetime.cpp
    src/system/system.cpp

    # Miscellaneous
    src/common_types.cpp
    src/string.cpp
    src/timer.cpp
    )

set( HEADERS
    ${HEADERS}
    # cJSON Library
    include/common/cjson/cJSON.h

    # Commands
    include/common/command/command.h
    include/common/command/command_console.h
    include/common/command/command_datetime.h
    include/common/command/command_gpio.h
    include/common/command/command_handler.h
    include/common/command/command_help.h
    include/common/command/command_led.h
    include/common/command/command_server.h
    include/common/command/command_system.h
    include/common/command/command_template.h
    include/common/command/command_heartbeat.h
    include/common/command/command_venus638flpx.h

    # Control
    include/common/control/control.h
    include/common/control/control_template.h

    # Drivers
    include/common/drivers/am335x/gpio.h
    include/common/drivers/am335x/clock_module.h
    include/common/drivers/am335x/control_module.h
    include/common/drivers/devices/displays/ssd1306.h
    include/common/drivers/devices/gps/gps.h
    include/common/drivers/devices/gps/venus638flpx.h
    include/common/drivers/i2c.h
    include/common/drivers/led.h
    include/common/drivers/serial.h

    # Error
    include/common/error/error.h

    # Hardware library
    include/common/hardware/hardware_base.h

    # HTTP
    include/common/http/http.h
    # include/common/http/client.h
    include/common/http/client/client.h
    include/common/http/server/request.h
    include/common/http/server/server.h

    # Logger
    include/common/logger/log.h

    # Option parser
    include/common/option_parser.h

    # Resources
    include/common/resources/resources.h
    include/common/resources/font/glcd.h

    # System
    include/common/system/datetime.h
    include/common/system/system.h

    # Console
    include/common/console/console.h

    # Miscellaneous
    include/common/common_types.h
    include/common/register.h
    include/common/singleton.h
    include/common/string.h
    include/common/timer.h
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
