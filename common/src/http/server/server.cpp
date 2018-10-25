#include "common/http/server/server.h"

#define POST_BUFFER_SIZE 512

const char *askpage = "<html><body>\n\
                       Upload a file, please!<br>\n\
                       There are %u clients uploading at the moment.<br>\n\
                       <form action=\"/filepost\" method=\"post\" enctype=\"multipart/form-data\">\n\
                       <input name=\"file\" type=\"file\">\n\
                       <input type=\"submit\" value=\" Send \"></form>\n\
                       </body></html>";


static int answerToConnection (
        void *cls
        , MHD_Connection *connection
        , const char *url, const char *method
        , const char *version, const char *upload_data
        , size_t *upload_data_size
        , void **con_cls
        )
{
    // Unused
    ( void )( cls );
    ( void )( version );


    HttpServer *server = static_cast< HttpServer* >( cls );
    Request *request = static_cast< Request* >( *con_cls );

    if( request == NULL ) {
        // Handles a new request
        return server->onRequest( connection, method, url, con_cls );
    } else if( *upload_data_size > 0 ) {
        // Handles an existing request
        return server->onRequestBody( request, upload_data, upload_data_size );
    } else {
        // Handles a finished request
        return server->onRequestDone( request );
    }

}


/**
 * @brief Handles clean up of the request after a response has been sent
 * @param cls
 * @param connection
 * @param request
 * @param rtc
 */
static void onResponseSent(
        void *cls
        , MHD_Connection *connection
        , void **request
        , MHD_RequestTerminationCode *rtc)
{
    printf( "Finished Request\n" );
    // Unused
    (void)cls;
    (void)connection;
    (void)rtc;

    Request *r = static_cast< Request* >( *request );

    if( r->mPostProcessor != NULL ) {
        MHD_destroy_post_processor( r->mPostProcessor );
    }

    if( r->mFp ) {
        fclose( r->mFp );
    }

    // Delete the data contained within the request
    delete[] r->mData;

    // Delete the request
    delete r;
}

int HttpServer::iterateHeaderValues(
        void *cls
        , MHD_ValueKind kind
        , const char *key
        , const char *value )
{
    // Unused
    (void)kind;

    // printf( "%20s: %s\n", key, value );
    CharHashMap< Request::Header > *headers
            = static_cast< CharHashMap< Request::Header >* >( cls );
    // Request::Header *h = new Request::Header( value );
    // headers[ (char*)key ] = Request::Header( value );
    headers->insert( CharHashMap< Request::Header>::value_type( key
                                              , Request::Header( value ) ) );
    // (*headers)[ key ] = Request::Header( value );
    return MHD_YES;
}

int HttpServer::iteratePost(
        void *coninfo_cls
        , MHD_ValueKind kind
        , const char *key
        , const char *filename
        , const char *content_type
        , const char *transfer_encoding
        , const char *data
        , uint64_t off, size_t size )
{
    printf( "%s\n", __FUNCTION__ );
    // Unused
    (void)( key );
    (void)( content_type );
    (void)( transfer_encoding );
    (void)( kind );
    (void)( off );

    Request *r = static_cast< Request* >( coninfo_cls );
    // if( isVerbose() ) { printf( "%s\n", r->mMethod ); }

    // printf( "%s\n", content_type );
    // printf( "%s\n", transfer_encoding );

    // con_info->answerstring = servererrorpage;
    // con_info->answercode = MHD_HTTP_INTERNAL_SERVER_ERROR;

    // if( 0 != strcmp( key, "file" ) ) {
    //     return MHD_NO;
    // }

    if( !r->mFp ) {
        // if( NULL != ( fp = fopen( filename, "rb" ) ) ) {
        //     fclose( fp );
        //     con_info->answerstring = fileexistspage;
        //     con_info->answercode = MHD_HTTP_FORBIDDEN;
        //     return MHD_NO;
        // }

        // r->mFp = fopen( filename, "ab" );
        r->mFp = fopen( filename, "wb" );
        if(!r->mFp ) {
            return MHD_NO;
        }
    }

    if( size > 0 ) {
        if( !fwrite( data, size, sizeof(char), r->mFp ) ) {
            return MHD_NO;
        }
    }

    // con_info->answerstring = completepage;
    // con_info->answercode = MHD_HTTP_OK;

    return MHD_YES;
}

HttpServer::HttpServer( const char *index, const char *main
                        , uint32_t port, bool secure )
    : Control()
    , mServerDaemon( NULL )
    , mIndexHtml( index )
    , mMainJs( main )
    , mPort( port )
    , mSecure( secure )
{

}

/**
 * @brief Starts the HTTP server Daemon
 * @return Boolean indicating success of the operation
 */
bool HttpServer::listen()
{
    bool success = false;

    if( mSecure ) {
        printf( "Server is secure\n" );
        // mServerDaemon = MHD_start_daemon(
        //             MHD_USE_POLL_INTERNALLY | MHD_USE_PEDANTIC_CHECKS | MHD_USE_THREAD_PER_CONNECTION | MHD_USE_SSL
        //             , mPort
        //             , NULL
        //             , NULL
        //             , &AnswerToConnection, this
        //             , MHD_OPTION_NOTIFY_COMPLETED, &OnResponseSent, this
        //             , MHD_OPTION_HTTPS_MEM_KEY, mServerKey.c_str()
        //             , MHD_OPTION_HTTPS_MEM_CERT, mServerCrt.c_str()
        //             , MHD_OPTION_END
        //             );
    } else {
        mServerDaemon = MHD_start_daemon(
                    MHD_USE_POLL_INTERNALLY | MHD_USE_PEDANTIC_CHECKS | MHD_USE_THREAD_PER_CONNECTION
                    , mPort
                    , NULL
                    , NULL
                    , &answerToConnection, this
                    , MHD_OPTION_NOTIFY_COMPLETED, &onResponseSent, this
                    // , MHD_OPTION_SOCK_ADDR, &socket
                    , MHD_OPTION_END
                    );
    }

    if( mServerDaemon == NULL ) {
        printf( "Server daemon failed to start\n" );
    } else {
        printf( "Server daemon started successfully\n" );
        if( isVerbose() ){ printf( "Listening on port %d\n", mPort ); }
        success = true;
    }
    return success;
}

/**
 * @brief Stops the HTTP server Daemon
 */
void HttpServer::stop()
{
    if( mServerDaemon != NULL ) {
        printf( "Stopping Server...\n" );
        MHD_stop_daemon( mServerDaemon );
        mServerDaemon = NULL;
    }
}


/**
 * @brief Handles new incoming connections
 * @param connection Pointer to the connection
 * @param method HTTP Method
 * @param path Route of received connection
 * @param request Pointer to the request object
 * @return Integer value return code
 */
int HttpServer::onRequest( MHD_Connection *connection
        , const char *method
        , const char *path
        , void **request )
{
    // Create a new requst to handle the connection
    Request *r = new Request( connection );

    // Parse the header value
    MHD_get_connection_values(
                connection
                , MHD_HEADER_KIND
                , &iterateHeaderValues
                , &r->mHeaders );

    // Create a new post processor
    r->mPostProcessor = MHD_create_post_processor(
                connection
                , POST_BUFFER_SIZE
                , iteratePost
                , (void*)r );

    // A NULL post processor means we will have to handle it ourselves
    if( r->mPostProcessor == NULL ) {
        printf( "PostProcessor is NULL\n" );
    }

    // Fill out the rest of the request
    r->mFp       = NULL;
    r->mMethod   = (char*)method;
    r->mUrl      = (char*)path;
    r->mData     = NULL;
    r->mDataSize = 0;

    *request = r;

    return MHD_YES;
}

/**
 * @brief Handles existing connections
 * @param request Pointer to the request object
 * @param data New incoming data
 * @param size New incoming data size
 * @return Integer value return code
 */
int HttpServer::onRequestBody(
        Request *request
        , const char *data
        , size_t *size )
{
    request->mDataSize += *size;
    if( request->mData == NULL ) {
        // If we currently have no data lets copy over the incoming data
        request->mData = new char[ *size ];
        memcpy( (void*)request->mData, data, *size );
    } else {
        // Concatinate new data with the old data

        // Temporary pointer to the old data
        const char* t = request->mData;

        // Create a new memory block the size of total data received so far
        request->mData = new char[ request->mDataSize ];

        // Copy the old data to the new data block
        memcpy( (void*)request->mData, t, request->mDataSize - *size );

        // Get a pointer to the end of the old data
        const char* n = &request->mData[ request->mDataSize - *size ];

        // Copy the new data to the end of the old
        memcpy( (void*)n, data, *size );

        // Delete the old data block
        delete[] t;
    }

    *size = 0;
    return MHD_YES;
}

/**
 * @brief Handle the completion of received data from an incoming connection
 * @param request Pointer to the request object
 * @return Integer value return code
 */
int HttpServer::onRequestDone( Request *request )
{
    processRequest( request );
    return MHD_YES;
}

/**
 * @brief Process a received request
 * @param request Pointer to the request
 * @note Might need to come up with a more clever way to handle the routes.
 * Could build a router similar to other software designs.
 * @note This section should take all requests and put them in to a request
 * queue. When a new request comes in it will get added to the queue. While a
 * separate thread handles the requests and responds accordingly
 */
void HttpServer::processRequest( Request *request )
{

    if( strcmp( request->mMethod, MHD_HTTP_METHOD_GET ) == 0 ) {
        // GET request handler

        if( strcmp( request->mUrl, "/" ) == 0
                || strcmp( request->mUrl, "/index.html" ) == 0 ) {
            // The index has been requested
            printf( "Serving up the index\n" );
            request->sendResponse( mIndexHtml, "text/html", MHD_HTTP_OK );
        } else if( strcmp( request->mUrl, "/main.js" ) == 0 ) {
            // The main script file has been requested
            printf( "Serving up the main.js\n" );
            request->sendResponse( mMainJs, "text/javascript", MHD_HTTP_OK );
        // } else if( request->mUrl == "/stylesheet.css" ) {
        //     // Send the style file
        //     request->sendResponse( mStyleBuffer, "text/css", MHD_HTTP_OK );
        }
    }

    if( strcmp( request->mMethod, MHD_HTTP_METHOD_POST ) == 0 ) {
        // POST request handler

        if( request->mPostProcessor ) {
            // A post processor exists, just let libmicro handle the data

            int ret = MHD_post_process( request->mPostProcessor
                              , request->mData
                              , request->mDataSize );

            // Leaving this here for now to get rid of the warning, eventually
            // we will use this
            ( void ) ret;

        } else {
            // A postprocessor doesn't exist, we need to handle the data

            const char **lastByte = NULL;
            cJSON *parsed = cJSON_ParseWithOpts( request->mData, lastByte, false );
            char *temp = cJSON_Print( parsed );
            printf( "%s\n", temp );
            free( temp );
            cJSON_Delete( parsed );

            // Get the body data
            // QString body = QString::fromUtf8( request->mData, request->mDataSize );

            // Parse the command as JSON
            // QVariantMap command = ParseJson( body );
            /*
            // Check the command type
            QString type = command.value( "cmd" ).toString();
            if( mCommandHandler->mCommandList.contains( type ) ) {
                // If the command type is available
                Command *cmd = mCommandHandler->mCommandList.value( type );
                if( command.contains( "parameters" ) ) {
                    // If there are parameters, then we are setting values
                    QVariantMap parameters = command.value( "parameters" ).toMap();
                    cmd->Set( parameters );
                } else {
                    // If there are parameters, then we are querying
                    cmd->Query();
                }

            }

            // Send response to post
            QJsonObject message;
            message.insert( "rsp", QJsonValue::fromVariant( "success" ) );
            QJsonDocument response( message );
            request->sendResponse( response.toJson().toStdString().c_str()
                                   , "application/json"
                                   , MHD_HTTP_OK );
           */
        }

    }

    /// @todo Need to figure out how to properly finish a request

    // If we've made it here, then the client has made a bad request
    request->sendResponse( askpage, "text/html", MHD_HTTP_BAD_REQUEST );
}

void HttpServer::printHeaders(Request *request)
{
    CharHashMap< Request::Header >::const_iterator it;
    printf( "-- HEADERS START ----------------------------------------\n");
    for( it = request->mHeaders.begin(); it != request->mHeaders.end(); it++ ) {
        printf( "%20s: %s\n", it->first, it->second.mValue );
    }
    printf( "-- HEADERS END ------------------------------------------\n");
}

void HttpServer::printBody( Request *request )
{
    printf( "-- BODY START -------------------------------------------\n");
    printf( "%.*s\n", request->mDataSize, request->mData );
    printf( "-- BODY END ---------------------------------------------\n");
}
