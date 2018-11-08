/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "nan.h"
#include "../getc.h"
#include "../ungetc.h"
#include <ctype.h>
#include <errno.h>
#include <tgmath.h>

static Scalar _scaninf(int cache, FILE stream[static 1])
{
    unsigned char buffer[8] = { cache };
    int matched = 1;

    while (matched < 8 && ((buffer[matched] = getc(stream)) | 32) == "infinity"[matched])
        ++matched;

    int valid = matched >= 3;

    if (matched < 8)
        for (; matched >= 3 * valid; --matched)
            ungetc(buffer[matched], stream);

    return valid ? INFINITY : 0;
}

static Scalar _scannan(int cache, FILE stream[static 1])
{
    unsigned char buffer[3] = { cache };
    int matched = 1;

    while (matched < 3 && ((buffer[matched] = getc(stream)) | 32) == "nan"[matched])
        ++matched;

    if (matched < 3) {
        for (; matched >= 0; --matched)
            ungetc(buffer[matched], stream);
        return 0;
    }

    if ((cache = getc(stream)) == '(')
        return _nan(stream, ')');

    ungetc(cache, stream);
    return NAN;
}

static int _exp(int antecedent, int cache, FILE stream[static 1])
{
    int sign = 1;
    int magnitude = 0;

    if (antecedent != (cache | 32))
        return 0;
    
    switch (cache = getc(stream)) {
        case '-':
            sign = -1;
            /* fallthrough */
        case '+':
            cache = getc(stream);
    }

    //TODO
}

static Scalar _hexfloat(int antecedent, FILE stream[static 1])
{
    const int capacity = 2 * sizeof(Bitset);
    Bitset x = 0;
    int places = 0;
    int read = 0;
    int cache;
    _Bool pointed = 0;

    while ((cache = getc(stream)) == '0');

    if (cache == '.') {
        pointed = 1;
        while ((cache = getc(stream)) == '0')
            --places;
    }

    for (int digit = _xdigit(cache); ; digit = _xdigit(cache = getc(stream))) {
        if (digit >= 0) {
            if (++read <= capacity)
                x = x << 4 | digit;
        }
        else if (cache == '.' && !pointed) {
            places = read;
            pointed = 1;
        }
        else break;
    }

    if (!read) {
        if (pointed)
            ungetc('.', stream);
        ungetc(antecedent, stream);
        return 0;
    }

    if (read < capacity)
        x <<= (capacity - read) << 2;

    if (!pointed)
        places = read;

    return ldexp((Scalar)x, 4 * (places - capacity) + _exp('p', cache, stream));
}

static Scalar _scientific(int cache, FILE stream[static 1]);

static Scalar _scanliteral(int cache, FILE stream[static 1])
{
    switch (cache | 32) {
        case 'i':
            return _scaninf(cache, stream);
        case 'n':
            return _scannan(cache, stream);
    }

    Scalar finite = (cache == '0' && ((cache = getc(stream)) | 32) == 'x'
        ? _hexfloat
        : _scientific)(cache, stream);

    if (finite == INFINITY)
        errno = ERANGE;

    return finite;
}

static Scalar _scanfloat(FILE stream[static 1])
{
    Scalar sign = 1;
    int cache;

    while (isspace(cache = getc(stream)));
    
    switch (cache) {
        case '-':
            sign = -1;
            /* fallthrough */
        case '+':
            cache = getc(stream);
    }

    return copysign(_scanliteral(cache, stream), sign);
}
