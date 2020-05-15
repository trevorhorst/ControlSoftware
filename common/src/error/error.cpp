#include "common/error/error.h"

const char* Error::mCodeString[] = {
    "No Error"
    , "Syntax Error"
    , "Command Invalid"
    , "Command Missing"
    , "Command Failed"
    , "Parameter Invalid"
    , "Parameter Missing"
    , "Parameter Out of Range"
    , "Parameter Access Denied"
    , "Control Missing"
    , "Generic Error"
};

/**
 * @brief Constructor
 * @param c Error code
 * @param details Error details
 */
Error::Error( Code c, const char *details )
    : mCode( c )
{
    // CharArrayCopy( mDetails, details, ERROR_DETAILS_SIZE_MAX );
    if( details == nullptr ) {
        mDetails[ 0 ] = '\0';
    } else {
        uint32_t size = sizeof( mDetails );
        strncpy( mDetails, details, size );
        mDetails[ size - 1 ] = '\0';
    }
}

/**
 * @brief Destructor
 */
Error::~Error()
{

}

/**
 * @brief Retrieves the error code
 * @return Error code
 */
uint32_t Error::getCode()
{
    return mCode;
}

/**
 * @brief Retrieves the error code string
 * @return Error code string
 */
const char *Error::getCodeString()
{
    return mCodeString[ getCode() ];
}

/**
 * @brief Retrieves the error details
 * @return Error details
 */
const char *Error::getDetails()
{
    return mDetails;
}

const char *Error::getCodeString( uint32_t c )
{
    return mCodeString[ c ];
}

const char *Error::getCodeString( Code c )
{
    return mCodeString[ c ];
}
