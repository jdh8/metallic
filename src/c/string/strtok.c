
/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <string.h>

char* strtok_r(char* restrict string, const char* restrict separators, char* state[static restrict 1])
{
    if (!(string || (string = *state)))
        return 0;

    string += strspn(string, separators);

    if (!*string)
        return *state = 0;

    *state = strpbrk(string, separators);

    if (*state)
        *(*state)++ = '\0';

    return string;
}

char* strtok(char* restrict string, const char* restrict separators)
{
    static char* state;

    return strtok_r(string, separators, &state);
}
