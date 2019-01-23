#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <curl/curl.h>

#include "common/http/http.h"

class HttpClient
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

    HttpClient( const char *address = HTTP_LOCALHOST
            , uint16_t port = HTTP_DEFAULT_PORT );
    ~HttpClient();

    void send( const char *str );
    static size_t receive( void *ptr, size_t size, size_t nmemb, Data *data );
private:

    CURL *mCurl;
    Data mData;
    char mUrl[ HTTP_URL_SIZE_MAX ];
    uint16_t mPort;

    char *mResponse;
    uint16_t mResponseSize;
};

#endif // HTTP_CLIENT_H
