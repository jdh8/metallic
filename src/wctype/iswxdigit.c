#include <wctype.h>
#include <ctype.h>

int iswxdigit(wint_t c) { return c < 128 ? isxdigit((int)c) : 0; }
