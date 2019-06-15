typedef long Integer;

#include <limits.h>
#if LONG_MAX > INT_MAX
#define SCALBNF scalblnf
#include "scalbnf.h"
#endif
