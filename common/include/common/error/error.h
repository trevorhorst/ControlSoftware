#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>
#include <string.h>

#include "common/common_types.h"

#define ERROR_DETAILS_SIZE_MAX 256

class Error
{
    static const char *mCodeString[];
public:

    enum Code {
        NONE    = 0
        , SYNTAX
        , CMD_INVALID
        , CMD_MISSING
        , CMD_FAILED
        , PARAM_INVALID
        , PARAM_MISSING
        , PARAM_OUT_OF_RANGE
        , PARAM_ACCESS_DENIED
        , MAX
    };

    Error( Code c = NONE, const char *details = "" );
    virtual ~Error();

    uint32_t getCode();
    const char *getCodeString();
    const char *getDetails();

    static const char *getCodeString( uint32_t e );
    static const char *getCodeString( Code e );

    uint32_t operator []( Code e ) { return static_cast< uint32_t >( e ); }

private:
    Code mCode;
    char mDetails[ ERROR_DETAILS_SIZE_MAX ];
};

#endif // ERROR_H
