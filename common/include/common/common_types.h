#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <map>
#include <unordered_map>
#include <string.h>

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

namespace Types {

struct CharArrayComparator
{
    bool operator()( const char *a, const char *b ) const;
};

struct CharArrayHash
{
    u_long operator()( const char *str ) const;
};

struct CharArrayHashComparator
{
    bool operator()( const char *a, const char *b ) const;
};

template< class T >
using CharHashMap = std::unordered_map< const char*, T, CharArrayHash, CharArrayHashComparator >;

template< class T >
using CharMap = std::map< const char*, T, CharArrayComparator >;

}


#endif // COMMON_TYPES_H
