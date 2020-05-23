#ifndef LED_H
#define LED_H

#include "common/control/control_template.h"

#include "common/drivers/am335x/gpio.h"

class Led
        : public ControlTemplate< Led >
{
public:
    Led(AM335X::Gpio *bank, uint32_t pin);

    uint32_t setEnable( bool enable );

    bool isEnabled();

private:
    bool mEnable;
    uint32_t mPin;
    AM335X::Gpio *mBank;
};

#endif // LED_H
