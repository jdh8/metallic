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

## Linking model

Metallic is split into two static archives:

| Archive       | Role                                                     |
|---------------|----------------------------------------------------------|
| `metallic.a`  | Pure libc — no host I/O. Imports a small set of `__*` symbols. |
| `wasi.a`      | Host backend implementing those imports against WASI snapshot_preview1. |

Programs targeting WASI link both:

    clang --target=wasm32-unknown-unknown-wasm -nostdlib \
        -Wl,--allow-undefined -I include \
        main.c metallic.a wasi.a -o app.out

Programs targeting another host (a custom JS shim, a browser polyfill,
a non-WASI runtime) link `metallic.a` and provide their own implementations
of the host import contract below.

## Host import contract

Any backend used in place of `wasi.a` must define these symbols. The convention
is **positive return on success, negative POSIX errno on failure**.

| Symbol            | Signature                                              | Purpose                  |
|-------------------|--------------------------------------------------------|--------------------------|
| `__read`          | `long (int fd, void *, size_t)`                        | read bytes from fd       |
| `__write`         | `long (int fd, const void *, size_t)`                  | write bytes to fd        |
| `__lseek`         | `long (int fd, long offset, int whence)`               | seek (LP64 path)         |
| `__llseek`        | `int (int fd, long hi, unsigned long lo, off_t *, int)` | seek (ILP32 path)        |
| `__close`         | `int (int fd)`                                          | close fd                 |
| `__open`          | `int (const char *, int flags, int mode)`               | open file                |
| `__fcntl`         | `int (int fd, int cmd, unsigned long arg)`              | F_GETFL/F_SETFL/etc.     |
| `__stat`          | `int (const char *, struct stat *)`                    | stat by path             |
| `__fstat`         | `int (int fd, struct stat *)`                          | stat by fd               |
| `__lstat`         | `int (const char *, struct stat *)`                    | lstat by path            |
| `__unlink`        | `int (const char *)`                                   | unlink file              |
| `__rmdir`         | `int (const char *)`                                   | remove directory         |
| `__rename`        | `int (const char *, const char *)`                     | rename file              |
| `__clock_gettime` | `int (clockid_t, struct timespec *)`                   | wall/monotonic/cpu clock |
| `__argc`          | `int (void)`                                            | argument count           |
| `__argv`          | `size_t (char *buf, size_t size)`                       | argv null-separated      |

## Status

Implemented C11 hosted-environment headers:

* `<assert.h>` `<complex.h>` `<ctype.h>` `<errno.h>` `<fenv.h>` `<float.h>`
  `<limits.h>` `<locale.h>` `<math.h>` `<setjmp.h>` `<signal.h>` `<stdarg.h>`
  `<stdbool.h>` `<stddef.h>` `<stdint.h>` `<stdio.h>` `<stdlib.h>` `<string.h>`
  `<time.h>` `<wchar.h>`

Not yet provided: `<inttypes.h>` `<stdalign.h>` `<stdatomic.h>` `<stdnoreturn.h>`
`<tgmath.h>` `<threads.h>` `<uchar.h>` `<wctype.h>`.

## Limitations

* `longjmp` aborts the process. WebAssembly has no native stack unwinding;
  programs that need real setjmp/longjmp should compile with clang's
  `-mllvm -wasm-enable-sjlj`, which lowers `setjmp`/`longjmp` to clang
  intrinsics and bypasses libc.
* `signal`/`raise` track handlers but do not asynchronously dispatch — WASI
  preview1 delivers no signals. `raise(SIGABRT)` is correctly routed.
* `localtime` aliases `gmtime` — WASI preview1 has no timezone info.
* Only the `C` locale is supported.
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
