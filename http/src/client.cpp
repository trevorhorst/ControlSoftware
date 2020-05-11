#include <cstdlib>
#include <string.h>

#include "common/cjson/cJSON.h"

#include "http/client.h"

namespace Http {

const std::string Client::default_url = "http://localhost:8080";

/**
 * @brief Constructor
 * @param address Address to
 * @param port
 */
Client::Client()
    : mWriteFunction( nullptr )
    , mCurl( nullptr )
{
    curl_global_init( CURL_GLOBAL_ALL );
    mCurl = curl_easy_init();

    // curl_easy_setopt(mCurl, CURLOPT_USERPWD, "user:pass");
    curl_easy_setopt( mCurl, CURLOPT_NOPROGRESS, 1L );
    curl_easy_setopt( mCurl, CURLOPT_USERAGENT, "curl/7.42.0" );
    curl_easy_setopt( mCurl, CURLOPT_MAXREDIRS, 50L );
    curl_easy_setopt( mCurl, CURLOPT_TCP_KEEPALIVE, 1L );

    applyUrl( default_url );
    applyWriteFunction( Client::writeFunction );
}

/**
 * @brief Destructor
 */
Client::~Client()
{
    clearHeaders();
    curl_easy_cleanup( mCurl );
    mCurl = nullptr;
    curl_global_cleanup();
}

/**
 * @brief Receives data from a connection
 * @param ptr Pointer to the data received
 * @param size Size is always 1
 * @param nmemb Size of the data received
 * @param d Pointer to the existing data
 * @return
 */
size_t Client::writeFunction(
        void *ptr, size_t size, size_t nmemb, std::string *d )
{
    d->append( static_cast< char* >( ptr ), size * nmemb );
    return size * nmemb;
}

/**
 * @brief Clear all headers
 * @return Error code
 */
uint32_t Client::clearHeaders()
{
    uint32_t error = Error::Code::NONE;
    if( mHeaders ) {
        curl_slist_free_all( mHeaders );
        mHeaders = nullptr;
    }
    return error;
}

/**
 * @brief Applies URL to communicate with
 * @param url Desired URL
 * @return Error code
 */
uint32_t Client::applyUrl( const std::string &url )
{
    uint32_t error = Error::Code::NONE;
    mUrl = url;
    return error;
}

/**
 * @brief Appliers headers to send data with
 * @param headers Desired vector of headers
 * @return Error code
 */
uint32_t Client::applyHeaders( const std::vector<std::string> &headers )
{
    uint32_t error = Error::Code::NONE;
    for( auto i = headers.begin(); i != headers.end(); i++ ) {
        mHeaders = curl_slist_append( mHeaders, (*i).c_str() );
    }
    return error;
}

/**
 * @brief Applies the write function to record data
 * @param function Desired write function
 * @return Error code
 */
uint32_t Client::applyWriteFunction( WriteFunction *function )
{
    uint32_t error = Error::Code::NONE;
    mWriteFunction = function;
    return error;
}

/**
 * @brief Sends a message
 * @param str Message to send
 */
uint32_t Client::send( const char *str )
{
    uint32_t error = Error::Code::NONE;

    // Create the headers
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append( headers, "Content-Type: application/json" );
    headers = curl_slist_append( headers, "charset=utf-8" );

    // Set the options for cURL
    curl_easy_setopt( mCurl,           CURLOPT_URL, mUrl.c_str() );
    curl_easy_setopt( mCurl,    CURLOPT_HTTPHEADER, headers );
    curl_easy_setopt( mCurl,  CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1 );
    curl_easy_setopt( mCurl,    CURLOPT_POSTFIELDS, str );
    curl_easy_setopt( mCurl,      CURLOPT_NOSIGNAL, 0 );
    curl_easy_setopt( mCurl, CURLOPT_WRITEFUNCTION, Client::writeFunction );
    curl_easy_setopt( mCurl,     CURLOPT_WRITEDATA, &mDataString );

    // Set up for cookie usage
    curl_easy_setopt( mCurl, CURLOPT_COOKIESESSION, true );
    curl_easy_setopt( mCurl,     CURLOPT_COOKIEJAR, "cs-cookie" );
    curl_easy_setopt( mCurl,    CURLOPT_COOKIEFILE, "/tmp" );

    // Perform the cURL
    CURLcode res = curl_easy_perform( mCurl );

    if( res != CURLE_OK ) {
        // cURL failed
        fprintf( stderr, "curl_easy_perform() failed: %s\n",
                      curl_easy_strerror( res ) );
    } else {
        // Handle the response
        printf( "%s\n", mDataString.c_str() );
    }

    mDataString.clear();

    // Free the headers
    curl_slist_free_all( headers );

    return error;
}

/**
 * @brief Performs a GET request
 * @return Response string
 */
std::string Client::get()
{
    std::string response_string;
    if( mCurl ) {
        std::string header_string;
        curl_easy_setopt(mCurl, CURLOPT_URL, mUrl.c_str() );
        curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, mWriteFunction);
        curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(mCurl, CURLOPT_HEADERDATA, &header_string);

        char* url;
        long response_code;
        double elapsed;
        curl_easy_getinfo(mCurl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_getinfo(mCurl, CURLINFO_TOTAL_TIME, &elapsed);
        curl_easy_getinfo(mCurl, CURLINFO_EFFECTIVE_URL, &url);

        curl_easy_perform(mCurl);
    }

    return response_string;
}

}
