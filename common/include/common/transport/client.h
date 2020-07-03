#ifndef CLIENT_H
#define CLIENT_H

#include "common/error/error.h"

namespace Transport {

class Client
{
public:
    virtual ~Client();
    virtual uint32_t send( const char *str ) = 0;
};

}

#endif // CLIENT_H
