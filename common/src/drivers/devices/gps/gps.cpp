#include "common/drivers/devices/gps/gps.h"

namespace Gps
{

namespace Nmea {

const SentenceInfo sentence[] = {
    { GPBOD, str_gpbod_code}
    , { GPBWC,  str_gpbwc_code }
    , { GPGGA,  str_gpgga_code }
    , { GPGLL,  str_gpgll_code }
    , { GPGSA,  str_gpgsa_code }
    , { GPGSV,  str_gpgsv_code }
    , { GPHDT,  str_gphdt_code }
    , { GPR00,  str_gpr00_code }
    , { GPRMA,  str_gprma_code }
    , { GPRMB,  str_gprmb_code }
    , { GPRMC,  str_gprmc_code }
    , { GPRTE,  str_gprte_code }
    , { GPTRF,  str_gptrf_code }
    , { GPSTN,  str_gpstn_code }
    , { GPVBW,  str_gpvbw_code }
    , { GPVTG,  str_gpvtg_code }
    , { GPWPL,  str_gpwpl_code }
    , { GPXTE,  str_gpxte_code }
    , { GPZDA,  str_gpzda_code }
};

const char *str_gpbod_code = "$GPBOD";
const char *str_gpbwc_code = "$GPBWC";
const char *str_gpgga_code = "$GPGGA";
const char *str_gpgll_code = "$GPGLL";
const char *str_gpgsa_code = "$GPGSA";
const char *str_gpgsv_code = "$GPGSV";
const char *str_gphdt_code = "$GPHDT";
const char *str_gpr00_code = "$GPR00";
const char *str_gprma_code = "$GPRMA";
const char *str_gprmb_code = "$GPRMB";
const char *str_gprmc_code = "$GPRMC";
const char *str_gprte_code = "$GPRTE";
const char *str_gptrf_code = "$GPTRF";
const char *str_gpstn_code = "$GPSTN";
const char *str_gpvbw_code = "$GPVBW";
const char *str_gpvtg_code = "$GPVTG";
const char *str_gpwpl_code = "$GPWPL";
const char *str_gpxte_code = "$GPXTE";
const char *str_gpzda_code = "$GPZDA";

}

}
