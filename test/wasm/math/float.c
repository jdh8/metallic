#include "../../assert.h"
#include "../../../src/math/reinterpret.h"
#include <math.h>
#include <float.h>
#include <limits.h>
#include <stdint.h>

typedef float Scalar;
typedef int32_t Signed;
typedef uint32_t Unsigned;

#define SUFFIX(x) (x ## f)
#define LIMITS(identifier) (FLT_ ## identifier)

#include "log.h"
#include "frexp.h"
#include "scalbn.h"

int main(void) {}
