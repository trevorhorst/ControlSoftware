#ifndef HARDWARE_EXAMPLE_H
#define HARDWARE_EXAMPLE_H

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
#include "common/drivers/devices/gps/venus638flpx.h"
#include "common/drivers/serial.h"

#include "smtp/client.h"
#include "smtp/command.h"

#include "http/command.h"
#include "http/client.h"

#include "hardware/resources/resources.h"

class Hardware
        : public HardwareBase
        , public Singleton< Hardware >
{
    friend class Singleton< Hardware >;

    static const char *smtp_gmail_server;
    static const uint32_t heartbeat_delay_1000_ms;

public:
    Transport::Client *getClient() override;

private:
    Hardware();
    ~Hardware() override;

    const char *mIndexHtml;
    const char *mBundleJs;

    DateTime mDateTime;
    NewHttp::Server mServer;
    System mSystem;
    Timer mHeartbeatTimer;
    Smtp::Client mSmtpClient;
    NewHttp::Client mHttpClient;

    CommandHelp mCmdHelp;
    CommandDateTime mCmdDateTime;
    CommandHeartbeat mCmdHeartbeat;
    CommandServer mCmdServer;
    CommandSystem mCmdSystem;
    CommandSmtp mCmdSmtp;

    void heartbeat();
};

#endif // HARDWARE_EXAMPLE_H
