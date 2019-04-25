#ifndef COMMAND_TEMPLATE_H
#define COMMAND_TEMPLATE_H

#include "common/command/command.h"

template< typename T >
class CommandTemplate
        : public Command
{
public:

    /**
     * @brief Constructor
     * @param mutator Mutator name
     * @param accessor Accessor name
     */
    CommandTemplate( const char *mutator, const char *accessor )
        : Command( mutator, accessor )
        , mControlObject( nullptr )
    {
        mMutatorMap[ PARAM_VERBOSE ] = PARAMETER_CALLBACK(
                    &CommandTemplate< T >::setVerbose );

        mAccessorMap[ PARAM_VERBOSE ] = PARAMETER_CALLBACK(
                    &CommandTemplate< T >::getVerbose );

        if( T::getCount() == 1 ) {
            // If there is only a count of 1, just set it as the control object
            mControlObject = T::getControlObject( 0 );
        }
    }

    /**
     * @brief Access handler, retrieves status of accessible parameters
     * @param params List of parameters
     * @return Response object
     */
    cJSON *access( cJSON *params )
    {
        uint32_t r = Error::Code::NONE;
        const char *details = nullptr;
        cJSON* response = cJSON_CreateObject();

        // Add the command parameter to the response object
        cJSON_AddStringToObject( response, PARAM_COMMAND, getAccessorName() );

        r = handleRequiredParameters( params, details );

        if( r == Error::Code::NONE ) {
            if( mControlObject == nullptr ) {
                // The control object is invalid
                r = Error::Code::CMD_FAILED;
                details = error_control_unavailable;
            }
        }

        cJSON* result = cJSON_CreateObject();
        if( r == Error::Code::NONE ) {
            for( auto it = mAccessorMap.begin()
                 ; it != mAccessorMap.end() && r == Error::Code::NONE
                 ; it++ ) {

                ParameterCallback cb = it->second;
                r = cb( result );

                if( r != Error::Code::NONE ) {
                    // Failed to access parameter
                    details = it->first;
                }
            }
        }

        if( r == Error::Code::NONE ) {
            // Add the result to the response object
            cJSON_AddItemToObject( response, PARAM_RESULT, result );
        } else {
            // There was an error and our result is invalid, free it
            cJSON_free( result );
        }

        bool success = false;
        if( r == Error::Code::NONE ) {
            success = true;
        } else {
            // Only set an error if there is an error
            setError( r, details, response );
        }

        // Add the success parameter to the response object
        cJSON_AddBoolToObject( response, PARAM_SUCCESS, success );

        return response;
    }

    /**
     * @brief Mutate handler, modifies status of mutable parameters
     * @param params List of parameters
     * @return Response object
     */
    cJSON *mutate( cJSON *params )
    {
        uint32_t r = Error::Code::NONE;
        const char *details = nullptr;
        cJSON *response = cJSON_CreateObject();

        // Add the command parameter to the response object
        cJSON_AddStringToObject( response, PARAM_COMMAND, getMutatorName() );

        if( params == nullptr
                   || cJSON_IsInvalid( params )
                   || cJSON_IsNull( params ) ) {
            // The params object is invalid
            r = Error::Code::PARAM_MISSING;
            details = PARAM_PARAMS;
        }


        if( r == Error::Code::NONE ) {
            r = handleRequiredParameters( params, details );
        }

        if( r == Error::Code::NONE ) {
            if( mControlObject == nullptr ) {
                // The control object isn't valid
                r = Error::Code::CMD_FAILED;
                details = error_control_unavailable;
            }
        }

        if( r == Error::Code::NONE ) {
            // The control object is valid and we have parameters to work with
            for( cJSON *param = params->child
                 ; r == Error::Code::NONE && param != nullptr
                 ; param = param->next ) {
                auto it = mMutatorMap.find( param->string );
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
                        // Failed to mutate parameter
                        details = param->string;
                    }
                }
            }
        }

        bool success = false;
        if( r == Error::Code::NONE ) {
            success = true;
        } else {
            // Only set an error if there is an error
            setError( r, details, response );
        }

        // Add the success parameter to the response object
        cJSON_AddBoolToObject( response, PARAM_SUCCESS, success );

        return response;
    }

    /**
     * @brief Sets the verbosity of the control object
     * @param val Desired value
     * @return Error code
     */
    virtual uint32_t setVerbose( cJSON *val )
    {
        uint32_t r = Error::Code::NONE;
        if( cJSON_IsTrue( val ) ) {
            mControlObject->setVerbose( true );
        } else if( cJSON_IsFalse( val ) ){
            mControlObject->setVerbose( false );
        } else {
            // The type isn't what we expect, syntax error
            r = Error::Code::SYNTAX;
        }
        return r;
    }

    /**
     * @brief Retrieves the verbosity of the control object
     * @param response Retrived value
     * @return Error code
     */
    virtual uint32_t getVerbose( cJSON *response )
    {
        uint32_t r = Error::Code::NONE;
        cJSON_AddBoolToObject( response, PARAM_VERBOSE, mControlObject->isVerbose() );
        return r;
    }

protected:
    T *mControlObject;
};

#endif // COMMAND_TEMPLATE_H
