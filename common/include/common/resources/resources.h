#ifndef RESOURCES_H
#define RESOURCES_H

#include <string.h>

namespace Resources
{
    const char *load( const char *resource, unsigned int size );
    void unload( const char *resource );
}

#endif // RESOURCES_H
