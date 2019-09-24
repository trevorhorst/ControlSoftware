#ifndef COMMAND_VENUS638FLPX_H
#define COMMAND_VENUS638FLPX_H

#include "common/command/command_template.h"
#include "common/drivers/devices/gps/venus638flpx.h"

#define COMMAND_GPS     "gps"
#define COMMAND_QGPS    "qgps"

#define PARAM_BAUD      "baud"
#define PARAM_GPGGA     "gpgga"
#define PARAM_GPGSA     "gpgsa"
#define PARAM_GPRMC     "gprmc"
#define PARAM_GPVTG     "gpvtg"

class CommandVenus638FLPx
        : public CommandTemplate< Gps::Venus638FLPx >
{
public:
    CommandVenus638FLPx();

    virtual uint32_t setBaud( cJSON *val );

    virtual uint32_t getBaud( cJSON *response );
    virtual uint32_t getGpgga( cJSON *response );
    virtual uint32_t getGpgsa( cJSON *response );
    virtual uint32_t getGprmc( cJSON *response );
    virtual uint32_t getGpvtg( cJSON *response );
};

#endif // COMMAND_VENUS638FLPX_H
