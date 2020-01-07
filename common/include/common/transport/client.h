#ifndef CLIENT_H
#define CLIENT_H

namespace Transport {

class Client
{
public:
    virtual ~Client();
    virtual void send( const char *str ) = 0;
};

}

#endif // CLIENT_H
