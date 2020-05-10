#include <cstdlib>
#include <string.h>

#include "common/cjson/cJSON.h"
#include "http/client.h"

namespace NewHttp {

/**
 * @brief Constructor
 */
Client::DataString::DataString()
    : mDataString( nullptr )
    , mSize( 0 )
{
}

/**
 * @brief Destructor
 */
Client::DataString::~DataString()
{
    clear();
}

/**
 * @brief Read what has been written to the data block
 * @return Pointer to the data block
 */
const char *Client::DataString::read()
{
    return mDataString;
}

/**
 * @brief Writes to our data block
 * @param data DataString to write
 * @param size Size of the data to write
 */
void Client::DataString::write( const char *data, size_t size )
{
    // Increase the size of our data
    mSize += size;
    // Save off a pointer to the old data
    char *t = mDataString;
    // Create a new block of memory
    mDataString = new char[ mSize + 1 ];
    // Copy the old data to the new block
    memcpy( static_cast< void* >( mDataString ), t, mSize - size );
    // Copy the new data to the end of the new block
    char *n = &mDataString[ mSize - size ];
    memcpy( static_cast< void* >( n ), data, size );
    // Append the null character
    mDataString[ mSize ] = '\0';

    if( t ) {
        // Delete the pointer to the old data block
        delete[] t;
    }
}

/**
 * @brief Clears the data
 */
void Client::DataString::clear()
{
    if( mDataString ) {
        mSize = 0;
        delete[] mDataString;
        mDataString = nullptr;
    }
}

/**
 * @brief Constructor
 * @param address Address to
 * @param port
 */
Client::Client( const char *address , uint16_t port )
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
Client::~Client()
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
size_t Client::receive(
        void *ptr, size_t size, size_t nmemb, struct DataString *d )
{
    d->write( static_cast< char* >( ptr ), size * nmemb );
    return size * nmemb;
}

/**
 * @brief Sends a message
 * @param str Message to send
 */
void Client::send( const char *str )
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
    curl_easy_setopt( mCurl, CURLOPT_WRITEFUNCTION, Client::receive );
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
        printf( "%s\n", mDataString.read() );
    }

    mDataString.clear();

    // Free the headers
    curl_slist_free_all( headers );
}

}
