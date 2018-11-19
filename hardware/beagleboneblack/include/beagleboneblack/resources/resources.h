/**
 * \brief For more details on this, review this stack overflow thread:
 * https://stackoverflow.com/questions/4864866/c-c-with-gcc-statically-add-resource-files-to-executable-library/4910421#4910421
 * */
#ifndef BEAGLEBONEBLACK_RESOURCES_H
#define BEAGLEBONEBLACK_RESOURCES_H

#include "common/resources/resources.h"

namespace Resources
{
    extern "C" char MAIN_JS[];
    extern "C" unsigned int MAIN_JS_SIZE;

    extern "C" const char INDEX_HTML[];
    extern "C" const unsigned int INDEX_HTML_SIZE;
}

#endif // BEAGLEBONEBLACK_RESOURCES_H
