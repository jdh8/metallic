#define SCALBNF scalbnf

typedef int Integer;

#include "scalbnf.h"
#include <limits.h>

float ldexpf(float, int) __attribute__((alias("scalbnf")));

#if LONG_MAX == INT_MAX
float scalblnf(float, long) __attribute__((alias("scalbnf")));
#endif
