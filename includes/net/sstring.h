#ifndef SSTRING_H_INCLUDED
#define SSTRING_H_INCLUDED









#include <brickbox.h>


typedef struct _SString {
    char* string;
    u64 length;
} SString;


#ifdef __cplusplus
extern "C"
{
#endif
SString NetCreateSString(_IN_ const char* string);
usize NetSplitSString(_IN_ const SString str, _IN_ char delim, _IN_ usize length, _OUT_ SString* array);
void NetSStringPuts(_IN_ const SString str);
#ifdef __cplusplus
}
#endif












#endif
