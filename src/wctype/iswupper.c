#include <wctype.h>
#include <ctype.h>

int iswupper(wint_t c) { return c < 128 ? isupper((int)c) : 0; }
