#ifndef COMMAND_API_H
#define COMMAND_API_H

#include "common/command/command_template.h"

#include "hardware/command/command_api.h"

#define COMMAND_API     "api"
#define COMMAND_QAPI    "qapi"

class CommandApi
        : public CommandTemplate< Api >
{
public:
    CommandApi();
};

#endif // COMMAND_API_H
