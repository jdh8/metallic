#include <wctype.h>
#include <ctype.h>

wint_t towlower(wint_t c) { return c < 128 ? (wint_t)tolower((int)c) : c; }
