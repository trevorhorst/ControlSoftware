/** ****************************************************************************
 * @file response.cpp
 * @author Trevor Horst
 * @copyright
 * @brief Response class definitions
 *
 * Provides a response interface to interactions with the HTTP server
 * ****************************************************************************/
#include "http/server/response.h"

namespace NewHttp {

static std::string toHeaderCase( const std::string &header )
{
    std::regex re( "(?:^|-|\\s)([a-z])" );

    // Convert the header to lower case
    std::string result;
    for( std::string::size_type i = 0; i < header.size(); i++ ) {
        char lower = static_cast< char >( ::tolower( header[ i ] ) );
        result.append( 1, lower);
    }

    std::smatch match;
    std::regex_search( result, match, re );

    return result;
}

/**
 * @brief Constructor
 * @param mhdConnection Pointer to the microhttp daemon
 */
Response::Response( MHD_Connection *mhdConnection )
    : mMhdConnection( mhdConnection )
    , mStatus( Status::OK )
    , mBodyFileDescriptor( -1 )
    , mBodyFileOffset( 0 )
    , mBodyFileSize( 0 )
    , mSent( false )
{

}

/**
 * @brief Retrieves response status
 * @return Response status
 */
Status Response::getStatus() const
{
    return mStatus;
}

void Response::setStatus( Status status )
{
    mStatus = status;
}

void Response::setHeader( const std::string &name, const std::string &value )
{
    mHeaders[ name ] = value;
}

void Response::setHeaders( const std::unordered_map<std::string, std::string> &headers )
{
    mHeaders.clear();
    for( auto it = headers.begin(); it != headers.end(); it++ ) {
        setHeader( (*it).first, (*it).second );
    }
}

void Response::send()
{
    MHD_Response* response;

    if( mSent == false) {
        if( mBodyFileDescriptor >= 0 ) {
            response = MHD_create_response_from_fd_at_offset(
                        mBodyFileSize
                        , mBodyFileDescriptor
                        , mBodyFileOffset );
        } else {
            response = MHD_create_response_from_buffer(
                        mBody.getSize()
                        , (void*) mBody.getData()
                        , MHD_RESPMEM_PERSISTENT );
        }

        for( auto it = mHeaders.begin(); it != mHeaders.end(); ++it ) {
            MHD_add_response_header(
                        response
                        , toHeaderCase( (*it).first ).c_str()
                        , (*it).second.c_str() );
        }

        MHD_queue_response( mMhdConnection, (uint32_t)mStatus, response);
        MHD_destroy_response(response);

        mSent = true;

        // emit sent();
    }

}

}
