#include "beagleboneblack/hardware_beagleboneblack.h"

BeagleboneBlack::BeagleboneBlack()
    : Hardware()
    , mIndexPage( nullptr )
{
    // mIndexPage = loadResource( ask, ask_size );
    // printf( "%s\n", ask );
    printf( "'%d'\n", resource_index_html[ resource_index_html_size ] );
}

BeagleboneBlack::~BeagleboneBlack()
{
    // unloadResource( mIndexPage );
}
