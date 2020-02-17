#ifndef HTTP_H
#define HTTP_H

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>

#define HTTP_URL_TEMPLATE   "http://%s:%d"
#define HTTP_LOCALHOST      "localhost"

#define HTTP_DEFAULT_PORT 8080
#define HTTP_URL_SIZE_MAX 256

namespace NewHttp
{

extern const char *localhost;

extern const uint16_t default_port;

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
