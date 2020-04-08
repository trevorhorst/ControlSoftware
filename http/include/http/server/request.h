#ifndef NEWHTTP_REQUEST_H
#define NEWHTTP_REQUEST_H

#include <microhttpd.h>

#include "common/common_types.h"

#include "http/http.h"
#include "http/server/response.h"

namespace NewHttp
{

class Request
{
public:
    using HeaderMap = Types::CharHashMap< const char* >;

    explicit Request( MHD_Connection *connection );
    ~Request();

    void appendHeader( const char *key, const char *value );
    void appendData( const char *data, size_t size );

    int sendResponse( const char *responseData
                      , const char *responseType
                      , int statusCode );

    void setMethod( const char *method );
    void setPath( const char *path );

    const char *getMethod();
    const char *getPath();

    Body *getBody();
    HeaderMap *getHeaders();

    FILE* mFp;
    MHD_PostProcessor *mPostProcessor;

private:
    MHD_Connection *mConnection;
    Response *mResponse;
    HeaderMap mHeaders;
    const char *mMethod;
    const char *mPath;
    Body mBody;
    char* mData;
    uint32_t mDataSize;


    static const uint32_t header_key_size_max;
    static const uint32_t header_value_size_max;

};

}


#endif // NEWHTTP_REQUEST_H
