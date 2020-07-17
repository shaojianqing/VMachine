#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../common/constants.h"
#include "../common/commonType.h"

bool isEquals(const char* str1, const char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if (len1!=len2) {
        return false;
    }

    if (strcmp(str1, str2) == 0) {
        return true;
    } else {
        return false;
    }
}

bool isStartWith(const char* source, const char* prefix) {
    if (NULL == source || NULL == prefix) {
        return false;
    }

    int slen = strlen(source);
    int plen = strlen(prefix);

    if (slen < plen) {
        return false;
    }

    if (strncmp(source, prefix, plen) == 0) {
        return true;
    } else {
        return false;
    }
}

bool isEndWith(const char* source, const char* postfix) {
    if (NULL == source || NULL == postfix) {
        return false;
    }

    int slen = strlen(source);
    int plen = strlen(postfix);

    if (slen < plen) {
        return false;
    }

    if (strncmp(source + (slen - plen), postfix, plen) == 0) {
        return true;
    } else {
        return false;
    }
}

int indexOfString(const char* source, u32 start, char letter) {
    if (source==NULL) {
        return -1;
    }
    
    u32 length = strlen(source);
    if (start>=length) {
        return -1;
    }

    u32 i=start;
    for (i=start; i<length;++i) {
        if (source[i]==letter) {
            return i;
        }
    }
    return -1;
}