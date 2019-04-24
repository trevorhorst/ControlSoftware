#ifndef LED_H
#define LED_H

#include "common/control/control_template.h"

class Led
        : public ControlTemplate< Led >
{
public:
    Led();
};

#endif // LED_H
