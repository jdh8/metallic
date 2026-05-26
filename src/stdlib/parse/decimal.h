#ifndef METALLIC_STDLIB_PARSE_DECIMAL_H_
#define METALLIC_STDLIB_PARSE_DECIMAL_H_

#include <stdint.h>

typedef struct {
    unsigned __int128 mant;
    int dec_exp;
    _Bool truncated;
    const char* digits;
    int total_digits;
} decimal_t;

#endif
