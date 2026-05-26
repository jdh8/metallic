#include <locale.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

int main(void)
{
    /* Query mode: NULL locale returns the active locale name, which is
     * always "C" under metallic. */
    char* name = setlocale(LC_ALL, NULL);
    assert(name != NULL);
    assert(strcmp(name, "C") == 0);

    /* Setting to "C" or "" must succeed. */
    assert(strcmp(setlocale(LC_ALL, "C"), "C") == 0);
    assert(strcmp(setlocale(LC_NUMERIC, ""), "C") == 0);
    assert(strcmp(setlocale(LC_TIME, "POSIX"), "C") == 0);

    /* Any other locale is unsupported. */
    assert(setlocale(LC_ALL, "en_US.UTF-8") == NULL);

    /* Out-of-range categories yield NULL. */
    assert(setlocale(-1, NULL) == NULL);
    assert(setlocale(99, NULL) == NULL);

    struct lconv* lc = localeconv();
    assert(lc != NULL);
    assert(strcmp(lc->decimal_point, ".") == 0);
    assert(strcmp(lc->thousands_sep, "") == 0);
    assert(strcmp(lc->grouping, "") == 0);
    assert(strcmp(lc->currency_symbol, "") == 0);
    assert(lc->frac_digits     == CHAR_MAX);
    assert(lc->p_cs_precedes   == CHAR_MAX);
    assert(lc->int_frac_digits == CHAR_MAX);

    return 0;
}
