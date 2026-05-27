#include <wchar.h>
#include <stdio.h>

wint_t putwchar(wchar_t c) { return fputwc(c, stdout); }
