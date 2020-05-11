#include <string.h>

#include "http/http.h"

namespace Http
{
const char *localhost     = "localhost";

const uint16_t default_port     = 8080;

const char *method_strings[] = {
    "GET"
    , "POST"
    , "PUT"
    , "PATCH"
    , "DELETE"
    , nullptr
};

Method stringToMethod( const char *method )
{
    Method methodId = Method::UNKNOWNMETHOD;
    for( int i = 0; method_strings[i] != nullptr; i++ ) {
        if( strcmp( method_strings[ i ], method ) == 0 ) {
            methodId = static_cast< Method >( i );
        }
    }
    return methodId;
}

const char *methodToString( Method method )
{
    int m = method;
    if( m >= 0 && m < 6 ) {
        return method_strings[ m ];
    }
    return nullptr;
}

HttpVersion stringToHttpVersion( const char *version )
{
    HttpVersion httpVersion = HttpVersion::UNKNOWNVERSION;
    if( strcmp( "HTTP/1.0", version ) == 0 ) {
        httpVersion = HttpVersion::HTTP_1_0;
    } else if( strcmp( "HTTP/1.1", version ) ) {
        httpVersion = HttpVersion::HTTP_1_1;
    }
    return httpVersion;
}

/**
 * @brief Body Constructor
 */
Body::Body()
    : mData( nullptr )
    , mSize( 0 )
{
}

/**
 * @brief Body Destructor
 */
Body::~Body()
{
    clear();
}

/**
 * @brief Get the data
 * @return Pointer to the data
 */
const char *Body::getData()
{
    return mData;
}

uint32_t Body::getSize()
{
    return mSize;
}

/**
 * @brief Append data to the body
 * @param data Pointer to the data
 * @param size Size of the data
 */
void Body::append( const char *data, size_t size )
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
 * @brief Clear the body data
 */
void Body::clear()
{
    if( mData ) {
        delete[] mData;
        mData = nullptr;
        mSize = 0;
    }
}

}
