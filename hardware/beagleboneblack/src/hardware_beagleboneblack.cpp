#include "beagleboneblack/hardware_beagleboneblack.h"

BeagleboneBlack::BeagleboneBlack()
    : Hardware()
    , mIndexPage( nullptr )
{
    // mIndexPage = loadResource( ask, ask_size );
    // printf( "%s\n", ask );
    printf( "index_html:    size: %d\t\tlast: %02X\n"
            , resource_index_html_size, resource_index_html[ resource_index_html_size ] );
    printf( "   main.js:    size: %d\t\tlast: %02X\n"
            , resource_main_js_size, resource_main_js[ resource_main_js_size ] );
}

BeagleboneBlack::~BeagleboneBlack()
{
    // unloadResource( mIndexPage );
}
