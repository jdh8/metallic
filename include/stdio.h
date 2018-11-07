/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _STDIO_H
#define _STDIO_H

#include "bits/null.h"
#include "bits/specifiers.h"

typedef struct __FILE FILE;
typedef struct __fpos_t fpos_t;
typedef __SIZE_TYPE__ size_t;

#ifdef __cplusplus
extern "C" {
#endif

FILE* fopen(const char[_RESTRICT _STATIC 1], const char[_RESTRICT _STATIC 1]);
FILE* freopen(const char[_RESTRICT _STATIC 1], const char[_RESTRICT _STATIC 1], FILE* __restrict);
int fclose(FILE*);
int fflush(FILE*);
void setbuf(FILE* __restrict, char* __restrict);
int setvbuf(FILE* __restrict, char* __restrict, int, size_t);

size_t fread(void* __restrict, size_t, size_t, FILE* __restrict);
size_t fwrite(const void* __restrict, size_t, size_t, FILE* __restrict);

int fgetc(FILE*);
int getc(FILE*);
char* fgets(char[_RESTRICT _STATIC 1], int, FILE* __restrict);

int fputc(int, FILE*);
int putc(int, FILE*);
int fputs(const char* __restrict, FILE* __restrict);

int getchar(void);

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ < 201112L) || (defined(__cplusplus) && __cplusplus < 201402L)
char* gets(char*);
#endif

int putchar(int);
int puts(const char[_STATIC 1]);

int ungetc(int, FILE*);

int scanf(const char[_RESTRICT _STATIC 1], ...);
int fscanf(FILE* __restrict, const char[_RESTRICT _STATIC 1], ...);
int sscanf(const char* __restrict, const char[_RESTRICT _STATIC 1], ...);

int vscanf(const char[_RESTRICT _STATIC 1], __builtin_va_list);
int vfscanf(FILE* __restrict, const char[_RESTRICT _STATIC 1], __builtin_va_list);
int vsscanf(const char* __restrict, const char[_RESTRICT _STATIC 1], __builtin_va_list);

int printf(const char[_RESTRICT _STATIC 1], ...);
int fprintf(FILE* __restrict, const char[_RESTRICT _STATIC 1], ...);
int sprintf(char[_RESTRICT _STATIC 1], const char[_RESTRICT _STATIC 1], ...);
int snprintf(char* __restrict, size_t, const char[_RESTRICT _STATIC 1], ...);

int vprintf(const char[_RESTRICT _STATIC 1], __builtin_va_list);
int vfprintf(FILE* __restrict, const char[_RESTRICT _STATIC 1], __builtin_va_list);
int vsprintf(char[_RESTRICT _STATIC 1], const char[_RESTRICT _STATIC 1], __builtin_va_list);
int vsnprintf(char* __restrict, size_t, const char[_RESTRICT _STATIC 1], __builtin_va_list);

long ftell(FILE*);
int fgetpos(FILE* __restrict, fpos_t* __restrict);
int fseek(FILE*, long, int);
int fsetpos(FILE*, const fpos_t*);
void rewind(FILE*);

void clearerr(FILE*);
int feof(FILE*);
int ferror(FILE*);
void perror(const char[_STATIC 1]);

int remove(const char[_STATIC 1]);
int rename(const char[_STATIC 1], const char[_STATIC 1]);
FILE* tmpfile(void);
char* tmpnam(char*);

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

#ifdef __cplusplus
}
#endif

#define stdin stdin
#define stdout stdout
#define stderr stderr

#define EOF -1
#define FOPEN_MAX 16
#define FILENAME_MAX 4096
#define BUFSIZ 4096

#define _IONBF 0
#define _IOLBF 1
#define _IOFBF 2

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define TMP_MAX 238328
#define L_tmpnam 20

#endif /* stdio.h */
