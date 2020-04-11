#include "http/server/server.h"

namespace NewHttp {

static int32_t answerToConnection(
        void *cls
        , MHD_Connection *connection
        , const char *url, const char *method
        , const char *version, const char *uploadData
        , size_t *uploadDataSize
        , void **conCls)
{
    uint32_t error = Error::Code::NONE;

    Server *server = static_cast< Server* >( cls );
    Request *request = static_cast< Request* >( *conCls );

    if( request == nullptr ) {
        server->onRequest( connection, method, url, version, conCls );
    } else if( *uploadDataSize > 0 ) {
        server->onRequestBody( request, uploadData, uploadDataSize );
    } else {
        error = server->onRequestDone( request );
    }

    return error;
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

    Request *r = static_cast< Request* >( *request );

    if( r->mPostProcessor != nullptr ) {
        MHD_destroy_post_processor( r->mPostProcessor );
    }

    if( r->mFp ) {
        fclose( r->mFp );
    }

    // Delete the data contained within the request
    // delete[] r->mData;

    // Delete the request
    delete r;
}

const uint32_t Server::body_buffer_size = 512;

/**
 * @brief Iterates over headers values found in the request
 * @param cls
 * @param kind
 * @param key
 * @param value
 * @return
 */
int Server::iterateHeaderValues(
        void *cls
        , MHD_ValueKind kind
        , const char *key
        , const char *value )
{
    // Unused
    (void)kind;

    std::unordered_map< std::string, std::string > *hash =
            static_cast< std::unordered_map< std::string, std::string >* >( cls );

    // Insert the new header
    (*hash)[ key ] = value;

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
int NewHttp::Server::iteratePost(
        void *coninfo_cls
        , MHD_ValueKind kind
        , const char *key
        , const char *filename
        , const char *content_type
        , const char *transfer_encoding
        , const char *data
        , uint64_t off, size_t size )
{
    // Unused
    (void)( kind );
    (void)( off );

    Request *r = static_cast< Request* >( coninfo_cls );

    if( key != nullptr ) {
        LOG_INFO( "Key: %s\n", key );
    }

    if( content_type != nullptr ) {
        LOG_INFO( "Content-Type: %s\n", content_type );
    }

    if( transfer_encoding != nullptr ) {
        LOG_INFO( "Transfer-Encoding: %s\n", transfer_encoding );
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
 */
Server::Server()
    : mServerDaemon( nullptr )
{
}

/**
 * @brief Start the microhttp daemon
 * @param port
 * @return
 */
bool Server::listen(uint32_t port)
{
   bool success = false;

    uint32_t flags = MHD_USE_POLL_INTERNALLY
            | MHD_USE_THREAD_PER_CONNECTION
            | MHD_USE_PEDANTIC_CHECKS;

    mServerDaemon = MHD_start_daemon(
                flags
                , port
                , nullptr
                , nullptr
                , &answerToConnection, this
                , MHD_OPTION_NOTIFY_COMPLETED, &onResponseSent, this
                , MHD_OPTION_END
                );

    if( mServerDaemon == nullptr ) {
        LOG_WARN( "server daemon failed to start\n" );
    } else {
        LOG_INFO( "server daemon started succesfully\n" );
        LOG_INFO( "listening on port %d", port );
        success = true;
    }

    return success;
}

uint32_t Server::onRequest(
        MHD_Connection *mhdConnection
        , const char *method
        , const char *path
        , const char *httpVersion
        , void **requestPtr)
{
    LOG_INFO( "New request received" );
    Request *request = new Request( mhdConnection );
    std::unordered_map< std::string, std::string > query;
    std::unordered_map< std::string, std::string > headers;

    MHD_get_connection_values( mhdConnection, MHD_GET_ARGUMENT_KIND, &iterateHeaderValues, &query );
    MHD_get_connection_values( mhdConnection, MHD_HEADER_KIND, &iterateHeaderValues, &headers );

    request->mPostProcessor = MHD_create_post_processor(
                mhdConnection
                , body_buffer_size
                , iteratePost
                , static_cast< void* >( request ) );

    // Fill out the rest of the request
    request->setMethod( method );
    request->setPath( path );
    request->setQuery( query );
    // request->setHttpVersion();
    request->setHeaders( headers );
    request->setResponse( new Response( mhdConnection ) );

    *requestPtr = request;

    return MHD_YES;
}

uint32_t Server::onRequestBody( Request* request, const char* body, size_t* length )
{
    request->appendData( body, *length );
    *length = 0;
    return  MHD_YES;
}

uint32_t Server::onRequestDone( Request *request )
{
    process( request );
    return MHD_YES;
}

void Server::process( Request *request )
{
    (void)request;
    // const char data[6] = "hello";
    // request->sendResponse( data, 6 );
}

bool Server::isListening()
{
    bool listening = false;
    if( mServerDaemon ) {
        listening = true;
    }
    return listening;
}

}
