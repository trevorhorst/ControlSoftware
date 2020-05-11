#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <curl/curl.h>
#include <vector>

#include "common/transport/client.h"

#include "http/http.h"

namespace NewHttp {

class Client
        : public Transport::Client
{
    static const std::string default_url;

public:

    using WriteFunction = size_t ( void *, size_t, size_t, std::string* );

    Client();
    ~Client();

    uint32_t send( const char *str );
    std::string get();

    uint32_t clearHeaders();

    uint32_t applyUrl( const std::string &url );
    uint32_t applyHeaders( const std::vector< std::string > &headers );
    uint32_t applyWriteFunction( WriteFunction *function );

private:

    std::string mDataString;
    std::string mUrl;
    WriteFunction *mWriteFunction;
    curl_slist *mHeaders;
    CURL *mCurl;

    static WriteFunction writeFunction;
};

}

#endif // HTTP_CLIENT_H
