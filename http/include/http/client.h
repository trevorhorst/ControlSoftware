#ifndef NEWHTTP_CLIENT_H
#define NEWHTTP_CLIENT_H

#include <vector>
#include <curl/curl.h>

#include "common/transport/client.h"

#include "http/http.h"

namespace NewHttp {

class Client
        : public Transport::Client
{

public:

    struct Data {
        Data();
        ~Data();
        const char* read();
        void write( const char *data, size_t size );
        void clear();
    private:
        char *mData;
        size_t mSize;
    };

    using WriteFunction = size_t ( void *, size_t, size_t, std::string* );

    Client( const char *address = HTTP_LOCALHOST
            , uint16_t port = HTTP_DEFAULT_PORT );
    ~Client();

    void close();
    uint32_t clearHeaders();

    std::string get();

    void send( const char *str );

    uint32_t applyUrl( const std::string &url );
    uint32_t applyHeaders( const std::vector< std::string > &headers );
    uint32_t applyWriteFunction( WriteFunction *writeFunction );
    uint32_t applyBuffer( Data &buffer );

    static WriteFunction writeFunction;

private:
    CURL *mCurl;
    Data mData;
    char mUrl[ HTTP_URL_SIZE_MAX ];

    curl_slist *mHeaders;

    uint16_t mPort;

    char *mResponse;
    uint16_t mResponseSize;
};

}

#endif // NEWHTTP_CLIENT_H
