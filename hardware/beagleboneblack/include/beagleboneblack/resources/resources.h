/**
 * \brief For more details on this, review this stack overflow thread:
 * https://stackoverflow.com/questions/4864866/c-c-with-gcc-statically-add-resource-files-to-executable-library/4910421#4910421
 * */
#ifndef RESOURCES_H
#define RESOURCES_H

extern char resource_main_js[];
extern unsigned int resource_main_js_size;

extern char resource_index_html[];
extern unsigned int resource_index_html_size;

char *loadResource( char *resource, unsigned int resourceSize );
void unloadResource( char *resource );

#endif //  RESOURCES_H
