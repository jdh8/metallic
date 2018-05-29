#include "../../assert.h"
#include "../../../src/math/reinterpret.h"
#include <math.h>
#include <float.h>
#include <limits.h>
#include <stdint.h>

typedef double Scalar;
typedef int64_t Signed;
typedef uint64_t Unsigned;

#define SUFFIX(x) (x)
#define LIMITS(identifier) (DBL_ ## identifier)

#include "frexp.h"

int main(void) {}
