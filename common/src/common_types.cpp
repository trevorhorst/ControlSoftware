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


// namespace Common
// {
//
// /**
//  * @brief The character array comparator
//  */
// struct CharArrayCompare
// {
//     bool operator()( const char *a, const char *b ) const
//     {
//         return !strcmp( a, b );
//     }
// };
//
// struct CharArrayHash
// {
//     /**
//      * @brief DJB2 hash algorithm
//      * @param str Char array to hash
//      * @return Integer representation of the hash value
//      */
//     unsigned long operator()( const char *str ) const
//     {
//         unsigned long hash = 5381;
//         int c;
//
//         while( ( c = *str++ ) ) {
//             hash = ( ( hash << 5 ) + hash ) + c; /* hash * 33 + c */
//         }
//
//         return hash;
//     }
// };
//
// void CharArrayCopy( char *dst, const char *src, size_t size )
// {
//     if( src == nullptr && src[ 0 ] == '\0' ) {
//         strncpy( dst, "\0", 1 );
//     } else {
//         strncpy( dst, src, size );
//         dst[ size - 1 ] = '\0';
//     }
// }
//
// }
