#ifndef COMMAND_VENUS638FLPX_H
#define COMMAND_VENUS638FLPX_H

#include "common/command/command_template.h"
#include "common/drivers/devices/gps/venus638flpx.h"

#define COMMAND_GPS     "gps"
#define COMMAND_QGPS    "qgps"

#define PARAM_BAUD      "baud"

class CommandVenus638FLPx
        : public CommandTemplate< Venus638FLPx >
{
public:
    CommandVenus638FLPx();

    virtual uint32_t setBaud( cJSON *val );

    virtual uint32_t getBaud( cJSON *response );
};

#endif // COMMAND_VENUS638FLPX_H
