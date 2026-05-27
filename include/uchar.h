#ifndef _UCHAR_H
#define _UCHAR_H

#include "bits/types/mbstate.h"
#include <stdint.h>

typedef __SIZE_TYPE__ size_t;

#if !defined(__cplusplus) || __cplusplus < 201103L
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

size_t mbrtoc16(char16_t* __restrict, const char* __restrict, size_t, mbstate_t* __restrict);
size_t c16rtomb(char* __restrict, char16_t, mbstate_t* __restrict);

size_t mbrtoc32(char32_t* __restrict, const char* __restrict, size_t, mbstate_t* __restrict);
size_t c32rtomb(char* __restrict, char32_t, mbstate_t* __restrict);

#ifdef __cplusplus
}
#endif

#endif /* uchar.h */
