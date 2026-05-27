#include <string.h>

/* C locale only: the transformed form equals the source.  Per C11
 * §7.24.4.5, return the length the full copy would require and write at
 * most n-1 chars plus a NUL when n is non-zero. */
size_t strxfrm(char* restrict dst, const char* restrict src, size_t n)
{
    size_t len = strlen(src);
    if (n) {
        size_t copy = len < n ? len : n - 1;
        memcpy(dst, src, copy);
        dst[copy] = '\0';
    }
    return len;
}
