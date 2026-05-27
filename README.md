# Metallic

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
* The math library is the project's strength — most functions are faithfully
  rounded (error < 1 ulp).

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
