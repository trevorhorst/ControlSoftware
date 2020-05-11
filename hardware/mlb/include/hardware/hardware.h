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
#include "common/drivers/devices/gps/venus638flpx.h"
#include "common/drivers/serial.h"

#include "smtp/client.h"
#include "smtp/command.h"

#include "http/client.h"
#include "http/server/server.h"
#include "http/command.h"

#include "hardware/resources/resources.h"

class Hardware
        : public HardwareBase
        , public Singleton< Hardware >
{
    friend class Singleton< Hardware >;

    static const uint32_t heartbeat_delay_1000_ms;

    static const char *mlb_api_host;
    static const char *mlb_api_path;
    static const char *endpoint_transaction_all;
    static const char *endpoint_query_results;
    static const char *endpoint_total_size;
    static const char *endpoint_wsfb_news_injury;

    static const char *mlb_api;
    static const char *mlb_api_date;
    static const char *smtp_gmail_server;
    static const char *email_list[256];

public:
    Transport::Client * getClient() override;

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
    NewHttp::Client mMlbApiClient;
    NewHttp::Client mHttpClient;

    CommandHelp mCmdHelp;
    CommandDateTime mCmdDateTime;
    CommandHeartbeat mCmdHeartbeat;
    CommandServer mCmdServer;
    CommandSystem mCmdSystem;
    CommandSmtp mCmdSmtp;

    uint32_t getDate( char *buffer, size_t size );

    uint32_t queryTransactions( const char *startDate,  const char *stopDate );

    uint32_t testApi();

    void heartbeat();
};

#endif // HARDWARE_BEAGLEBONEBLACK_H
