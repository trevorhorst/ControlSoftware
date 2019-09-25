#ifndef COMMAND_GPIO_H
#define COMMAND_GPIO_H

#include "common/command/command_template.h"
#include "common/drivers/am335x/gpio.h"

#define COMMAND_GPIO    "gpio"
#define COMMAND_QGPIO   "qgpio"

#define PARAM_BANK      "bank"
#define PARAM_PIN       "pin"
#define PARAM_DIR       "dir"
#define PARAM_INPUT     "input"
#define PARAM_OUTPUT    "output"

class CommandGpio
        : public CommandTemplate< AM335X::Gpio >
{
public:
    CommandGpio( const char *mutator = COMMAND_GPIO
            , const char *accessor = COMMAND_QGPIO );

    virtual uint32_t setPin( cJSON *val );
    virtual uint32_t setBank( cJSON *val );
    virtual uint32_t setOutput( cJSON *val );
    virtual uint32_t setDirection( cJSON *val );

    virtual uint32_t getBank( cJSON *response );
    virtual uint32_t getPin( cJSON *response );
    virtual uint32_t getOutput( cJSON *response );
    virtual uint32_t getInput( cJSON *response );
    virtual uint32_t getDirection( cJSON *response );

    virtual uint32_t getBankOutput( cJSON *response );
    virtual uint32_t getBankInput( cJSON *response );

protected:
    uint32_t mPin;
    uint32_t mBank;
};

#endif // COMMAND_GPIO_H
