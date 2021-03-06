#include "http/server/request.h"

namespace Http
{

const uint32_t Request::header_key_size_max   = 128;
const uint32_t Request::header_value_size_max = 1024;

/**
 * @brief Constructor
 * @param connection Connections associated with the request
 */
Request::Request( MHD_Connection *connection )
  : mConnection( connection )
  , mPostProcessor( nullptr )
  , mFp( nullptr )
  , mMethod( nullptr )
  , mPath( nullptr )
  , mData( nullptr )
  , mDataSize( 0 )
{
}

/**
 * @brief Destructor
 */
Request::~Request()
{
}

/**
 * @brief Adds a header to the request
 * @param key Header key
 * @param value Header value
 */
void Request::addHeader( const char *key, const char *value )
{
    mHeaders[ key ] = value;
}

/**
 * @brief Append data to the request
 * @param data Data to append
 * @param size Size of the data to append
 */
void Request::appendData( const char *data, size_t size )
{
    mBody.append( data, size );
}

/**
 * @brief Send a response to the incoming request
 * @param responseData Data to send
 * @param responseType Type of response
 * @param statusCode Status code in relation to the response
 * @return Integer indicating the success of the operation
 */
int Request::sendResponse( const char *responseData, const char *responseType
                           , int statusCode )
{
    int ret;

    struct MHD_Response *response = nullptr;
    response = MHD_create_response_from_buffer(
                strlen( responseData )
                , (void*)( responseData )
                , MHD_RESPMEM_MUST_COPY );

    if( !response ) {
        return MHD_NO;
    }
    MHD_add_response_header( response
                             , MHD_HTTP_HEADER_CONTENT_TYPE
                             , responseType );
    ret = MHD_queue_response( mConnection, statusCode, response );

    MHD_destroy_response( response );

    return ret;
}

/**
 * @brief Sets the method type of the request
 * @param method Desired method type
 */
void Request::setMethod( const char *method )
{
    mMethod = method;
}

/**
 * @brief Sets the path of the request
 * @param path Desired path type
 */
void Request::setPath( const char *path )
{
    mPath = path;
}

/**
 * @brief Retrieves the method type of the request
 * @return Character array representation of the method type
 */
const char *Request::getMethod()
{
    return mMethod;
}

/**
 * @brief Retrieves the path type of the request
 * @return Character array representation of the path
 */
const char *Request::getPath()
{
    return mPath;
}

/**
 * @brief Retrieves the body of the request
 * @return
 */
Body *Request::getBody()
{
    return &mBody;
}

/**
 * @brief Retrieves the headers of the request
 * @return Pointer to the header map of the request
 */
Request::HeaderMap *Request::getHeaders()
{
    return &mHeaders;
}

}
