/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
_Noreturn void abort(void);

#define STRINGIFY0(x) #x
#define STRINGIFY(x) STRINGIFY0(x)

#define ASSERTF(cond, ...) if (!(cond)) {              \
    fputs(__FILE__":"STRINGIFY(__LINE__)": ", stderr); \
    fputs(__func__, stderr);                           \
    fputs(": Assertion `"#cond"' failed: ", stderr);   \
    fprintf(stderr, __VA_ARGS__);                      \
    abort();                                           \
}
