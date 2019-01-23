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

namespace Http
{
    extern const char *LOCALHOST;

    extern const uint16_t DEFAULT_PORT;
}

#endif // HTTP_H
