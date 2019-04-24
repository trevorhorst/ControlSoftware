#ifndef HARDWARE_BEAGLEBONEBLACK_H
#define HARDWARE_BEAGLEBONEBLACK_H

#include <string.h>
#include <stdio.h>

#include "common/drivers/led.h"
#include "common/hardware/hardware.h"
#include "common/singleton.h"
#include "common/system/system.h"

#include "common/command/command_datetime.h"
#include "common/command/command_led.h"
#include "common/command/command_server.h"
#include "common/command/command_system.h"

#include "beagleboneblack/resources/resources.h"

#define LED_HEADERS 4

class BeagleboneBlack
        : public Hardware
        , public Singleton< BeagleboneBlack >
{
    friend class Singleton< BeagleboneBlack >;
public:
private:
    BeagleboneBlack();
    ~BeagleboneBlack();

    const char *mIndexHtml;
    const char *mMainJs;

    DateTime mDateTime;
    Led mLed[ LED_HEADERS ];
    Http::Server mServer;
    System mSystem;

    CommandDateTime mCmdDateTime;
    CommandLed mCmdLed;
    CommandServer mCmdServer;
    CommandSystem mCmdSystem;

};

#endif // HARDWARE_BEAGLEBONEBLACK_H
