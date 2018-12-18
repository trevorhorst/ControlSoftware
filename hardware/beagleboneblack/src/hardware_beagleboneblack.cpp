#include "beagleboneblack/hardware_beagleboneblack.h"

BeagleboneBlack::BeagleboneBlack()
    : Hardware()
{
    printf( "index_html:    size: %d\t\tlast: %02X\n"
            , Resources::INDEX_HTML_SIZE
            , Resources::INDEX_HTML[ Resources::INDEX_HTML_SIZE - 1] );

    printf( "   main.js:    size: %d\t\tlast: %02X\n"
            , Resources::MAIN_JS_SIZE
            , Resources::MAIN_JS[ Resources::MAIN_JS_SIZE - 1 ] );

}

BeagleboneBlack::~BeagleboneBlack()
{
}
