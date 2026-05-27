#include <wctype.h>
#include <ctype.h>

int iswlower(wint_t c) { return c < 128 ? islower((int)c) : 0; }
