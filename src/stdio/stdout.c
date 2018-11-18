/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "invalid.h"
#include "stdout.h"
#include "FILE.h"
#include <stdio.h>

static int _put(int c, FILE* stream)
{
    return putchar(c);
}

static FILE _stdout = (FILE){
    ._get = __invalid_get,
    ._put = _put,
    ._unget = __invalid_put,
    ._read = __invalid_read,
    ._write = __stdout
};

FILE* stdout = &_stdout;
