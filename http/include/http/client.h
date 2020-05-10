#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <curl/curl.h>

#include "common/transport/client.h"

#include "http/http.h"

namespace NewHttp {

class Client
        : public Transport::Client
{
public:

    struct DataString {
        DataString();
        ~DataString();
        const char* read();
        void write( const char *data, size_t size );
        void clear();

    private:
        char *mDataString;
        size_t mSize;
    };

    Client( const char *address = HTTP_LOCALHOST
            , uint16_t port = HTTP_DEFAULT_PORT );
    ~Client();

    void send( const char *str );
    static size_t receive( void *ptr, size_t size, size_t nmemb, DataString *data );
private:

    CURL *mCurl;
    DataString mDataString;
    char mUrl[ HTTP_URL_SIZE_MAX ];
    uint16_t mPort;

    char *mResponse;
    uint16_t mResponseSize;
};

}

#endif // HTTP_CLIENT_H
