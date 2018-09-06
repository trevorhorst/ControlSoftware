#include <string.h>

#include "beagleboneblack/resources/resources.h"

/**
 * @brief Loads a resource file as a string
 * @param resource Pointer
 * @param resourceSize
 * @note Do we actually need to do this? Not sure if the file we read in is
 * guaranteed to be null terminated. The files tested so far have all been null
 * terminated
 * @return
 */
char *loadResource( char *resource, unsigned int resourceSize )
{
    char *r = new char[ resourceSize + 1 ];
    memcpy( r, resource, resourceSize );
    r[ resourceSize ] = '\0';
    return r;
}

/**
 * @brief unloadResource
 * @param resource
 */
void unloadResource( char *resource )
{
    if( resource ) {
        delete[] resource;
    }
}
