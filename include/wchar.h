#ifndef _WCHAR_H
#define _WCHAR_H

#include "bits/types/mbstate.h"
#include "bits/null.h"
#include "bits/specifiers.h"

typedef __SIZE_TYPE__ size_t;
typedef __WINT_TYPE__ wint_t;

#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif

struct tm;

#define WEOF (-1)
#define WCHAR_MAX __WCHAR_MAX__
#define WCHAR_MIN __WCHAR_MIN__

#ifdef __cplusplus
extern "C" {
#endif

int wmemcmp(const wchar_t*, const wchar_t*, size_t);
wchar_t* wmemchr(const wchar_t*, wchar_t, size_t);
wchar_t* wmemset(wchar_t*, wchar_t, size_t);
wchar_t* wmemcpy(wchar_t* __restrict, const wchar_t* __restrict, size_t);
wchar_t* wmemmove(wchar_t*, const wchar_t*, size_t);

size_t wcslen(const wchar_t[_STATIC 1]);

int wcscmp(const wchar_t[_STATIC 1], const wchar_t rhs[_STATIC 1]);
int wcsncmp(const wchar_t[_STATIC 1], const wchar_t rhs[_STATIC 1], size_t);

wchar_t* wcschr(const wchar_t[_STATIC 1], wchar_t);
wchar_t* wcsrchr(const wchar_t[_STATIC 1], wchar_t);

wchar_t* wcscpy(wchar_t[_STATIC _RESTRICT 1], const wchar_t[_STATIC _RESTRICT 1]);
wchar_t* wcsncpy(wchar_t[_STATIC _RESTRICT 1], const wchar_t[_STATIC _RESTRICT 1], size_t);

wchar_t* wcscat(wchar_t[_STATIC _RESTRICT 1], const wchar_t[_STATIC _RESTRICT 1]);
wchar_t* wcsncat(wchar_t[_STATIC _RESTRICT 1], const wchar_t[_STATIC _RESTRICT 1], size_t);

size_t wcsspn(const wchar_t[_STATIC 1], const wchar_t[_STATIC 1]);
size_t wcscspn(const wchar_t[_STATIC 1], const wchar_t[_STATIC 1]);
wchar_t* wcspbrk(const wchar_t[_STATIC 1], const wchar_t[_STATIC 1]);

wchar_t* wcsstr(const wchar_t[_STATIC 1], const wchar_t[_STATIC 1]);
wchar_t* wcstok(wchar_t* __restrict, const wchar_t[_STATIC _RESTRICT 1]);

int wcscoll(const wchar_t[_STATIC 1], const wchar_t[_STATIC 1]);
size_t wcsxfrm(wchar_t* __restrict, const wchar_t[_STATIC _RESTRICT 1], size_t);

wint_t btowc(int);
int wctob(wint_t);

int mbsinit(const mbstate_t* ps);
size_t mbrlen(const char* __restrict, size_t, mbstate_t* __restrict);
size_t mbrtowc(wchar_t* __restrict, const char* __restrict, size_t, mbstate_t* __restrict);
size_t wcrtomb(char* __restrict, wchar_t, mbstate_t* __restrict);
size_t mbsrtowcs(wchar_t* __restrict, const char** __restrict, size_t, mbstate_t* __restrict);
size_t wcsrtombs(char* __restrict, const wchar_t** __restrict, size_t, mbstate_t* __restrict);

size_t wcsftime(wchar_t*, size_t, const wchar_t*, struct tm*);

long wcstol(const wchar_t[_STATIC 1], wchar_t**, int);
unsigned long wcstoul(const wchar_t[_STATIC 1], wchar_t**, int);
double wcstod(const wchar_t[_STATIC 1], wchar_t**);

#if __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L
long long wcstoll(const wchar_t[_STATIC 1], wchar_t**, int);
unsigned long long wcstoull(const wchar_t[_STATIC 1], wchar_t**, int);

float wcstof(const wchar_t[_STATIC 1], wchar_t**);
long double wcstold(const wchar_t[_STATIC 1], wchar_t**);
#endif

#ifdef __cplusplus
}
#endif

#endif /* wchar.h */
