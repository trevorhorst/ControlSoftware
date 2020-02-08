#include "common/command/command_smtp.h"

CommandSmtp::CommandSmtp()
    : CommandTemplate< Smtp::Client >( COMMAND_SMTP, COMMAND_QSMTP )
{
    mMutatorMap[ PARAM_USERNAME ] = PARAMETER_CALLBACK( &CommandSmtp::setUsername );
    mMutatorMap[ PARAM_PASSWORD ] = PARAMETER_CALLBACK( &CommandSmtp::setPassword );
    mMutatorMap[ PARAM_SEND ]     = PARAMETER_CALLBACK( &CommandSmtp::setSend );
    mMutatorMap[ PARAM_SERVER ]   = PARAMETER_CALLBACK( &CommandSmtp::setServer );
    mMutatorMap[ PARAM_TO ]         = PARAMETER_CALLBACK( &CommandSmtp::setTo );
    mMutatorMap[ PARAM_CC ]         = PARAMETER_CALLBACK( &CommandSmtp::setCc );

    mAccessorMap[ PARAM_SERVER ]  = PARAMETER_CALLBACK( &CommandSmtp::getServer );
    mAccessorMap[ PARAM_RECIPIENTS ] = PARAMETER_CALLBACK( &CommandSmtp::getRecipients );
}

uint32_t CommandSmtp::getServer( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddStringToObject( response, PARAM_SERVER, mControlObject->getServer().c_str() );
    return r;
}

uint32_t CommandSmtp::getRecipients( cJSON *response )
{
    uint32_t error = Error::Code::NONE;
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject( response, PARAM_RECIPIENTS, array );
    const curl_slist *recipients = mControlObject->getRecipientsList();
    for( const curl_slist *it = recipients; it != nullptr; it = it->next ) {
        cJSON *str = cJSON_CreateString( it->data );
        cJSON_AddItemToArray( array, str );
    }
    return error;
}

uint32_t CommandSmtp::setTo( cJSON *val )
{
    uint32_t error = Error::Code::NONE;
    if( cJSON_IsString( val ) ) {
        error = mControlObject->addTo( val->valuestring );
    } else {
        error = Error::Code::SYNTAX;
    }
    return error;
}

uint32_t CommandSmtp::setCc( cJSON *val )
{
    uint32_t error = Error::Code::NONE;
    if( cJSON_IsString( val ) ) {
        error = mControlObject->addCarbonCopy( val->valuestring );
    } else {
        error = Error::Code::SYNTAX;
    }
    return error;
}

uint32_t CommandSmtp::setServer( cJSON *val )
{
    uint32_t error = Error::Code::NONE;
    if( cJSON_IsString( val ) ) {
        error = mControlObject->setServer( val->valuestring );
    } else {
        error = Error::Code::SYNTAX;
    }
    return error;
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
