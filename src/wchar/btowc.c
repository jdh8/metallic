#include <wchar.h>
#include <stdio.h>

wint_t btowc(int c)
{
    return c == EOF || (unsigned)c >= 0x80 ? WEOF : (wint_t)(unsigned char)c;
}
