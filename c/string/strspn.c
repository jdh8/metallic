/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>

static const char* _find(const char source[static 1], char character)
{
    for (; *source; ++source)
        if (*source == character)
            return source;

    return 0;
}

size_t strspn(const char string[static 1], const char accept[static 1])
{
    size_t count = 0;

    while (_find(accept, string[count]))
        ++count;

    return count;
}
