#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int vasprintf(char** restrict out, const char* restrict format, va_list list)
{
    va_list copy;
    va_copy(copy, list);
    int n = vsnprintf((void*)0, 0, format, copy);
    va_end(copy);

    if (n < 0) {
        *out = (void*)0;
        return -1;
    }

    char* buf = malloc((size_t)n + 1);
    if (!buf) {
        *out = (void*)0;
        return -1;
    }

    int m = vsnprintf(buf, (size_t)n + 1, format, list);
    if (m < 0) {
        free(buf);
        *out = (void*)0;
        return -1;
    }

    *out = buf;
    return m;
}
