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
    : mControlObject( nullptr )
    , mAccessible( false )
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

    bindMutatorCallback( PARAM_VERBOSE, &Command::setVerbose );
    bindAccessorCallback( PARAM_VERBOSE, &Command::getVerbose );
}

/**
 * @brief Command Destructor
 */
Command::~Command()
{
}

/**
 * @brief Binds a mutator callback function
 * @param paramName Parameter name associated with the callback
 */
void Command::bindMutatorCallback( const char *paramName
                                     , uint32_t (Command::*cb)( cJSON * ) )
{
    if( cb == NULL ) {
        // Function is NULL, don't add it to the list
        printf( "Callback function is invalid, ignoring bind\n" );
    } else if( paramName == NULL || paramName[ 0 ] == '\0' ) {
        printf( "Parameter is empty, ignoring bind\n" );
    } else {
        ParameterMap::const_iterator it = mMutatorMap.find( paramName );
        if( it == mMutatorMap.end() ) {
            mMutatorMap[ paramName ] = std::bind( cb, this, std::placeholders::_1 );
        } else {
            printf( "The '%s' parameter already exists, ignoring bind\n"
                    , paramName );
        }
    }
}

/**
 * @brief Binds an accessor callback function
 * @param paramName Parameter name associated with the callback
 */
void Command::bindAccessorCallback( const char *paramName
                                     , uint32_t (Command::*cb)( cJSON * ) )
{
    if( cb == NULL ) {
        // Function is NULL, don't add it to the list
        printf( "Callback function is invalid, ignoring bind\n" );
    } else if( paramName == NULL || paramName[ 0 ] == '\0' ) {
        printf( "Parameter is empty, ignoring bind\n" );
    } else {
        ParameterMap::const_iterator it = mAccessorMap.find( paramName );
        if( it == mAccessorMap.end() ) {
            mAccessorMap[ paramName ] = std::bind( cb, this, std::placeholders::_1 );
        } else {
            printf( "The '%s' parameter already exists, ignoring bind\n"
                    , paramName );
        }
    }
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

const char *Command::usage()
{
    return mUsage;
}

/**
 * @brief Access handler, retrieves status of accessible parameters
 * @param params List of parameters
 * @return Response object
 */
cJSON *Command::access( cJSON *params )
{
    // printf( "%s\n", __FUNCTION__ );
    cJSON* response = cJSON_CreateObject();
    cJSON_AddStringToObject( response, PARAM_COMMAND, getAccessorName() );

    bool ok = mControlObject;
    if( !ok ) {
        setError( Error::Code::CMD_FAILED, "Control is unavailable", response );
    } else {
        ok = handleRequiredParameters( params, response );
    }

    if( ok ) {
        uint32_t r = Error::Code::NONE;
        cJSON* result = cJSON_CreateObject();

        ParameterMap::const_iterator it = mAccessorMap.begin();
        for( it = mAccessorMap.begin()
             ; r == Error::Code::NONE && it != mAccessorMap.end()
             ; it++ ) {
            const char *param = it->first;
            ParameterCallback cb = it->second;
            r = cb( result );
            if( r != Error::Code::NONE ) {
                setError( r, param, response );
            }
        }

        cJSON_AddItemToObject( response, PARAM_RESULT, result );
    }

    return response;
}

/**
 * @brief Mutate handler, modifies status of mutable parameters
 * @param params List of parameters
 * @return Response object
 */
cJSON *Command::mutate( cJSON *params )
{
    printf( "%s\n", __FUNCTION__ );
    uint32_t r = Error::Code::NONE;
    cJSON *response = cJSON_CreateObject();

    // Add the command parameter to the response object
    cJSON_AddStringToObject( response, PARAM_COMMAND, getMutatorName() );

    bool ok = mControlObject;
    if( !ok ) {
        // The control object isn't valid
        r = Error::Code::CMD_FAILED;
        setError( r, "Control is unavailable", response );
    } else {
        // The control object is valid
        for( cJSON *param = params->child
             ; r == Error::Code::NONE && param != NULL
             ; param = param->next ) {
            ParameterMap::const_iterator it = mMutatorMap.find( param->string );
            if( it == mMutatorMap.end() ) {
                // No callback was found
                r = Error::Code::PARAM_INVALID;
            } else {
                // A callback was found
                ParameterCallback cb = it->second;
                r = cb( param );
                if( r == Error::Code::NONE ) {
                    // Parameter handled successfully
                    char *str = cJSON_Print( param );
                    printf( "'%s' set to %s\n", param->string, str );
                    free( str );
                } else {
                    // Failed to set parameter
                    setError( r, param->string, response );
                }
            }
        }
    }

    // Add the success parameter to the response object
    bool success = ( r == Error::Code::NONE ) ? true : false;
    cJSON_AddBoolToObject( response, PARAM_SUCCESS, success );

    return response;
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
 * @brief Makes an object available for control
 * @param obj Controllable object
 */
void Command::addControlObject( Control *obj )
{
    mCtrlObjList.push_back( obj );

    if( mCtrlObjList.size() == 1 ) {
        // If we have one object, set it as our default object. I don't think
        // this is the right way to do this still..
        mControlObject = mCtrlObjList.front();
    }
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

/**
 * @brief Verbose setter wrapper method
 * @param val Desired verbose value
 * @return Integer response code
 */
uint32_t Command::setVerbose( cJSON *val )
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsTrue( val ) ) {
        mControlObject->setVerbose( true );
    } else if( cJSON_IsFalse( val ) ){
        mControlObject->setVerbose( false );
    } else {
        r = Error::Code::PARAM_OUT_OF_RANGE;
    }
    return r;
}

uint32_t Command::getVerbose( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddBoolToObject( response, PARAM_VERBOSE, mControlObject->isVerbose() );
    return r;
}

void Command::setError( uint32_t code, const char *details, cJSON *response )
{
    cJSON *error = cJSON_CreateObject();
    cJSON_AddNumberToObject( error, PARAM_CODE, code );
    cJSON_AddStringToObject( error, PARAM_TYPE, Error::getCodeString( code ) );
    cJSON_AddStringToObject( error, PARAM_DETAILS, details );
    cJSON_AddItemToObject( response, PARAM_ERROR, error );
}
