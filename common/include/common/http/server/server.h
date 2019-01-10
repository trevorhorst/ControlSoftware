#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string.h>
#include <microhttpd.h>

#include "common/cjson/cJSON.h"
#include "common/control/control.h"
#include "common/http/server/request.h"
#include "common/command/command_handler.h"

#define POST_BUFFER_SIZE 512

#define HTTP_SERVER_PORT_DEFAULT 8080

#define HTTP_SERVER_SUCCESS 	"Success"
#define HTTP_SERVER_FAILED   	"Failed"
#define HTTP_SERVER_BAD_REQUEST "Bad Request"

class HttpServer
        : public ControlTemplate< HttpServer >
{
public:
    HttpServer( const char *index
                , const char *main
                , uint16_t port = HTTP_SERVER_PORT_DEFAULT
                , bool secure = false );
    ~HttpServer();

    bool listen();
    void stop();

    void setCommandHandler( CommandHandler *handler );

    bool isRunning();

    uint32_t getPort();

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

    // bool mDone;


private:
    MHD_Daemon *mServerDaemon;
    CommandHandler *mCommandHandler;
    const char *mIndexHtml;
    const char *mMainJs;
    uint16_t mPort;
    bool mSecure;
};

#endif // HTTP_SERVER_H
