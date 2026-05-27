#include <wctype.h>
#include <ctype.h>

int iswpunct(wint_t c) { return c < 128 ? ispunct((int)c) : 0; }
