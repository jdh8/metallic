# Hello, metallic

A minimal program that uses `metallic.a` + `wasi.a` to run under `wasmtime`.

## Build & run

From this directory, after running `make` in the project root:

    make            # builds hello.out
    make run        # wasmtime hello.out one two three

Or invoke wasmtime directly:

    wasmtime --env PATH=/usr/bin hello.out one two three

Expected output:

    Hello from metallic + WASI!
    argc = 4
      argv[0] = hello.out
      argv[1] = one
      argv[2] = two
      argv[3] = three
    PATH = /usr/bin

## What this demonstrates

* C standard library entry point (`main(argc, argv)`).
* `printf` via metallic's stdio + wasi.a's `fd_write`.
* `getenv` via wasi.a's environ table populated from `wasmtime --env`.
* The two-library linking model: pure libc + pluggable host backend.
