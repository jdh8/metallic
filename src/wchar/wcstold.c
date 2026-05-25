#include <wchar.h>

/* On wasm32 with clang, `long double` == `double`, so wcstold reduces to
 * a wcstod call. */
long double wcstold(const wchar_t* s, wchar_t** end)
{
    return (long double)wcstod(s, end);
}
