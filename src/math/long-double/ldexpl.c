/* Precision-limited stub.
 *
 * long double on this target is IEEE 754 binary128 (sizeof == 16), so
 * a fully precise ldexpl would mirror src/math/double/scalbn.h with
 * 128-bit integer reinterpret. For M5a we cast through double; scaling
 * a double by an integer power of two is exact except for overflow,
 * underflow, and subnormal flush — all of which behave identically
 * once the result is widened back to long double, since long double
 * has strictly more range and precision than double.
 *
 * Programs needing the extra range/precision of binary128 should not
 * rely on this until M5b promotes it to a real implementation.
 */
double scalbn(double, int);

long double ldexpl(long double x, int n)
{
    return (long double)scalbn((double)x, n);
}
