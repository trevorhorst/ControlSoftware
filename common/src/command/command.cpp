#include "common/command/command.h"

#define GENERIC_USAGE_TEXT \
               "Usage:   [cmd] [param] [val]\n" \
               "         cmd: Available commands\n" \
               "            mutator: %s\n" \
               "            accessor: %s\n" \
               "         param: Parameter to be changed (see ICD)\n" \
               "         val:   New value to set parameter (see ICD)\n"

/**
 * @brief Command Constructor
 * @param name Command name
 */
Command::Command( const char *mutator, const char *accessor )
    : mAccessible( false )
    , mMutable( false )
{
    // Add an accessor
    if( accessor == nullptr || accessor[ 0 ] == '\0' ) {
        strncpy( mAccessor, "\0", 1 );
    } else {
        strncpy( mAccessor, accessor, sizeof( mAccessor ) );
        mAccessible = true;
    }

    // Add a mutator
    if( mutator == nullptr || mutator[ 0 ] == '\0' ) {
        strncpy( mMutator, "\0", 1 );
    } else {
        strncpy( mMutator, mutator, sizeof( mMutator ) );
        mMutable = true;
    }

    // Create the usage text
    snprintf( mUsage, sizeof( mUsage ), GENERIC_USAGE_TEXT
              , getMutatorName(), getAccessorName() );
}

/**
 * @brief Command Destructor
 */
Command::~Command()
{
}

/**
 * @brief Handles any parameters that are required by the command
 * @param params List of parameters
 * @param response Response object to fill out
 * @return Boolean indicating success of the operation
 */
bool Command::handleRequiredParameters( cJSON*params, cJSON *response )
{
    // printf( "%s\n", __FUNCTION__ );
    uint32_t r = Error::Code::NONE;
    cJSON *p = nullptr;

    for( ParameterMap::const_iterator it = mRequiredMap.begin()
         ; it != mRequiredMap.end() && r == Error::Code::NONE; it++ ) {

        // Detach the required parameter
        const char *param = it->first;
        p = cJSON_DetachItemFromObject( params, param );

        if( p == nullptr ) {
            // Parameter missing
            r = Error::Code::PARAM_MISSING;
        } else {
            // Parameter found
            r = it->second( p );
        }

        if( r != Error::Code::NONE ) {
            // Need to handle the error before we delete the parameter
            setError( r, param, response );
        }

        // Delete all the parameters we detach
        if( p ) { cJSON_Delete( p ); }
    }

    // Required parameters handled successfully
    return ( r == Error::Code::NONE );
}

/**
 * @brief Retrieves the usage text
 * @return Char array of the usage text
 */
const char *Command::usage()
{
    return mUsage;
}

/**
 * @brief Retrieves the name of the accessor
 * @return Accessor name
 */
const char *Command::getAccessorName()
{
    return mAccessor;
}

/**
 * @brief Retrieves the name of the mutator
 * @return Mutator name
 */
const char *Command::getMutatorName()
{
    return mMutator;
}

/**
 * @brief Retrieves accessible status of a command
 * @return Boolean indicating accessibility
 */
bool Command::isAccessible()
{
    return mAccessible;
}

/**
 * @brief Retrieves mutable status a command
 * @return Boolean indicating mutability
 */
bool Command::isMutable()
{
    return mMutable;
}

void Command::setError( uint32_t code, const char *details, cJSON *response )
{
    cJSON *error = cJSON_CreateObject();
    cJSON_AddNumberToObject( error, PARAM_CODE, code );
    cJSON_AddStringToObject( error, PARAM_TYPE, Error::getCodeString( code ) );
    cJSON_AddStringToObject( error, PARAM_DETAILS, details );
    cJSON_AddItemToObject( response, PARAM_ERROR, error );
}
