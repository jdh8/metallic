#include <stdio.h>

/* Metallic's FILE has no userland write buffer to install (only the small
 * ungetc cache).  Validate the mode and return success; the size and buffer
 * arguments are intentionally ignored. */
int setvbuf(FILE* restrict stream, char* restrict buf, int mode, size_t size)
{
    (void)stream;
    (void)buf;
    (void)size;

    if (mode != _IONBF && mode != _IOLBF && mode != _IOFBF)
        return 1;

    return 0;
}
