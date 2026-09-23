#include <net/sstring.h>
#include <string.h>
#include <string.h>
#include <stdio.h>







SString NetCreateSString(_IN_ const char* string){
    SString s = {0};
    s.string = (char*)string;
    s.length = strlen(string);
    return s;
}

void NetSStringPuts(_IN_ const SString str){
    for(usize i = 0; i < str.length; i++){
        putchar(str.string[i]);
    }
}


usize NetSplitSString(_IN_ const SString str, _IN_ char delim, _IN_ usize length, _OUT_ SString* array){
    usize cur = 0;
    usize curOffset = 0;
    usize i = 0;
    for(i = 0; i < str.length; i++){
        if(cur >= length){
            break;
        }
        if(str.string[i] == delim){
            array[cur].string = (char*)((u64)str.string + curOffset);
            array[cur].length = (i) - curOffset;
            curOffset = (i + 1);
            cur++;
        }
    }
    array[cur].string = (char*)((u64)str.string + curOffset);
    array[cur].length = (i) - curOffset;
    curOffset = (i + 1);
    cur++;
    return cur;
}
