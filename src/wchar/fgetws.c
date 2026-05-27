#include <wchar.h>
#include <stdio.h>

wchar_t* fgetws(wchar_t* restrict s, int n, FILE* restrict stream)
{
    if (n <= 0)
        return (void*)0;

    int i = 0;
    while (i < n - 1) {
        wint_t c = fgetwc(stream);
        if (c == WEOF) {
            if (!i)
                return (void*)0;
            break;
        }
        s[i++] = (wchar_t)c;
        if (c == L'\n')
            break;
    }

    s[i] = L'\0';
    return s;
}
