#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include <vector>
#include <string>
#include <regex>

#include "http/http.h"
#include "http/server/request.h"

namespace NewHttp
{

class Route
{
public:
    Route();
    const std::vector< NewHttp::Method > &getVerbs() const;
    uint32_t setVerbs( const std::vector< NewHttp::Method > &verbs );
    uint32_t setAction( const std::string &action );
    uint32_t setPath( const std::string &path );

    bool match( const std::string &path
                , Method verb
                , std::unordered_map< std::string, std::string > *paramsPtr
                , bool *pathOkPtr
                , bool *methodOkPtr );

private:
    std::vector< NewHttp::Method > mVerbs;
    std::string mPath;
    std::vector< std::string > mPathKeys;
    std::string mAction;
    std::regex mRegex;
};

class Router
{

public:

    void addRoute( const std::string &method
                   , const std::string &path
                   , const std::string action );

    void processRequest( Request *request );

private:
    std::vector< Route > mRoutes;

};

}

#endif // HTTP_ROUTER_H
