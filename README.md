# Metallic

[![CI](https://github.com/jdh8/metallic/actions/workflows/ci.yml/badge.svg)](https://github.com/jdh8/metallic/actions/workflows/ci.yml)
[![Oracle](https://github.com/jdh8/metallic/actions/workflows/oracle.yml/badge.svg)](https://github.com/jdh8/metallic/actions/workflows/oracle.yml)
[![Benchmark](https://github.com/jdh8/metallic/actions/workflows/bench.yml/badge.svg)](https://jdh8.github.io/metallic/dev/bench/)

A from-scratch C library for WebAssembly. The name comes from Gary Bernhardt's
[The Birth & Death of JavaScript][bdjs] — a vision of "metal" web applications
running at near-native speed. Metallic aims to be the runtime for that world.

[bdjs]: https://www.destroyallsoftware.com/talks/the-birth-and-death-of-javascript

## Quickstart

    # Build the libraries (requires clang with the wasm32 target and llvm-link).
    make

    # Run the included hello-world example under wasmtime.
    cd examples/hello && make run

## Linking

Metallic builds to a single static archive, `metallic.a`, that calls
WASI snapshot_preview1 directly.  Programs link it like any other library:

    clang --target=wasm32-unknown-unknown-wasm -nostdlib \
        -I include main.c metallic.a -o app.out

Host I/O lives in [src/wasi/wasi.h](src/wasi/wasi.h), where each binding
carries the `import_module("wasi_snapshot_preview1")` attribute so the
linker emits them as wasm imports without needing `--allow-undefined`.

## Status

Implemented C11 hosted-environment headers:

* `<assert.h>` `<complex.h>` `<ctype.h>` `<errno.h>` `<fenv.h>` `<float.h>`
  `<inttypes.h>` `<iso646.h>` `<limits.h>` `<locale.h>` `<math.h>`
  `<setjmp.h>` `<signal.h>` `<stdalign.h>` `<stdarg.h>` `<stdbool.h>`
  `<stddef.h>` `<stdint.h>` `<stdio.h>` `<stdlib.h>` `<stdnoreturn.h>`
  `<string.h>` `<tgmath.h>` `<time.h>` `<uchar.h>` `<wchar.h>` `<wctype.h>`

Opted out via `__STDC_NO_THREADS__`: `<threads.h>` is intentionally not
shipped.  `<stdatomic.h>` comes from clang's freestanding headers and
lowers to plain operations on single-threaded WASI.

## Math accuracy

Correct rounding is the math library's goal, and most transcendentals reach it.
A function is **correctly rounded** when its result is the true value rounded to
the nearest representable number (error ≤ 0.5 ulp); **faithfully rounded** is the
weaker guarantee of one of the two nearest (error < 1 ulp). For `float`, correct
rounding is *proven* by an exhaustive sweep of all 2³² bit patterns against an
MPFR oracle; for `double` it is strong evidence from a sampler that hammers the
published hard-to-round cases plus a broad random sample. See `test/oracle/` and
`make check.oracle` (native, MPFR-backed; not part of the wasm CI gate).

* **`float`, correctly rounded (≤ 0.5 ulp).** Unary, proven by exhaustive sweep:
  `expf` `exp2f` `expm1f` `logf` `log2f` `log10f` `log1pf` `sinf` `cosf` `tanf`
  `asinf` `acosf` `atanf` `asinhf` `acoshf` `atanhf` `sinhf` `coshf` `tanhf`
  `cbrtf` `erff` `erfcf`.  Bivariate (sampler evidence, since the 2⁶⁴ domain
  cannot be swept): `atan2f` `hypotf`.
* **`float`, faithfully rounded (< 1 ulp).** `powf` `lgammaf` `tgammaf`.
* **`double`, correctly rounded (≤ 0.5 ulp), sampler evidence.** `exp` `exp2`
  `expm1` `log` `log2` `log10` `log1p` `cbrt` `atan` `asin` `acos` `hypot`
  `asinh` `acosh` `atanh` `erf`.
* **`double`, faithfully rounded (< 1 ulp).** `sin` `cos` `tan` `sinh` `cosh`
  `tanh` `atan2` `erfc` `lgamma` `tgamma` — pending a wider accurate path.
* **Complex `double`** (`cabs`, `cexp`, `clog`, `csqrt`, the trig/hyperbolic
  families, …) is composed from the real kernels and inherits their accuracy.

## Limitations

* `longjmp` aborts the process. WebAssembly has no native stack unwinding;
  programs that need real setjmp/longjmp should compile with clang's
  `-mllvm -wasm-enable-sjlj`, which lowers `setjmp`/`longjmp` to clang
  intrinsics and bypasses libc.
* `signal`/`raise` track handlers but do not asynchronously dispatch — WASI
  preview1 delivers no signals. `raise(SIGABRT)` is correctly routed.
* `localtime` aliases `gmtime` — WASI preview1 has no timezone info.
* Only the `C` locale is supported.
* No threads.  `__STDC_NO_THREADS__` is predefined, so `<threads.h>` is
  absent and feature-test code routes around the C11 thread API.  Atomics
  use clang's freestanding `<stdatomic.h>`; on single-threaded WASI they
  lower to plain loads and stores.

## Testing

* `make check.native` — native tests (some pre-existing math accuracy failures
  against the host libm).
* `make check.wasm.fast` — wasm tests under `wasmtime`, excluding the 11
  pre-existing soft-float/integer/long-double failures. CI runs this.
* `make check.wasm` — full wasm suite including the known-broken tests.

## Known issues

11 pre-existing test failures in the soft-float / 128-bit integer / long-double
sqrt code paths. These are not regressions and are excluded from
`check.wasm.fast`. See [Makefile](Makefile) `KNOWN_BROKEN_WASM`.

## Acknowledgements

This was originally Chen-Pang He's libc-for-WebAssembly experiment. Revival
work brought it up to a hosted C11 + WASI snapshot_preview1 runtime; the
math kernels predate the revival.

## License

See [LICENSE](LICENSE).
