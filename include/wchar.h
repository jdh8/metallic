#ifndef _WCHAR_H
#define _WCHAR_H

#include "bits/types/mbstate.h"
#include "bits/null.h"

struct __FILE;
typedef struct __FILE FILE;

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

size_t wcslen(const wchar_t*);

int wcscmp(const wchar_t*, const wchar_t*);
int wcsncmp(const wchar_t*, const wchar_t*, size_t);

wchar_t* wcschr(const wchar_t*, wchar_t);
wchar_t* wcsrchr(const wchar_t*, wchar_t);

wchar_t* wcscpy(wchar_t* __restrict, const wchar_t* __restrict);
wchar_t* wcsncpy(wchar_t* __restrict, const wchar_t* __restrict, size_t);

wchar_t* wcscat(wchar_t* __restrict, const wchar_t* __restrict);
wchar_t* wcsncat(wchar_t* __restrict, const wchar_t* __restrict, size_t);

size_t wcsspn(const wchar_t*, const wchar_t*);
size_t wcscspn(const wchar_t*, const wchar_t*);
wchar_t* wcspbrk(const wchar_t*, const wchar_t*);

wchar_t* wcsstr(const wchar_t*, const wchar_t*);
wchar_t* wcstok(wchar_t* __restrict, const wchar_t* __restrict, wchar_t** __restrict);

int wcscoll(const wchar_t*, const wchar_t*);
size_t wcsxfrm(wchar_t* __restrict, const wchar_t* __restrict, size_t);

wint_t btowc(int);
int wctob(wint_t);

int mbsinit(const mbstate_t* ps);
size_t mbrlen(const char* __restrict, size_t, mbstate_t* __restrict);
size_t mbrtowc(wchar_t* __restrict, const char* __restrict, size_t, mbstate_t* __restrict);
size_t wcrtomb(char* __restrict, wchar_t, mbstate_t* __restrict);
size_t mbsrtowcs(wchar_t* __restrict, const char** __restrict, size_t, mbstate_t* __restrict);
size_t wcsrtombs(char* __restrict, const wchar_t** __restrict, size_t, mbstate_t* __restrict);

size_t wcsftime(wchar_t*, size_t, const wchar_t*, struct tm*);

/* Wide-character I/O */
int fwide(FILE*, int);

wint_t fgetwc(FILE*);
wint_t getwc(FILE*);
wint_t getwchar(void);
wint_t fputwc(wchar_t, FILE*);
wint_t putwc(wchar_t, FILE*);
wint_t putwchar(wchar_t);
wint_t ungetwc(wint_t, FILE*);

wchar_t* fgetws(wchar_t* __restrict, int, FILE* __restrict);
int fputws(const wchar_t* __restrict, FILE* __restrict);

int fwprintf(FILE* __restrict, const wchar_t* __restrict, ...);
int wprintf(const wchar_t* __restrict, ...);
int swprintf(wchar_t* __restrict, size_t, const wchar_t* __restrict, ...);
int vfwprintf(FILE* __restrict, const wchar_t* __restrict, __builtin_va_list);
int vwprintf(const wchar_t* __restrict, __builtin_va_list);
int vswprintf(wchar_t* __restrict, size_t, const wchar_t* __restrict, __builtin_va_list);

int fwscanf(FILE* __restrict, const wchar_t* __restrict, ...);
int wscanf(const wchar_t* __restrict, ...);
int swscanf(const wchar_t* __restrict, const wchar_t* __restrict, ...);
int vfwscanf(FILE* __restrict, const wchar_t* __restrict, __builtin_va_list);
int vwscanf(const wchar_t* __restrict, __builtin_va_list);
int vswscanf(const wchar_t* __restrict, const wchar_t* __restrict, __builtin_va_list);

long wcstol(const wchar_t*, wchar_t**, int);
unsigned long wcstoul(const wchar_t*, wchar_t**, int);
double wcstod(const wchar_t*, wchar_t**);

#if __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L
long long wcstoll(const wchar_t*, wchar_t**, int);
unsigned long long wcstoull(const wchar_t*, wchar_t**, int);

float wcstof(const wchar_t*, wchar_t**);
long double wcstold(const wchar_t*, wchar_t**);
#endif

#ifdef __cplusplus
}
#endif

#endif /* wchar.h */
