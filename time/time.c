/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <time.h>

extern double _Date_now(void);

inline time_t time(time_t* timer)
{
    time_t result = _Date_now() / 1000;
    if (timer) *timer = result;
    return result;
}
