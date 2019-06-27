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
     * @brief Call the command
     * @param params List of parameters
     * @param response
     * @param type
     * @return
     */
    bool call( cJSON *params, cJSON *response, Type type )
    {
        bool success = false;
        const char *command = nullptr;

        /// @todo Required parameter handling goes here. They should be handled
        /// the same for both access and mutate

        switch( type ) {
        case Type::ACCESSOR:
            command = getAccessorName();
            success = access( params, response );
            break;
        case Type::MUTATOR:
            command = getMutatorName();
            success = mutate( params, response );
            break;
        }

        // Add the command parameter to the response object
        cJSON_AddStringToObject( response, PARAM_COMMAND, command );

        return success;
    }

    /**
     * @brief Access handler, retrieves status of accessible parameters
     * @param params List of parameters
     * @return Response object
     */
    bool access( cJSON *params, cJSON *response )
    {
        uint32_t r = Error::Code::NONE;
        const char *details = nullptr;

        if( r == Error::Code::NONE ) {
            if( mRequiredMap.size() > 0 ) {
                if( params == nullptr ) {
                    // The params object is invalid
                    r = Error::Code::PARAM_MISSING;
                    details = PARAM_PARAMS;
                } else {
                    // Only handle the required parameters if they exist? Not sure
                    // what the best way to handle this is.
                    r = handleRequiredParameters( params, details );
                }
            }
        }

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

        bool success = false;
        if( r == Error::Code::NONE ) {
            // Add the result to the response object
            cJSON_AddItemToObject( response, PARAM_RESULT, result );
            success = true;
        } else {
            // There was an error and our result is invalid, free it
            setError( r, details, response );
            cJSON_free( result );
        }

        return success;
    }

    /**
     * @brief Mutate handler, modifies status of mutable parameters
     * @param params List of parameters
     * @return Response object
     */
    bool mutate( cJSON *params, cJSON *response )
    {
        uint32_t r = Error::Code::NONE;
        const char *details = nullptr;

        if( r == Error::Code::NONE ) {
            if( mRequiredMap.size() > 0 ) {
                // Required parameters are expected
                if( params == nullptr ) {
                    // The params object is invalid
                    r = Error::Code::PARAM_MISSING;
                    details = PARAM_PARAMS;
                } else {
                    // Only handle the required parameters if they exist? Not sure
                    // what the best way to handle this is.
                    r = handleRequiredParameters( params, details );
                }
            }
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

        return success;
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
