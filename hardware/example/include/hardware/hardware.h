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

#include "common/command/command_datetime.h"
#include "common/command/command_gpio.h"
#include "common/command/command_led.h"
#include "common/command/command_server.h"
#include "common/command/command_system.h"
#include "common/command/command_heartbeat.h"
#include "common/command/command_venus638flpx.h"

#include "common/drivers/am335x/control_module.h"
#include "common/drivers/am335x/clock_module.h"
#include "common/drivers/am335x/gpio.h"
#include "common/drivers/devices/gps/venus638flpx.h"
#include "common/drivers/serial.h"

#include "hardware/resources/resources.h"

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

    DateTime mDateTime;
    Http::Server mServer;
    System mSystem;
    Timer mHeartbeatTimer;

    CommandDateTime mCmdDateTime;
    CommandHeartbeat mCmdHeartbeat;
    CommandServer mCmdServer;
    CommandSystem mCmdSystem;

    void heartbeat();
};

#endif // HARDWARE_BEAGLEBONEBLACK_H
