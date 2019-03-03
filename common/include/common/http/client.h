#ifndef POCO_HTTP_CLIENT_H
#define POCO_HTTP_CLIENT_H

#include "Poco/StreamCopier.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include "common/http/http.h"
#include "common/error/error.h"

namespace Http
{

class Client
{
public:
    Client( std::string host = Http::localhost
            , uint16_t port = Http::default_port );
    ~Client();

    uint32_t get( Poco::Net::HTTPResponse &response );

    uint32_t handleRequest( Poco::Net::HTTPRequest &request
                            , Poco::Net::HTTPResponse &response );
private:
    Poco::Net::HTTPClientSession mSession;
};

}

#endif // POCO_HTTP_CLIENT_H
