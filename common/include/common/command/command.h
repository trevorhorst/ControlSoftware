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

    Command( const char *mutator, const char *accessor, const char *optional );
    virtual ~Command();

    bool isMutable();
    bool isAccessible();

    const char *getMutatorName();
    const char *getAccessorName();

    virtual const char *usage();
    virtual bool call( cJSON *params, cJSON *response, Type ) = 0;
    virtual bool access( cJSON* params, cJSON *response ) = 0;
    virtual bool mutate( cJSON* params, cJSON *response ) = 0;

    virtual uint32_t setVerbose( cJSON *val ) = 0;
    virtual uint32_t getVerbose( cJSON *response ) = 0;

    void setError( uint32_t code, const char* details, cJSON *response );

protected:

    std::vector< Control* > mCtrlObjList;

    virtual uint32_t handleRequiredParameters( cJSON *params, const char *&details );
    virtual uint32_t handleOptionalParameters( cJSON *params, const char *&details );

    ParameterMap mRequiredMap;
    ParameterMap mAccessorMap;
    ParameterMap mMutatorMap;

    ParameterCallback mOptional;
    ParameterMap mOptionalAccessorMap;
    ParameterMap mOptionalMutatorMap;

    char mOptionalParameter[ COMMAND_NAME_MAX_SIZE ];

    static const char *error_control_unavailable;

private:
    bool mAccessible;
    bool mMutable;

    char mUsage[ COMMAND_USAGE_MAX_SIZE ];
    char mAccessor[ COMMAND_NAME_MAX_SIZE ];
    char mMutator[ COMMAND_NAME_MAX_SIZE ];

};

#endif // COMMAND_H
