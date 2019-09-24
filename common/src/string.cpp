#include "common/string.h"

const size_t String::max_size = 4096;

const char String::null_char = '\0';

/**
 * @brief Constructor
 * @param str (Optional) Desired initializer string
 * @param len (Optional) Desired initializer length
 */
String::String( const char *str, size_t len )
    : mString( nullptr )
    , mLength( 0 )
{
    append( str, len );
}

/**
 * @brief Clears all string data
 */
void String::clear()
{
    if( mString ) {
        delete[] mString;
        mString = nullptr;
        mLength = 0;
    }
}

/**
 * @brief Appends a string to the existing data
 * @param str Desired string to append
 * @param len (Optional) Length of the desired string
 */
void String::append( const char *str, const size_t len )
{
    if( str != nullptr && str[ 0 ] != null_char ) {
        // Increase the len of our str
        if( len > 0 ) {
            mLength += len;
        } else {
            mLength += strlen( str );
        }
        // Save off a pointer to the old str
        char *t = mString;
        // Create a new block of memory
        mString = new char[ mLength + 1 ];
        // Copy the old str to the new block
        memcpy( static_cast< void* >( mString ), t, mLength - len );
        // Copy the new str to the end of the new block
        char *n = &mString[ mLength - len ];
        memcpy( static_cast< void* >( n ), str, len );
        // Append the null character
        mString[ mLength ] = '\0';

        if( t ) {
            // Delete the pointer to the old str block
            delete[] t;
        }
    }
}
