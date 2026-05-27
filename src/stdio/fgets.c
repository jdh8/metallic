#include "getc.h"
#include <stdio.h>

char* fgets(char* restrict s, int n, FILE* restrict stream)
{
    if (n <= 0)
        return (void*)0;

    int i = 0;
    while (i < n - 1) {
        int c = getc_(stream);
        if (c == EOF) {
            if (!i)
                return (void*)0;
            break;
        }
        s[i++] = (char)c;
        if (c == '\n')
            break;
    }

    s[i] = '\0';
    return s;
}
