#include "common/http/server/request.h"


Request::Request( MHD_Connection *connection )
  : mConnection( connection )
  , mPostProcessor( NULL )
  , mMethod( NULL )
  , mUrl( NULL )
  , mBody( NULL )
  , mData( NULL )
  , mFp( NULL )
{

}

void Request::handleRequest()
{
    sendResponse( DEMO_PAGE, "text/html", MHD_HTTP_OK );
}

int Request::sendResponse(
        const char *responseData
        , const char *responseType
        , int statusCode )
{
    int ret;

    struct MHD_Response *response = NULL;
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

const char* Request::getBody()
{
    return mBody;
}
