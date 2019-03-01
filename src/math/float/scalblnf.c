#include <limits.h>

#if LONG_MAX > INT_MAX

#define SCALBNF scalblnf

typedef long Integer;

#include "scalbnf.h"

#endif
