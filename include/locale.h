#ifndef _LOCALE_H
#define _LOCALE_H

#include "bits/null.h"

/* C11 §7.11. The numeric values are arbitrary but must be distinct.
 * LC_ALL == 0 matches glibc/musl convention so programs using the
 * constant in a switch keep working. */
#define LC_ALL      0
#define LC_COLLATE  1
#define LC_CTYPE    2
#define LC_MONETARY 3
#define LC_NUMERIC  4
#define LC_TIME     5

struct lconv
{
    /* Non-monetary numeric formatting. */
    char* decimal_point;
    char* thousands_sep;
    char* grouping;

    /* Monetary numeric formatting. */
    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;
    char* currency_symbol;
    char  frac_digits;
    char  p_cs_precedes;
    char  n_cs_precedes;
    char  p_sep_by_space;
    char  n_sep_by_space;
    char  p_sign_posn;
    char  n_sign_posn;

    char* int_curr_symbol;
    char  int_frac_digits;
    char  int_p_cs_precedes;
    char  int_n_cs_precedes;
    char  int_p_sep_by_space;
    char  int_n_sep_by_space;
    char  int_p_sign_posn;
    char  int_n_sign_posn;
};

#ifdef __cplusplus
extern "C" {
#endif

char* setlocale(int category, const char* locale);
struct lconv* localeconv(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* locale.h */
