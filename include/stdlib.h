#ifndef _STDLIB_H
#define _STDLIB_H

#include "bits/null.h"
#include "bits/specifiers.h"

typedef __SIZE_TYPE__ size_t;

#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#else
extern "C" {
#endif

/******************* Program utilities ********************/
#define EXIT_SUCCESS 0
#define EXIT_FAILURE (-1)

_NORETURN void abort(void) _NOEXCEPT;
_NORETURN void exit(int);
int atexit(void(void));

#if __STDC_VERSION__ >= 201112L || __cplusplus >= 201103L
_NORETURN void _Exit(int) _NOEXCEPT;
_NORETURN void quick_exit(int) _NOEXCEPT;
int at_quick_exit(void(void));
#endif

int system(const char*);
char* getenv(const char[_STATIC 1]);

/******************* Memory management ********************/
void* malloc(size_t);
void* calloc(size_t, size_t);
void* realloc(void*, size_t);
void free(void*);

#if __STDC_VERSION__ >= 201112L || __cplusplus > 201402L
void* aligned_alloc(size_t, size_t);
#endif

/********************* Number parsing *********************/
double atof(const char[_STATIC 1]);
int atoi(const char[_STATIC 1]);
long atol(const char[_STATIC 1]);

long strtol(const char[_STATIC 1], char**, int);
unsigned long strtoul(const char[_STATIC 1], char**, int);

double strtod(const char[_STATIC 1], char**);

#if __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L
long long atoll(const char[_STATIC 1]);

long long strtoll(const char[_STATIC 1], char**, int);
unsigned long long strtoull(const char[_STATIC 1], char**, int);

float strtof(const char[_STATIC 1], char**);
long double strtold(const char[_STATIC 1], char**);
#endif

/********************** Wide strings **********************/
#define MB_CUR_MAX sizeof(wchar_t)

int mblen(const char*, size_t);
int mbtowc(wchar_t*__restrict, const char*__restrict, size_t);
int wctomb(char*, wchar_t);

size_t mbstowcs(wchar_t[_STATIC 1], const char[_STATIC 1], size_t);
size_t wcstombs(char[_STATIC 1], const wchar_t[_STATIC 1], size_t);

/*********************** Algorithms ***********************/
#define RAND_MAX 2147483646

int rand(void);
void srand(unsigned);
void qsort(void*, size_t, size_t, int(const void*, const void*));
void* bsearch(const void*, const void*, size_t, size_t, int(const void*, const void*));

/******************* Integer arithmetic *******************/
typedef struct { int quot; int rem; } div_t;
typedef struct { long quot; long rem; } ldiv_t;

int abs(int);
long labs(long);

div_t div(int, int);
ldiv_t ldiv(long, long);

#if __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L
typedef struct { long long quot; long long rem; } lldiv_t;

long long llabs(long long);
lldiv_t lldiv(long long, long long);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* stdlib.h */
