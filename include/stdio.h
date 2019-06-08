#ifndef _STDIO_H
#define _STDIO_H

#include "bits/null.h"

typedef struct __FILE FILE;
typedef struct __fpos_t fpos_t;
typedef __SIZE_TYPE__ size_t;

#ifdef __cplusplus
extern "C" {
#endif

FILE* fopen(const char* __restrict, const char* __restrict);
FILE* freopen(const char* __restrict, const char* __restrict, FILE* __restrict);
int fclose(FILE*);
int fflush(FILE*);
void setbuf(FILE* __restrict, char* __restrict);
int setvbuf(FILE* __restrict, char* __restrict, int, size_t);

size_t fread(void* __restrict, size_t, size_t, FILE* __restrict);
size_t fwrite(const void* __restrict, size_t, size_t, FILE* __restrict);

int fgetc(FILE*);
int getc(FILE*);
char* fgets(char* __restrict, int, FILE* __restrict);

int fputc(int, FILE*);
int putc(int, FILE*);
int fputs(const char* __restrict, FILE* __restrict);

int getchar(void);

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ < 201112L) || (defined(__cplusplus) && __cplusplus < 201402L)
char* gets(char*);
#endif

int putchar(int);
int puts(const char*);

int ungetc(int, FILE*);

int scanf(const char* __restrict, ...);
int fscanf(FILE* __restrict, const char* __restrict, ...);
int sscanf(const char* __restrict, const char* __restrict, ...);

int vscanf(const char* __restrict, __builtin_va_list);
int vfscanf(FILE* __restrict, const char* __restrict, __builtin_va_list);
int vsscanf(const char* __restrict, const char* __restrict, __builtin_va_list);

int printf(const char* __restrict, ...);
int fprintf(FILE* __restrict, const char* __restrict, ...);
int sprintf(char* __restrict, const char* __restrict, ...);
int snprintf(char* __restrict, size_t, const char* __restrict, ...);

int vprintf(const char* __restrict, __builtin_va_list);
int vfprintf(FILE* __restrict, const char* __restrict, __builtin_va_list);
int vsprintf(char* __restrict, const char* __restrict, __builtin_va_list);
int vsnprintf(char* __restrict, size_t, const char* __restrict, __builtin_va_list);

long ftell(FILE*);
int fgetpos(FILE* __restrict, fpos_t* __restrict);
int fseek(FILE*, long, int);
int fsetpos(FILE*, const fpos_t*);
void rewind(FILE*);

void clearerr(FILE*);
int feof(FILE*);
int ferror(FILE*);
void perror(const char*);

int remove(const char*);
int rename(const char*, const char*);
FILE* tmpfile(void);
char* tmpnam(char*);

extern FILE __stdin;
extern FILE __stdout;
extern FILE __stderr;

static FILE* const stdin = &__stdin;
static FILE* const stdout = &__stdout;
static FILE* const stderr = &__stderr;

#ifdef __cplusplus
}
#endif

#define stdin (&__stdin)
#define stdout (&__stdout)
#define stderr (&__stderr)

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
