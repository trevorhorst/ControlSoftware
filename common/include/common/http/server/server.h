#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string.h>
#include <microhttpd.h>

#include "common/cjson/cJSON.h"
#include "common/control/control.h"
#include "common/http/server/request.h"

class HttpServer : public Control
{
public:
    HttpServer( const char *index, const char *main
                , uint32_t port = 8000, bool secure = false );
    bool listen();
    void stop();

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

    int onRequest(MHD_Connection *connection
            , const char *method
            , const char *path
            , void **request );

    int onRequestBody(Request *request
            , const char *data
            , size_t *size );

    int onRequestDone(
            Request *request );

    void processRequest( Request *request );
    void printHeaders( Request *request );
    void printBody( Request *request );

private:
    MHD_Daemon *mServerDaemon;
    const char *mIndexHtml;
    const char *mMainJs;
    uint32_t mPort;
    bool mSecure;
};

#endif // HTTP_SERVER_H
