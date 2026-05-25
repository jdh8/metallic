#include <wchar.h>
#include <stdio.h>

int wctob(wint_t c)
{
    return c < 0x80 ? (int)c : EOF;
}
