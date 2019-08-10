#include "common/http/server/server.h"

/**
 * @brief Static method responsible for handling new connections
 * @param cls
 * @param connection
 * @param url
 * @param method
 * @param version
 * @param upload_data
 * @param upload_data_size
 * @param con_cls
 * @return
 */
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

    Http::Server *server = static_cast< Http::Server* >( cls );
    Http::Request *request = static_cast< Http::Request* >( *con_cls );

    int32_t ret = 0;

    if( request == nullptr ) {
        // Handles a new request
        ret = server->onRequest( connection, method, url, con_cls );
    } else if( *upload_data_size > 0 ) {
        // Handles an existing request
        ret = server->onRequestBody( request, upload_data, upload_data_size );
    } else {
        // Handles a finished request
        ret = server->onRequestDone( request );
    }

    return ret;
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
    // printf( "Finished Request\n" );
    // Unused
    (void)cls;
    (void)connection;
    (void)rtc;

    Http::Request *r = static_cast< Http::Request* >( *request );

    if( r->mPostProcessor != nullptr ) {
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

namespace Http {

const char *Server::response_success     = "Success";
const char *Server::response_failed      = "Failed";
const char *Server::response_bad_request = "Bad Request";

const char *Server::type_text_html       = "text/html";
const char *Server::type_text_javascript = "text/javascript";

const char *Server::path_base            = "/";
const char *Server::path_index_html      = "/index.html";
const char *Server::path_main_js         = "/main.js";

/**
 * @brief Iterates over headers values found in the request
 * @param cls
 * @param kind
 * @param key
 * @param value
 * @return
 */
int Http::Server::iterateHeaderValues(
        void *cls
        , MHD_ValueKind kind
        , const char *key
        , const char *value )
{
    // Unused
    (void)kind;

    // Cast the existing headers map
    Request *header = static_cast< Request* >( cls );
    // Insert the new header
    header->addHeader( key, value );

    return MHD_YES;
}

/**
 * @brief Iterates over the data found in a POST request
 * @param coninfo_cls
 * @param kind
 * @param key
 * @param filename
 * @param content_type
 * @param transfer_encoding
 * @param data
 * @param off
 * @param size
 * @return
 */
int Http::Server::iteratePost(
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
    (void)( kind );
    (void)( off );

    Request *r = static_cast< Request* >( coninfo_cls );

    if( key != nullptr ) {
        printf( "Key: %s\n", key );
    }

    if( content_type != nullptr ) {
        printf( "Content-Type: %s\n", content_type );
    }

    if( transfer_encoding != nullptr ) {
        printf( "Transfer-Encoding: %s\n", transfer_encoding );
    }

    // con_info->answerstring = servererrorpage;
    // con_info->answercode = MHD_HTTP_INTERNAL_SERVER_ERROR;

    if( !r->mFp ) {
        // The file pointer is invalid
        // r->mFp = fopen( filename, "rb" );
        // if( !r->mFp ) {
        //     // We could read the file, which means it already exists
        //     fclose( r->mFp );
        //     // con_info->answerstring = fileexistspage;
        //     // con_info->answercode = MHD_HTTP_FORBIDDEN;
        //     return MHD_NO;
        // }

        // Just write over the file
        r->mFp = fopen( filename, "wb" );
        if( !r->mFp ) {
            // We failed to open the file for writing
            // con_info->answercode = MHD_HTTP_FORBIDDEN;
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

/**
 * @brief Constructor
 * @param index Index page to serve up
 * @param main Main javascript to serve up
 * @param port Port number to attach to
 * @param secure Enable/Disable secure server
 */
Server::Server( const char *index, const char *main
                          , uint16_t port, bool secure )
    // , mDone( false )
    : mServerDaemon( nullptr )
    , mCommandHandler( nullptr )
    , mIndexHtml( index )
    , mMainJs( main )
    , mPort( port )
    , mSecure( secure )
{
}

/**
 * @brief Destructor
 */
Server::~Server()
{
    stop();
}

void Server::setCommandHandler( CommandHandler *handler )
{
    mCommandHandler = handler;
}

/**
 * @brief Retrieves the server port
 * @return uint16_t representation of the port used by the server
 */
uint32_t Server::getPort()
{
    return mPort;
}

/**
 * @brief Determines if the server is running
 * @return Boolean indicating server status
 */
bool Server::isRunning()
{
    bool running = false;
    if( mServerDaemon != nullptr ) {
        running = true;
    }
    return running;
}

/**
 * @brief Retrieves secure status of the server
 * @return bool indicating secure status
 */
bool Server::isSecure()
{
    return mSecure;
}

/**
 * @brief Starts the HTTP server Daemon
 * @return Boolean indicating success of the operation
 */
bool Server::listen()
{
    bool success = false;

    uint flags = MHD_USE_POLL_INTERNALLY
            | MHD_USE_THREAD_PER_CONNECTION
            | MHD_USE_PEDANTIC_CHECKS;

    if( mSecure ) {
        printf( "Server is secure\n" );
        flags |= MHD_USE_SSL;
    }

    /*
    if( mThreaded ) {
        flags |= MHD_USE_THREAD_PER_CONNECTION;
    }
    */

    /*
    if( mSecure ) {
        printf( "Server is secure\n" );
        mServerDaemon = MHD_start_daemon(
                    MHD_USE_POLL_INTERNALLY | MHD_USE_PEDANTIC_CHECKS | MHD_USE_THREAD_PER_CONNECTION | MHD_USE_SSL
                    , mPort
                    , NULL
                    , NULL
                    , &AnswerToConnection, this
                    , MHD_OPTION_NOTIFY_COMPLETED, &OnResponseSent, this
                    , MHD_OPTION_HTTPS_MEM_KEY, mServerKey.c_str()
                    , MHD_OPTION_HTTPS_MEM_CERT, mServerCrt.c_str()
                    , MHD_OPTION_END
                    );
    } else {
        mServerDaemon = MHD_start_daemon(
                    MHD_USE_POLL_INTERNALLY | MHD_USE_PEDANTIC_CHECKS | MHD_USE_THREAD_PER_CONNECTION
                    , mPort
                    , nullptr
                    , nullptr
                    , &answerToConnection, this
                    , MHD_OPTION_NOTIFY_COMPLETED, &onResponseSent, this
                    // , MHD_OPTION_SOCK_ADDR, &socket
                    , MHD_OPTION_END
                    );
    }
    */

    mServerDaemon = MHD_start_daemon(
                flags
                , mPort
                , nullptr
                , nullptr
                , &answerToConnection, this
                , MHD_OPTION_NOTIFY_COMPLETED, &onResponseSent, this
                // , MHD_OPTION_SOCK_ADDR, &socket
                , MHD_OPTION_END
                );

    if( mServerDaemon == nullptr ) {
        printf( "Server daemon failed to start\n" );
    } else {
        printf( "Server daemon started successfully\n" );
        printf( "Listening on port %d\n", mPort );
        success = true;
    }
    return success;
}

/**
 * @brief Stops the HTTP server Daemon
 */
void Server::stop()
{
    if( mServerDaemon != nullptr ) {
        printf( "Stopping Server...\n" );
        MHD_stop_daemon( mServerDaemon );
        mServerDaemon = nullptr;
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
int Server::onRequest( MHD_Connection *connection
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
                , r );

    // Create a new post processor
    r->mPostProcessor = MHD_create_post_processor(
                connection
                , POST_BUFFER_SIZE
                , iteratePost
                , static_cast< void* >( r ) );

    // A NULL post processor means we will have to handle it ourselves
    if( r->mPostProcessor == nullptr ) {
        if( isVerbose() ) { printf( "PostProcessor is NULL\n" ); }
    }

    // Fill out the rest of the request
    r->setMethod( method );
    r->setPath( path );
    // r->mFp       = nullptr;
    // r->mData     = nullptr;
    // r->mDataSize = 0;

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
int Server::onRequestBody(
        Request *request
        , const char *data
        , size_t *size )
{
    request->appendData( data, *size );

    /*
    request->mDataSize += *size;
    if( request->mData == nullptr ) {
        // If we currently have no data lets copy over the incoming data
        request->mData = new char[ request->mDataSize + 1 ];
        memcpy( static_cast< void* >( request->mData ), data, request->mDataSize );

        // Append the null character
        request->mData[ request->mDataSize ] = '\0';
    } else {
        // Concatinate new data with the old data

        // Temporary pointer to the old data
        const char* t = request->mData;

        // Create a new memory block the size of total data received so far
        request->mData = new char[ request->mDataSize + 1 ];

        // Copy the old data to the new data block
        memcpy( static_cast< void* >( request->mData ), t, request->mDataSize - *size );

        // Get a pointer to the end of the old data
        char* n = &request->mData[ request->mDataSize - *size ];

        // Copy the new data to the end of the old
        memcpy( static_cast< void* >( n ), data, *size );

        // Append the null character
        request->mData[ request->mDataSize ] = '\0';

        // Delete the old data block
        delete[] t;
    }
    */

    *size = 0;
    return MHD_YES;
}

/**
 * @brief Handle the completion of received data from an incoming connection
 * @param request Pointer to the request object
 * @return Integer value return code
 */
int Server::onRequestDone( Request *request )
{
    // processRequest( request );
    process( request );
    return MHD_YES;
}

void Server::process(Request *request)
{
    if( isVerbose() ) { printHeaders( request ); }
    if( isVerbose() ) { printBody( request ); }

    const char *rspData = response_bad_request;
    const char *rspType = type_text_html;
    int rspCode   = MHD_HTTP_BAD_REQUEST;

    // In case we get a response string for the post
    char *rspStr = nullptr;

    if( strcmp( request->mMethod, MHD_HTTP_METHOD_GET ) == 0 ) {
        // Handles a GET request

        if( strcmp( request->getPath(), path_base ) == 0
                || strcmp( request->getPath(), mIndexHtml ) == 0 ) {
            // The index has been requested
            rspData = mIndexHtml;
            rspType = type_text_html;
            rspCode = MHD_HTTP_OK;

        } else if( strcmp( request->getPath(), path_main_js ) == 0 ) {
            // The main script file has been requested
            rspData = mMainJs;
            rspType = type_text_javascript;
            rspCode = MHD_HTTP_OK;
        }

    } else if ( strcmp( request->getMethod(), MHD_HTTP_METHOD_POST ) == 0 ) {
        // Handles a POST request

        if( request->mPostProcessor != nullptr ) {
            // A POST processor exists
            int ret = MHD_post_process( request->mPostProcessor
                              , request->getBody()->getData()
                              , request->getBody()->getSize() );

            if( ret == MHD_YES ) {
                rspData = response_success;
            } else {
                rspData = response_failed;
            }

            rspType = type_text_html;
            rspCode = MHD_HTTP_OK;
        } else if( mCommandHandler != nullptr ){
            // A Command Handler exists so we can attempt to handle the POST

            cJSON *response = cJSON_CreateObject();
            mCommandHandler->handle( request->getBody()->getData(), response );

            if( response ) {
                rspStr = cJSON_Print( response );
                rspData = rspStr;
                rspType = type_text_html;
                rspCode = MHD_HTTP_OK;
            }

            cJSON_Delete( response );

        }
    }

    // The last thing we will do is send the response
    request->sendResponse( rspData, rspType, rspCode );

    if( rspStr ) {
        // Free the post data
        cJSON_free( rspStr );
    }
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
/*
void Server::processRequest( Request *request )
{

    if( isVerbose() ) { printHeaders( request ); }
    if( isVerbose() ) { printBody( request ); }

    const char *rspData = response_bad_request;
    const char *rspType = type_text_html;
    int rspStatusCode   = MHD_HTTP_BAD_REQUEST;

    if( strcmp( request->mMethod, MHD_HTTP_METHOD_GET ) == 0 ) {
        // GET request handler

        if( strcmp( request->mUrl, "/" ) == 0
                || strcmp( request->mUrl, "/index.html" ) == 0 ) {
            if( isVerbose() ) { printf( "Serving up the index\n" ); }
            // The index has been requested
            rspData = mIndexHtml;
            rspType = type_text_html;
            rspStatusCode = MHD_HTTP_OK;
            request->sendResponse( mIndexHtml, "text/html", MHD_HTTP_OK );
        } else if( strcmp( request->mUrl, "/main.js" ) == 0 ) {
            // The main script file has been requested
            if( isVerbose() ){ printf( "Serving up the main.js\n" ); }
            rspData = mMainJs;
            rspType = type_text_javascript;
            rspStatusCode = MHD_HTTP_OK;
            request->sendResponse( mMainJs, "text/javascript", MHD_HTTP_OK );
        // } else if( request->mUrl == "/stylesheet.css" ) {
        //     // Send the style file
        //     request->sendResponse( mStyleBuffer, "text/css", MHD_HTTP_OK );
        }
    } else if( strcmp( request->mMethod, MHD_HTTP_METHOD_POST ) == 0 ) {
        // POST request handler

        if( request->mPostProcessor ) {
            // A post processor exists, just let libmicro handle the data

            int ret = MHD_post_process( request->mPostProcessor
                              , request->mData
                              , request->mDataSize );

            if( ret == MHD_YES ) {
                request->sendResponse( HTTP_SERVER_SUCCESS, "text/html"
                                       , MHD_HTTP_OK );
            } else {
                request->sendResponse( HTTP_SERVER_FAILED, "text/html"
                                       , MHD_HTTP_OK );
            }

        } else {
            // A postprocessor doesn't exist, we need to handle the data
            if( mCommandHandler != nullptr ) {
                cJSON *rsp = mCommandHandler->handle( request->mData );
                if( rsp == nullptr ) {
                    request->sendResponse( HTTP_SERVER_BAD_REQUEST, "text/html"
                                           , MHD_HTTP_OK );
                } else {
                    char *rspStr = nullptr;
                    rspStr = cJSON_Print( rsp );
                    request->sendResponse( rspStr, "text/html", MHD_HTTP_OK );
                    cJSON_free( rspStr );
                }
                /// @todo The rsp object should be created outside the command
                /// class and passed down
                cJSON_Delete( rsp );
                // if( rsp ) {
                //     char *prtStr = cJSON_Print( rsp );
                //     request->sendResponse( prtStr, "text/html", MHD_HTTP_OK );
                //     free( prtStr );
                // } else {
                //     request->sendResponse( HTTP_SERVER_BAD_REQUEST
                //                            , "text/html"
                //                            , MHD_HTTP_BAD_REQUEST );
                // }
            } else {
                request->sendResponse( HTTP_SERVER_BAD_REQUEST
                                       , "text/html"
                                       , MHD_HTTP_BAD_REQUEST );
            }

        }

    } else {
        // The user made a bad request
        request->sendResponse( HTTP_SERVER_BAD_REQUEST
                               , "text/html"
                               , MHD_HTTP_BAD_REQUEST );

    }
}
*/

uint32_t Server::getNumConnections()
{
    return MHD_get_daemon_info( mServerDaemon
                         , MHD_DAEMON_INFO_CURRENT_CONNECTIONS )->num_connections;
}

void Server::printHeaders(Request *request)
{
    Request::HeaderMap *headers = request->getHeaders();
    printf( "-- HEADERS START ----------------------------------------\n");
    for( auto it = headers->begin(); it != headers->end(); it++ ) {
        printf( "%20s: %s\n", it->first, it->second );
    }
    printf( "-- HEADERS END ------------------------------------------\n");
}

void Server::printBody( Request *request )
{
    printf( "-- BODY START -------------------------------------------\n");
    printf( "%.*s\n", request->getBody()->getSize(), request->getBody()->getData() );
    printf( "-- BODY END ---------------------------------------------\n");
}

}
