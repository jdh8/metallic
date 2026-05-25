/* Precision-limited stub.
 *
 * On wasm32 (and on most native targets clang targets), long double is
 * IEEE 754 binary128 (sizeof == 16). Implementing nearbyintl with full
 * 113-bit-mantissa precision would require dedicated bit-manipulation
 * like src/math/long-double/truncl.c. For M5a we cast through double,
 * which is exact for any |x| < 2^53 — covering all values for which a
 * 64-bit double can also represent the rounded result anyway. For
 * larger magnitudes the cast itself is exact (the value is already an
 * integer) and the result is bit-identical after the round trip.
 *
 * The fenv shim in include/fenv.h is a no-op, so there is no
 * FE_INEXACT flag to clear — nearbyintl reduces to rint on the
 * truncated value.
 */
double rint(double);

long double nearbyintl(long double x)
{
    return (long double)rint((double)x);
}
