#ifndef COMMAND_LED_H
#define COMMAND_LED_H

#include "common/command/command_template.h"
#include "common/drivers/led.h"

#define COMMAND_LED     "led"
#define COMMAND_QLED    "qled"

#define PARAM_ID    "id"

class CommandLed
        : public CommandTemplate< Led >
{
public:
    CommandLed();

    virtual uint32_t setId( cJSON *val );
};

#endif // COMMAND_LED_H
