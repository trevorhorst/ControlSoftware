#ifndef NEWHTTP_SERVER_H
#define NEWHTTP_SERVER_H

#include <microhttpd.h>

#include "common/control/control_template.h"

#include "http/server/request.h"

namespace NewHttp {

class Server
        : public ControlTemplate< Server >
{
    static const uint32_t body_buffer_size;
public:
    Server();

    bool listen( uint32_t port );

    static int iterateHeaderValues(
            void *cls
            , enum MHD_ValueKind kind
            , const char *key
            , const char *value);

    static int iteratePost(
            void *coninfo_cls
            , MHD_ValueKind kind
            , const char *key
            , const char *filename
            , const char *content_type
            , const char *transfer_encoding
            , const char *data
            , uint64_t off
            , size_t size );

    uint32_t onRequest( MHD_Connection* mhdConnection
                        , const char* method
                        , const char* path
                        , const char* httpVersion
                        , void** requestPtr );
    uint32_t onRequestBody( Request* request, const char* body, size_t* length );
    uint32_t onRequestDone( Request* request);

    void process( Request *request );

    bool isListening();

private:
    MHD_Daemon *mServerDaemon;
};

}

#endif // HTTP_SERVER_H
