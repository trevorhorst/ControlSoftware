#include "common/common_types.h"

void CharArrayCopy( char *dst, const char *src, size_t size )
{
    if( src == nullptr || src[ 0 ] == '\0' ) {
        strncpy( dst, "\0", 1 );
    } else {
        strncpy( dst, src, size );
        dst[ size - 1 ] = '\0';
    }
}

namespace Types {

/**
 * @brief Character array comparator
 * @param a First comparator
 * @param b Second comparator
 * @return bool returns true if the strings are equal
 */
bool CharArrayComparator::operator()( const char *a, const char *b ) const
{
    return strcmp( a, b ) < 0;
}

/**
 * @brief DJB2 hash algorithm
 * @param str Char array to hash
 * @return Integer representation of the hash value
 */
u_long CharArrayHash::operator()( const char *str ) const
{
    u_long hash = 5381;
    int c;

    while( ( c = *str++ ) ) {
        hash = ( ( hash << 5 ) + hash ) + c; /* hash * 33 + c */
    }

    return hash;
}

/**
 * @brief Character array hash comparator
 * @param a First comparator
 * @param b Second comparator
 * @return bool returns true if the strings are equal
 */
bool CharArrayHashComparator::operator()( const char *a, const char *b ) const
{
    return !strcmp( a, b );
}

}
