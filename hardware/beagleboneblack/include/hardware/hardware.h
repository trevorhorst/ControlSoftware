#ifndef HARDWARE_BEAGLEBONEBLACK_H
#define HARDWARE_BEAGLEBONEBLACK_H

#include <string.h>
#include <stdio.h>

#include "common/drivers/led.h"
#include "common/hardware/hardware_base.h"
#include "common/singleton.h"
#include "common/system/system.h"

#include "common/command/command_datetime.h"
#include "common/command/command_gpio.h"
#include "common/command/command_led.h"
#include "common/command/command_server.h"
#include "common/command/command_system.h"

#include "common/drivers/am335x/clock_module.h"
#include "common/drivers/am335x/gpio.h"

#include "hardware/resources/resources.h"

#define LED_HEADERS 4

class Hardware
        : public HardwareBase
        , public Singleton< Hardware >
{
    friend class Singleton< Hardware >;
public:
private:
    Hardware();
    ~Hardware();

    const char *mIndexHtml;
    const char *mBundleJs;

    AM335X::ClockModule mClockModule;
    AM335X::Gpio mGpio[ 4 ];
    DateTime mDateTime;
    Led mLed[ LED_HEADERS ];
    Http::Server mServer;
    System mSystem;

    CommandDateTime mCmdDateTime;
    CommandGpio mCmdGpio;
    CommandLed mCmdLed;
    CommandServer mCmdServer;
    CommandSystem mCmdSystem;


};

#endif // HARDWARE_BEAGLEBONEBLACK_H
