#ifndef HTTP_H
#define HTTP_H

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>

#include "common/logger/log.h"

#define HTTP_URL_TEMPLATE   "http://%s:%d"
#define HTTP_LOCALHOST      "localhost"

#define HTTP_DEFAULT_PORT 8080
#define HTTP_URL_SIZE_MAX 256

namespace Http
{

enum Method : int32_t {
    GET         = 0
    , POST      = 1
    , PUT       = 2
    , PATCH     = 3
    , DELETE    = 4
    , UNKNOWNMETHOD = -1
};

enum HttpVersion {
    UNKNOWNVERSION  = -1
    , HTTP_1_0      = 0x0100
    , HTTP_1_1      = 0x0101
};

enum Status {
    CONTINUE                         = 100
    , SWITCHINGPROTOCOLS             = 101
    , OK                             = 200
    , CREATED                        = 201
    , ACCEPTED                       = 202
    , NONAUTHORITATIVEINFORMATION    = 203
    , NOCONTENT                      = 204
    , RESETCONTENT                   = 205
    , PARTIALCONTENT                 = 206
    , MULTIPLECHOICES                = 300
    , MOVEDPERMANENTLY               = 301
    , MOVEDTEMPORARILY               = 302
    , SEEOTHER                       = 303
    , NOTMODIFIED                    = 304
    , USEPROXY                       = 305
    , TEMPORARYREDIRECT              = 307
    , PERMANENTREDIRECT              = 308
    , BADREQUEST                     = 400
    , UNAUTHORIZED                   = 401
    , PAYMENTREQUIRED                = 402
    , FORBIDDEN                      = 403
    , NOTFOUND                       = 404
    , METHODNOTALLOWED               = 405
    , NOTACCEPTABLE                  = 406
    , PROXYAUTHENTICATIONREQUIRED    = 407
    , REQUESTTIMEOUT                 = 408
    , CONFLICT                       = 409
    , GONE                           = 410
    , LENGTHREQUIRED                 = 411
    , PRECONDITIONFAILED             = 412
    , REQUESTENTITYTOOLARGE          = 413
    , REQUESTURITOOLONG              = 414
    , UNSUPPORTEDMEDIATYPE           = 415
    , REQUESTEDRANGENOTSATISFIABLE   = 416
    , EXPECTATIONFAILED              = 417
    , INTERNALSERVERERROR            = 500
    , NOTIMPLEMENTED                 = 501
    , BADGATEWAY                     = 502
    , SERVICEUNAVAILABLE             = 503
    , GATEWAYTIMEOUT                 = 504
    , HTTPVERSIONNOTSUPPORTED        = 505
};

extern const char *localhost;

extern const uint16_t default_port;

extern const char *method_strings[];

Method stringToMethod( const char *method );
const char *methodToString( Method method );

HttpVersion stringToHttpVersion( const char *version );

class Body {
public:
    Body();
    ~Body();

    void append( const char *data, size_t size );
    void clear();

    const char *getData();
    uint32_t getSize();

private:
    char *mData;
    uint32_t mSize;
};

}

#endif // HTTP_H
