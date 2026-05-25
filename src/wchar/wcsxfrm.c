#include <wchar.h>

size_t wcsxfrm(wchar_t* restrict destination, const wchar_t* restrict source, size_t length)
{
    /* C locale: transformation == identity. Write up to length wchars (including NUL). */
    size_t n = wcslen(source);

    if (length) {
        size_t copy = n < length ? n : length - 1;

        for (size_t i = 0; i < copy; ++i)
            destination[i] = source[i];

        destination[copy] = 0;
    }

    return n;
}
