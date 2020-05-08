#include "http/server/router.h"

namespace NewHttp {

static std::vector< NewHttp::Method > stringToHttpVerbs(const std::string &method)
{
    std::string str( method );
    std::string token;
    std::vector< Method > tokenized;
    size_t last = 0;
    size_t next = 0;

    // Split the input into segments by the delimeter
    while( ( next = str.find( ",", last ) ) != std::string::npos ) {
        if( next - last != 0 ) {
            // There is data in the string
            token = str.substr( last, next - last );
            tokenized.push_back( stringToMethod( token.c_str() ) );
        } else {
            // If the next and last values are the same, the string is empty
            // and we can ignore it
        }
        last = next + 1;
    }


    if( !str.substr( last ).empty() ) {
        // Need to place the the final substring in the list, unless it's empty
        tokenized.push_back( stringToMethod( str.substr( last ).c_str() ) );
    }

    return tokenized;
}

/**
 * @brief Route constructor
 */
Route::Route()
{

}

/**
 * @brief Get HTTP verbs
 * @return Reference to to the local verbs
 */
const std::vector< NewHttp::Method > &Route::getVerbs() const
{
    return mVerbs;
}

/**
 * @brief Set the local HTTP verbs
 * @param verbs Desired verbs to set
 * @return
 */
uint32_t Route::setVerbs( const std::vector<NewHttp::Method> &verbs )
{
    mVerbs = verbs;
    return 0;
}

/**
 * @brief Set the local HTTP verbs
 * @param verbs Desired verbs to set
 * @return
 */
uint32_t Route::setVerbs( NewHttp::Method verb )
{
    mVerbs.clear();
    mVerbs.push_back( verb );
    return 0;
}

/**
 * @brief Set the local action
 * @param action Desired action
 * @return
 */
uint32_t Route::setAction( const std::string &action )
{
    mAction = action;
    return 0;
}

/**
 * @brief Sets the path for the route
 * @param path Desired path
 * @return Error code
 */
uint32_t Route::setPath( const std::string &path )
{
    std::string pattern = "^" + path + "$";
    std::regex re( ":(\\w+)(\\(.*\\))?" );
    std::smatch match;

    mPath = path;
    mPathKeys.clear();

    bool noMatch = true;
    std::string pathIt = path;
    while( std::regex_search( pathIt, match, re ) ) {
        noMatch = false;
        std::string c = match[ 0 ].str(); // from
        std::string k = match[ 1 ].str();
        std::string p = match[ 2 ].str(); // to
        if( p.empty() ) {
            p = "([a-zA-Z0-9-$_.+!*'(),%]+)";
        }
        // Replace sub string with another string
        size_t start_pos = pattern.find( c );
        if( start_pos == std::string::npos ) {
            return false;
        }
        pattern.replace(start_pos, c.length(), p);

        mPathKeys.push_back( k );
        // pattern.rep;
        mRegex = std::regex( pattern );
        pathIt = match.suffix().str();
    }

    if( noMatch ) {
        mRegex = std::regex( path );
    }

    return 0;
}

bool Route::match(
        const std::string &path
        , Method verb
        , std::unordered_map<std::string, std::string> *paramsPtr
        , bool *pathOkPtr
        , bool *methodOkPtr ) const
{
    std::unordered_map< std::string, std::string > params;
    bool pathOk = false;
    bool methodOk = false;

    std::smatch match;
    std::string pathIt = path;
    if( std::regex_search( path, match, mRegex ) ) {
        pathOk = true;
        if( mPathKeys.size() > 0 ) {
            size_t i = 0;
            while( std::regex_search( pathIt, match, mRegex ) ) {
                // params.insert( mPathKeys[ i ], match[ 1 ].str() );
                i++;
            }
        }
    }

}







void Router::addRoute(
        const std::string &method
        , const std::string &path
        , const std::string action )
{
    Route route;
    route.setVerbs( stringToHttpVerbs( method ) );
    route.setPath( path );
    // route.setAction( action );

    mRoutes.push_back( route );
}

void Router::processRequest( Request *request )
{
    bool found      = false;
    bool processed  = false;

    for( uint32_t i = 0; !processed && i < mRoutes.size(); i++ ) {
        const Route &route = mRoutes[ i ];
        std::unordered_map< std::string, std::string > params;
        bool pathOk;
        bool methodOk;

        route.match(request->getPath(), request->getMethod(), &params, &pathOk, &methodOk);
    }
}

}
