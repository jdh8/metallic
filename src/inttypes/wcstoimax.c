#include <inttypes.h>
#include <wchar.h>

intmax_t wcstoimax(const wchar_t* restrict s, wchar_t** restrict end, int base)
{
    return wcstoll(s, end, base);
}
