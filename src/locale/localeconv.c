#include <locale.h>
#include <limits.h>

/* C11 §7.11.2.1: the C locale's lconv has "." for decimal_point, empty
 * strings for everything else, and CHAR_MAX in every char field to
 * signal "value not available in this locale". */
struct lconv* localeconv(void)
{
    static struct lconv c_locale = {
        .decimal_point      = (char*)".",
        .thousands_sep      = (char*)"",
        .grouping           = (char*)"",

        .mon_decimal_point  = (char*)"",
        .mon_thousands_sep  = (char*)"",
        .mon_grouping       = (char*)"",
        .positive_sign      = (char*)"",
        .negative_sign      = (char*)"",
        .currency_symbol    = (char*)"",
        .frac_digits        = CHAR_MAX,
        .p_cs_precedes      = CHAR_MAX,
        .n_cs_precedes      = CHAR_MAX,
        .p_sep_by_space     = CHAR_MAX,
        .n_sep_by_space     = CHAR_MAX,
        .p_sign_posn        = CHAR_MAX,
        .n_sign_posn        = CHAR_MAX,

        .int_curr_symbol    = (char*)"",
        .int_frac_digits    = CHAR_MAX,
        .int_p_cs_precedes  = CHAR_MAX,
        .int_n_cs_precedes  = CHAR_MAX,
        .int_p_sep_by_space = CHAR_MAX,
        .int_n_sep_by_space = CHAR_MAX,
        .int_p_sign_posn    = CHAR_MAX,
        .int_n_sign_posn    = CHAR_MAX,
    };
    return &c_locale;
}
