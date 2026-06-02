#ifndef METALLIC_MATH_FLOAT_LOG2_DD_H
#define METALLIC_MATH_FLOAT_LOG2_DD_H

/* log2(x) as a double-double for finite positive x, ~2^-94 accurate.  Shared by
 * the correctly-rounded float kernels that need a high-precision logarithm
 * (powf, clogf). */

#include "dd.h"
#include <stdint.h>

/* Double-double minimax of 2·log₂e·atanh(t)/t in u = t², low-degree first.
 * Degree 12, max error 2⁻⁹⁴ on u ∈ [0, 0.0295]. */
static const sum_t log2_dd_c_[13] = {
    { 2.8853900817779268,    4.07105474819096e-17   },
    { 0.9617966939259756,    5.057761610242735e-17  },
    { 0.5770780163555853,    5.255206816760004e-17  },
    { 0.4121985831111324,    1.297102661193637e-17  },
    { 0.3205988979753255,    2.1308118804202e-17    },
    { 0.2623081892524695,   -1.7505650820346252e-17 },
    { 0.22195308322393692,   4.474037836742117e-18  },
    { 0.192359337770499,     9.55737259836456e-18   },
    { 0.16972889698290997,  -6.916940455706479e-18  },
    { 0.15185945519033492,   4.8616033883771066e-18 },
    { 0.1374986651027388,    9.912843223531668e-18  },
    { 0.12347206144630882,  -3.215256324452949e-18  },
    { 0.13805426621912137,   1.1998429340524104e-18 },
};

/* log₂(x) as a double-double for finite positive normal x. */
static inline sum_t log2_dd_(double x)
{
    int64_t i = reinterpret(int64_t, x);
    int64_t exponent = (i - (int64_t)0x3FE6A09E667F3BCDLL) >> 52;
    double m = reinterpret(double, i - (exponent << 52));
    sum_t t = dd_quotient_(m - 1.0, m + 1.0);
    sum_t log2_m = dd_mul_dd_(t, poly_dd_(dd_mul_dd_(t, t), log2_dd_c_, 13));
    return dd_add_((sum_t){ (double)exponent, 0.0 }, log2_m);
}

#endif
