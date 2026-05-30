/* Lanczos approximation for the gamma function (g=7, n=9).
 *
 * Coefficients from Boost C++ Library (boost/math/special_functions/lanczos.hpp),
 * which in turn follow the derivation in:
 *   Lanczos, C. (1964). "A precision approximation of the gamma function."
 *   SIAM Journal on Numerical Analysis B 1: 86–96.
 *
 * For Re(z) > 0:
 *   Γ(z) = sqrt(2π) * ((z + g - 0.5) / e)^(z - 0.5) * A_g(z - 1)
 *
 * where A_g(z) = sum_{k=0}^{8} c[k] / (z + k), and g = 7.
 *
 * The approximation error is < 1e-15 relative, giving ~50 bits accuracy.
 */
#ifndef METALLIC_KERNEL_LANCZOS_DOUBLE_H
#define METALLIC_KERNEL_LANCZOS_DOUBLE_H

static const double lanczos_g_ = 7.0;

/* lanczos_sum_(z) = A_g(z) = c[0] + sum c[k]/(z+k) for k=1..8.
 * This is the partial-fractions rational approximation WITHOUT the sqrt(2π) factor.
 * Call with z = x - 1.  Used in lgamma where we take log(A_g) separately. */
static double lanczos_sum_(double z)
{
    /* Boost lanczos13m53 g=7 n=9 coefficients. */
    static const double c[] = {
         0.99999999999980993,
       676.5203681218851,
     -1259.1392167224028,
       771.32342877765313,
      -176.61502916214059,
        12.507343278686905,
        -0.13857109526572012,
         9.9843695780195716e-6,
         1.5056327351493116e-7,
    };

    double s = c[0];
    s += c[1] / (z + 1);
    s += c[2] / (z + 2);
    s += c[3] / (z + 3);
    s += c[4] / (z + 4);
    s += c[5] / (z + 5);
    s += c[6] / (z + 6);
    s += c[7] / (z + 7);
    s += c[8] / (z + 8);
    return s;
}

#endif /* METALLIC_KERNEL_LANCZOS_DOUBLE_H */
