#include <wctype.h>
#include <ctype.h>

wint_t towupper(wint_t c) { return c < 128 ? (wint_t)toupper((int)c) : c; }
