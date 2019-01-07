#include "common/command/command_datetime.h"

CommandDateTime::CommandDateTime()
    : CommandTemplate< DateTime >( COMMAND_DATETIME, COMMAND_QDATETIME )
{
    mAccessorMap[ PARAM_DATETIME ]
            = PARAMETER_CALLBACK( &CommandDateTime::getDateTime );
};

uint32_t CommandDateTime::getDateTime( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddStringToObject( response, PARAM_DATETIME
                             , mControlObject->getDateTime() );
    return r;
}

