#ifndef HARDWARE_BEAGLEBONEBLACK_H
#define HARDWARE_BEAGLEBONEBLACK_H

// #include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "beagleboneblack/resources/resources.h"

#include "common/singleton.h"
#include "common/hardware/hardware.h"
#include "common/system/system.h"

#include "common/command/command_system.h"
#include "common/command/command_datetime.h"
#include "common/command/command_server.h"

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

    System mSystem;
    DateTime mDateTime;
    HttpServer mServer;

    CommandSystem mCmdSystem;
    CommandDateTime mCmdDateTime;
    CommandServer mCmdServer;
};

#endif // HARDWARE_BEAGLEBONEBLACK_H
