#ifndef STRING_H
#define STRING_H

#include <string.h>

/**
 * @brief The common string class, to be used in place of the c++ string and so
 * I don't have to keep performing common string operations manually
 */
class String
{
    static const size_t max_size;
    static const char null_char;
public:

    String( const char *str = nullptr, size_t len = 0 );
    ~String();

    String& operator=(const char *str);
    String& operator=(const String &str);

    const char *getData();
    size_t getLength();

    void append( const char *str, const size_t len = 0 );
    void clear();

protected:
private:
    char *mString;
    size_t mLength;
};

#endif // STRING_H
