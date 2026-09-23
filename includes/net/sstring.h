#ifndef SSTRING_H_INCLUDED
#define SSTRING_H_INCLUDED









#include <brickbox.h>


typedef struct _SString {
    char* string;
    u64 length;
} SString;



SString CoreCreateSString(_IN_ const char* string);
usize CoreSplitSString(_IN_ const SString str, _IN_ char delim, _IN_ usize length, _OUT_ SString* array);
void CoreSStringPuts(_IN_ const SString str);













#endif