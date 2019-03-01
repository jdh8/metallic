#ifndef _WCTYPE_H
#define _WCTYPE_H

#include "bits/specifiers.h"

typedef __WINT_TYPE__ wint_t;
typedef unsigned wctrans_t;
typedef unsigned wctype_t;

#ifdef __cplusplus
extern "C" {
#endif

int iswcntrl(wint_t);
int iswprint(wint_t);
int iswblank(wint_t);
int iswspace(wint_t);
int iswgraph(wint_t);

int iswupper(wint_t);
int iswlower(wint_t);
int iswalpha(wint_t);

int iswdigit(wint_t);
int iswxdigit(wint_t);

int iswalnum(wint_t);
int iswpunct(wint_t);

wctype_t wctype(const char[_STATIC 1]);
int iswctype(wint_t, wctype_t);

wint_t towupper(wint_t);
wint_t towlower(wint_t);

wctrans_t wctrans(const char[_STATIC 1]);
wint_t towctrans(wint_t, wctrans_t);

#ifdef __cplusplus
}
#endif

#endif /* wctype.h */

