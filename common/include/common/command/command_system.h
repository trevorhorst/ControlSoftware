#ifndef COMMAND_SYSTEM_H
#define COMMAND_SYSTEM_H

#include "common/command/command_template.h"
#include "common/system/system.h"

#define COMMAND_SYSTEM  "system"
#define COMMAND_QSYSTEM "qsystem"

#define PARAM_HOSTNAME  "hostname"
#define PARAM_INTERFACE "interface"

class CommandSystem
        : public CommandTemplate< System >
{
public:
    CommandSystem();
    uint32_t getHostname( cJSON *response );
    uint32_t getInterfaces( cJSON *response );

    uint32_t setHostname( cJSON *val );
};

#endif // COMMAND_SYSTEM_H
