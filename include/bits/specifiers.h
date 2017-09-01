/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#if (__cplusplus >= 201103)
#define _METALLIC_NORETURN [[noreturn]]
#elif (__STDC_VERSION__ >= 201112)
#define _METALLIC_NORETURN _Noreturn
#elif defined(__GNUC__)
#define _METALLIC_NORETURN __attribute__((__noreturn__))
#endif

#if (__cplusplus >= 201103)
#define _METALLIC_NOEXCEPT noexcept
#define _METALLIC_NOEXCEPT_IF(condition) noexcept(condition)
#define _METALLIC_USE_NOEXCEPT noexcept
#define _METALLIC_THROW(exception)
#else
#define _METALLIC_NOEXCEPT
#define _METALLIC_NOEXCEPT_IF(condition)
#define _METALLIC_USE_NOEXCEPT throw()
#define _METALLIC_THROW(exception) throw(exception)
#endif
