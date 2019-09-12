#include "common/command/command_venus638flpx.h"

CommandVenus638FLPx::CommandVenus638FLPx()
    : CommandTemplate< Gps::Venus638FLPx > ( COMMAND_GPS, COMMAND_QGPS )
{
    mMutatorMap[ PARAM_BAUD ] = PARAMETER_CALLBACK( &CommandVenus638FLPx::setBaud );

    mAccessorMap[ PARAM_BAUD ] = PARAMETER_CALLBACK( &CommandVenus638FLPx::getBaud );
    mAccessorMap[ PARAM_GPGGA ] = PARAMETER_CALLBACK( &CommandVenus638FLPx::getGpgga );
    mAccessorMap[ PARAM_GPGSA ] = PARAMETER_CALLBACK( &CommandVenus638FLPx::getGpgsa );
    mAccessorMap[ PARAM_GPRMC ] = PARAMETER_CALLBACK( &CommandVenus638FLPx::getGprmc );
    mAccessorMap[ PARAM_GPVTG ] = PARAMETER_CALLBACK( &CommandVenus638FLPx::getGpvtg );
}

uint32_t CommandVenus638FLPx::setBaud( cJSON *val )
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsNumber( val ) ) {
        mControlObject->setBaud( static_cast< Serial::Speed>( val->valueint ) );
    } else {
        r = Error::Code::SYNTAX;
    }
    return r;
}

uint32_t CommandVenus638FLPx::getBaud( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddNumberToObject( response, PARAM_BAUD, mControlObject->getBaudRate() );
    return r;
}

uint32_t CommandVenus638FLPx::getGpgga( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    uint8_t buffer[ READ_BUFFER_SIZE ];
    mControlObject->getSentence( Gps::Nmea::Sentence::GPGGA, buffer, READ_BUFFER_SIZE );
    cJSON_AddStringToObject( response, PARAM_GPGGA, reinterpret_cast< char * >( buffer ) );
    return r;
}

uint32_t CommandVenus638FLPx::getGpgsa( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    uint8_t buffer[ READ_BUFFER_SIZE ];
    mControlObject->getSentence( Gps::Nmea::Sentence::GPGSA, buffer, READ_BUFFER_SIZE );
    cJSON_AddStringToObject( response, PARAM_GPGSA, reinterpret_cast< char * >( buffer ) );
    return r;
}

uint32_t CommandVenus638FLPx::getGprmc( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    uint8_t buffer[ READ_BUFFER_SIZE ];
    mControlObject->getSentence( Gps::Nmea::Sentence::GPRMC, buffer, READ_BUFFER_SIZE );
    cJSON_AddStringToObject( response, PARAM_GPRMC, reinterpret_cast< char * >( buffer ) );
    return r;
}

uint32_t CommandVenus638FLPx::getGpvtg( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    uint8_t buffer[ READ_BUFFER_SIZE ];
    mControlObject->getSentence( Gps::Nmea::Sentence::GPVTG, buffer, READ_BUFFER_SIZE );
    cJSON_AddStringToObject( response, PARAM_GPVTG, reinterpret_cast< char * >( buffer ) );
    return r;
}
