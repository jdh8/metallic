/* nearbyint differs from rint only in that it must not raise FE_INEXACT.
 *
 * On wasm32, src/math/double/rint.c already provides:
 *     double nearbyint(double) __attribute__((alias("rint")));
 * which is unconditionally correct because the WebAssembly floating-point
 * environment has no exception flags — see include/fenv.h, where every
 * fenv routine is a no-op shim.
 *
 * For non-wasm (native) builds, that alias is omitted, so we add a small
 * shim here. Since this libc's fenv is a no-op even on native, we can
 * delegate directly to rint without clearing flags.
 */
#ifndef __wasm__
double rint(double);

double nearbyint(double x)
{
    return rint(x);
}
#endif
