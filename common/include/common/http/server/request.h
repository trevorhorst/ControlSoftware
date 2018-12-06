#ifndef REQUEST_H
#define REQUEST_H

#include <microhttpd.h>

#include "common/common_types.h"

#define DEMO_PAGE "<html><head><title>libmicrohttpd demo</title>"\
             "</head><body>libmicrohttpd demo!!</body></html>"

#define REQUEST_HEADER_MAX_KEY_SIZE 128
#define REQUEST_HEADER_MAX_VALUE_SIZE 1024

class Request
{
public:

    struct Header {
        Header( const char *value )
            : mValue( "" )
        {
            // Add a value
            if( value == NULL || value[ 0 ] == '\0' ) {
                strncpy( (char*)mValue, "\0", 1 );
            } else {
                strncpy( (char*)mValue, value, sizeof( mValue ) );
            }
        }
        // char mKey[ REQUEST_HEADER_MAX_KEY_SIZE ];
        const char mValue[ REQUEST_HEADER_MAX_VALUE_SIZE ];
    };

    explicit Request( MHD_Connection *connection );


    void setMethod( char *method );
    void setUrl( char *url );

    MHD_Connection *mConnection;
    MHD_PostProcessor *mPostProcessor;
    CharHashMap< Header > mHeaders;
    char *mMethod;
    char *mUrl;
    char *mBody;
    char* mData;
    uint32_t mDataSize;
    FILE* mFp;

    const char* getBody();

    // void finished();

// public slots:
    int sendResponse( const char *responseData
                      , const char *responseType
                      , int statusCode );

    void handleRequest();

private:

};

#endif // REQUEST_H
