/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <string.h>

char* strncpy(char destination[static restrict 1], const char source[static restrict 1], size_t length)
{
    char* output = destination;

    for (; length && *source; --length)
        *output++ = *source++;

    if (length) // clang is happier with this
        memset(output, 0, length);

    return destination;
}
