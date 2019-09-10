#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <map>
#include <unordered_map>
#include <string.h>


struct CharArrayCompare
{
    bool operator()( const char *a, const char *b ) const
    {
        return !strcmp( a, b );
    }
};

/**
 * @brief The CharArrayHash struct
 */
struct CharArrayHash
{
    /**
     * @brief DJB2 hash algorithm
     * @param str Char array to hash
     * @return Integer representation of the hash value
     */
    unsigned long operator()( const char *str ) const
    {
        unsigned long hash = 5381;
        int c;

        while( ( c = *str++ ) ) {
            hash = ( ( hash << 5 ) + hash ) + c; /* hash * 33 + c */
        }

        return hash;
    }
};

template< typename C >
using CharHashMap = std::unordered_map< const char*, C, CharArrayHash, CharArrayCompare >;

void CharArrayCopy( char *dst, const char *src, size_t size );

// namespace Common
// {
//
// struct CharArrayCompare;
// struct CharArrayHash;
// template< typename C > using CharHashMap
//     = std::unordered_map< const char*, C, CharArrayHash, CharArrayCompare >;
//
// void CharArrayCopy( char *dst, const char *src, size_t size );
//
// }

#define COMMAND_CONSOLE     "console"
#define COMMAND_TUNER       "tuner"
#define COMMAND_QTUNER      "qtuner"
#define COMMAND_QUIT        "quit"

#define PARAM_CODE      "code"
#define PARAM_COMMAND   "cmd"
#define PARAM_DETAILS   "details"
#define PARAM_ENABLE    "enable"
#define PARAM_ERROR     "error"
#define PARAM_PARAMS    "params"
#define PARAM_QUIT      "quit"
#define PARAM_RESULT    "result"
#define PARAM_SUCCESS   "success"
#define PARAM_TYPE      "type"
#define PARAM_VERBOSE   "verbose"


#endif // COMMON_TYPES_H
