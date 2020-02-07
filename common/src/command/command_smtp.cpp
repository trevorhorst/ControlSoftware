#include "common/command/command_smtp.h"

CommandSmtp::CommandSmtp()
    : CommandTemplate< Smtp::Client >( COMMAND_SMTP, COMMAND_QSMTP )
{
    mMutatorMap[ PARAM_USERNAME ] = PARAMETER_CALLBACK( &CommandSmtp::setUsername );
    mMutatorMap[ PARAM_PASSWORD ] = PARAMETER_CALLBACK( &CommandSmtp::setPassword );
    mMutatorMap[ PARAM_SEND ]     = PARAMETER_CALLBACK( &CommandSmtp::setSend );

    mAccessorMap[ PARAM_SERVER ]  = PARAMETER_CALLBACK( &CommandSmtp::getServer );
}

uint32_t CommandSmtp::getServer( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddStringToObject( response, PARAM_SERVER, mControlObject->getServer().c_str() );
    return r;
}

uint32_t CommandSmtp::setUsername( cJSON *val )
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsString( val ) ) {
        r = mControlObject->setUsername( val->valuestring );
    } else {
        r = Error::Code::SYNTAX;
    }
    return r;
}

uint32_t CommandSmtp::setPassword( cJSON *val )
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsString( val ) ) {
        r = mControlObject->setPassword( val->valuestring );
    } else {
        r = Error::Code::SYNTAX;
    }
    return r;
}

uint32_t CommandSmtp::setSend( cJSON *val )
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsString( val ) ) {
        r = mControlObject->send( val->valuestring );
    } else {
        r = Error::Code::SYNTAX;
    }
    return r;
}
