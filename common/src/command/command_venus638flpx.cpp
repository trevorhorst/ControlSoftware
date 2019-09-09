#include "common/command/command_venus638flpx.h"

CommandVenus638FLPx::CommandVenus638FLPx()
    : CommandTemplate<Venus638FLPx> ( COMMAND_GPS, COMMAND_QGPS )
{
    mMutatorMap[ PARAM_BAUD ] = PARAMETER_CALLBACK( &CommandVenus638FLPx::setBaud );

    mAccessorMap[ PARAM_BAUD ] = PARAMETER_CALLBACK( &CommandVenus638FLPx::getBaud );
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
