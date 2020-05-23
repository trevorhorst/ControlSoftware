#ifndef HARDWARE_BEAGLEBONEBLACK_H
#define HARDWARE_BEAGLEBONEBLACK_H

#include <string.h>
#include <stdio.h>
#include <thread>

#include "common/drivers/led.h"
#include "common/hardware/hardware_base.h"
#include "common/singleton.h"
#include "common/system/system.h"
#include "common/timer.h"

#include "common/command/command_help.h"
#include "common/command/command_datetime.h"
#include "common/command/command_gpio.h"
#include "common/command/command_led.h"
#include "common/command/command_system.h"
#include "common/command/command_heartbeat.h"
#include "common/command/command_venus638flpx.h"

#include "common/drivers/am335x/control_module.h"
#include "common/drivers/am335x/clock_module.h"
#include "common/drivers/am335x/gpio.h"
#include "common/drivers/devices/displays/ssd1306.h"
#include "common/drivers/devices/gps/venus638flpx.h"
#include "common/drivers/serial.h"
#include "common/drivers/i2c.h"

#include "http/command.h"
#include "http/server/server.h"
#include "http/client.h"

#include "hardware/resources/resources.h"

#define NUM_GPIO_HEADERS 4
#define LED_HEADERS 4
#define NUM_I2C_BUSES 1

class Hardware
        : public HardwareBase
        , public Singleton< Hardware >
{
    friend class Singleton< Hardware >;
    static const char *str_gps_device;
    static const char *str_dev_i2c0;
    static const uint8_t ssd1306_address;
public:
    Transport::Client * getClient() override;

private:
    Hardware();
    ~Hardware();

    const char *mIndexHtml;
    const char *mBundleJs;


    Serial mGpsSerial;
    I2C mI2C[ NUM_I2C_BUSES ];
    SSD1306 mDisplay;
    Gps::Venus638FLPx mGps;
    AM335X::ControlModule mControlModule;
    AM335X::ClockModule mClockModule;
    AM335X::Gpio mGpio[ NUM_GPIO_HEADERS ];
    Led mLed[ LED_HEADERS ];
    DateTime mDateTime;
    Http::Server mServer;
    Http::Client mClient;
    System mSystem;
    Timer mHeartbeatTimer;

    CommandHelp mCmdHelp;
    CommandDateTime mCmdDateTime;
    CommandGpio mCmdGpio;
    CommandHeartbeat mCmdHeartbeat;
    CommandLed mCmdLed;
    CommandServer mCmdServer;
    CommandSystem mCmdSystem;
    CommandVenus638FLPx mCmdGps;

    void heartbeat();
};

#endif // HARDWARE_BEAGLEBONEBLACK_H
