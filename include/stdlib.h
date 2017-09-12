/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
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
int atexit(void (*)(void));

#if __STDC_VERSION__ >= 201112L || __cplusplus >= 201103L
_NORETURN void _Exit(int) _NOEXCEPT;
_NORETURN void quick_exit(int) _NOEXCEPT;
int at_quick_exit(void (*)(void));
#endif

int system(const char*);
char* getenv(const char*);

/******************* Memory management ********************/
void* malloc(size_t);
void* calloc(size_t, size_t);
void* realloc(void*, size_t);
void free(void*);

#if __STDC_VERSION__ >= 201112L || __cplusplus > 201402L
void* aligned_alloc(size_t, size_t);
#endif

/********************* Number parsing *********************/
double atof(const char*);
int atoi(const char*);
long atol(const char*);

long strtol(const char*, char**, int);
unsigned long strtoul(const char*, char**, int);

double strtod(const char*, char**);

#if __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L
long long atoll(const char*);

long long strtoll(const char*, char**, int);
unsigned long long strtoull(const char*, char**, int);

float strtof(const char*, char**);
long double strtold(const char*, char**);
#endif

/********************** Wide strings **********************/
#define MB_CUR_MAX __ctype_get_mb_cur_max()

int mblen(const char*, size_t);
int mbtowc(wchar_t*, const char*, size_t);
int wctomb(char*, wchar_t);

size_t mbstowcs(wchar_t*, const char*, size_t);
size_t wcstombs(char*, const wchar_t*, size_t);

/*********************** Algorithms ***********************/
#define RAND_MAX 2147483646

int rand(void);
void srand(unsigned);
void qsort(void*, size_t, size_t, int (*)(const void*, const void*));
void* bsearch(const void*, const void*, size_t, size_t, int (*)(const void*, const void*));

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
