#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include <vector>
#include <string>

#include "http/http.h"

namespace NewHttp
{

class Route
{
public:
    Route();
    const std::vector< NewHttp::Method > &getVerbs() const;
    uint32_t setVerbs( const std::vector< NewHttp::Method > &verbs );
    uint32_t setAction( const std::string &action );

private:
    std::vector< NewHttp::Method > mVerbs;
    std::string mPath;
    std::vector< std::string > mPathKeys;
    std::string mAction;
};

class Router
{

};

}

#endif // HTTP_ROUTER_H
