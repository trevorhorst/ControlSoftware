/** ****************************************************************************
 * @file response.h
 * @author Trevor Horst
 * @copyright
 * @brief HTTP Response class declarations
 * ****************************************************************************/

#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <microhttpd.h>
#include <unordered_map>
#include <string>
#include <regex>

#include "http/http.h"

namespace NewHttp {

class Response
{
public:
    Response( MHD_Connection *mhdConnection );

    void send( const char *data, size_t size );
    void send();

    bool isSent();

    Status getStatus() const;

    void setStatus( Status status );
    void setHeader( const std::string &name, const std::string &value );
    void setHeaders( const std::unordered_map< std::string, std::string > &headers );

private:
    MHD_Connection* mMhdConnection;
    Status mStatus;
    std::unordered_map< std::string, std::string > mHeaders;
    // QStringHash headers;
    // QByteArray body;
    Body mBody;
    int mBodyFileDescriptor;
    off_t mBodyFileOffset;
    size_t mBodyFileSize;
    bool mSent;
};

}

#endif // HTTP_RESPONSE_H
