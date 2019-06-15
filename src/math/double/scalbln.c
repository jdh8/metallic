typedef long Integer;

#include <limits.h>
#if LONG_MAX > INT_MAX
#define SCALBN scalbln
#include "scalbn.h"
#endif
