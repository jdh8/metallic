#include <wctype.h>
#include <ctype.h>

int iswblank(wint_t c) { return c < 128 ? isblank((int)c) : 0; }
