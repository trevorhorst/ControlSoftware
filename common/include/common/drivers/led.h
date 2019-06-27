#ifndef LED_H
#define LED_H

#include "common/control/control_template.h"

class Led
        : public ControlTemplate< Led >
{
public:
    Led();

    uint32_t setEnable( bool enable );

    bool isEnabled();

private:
    bool mEnable;
};

#endif // LED_H
