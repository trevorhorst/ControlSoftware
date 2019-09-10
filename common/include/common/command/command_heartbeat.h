#ifndef COMMAND_HEARTBEAT_H
#define COMMAND_HEARTBEAT_H

#include "common/command/command_template.h"
#include "common/timer.h"

#define COMMAND_HEARTBEAT   "heartbeat"
#define COMMAND_QHEARTBEAT  "qheartbeat"

class CommandHeartbeat
        : public CommandTemplate< Timer >
{
public:
    CommandHeartbeat();

    virtual uint32_t setEnable( cJSON *val );

    virtual uint32_t getEnable( cJSON *response );
};

#endif // COMMAND_HEARTBEAT_H
