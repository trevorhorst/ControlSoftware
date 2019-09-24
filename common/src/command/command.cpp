#include "common/command/command.h"

#define GENERIC_USAGE_TEXT \
               "Usage:   [cmd] [param] [val]\n" \
               "         cmd: Available commands\n" \
               "            mutator: %s\n" \
               "            accessor: %s\n" \
               "         param: Parameter to be changed (see ICD)\n" \
               "         val:   New value to set parameter (see ICD)\n"

const char *Command::error_control_unavailable = "Control is unavailable";

/**
 * @brief Command Constructor
 * @param name Command name
 */
Command::Command( const char *mutator, const char *accessor )
    : mAccessible( false )
    , mMutable( false )
{
    // Add an accessor
    if( accessor == nullptr && accessor[ 0 ] == '\0' ) {
        strncpy( mAccessor, "\0", 1 );
        mAccessible = false;
    } else {
        strncpy( mAccessor, accessor, sizeof( mAccessor ) );
        mAccessor[ COMMAND_NAME_MAX_SIZE - 1 ] = '\0';
        mAccessible = true;
    }

    // Add a mutator
    if( mutator == nullptr && mutator[ 0 ] == '\0' ) {
        strncpy( mMutator, "\0", 1 );
        mMutable = false;
    } else {
        strncpy( mMutator, mutator, sizeof( mMutator ) );
        mMutator[ COMMAND_NAME_MAX_SIZE - 1 ] = '\0';
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
 * @brief Handles any parameters that are required by the command. This modifies
 * the parameter list
 * @param params List of parameters
 * @param response Response object to populate
 * @return Boolean indicating success of the operation
 */
uint32_t Command::handleRequiredParameters( cJSON *params, const char *&details )
{
    uint32_t r = Error::Code::NONE;
    const char *param = nullptr;
    cJSON *p = nullptr;

    for( auto it = mRequiredMap.begin()
         ; it != mRequiredMap.end() && r == Error::Code::NONE
         ; it++ ) {

        // Detach the required parameter
        param = it->first;
        p = cJSON_DetachItemFromObject( params, param );

        if( p == nullptr ) {
            // Parameter missing
            r = Error::Code::PARAM_MISSING;
        } else {
            // Parameter found, perform a callback
            r = it->second( p );
        }

        // Delete all the parameters we detach
        if( p ) { cJSON_Delete( p ); }
    }

    if( r != Error::Code::NONE ) {
        details = param;
    }

    // Required parameters handled successfully
    return r;
}


/**
 * @brief Handles any optional parameters. This may modify how the command is
 * handled and therefore the programmer is respoonsible for overriding this with
 * actual functionality.
 * @param params List of parameters
 * @param response Response object to populate
 * @return Boolean indicating success of the operation
 */
uint32_t Command::handleOptionalParameters( cJSON *params, cJSON *response )
{
    (void)params;
    (void)response;
    return true;
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
