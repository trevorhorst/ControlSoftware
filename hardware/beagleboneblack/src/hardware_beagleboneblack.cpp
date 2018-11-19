#include "beagleboneblack/hardware_beagleboneblack.h"

BeagleboneBlack::BeagleboneBlack()
    : Hardware()
    , mIndexPage( nullptr )
{
    mIndexPage = Resources::load(
                Resources::INDEX_HTML, Resources::INDEX_HTML_SIZE );

    printf( "index_html:    size: %d\t\tlast: %02X\n"
            , Resources::INDEX_HTML_SIZE
            , Resources::INDEX_HTML[ Resources::INDEX_HTML_SIZE ] );

    printf( "   main.js:    size: %d\t\tlast: %02X\n"
            , Resources::MAIN_JS_SIZE
            , Resources::MAIN_JS[ Resources::MAIN_JS_SIZE ] );
}

BeagleboneBlack::~BeagleboneBlack()
{
    // unloadResource( mIndexPage );
}
