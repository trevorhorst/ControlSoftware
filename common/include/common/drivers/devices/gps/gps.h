#ifndef GPS_H
#define GPS_H

#include <stdint.h>

/**
 * @brief GPS namespace - important declarations for GPS devices
 */
namespace Gps
{

/**
 * @brief NMEA sub namespace
 */
namespace Nmea {

enum Sentence {
    GPBOD   // Bearing, origin to destination
    , GPBWC // Bearing and distance to waypoint, great circle
    , GPGGA // Global Positioning System Fix Data
    , GPGLL // Geographic position, latitude / longitude
    , GPGSA // GPS DOP and active satellites
    , GPGSV // GPS Satellites in view
    , GPHDT // Heading, True
    , GPR00 // List of waypoints in currently active route
    , GPRMA // Recommended minimum specific Loran-C data
    , GPRMB // Recommended minimum navigation info
    , GPRMC // Recommended minimum specific GPS/Transit data
    , GPRTE // Routes
    , GPTRF // Transit Fix Data
    , GPSTN // Multiple Data ID
    , GPVBW // Dual Ground / Water Speed
    , GPVTG // Track made good and ground speed
    , GPWPL // Waypoint location
    , GPXTE // Cross-track error, Measured
    , GPZDA // Date & Time
};

struct SentenceInfo {
    uint32_t code;
    const char *name;
};

extern const SentenceInfo sentence[];

extern const char *str_gpbod_code;
extern const char *str_gpbwc_code;
extern const char *str_gpgga_code;
extern const char *str_gpgll_code;
extern const char *str_gpgsa_code;
extern const char *str_gpgsv_code;
extern const char *str_gphdt_code;
extern const char *str_gpr00_code;
extern const char *str_gprma_code;
extern const char *str_gprmb_code;
extern const char *str_gprmc_code;
extern const char *str_gprte_code;
extern const char *str_gptrf_code;
extern const char *str_gpstn_code;
extern const char *str_gpvbw_code;
extern const char *str_gpvtg_code;
extern const char *str_gpwpl_code;
extern const char *str_gpxte_code;
extern const char *str_gpzda_code;

}

}

#endif // GPS_H
