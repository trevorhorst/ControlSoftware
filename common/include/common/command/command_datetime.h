#ifndef COMMAND_DATETIME_H
#define COMMAND_DATETIME_H

#include "common/command/command_template.h"
#include "common/system/datetime.h"

#define COMMAND_DATETIME    "chrono"
#define COMMAND_QDATETIME   "qchrono"

#define PARAM_DATETIME      "datetime"

class CommandDateTime
        : public CommandTemplate< DateTime >
{
public:
    CommandDateTime();
    uint32_t getDateTime( cJSON *response );
};

#endif // COMMAND_DATETIME_H
