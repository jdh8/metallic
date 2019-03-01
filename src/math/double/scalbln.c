#include <limits.h>

#if LONG_MAX > INT_MAX

#define SCALBN scalbln

typedef long Integer;

#include "scalbn.h"

#endif
