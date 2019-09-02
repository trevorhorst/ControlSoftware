#ifndef HARDWARE_BEAGLEBONEBLACK_H
#define HARDWARE_BEAGLEBONEBLACK_H

#include <string.h>
#include <stdio.h>
#include <thread>

#include "common/drivers/led.h"
#include "common/hardware/hardware_base.h"
#include "common/singleton.h"
#include "common/system/system.h"

#include "common/command/command_datetime.h"
#include "common/command/command_gpio.h"
#include "common/command/command_led.h"
#include "common/command/command_server.h"
#include "common/command/command_system.h"

#include "common/drivers/am335x/control_module.h"
#include "common/drivers/am335x/clock_module.h"
#include "common/drivers/am335x/gpio.h"
#include "common/drivers/devices/gps/venus638flpx.h"
#include "common/drivers/serial.h"

#include "hardware/resources/resources.h"

#define NUM_GPIO_HEADERS 4
#define LED_HEADERS 4

class Hardware
        : public HardwareBase
        , public Singleton< Hardware >
{
    friend class Singleton< Hardware >;
    static const char *str_gps_device;
public:
private:
    Hardware();
    ~Hardware();

    const char *mIndexHtml;
    const char *mBundleJs;


    Serial mGpsSerial;
    Venus638FLPx mGps;
    AM335X::ControlModule mControlModule;
    AM335X::ClockModule mClockModule;
    AM335X::Gpio mGpio[ NUM_GPIO_HEADERS ];
    DateTime mDateTime;
    Led mLed[ LED_HEADERS ];
    Http::Server mServer;
    System mSystem;

    CommandDateTime mCmdDateTime;
    CommandGpio mCmdGpio;
    CommandLed mCmdLed;
    CommandServer mCmdServer;
    CommandSystem mCmdSystem;

    std::thread *mSerialMonitor;


};

#endif // HARDWARE_BEAGLEBONEBLACK_H
