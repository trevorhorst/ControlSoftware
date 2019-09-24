/**
 * \brief For more details on this, review this stack overflow thread:
 * https://stackoverflow.com/questions/4864866/c-c-with-gcc-statically-add-resource-files-to-executable-library/4910421#4910421
 * */
#ifndef BEAGLEBONEBLACK_RESOURCES_H
#define BEAGLEBONEBLACK_RESOURCES_H

#include "common/resources/resources.h"

namespace Resources
{
    extern "C" char bundle_js[];
    extern "C" unsigned int bundle_js_size;

    extern "C" const char index_html[];
    extern "C" const unsigned int index_html_size;
}

#endif // BEAGLEBONEBLACK_RESOURCES_H
