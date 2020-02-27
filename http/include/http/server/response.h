#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <microhttpd.h>
#include <unordered_map>
#include <string>

#include "http/http.h"


namespace NewHttp {

class Response
{
    Response( MHD_Connection *mhdConnection );

    void send();

private:
    MHD_Connection* mMhdConnection;
    NewHttp::Status mStatus;
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
