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
    // using HeaderMap = Types::CharHashMap< const char* >;
    using HeaderMap = std::unordered_map< std::string, std::string >;

    explicit Request( MHD_Connection *connection );
    ~Request();

    void appendHeader( const char *key, const char *value );
    void appendData( const char *data, size_t size );

    int sendResponse( const char *responseData
                      , const char *responseType
                      , int statusCode );

    uint64_t parseBody( const char *data, uint64_t length );

    const char *getMethod();
    const char *getPath();
    HeaderMap *getHeaders();
    Body *getBody();

    void setMethod( const char *method );
    void setPath( const char *path );
    void setHttpVersion( HttpVersion version );
    void setQuery( const std::unordered_map< std::string, std::string > query );
    void setHeader( const std::string name, const std::string value );
    void setHeaders( const HeaderMap &headers );
    void setResponse( Response *response );

    FILE* mFp;
    MHD_PostProcessor *mPostProcessor;

private:
    MHD_Connection *mConnection;
    Response *mResponse;
    HeaderMap mHeaders;
    std::unordered_map< std::string, std::string > mQuery;
    const char *mMethod;
    const char *mPath;
    HttpVersion mVersion;
    Body mBody;
    char* mData;
    uint32_t mDataSize;


    static const uint32_t header_key_size_max;
    static const uint32_t header_value_size_max;

};

}


#endif // NEWHTTP_REQUEST_H
