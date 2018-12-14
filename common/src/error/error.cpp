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
};

/**
 * @brief Constructor
 * @param c Error code
 * @param details Error details
 */
Error::Error( Code c, const char *details )
    : mCode( c )
    , mDetails( "" )
{
    if( details == NULL || details[ 0 ] == '\0' ) {
        strncpy( (char*)mDetails, "\0", 1 );
    } else {
        strncpy( (char*)mDetails, details, sizeof( mDetails ) );
    }
}

/**
 * @brief Destructor
 */
Error::~Error()
{

}


const char *Error::getCodeString( uint32_t c )
{
    return mCodeString[ c ];
}

const char *Error::getCodeString( Code c )
{
    return mCodeString[ c ];
}
