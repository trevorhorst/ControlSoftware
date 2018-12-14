#include <cstdlib>
#include <string.h>

#include "common/cjson/cJSON.h"
#include "common/http/client/client.h"

/**
 * @brief Constructor
 */
HttpClient::Data::Data()
    : mData( nullptr )
    , mSize( 0 )
{
}

/**
 * @brief Destructor
 */
HttpClient::Data::~Data()
{
    if( mData ) {
        delete[] mData;
    }
}

/**
 * @brief Read what has been written to the data block
 * @return Pointer to the data block
 */
const char *HttpClient::Data::read()
{
    return mData;
}

/**
 * @brief Writes to our data block
 * @param data Data to write
 * @param size Size of the data to write
 */
void HttpClient::Data::write( const char *data, size_t size )
{
    // Increase the size of our data
    mSize += size;
    // Save off a pointer to the old data
    char *t = mData;
    // Create a new block of memory
    mData = new char[ mSize + 1 ];
    // Copy the old data to the new block
    memcpy( static_cast< void* >( mData ), t, mSize - size );
    // Copy the new data to the end of the new block
    char *n = &mData[ mSize - size ];
    memcpy( static_cast< void* >( n ), data, size );
    // Append the null character
    mData[ mSize ] = '\0';

    if( t ) {
        // Delete the pointer to the old data block
        delete[] t;
    }
}

/**
 * @brief Constructor
 * @param address Address to
 * @param port
 */
HttpClient::HttpClient( const char *address , uint16_t port )
    : mCurl( nullptr )
    , mPort( port )
    , mResponse( nullptr )
    , mResponseSize( 0 )
{
    curl_global_init( CURL_GLOBAL_ALL );
    mCurl = curl_easy_init();
    if( address == nullptr || address[ 0 ] == '\0' ) {
        snprintf( mUrl
                  , HTTP_URL_SIZE_MAX
                  , HTTP_URL_TEMPLATE
                  , HTTP_LOCALHOST
                  , port );
    } else {
        snprintf( mUrl
                  , HTTP_URL_SIZE_MAX
                  , HTTP_URL_TEMPLATE
                  , address
                  , port );
    }
}

/**
 * @brief Destructor
 */
HttpClient::~HttpClient()
{
    curl_easy_cleanup( mCurl );
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
size_t HttpClient::receive(
        void *ptr, size_t size, size_t nmemb, struct Data *d )
{
    d->write( static_cast< char* >( ptr ), size * nmemb );
    return size * nmemb;
}

/**
 * @brief Sends a message
 * @param str Message to send
 */
void HttpClient::send( const char *str )
{
    // Create the headers
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append( headers, "Content-Type: application/json" );
    headers = curl_slist_append( headers, "charset=utf-8" );

    // Set the options for cURL
    curl_easy_setopt( mCurl,           CURLOPT_URL, mUrl );
    curl_easy_setopt( mCurl,    CURLOPT_HTTPHEADER, headers );
    curl_easy_setopt( mCurl,  CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1 );
    curl_easy_setopt( mCurl,    CURLOPT_POSTFIELDS, str );
    curl_easy_setopt( mCurl,      CURLOPT_NOSIGNAL, 0 );
    curl_easy_setopt( mCurl, CURLOPT_WRITEFUNCTION, HttpClient::receive );
    curl_easy_setopt( mCurl,     CURLOPT_WRITEDATA, &mData );

    // Perform the cURL
    CURLcode res = curl_easy_perform( mCurl );

    if( res != CURLE_OK ) {
        // cURL failed
        fprintf( stderr, "curl_easy_perform() failed: %s\n",
                      curl_easy_strerror( res ) );
    } else {
        // Handle the response
        printf( "%s\n", mData.read() );
    }

    // Free the headers
    curl_slist_free_all( headers );
}
