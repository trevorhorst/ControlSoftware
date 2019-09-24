#include "common/http/client.h"

namespace Http
{

/**
 * @brief Constructor
 * @param host Desired host for the session
 * @param port Desired port for the session
 */
Client::Client( std::string host, uint16_t port )
    : mSession( host, port )
{
}

/**
 * @brief Handle client request
 * @param request Request to send
 * @param response Response to request
 * @return
 */
uint32_t Client::handleRequest( Poco::Net::HTTPRequest &request
                                , Poco::Net::HTTPResponse &response )
{
    mSession.sendRequest( request );
    std::istream& rs = mSession.receiveResponse( response );
    std::cout << response.getStatus() << " " << response.getReason()
              << std::endl;
    if( response.getStatus() != Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED ) {
        std::ofstream ofs( "Poco_banner.jpg", std::fstream::binary);
        Poco::StreamCopier::copyStream( rs, ofs );
        return Error::Code::NONE;
    } else {
        //it went wrong ?
        return Error::Code::CMD_FAILED;
    }
}

uint32_t Client::get( Poco::Net::HTTPResponse &response )
{

}

}
