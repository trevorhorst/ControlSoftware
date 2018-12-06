#ifndef COMMAND_H
#define COMMAND_H

#include <string.h>
#include <vector>
#include <functional>
#include "common/cjson/cJSON.h"
#include "common/control/control.h"

#include "common/common_types.h"

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
    using ParameterCallback     = std::function< uint32_t (cJSON*) >;
    using ParameterMap          = CharHashMap< ParameterCallback >;
public:

    enum Type {
        MUTATOR = 0
        , ACCESSOR = 1
    };

    Command( const char *mutator , const char *accessor );
    virtual ~Command();
    virtual void addControlObject( Control* obj );
    // virtual cJSON *call( cJSON *params ) = 0;

    bool isMutable();
    bool isAccessible();


    const char *getMutatorName();
    const char *getAccessorName();

    virtual const char *usage();
    virtual cJSON *access( cJSON* params );
    virtual cJSON *mutate( cJSON* params );

    void bindMutatorCallback(const char *paramName
                                     , uint32_t (Command::*cb)(cJSON *) );
    void bindAccessorCallback( const char *paramName
                                     , uint32_t (Command::*cb)( cJSON * ) );

    uint32_t setVerbose( cJSON *val );
    uint32_t getVerbose( cJSON *response );

    void setError( uint32_t code, const char* details, cJSON *response );

protected:
    Control *mControlObject;
    std::vector< Control* > mCtrlObjList;
    virtual bool handleRequiredParameters( cJSON *params, cJSON *response );

    ParameterMap mRequiredMap;
    ParameterMap mAccessorMap;
    ParameterMap mMutatorMap;

private:
    char mUsage[ COMMAND_USAGE_MAX_SIZE ];
    char mAccessor[ COMMAND_NAME_MAX_SIZE ];
    char mMutator[ COMMAND_NAME_MAX_SIZE ];

    bool mAccessible;
    bool mMutable;
};

#endif // COMMAND_H
