# Metallic

Metallic is a from-scratch C11 standard library for WebAssembly, targeting WASI snapshot_preview1. It compiles to `metallic.a` and provides a full hosted-environment libc for programs built with `--target=wasm32-unknown-unknown-wasm`.

## Build

```bash
make                        # produces metallic.a
cd examples/hello && make   # build + link a program against the library
wasmtime hello.out          # run under wasmtime
```

**Toolchain requirements:** clang with wasm32 target, llvm-link, wasmtime.

## Tests

```bash
make check.wasm.fast    # CI suite — must pass (excludes 11 known-broken tests)
make check.wasm         # full suite including known failures
make check.native       # informational native run
```

Tests are individual `.c` files under `test/wasm/` and `test/native/`. Each compiles to a `.out` (wasm) or `.exe` (native) and is run directly. There are 11 pre-existing failures in the soft-float / 128-bit integer / long-double paths excluded from CI via `check.wasm.fast`.

## Repository layout

```
include/        public C headers (C11 + POSIX subset)
include/bits/   internal type definitions
src/
  crt/          C runtime startup — crt1.c is the WASI entrypoint
  math/         math library; kernel/ subdirs hold internal kernel functions
  soft/         soft-float and wide-integer fallbacks
  stdio/        printf/scanf family + file I/O via WASI
  stdlib/       memory, environment, parse/
  string/       memcpy, strlen, etc.
  wasi/         WASI syscall bindings (errno mapping, preopens, args, environ)
  time/         clock_gettime, time, localtime (aliases gmtime — no TZ support)
  …             ctype, fenv, locale, setjmp, signal, unistd, sys/stat, wchar
test/
  wasm/         primary test suite
  native/       host-compiled tests (informational)
bench/          benchmarks
examples/hello/ hello-world example with its own Makefile
```

## Key constraints & known limitations

- `longjmp` aborts the process — WASM has no native stack unwinding.
- Signals are synchronous only; WASI preview1 has no async signal delivery.
- `localtime` is an alias for `gmtime`; timezone data is unavailable.
- Only the C locale is supported.
- No threads: `__STDC_NO_THREADS__` is predefined via the Makefile; `<threads.h>` is not shipped. Atomics use clang's freestanding `<stdatomic.h>` (single-threaded WASI lowers them to plain ops).
- Math accuracy is the library's strength: most functions are faithfully rounded (< 1 ulp).

## Coding conventions

- WASI syscalls are invoked via `__wasi_*` functions decorated with `__attribute__((import_module("wasi_snapshot_preview1")))`.
- Shared low-level patterns (copy loops, endian helpers) live in small internal headers rather than separate `.c` files.
- Keep implementations minimal; avoid abstractions that are not required by the current task.
- Comments explain non-obvious constraints (hardware bugs, ABI invariants, WASI limitations) — not what the code does.

## Working with subagents

When a piece of work splits naturally into independent sub-tasks — separate exploration targets, multi-area refactors, parallel verification — feel free to spawn subagents (Explore, Plan, general-purpose) and run them concurrently. Don't serialize work that doesn't depend on itself. Reserve a single agent for tasks that are small or strictly sequential.

## Commit discipline

When executing a large plan, **break it into atomic commits** — one logical change per commit. A commit should build and pass `make check.wasm.fast` on its own. Prefer many small commits over one large one; this makes bisect and review tractable and keeps the history readable.
