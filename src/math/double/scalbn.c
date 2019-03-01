#define SCALBN scalbn

typedef int Integer;

#include "scalbn.h"
#include <limits.h>

double ldexp(double, int) __attribute__((alias("scalbn")));

#if LONG_MAX == INT_MAX
double scalbln(double, long) __attribute__((alias("scalbn")));
#endif
