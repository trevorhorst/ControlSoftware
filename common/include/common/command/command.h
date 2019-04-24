#ifndef COMMAND_H
#define COMMAND_H

#include <string.h>
#include <vector>
#include <functional>
#include "common/cjson/cJSON.h"
#include "common/control/control.h"

#include "common/common_types.h"

#define COMMAND_EMPTY   ""

#define COMMAND_USAGE_MAX_SIZE 1024
#define COMMAND_NAME_MAX_SIZE 64

#define PARAMETER_CALLBACK( X ) std::bind( X, this, std::placeholders::_1 )

#define BIND_PARAMETER( X ) do { \
    std::bind( X, this, std::placeholders::_1 ); \
} while( true )

/**
 * @brief The Command class
 */
class Command
{
public:
    using ParameterCallback     = std::function< uint32_t (cJSON*) >;
    using ParameterMap          = CharHashMap< ParameterCallback >;

    enum Type {
        MUTATOR = 0
        , ACCESSOR = 1
    };

    Command( const char *mutator , const char *accessor );
    virtual ~Command();

    bool isMutable();
    bool isAccessible();

    const char *getMutatorName();
    const char *getAccessorName();

    virtual const char *usage();
    virtual cJSON *access( cJSON* params ) = 0;
    virtual cJSON *mutate( cJSON* params ) = 0;

    virtual uint32_t setVerbose( cJSON *val ) = 0;
    virtual uint32_t getVerbose( cJSON *response ) = 0;

    void setError( uint32_t code, const char* details, cJSON *response );

protected:

    std::vector< Control* > mCtrlObjList;
    virtual bool handleRequiredParameters( cJSON *params, cJSON *response );
    virtual bool handleOptionalParameters( cJSON *params, cJSON *response );

    ParameterMap mRequiredMap;
    ParameterMap mOptionalMap;
    ParameterMap mAccessorMap;
    ParameterMap mMutatorMap;

    static const char *error_control_unavailable;

private:
    char mUsage[ COMMAND_USAGE_MAX_SIZE ];
    char mAccessor[ COMMAND_NAME_MAX_SIZE ];
    char mMutator[ COMMAND_NAME_MAX_SIZE ];

    bool mAccessible;
    bool mMutable;
};

#endif // COMMAND_H
