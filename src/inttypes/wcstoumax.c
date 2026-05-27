#include <inttypes.h>
#include <wchar.h>

uintmax_t wcstoumax(const wchar_t* restrict s, wchar_t** restrict end, int base)
{
    return wcstoull(s, end, base);
}
