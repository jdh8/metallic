Metallic
========
A try to build a C runtime library for WebAssembly

This name is from Gary Bernhardt "[The Birth & Death of JavaScript][bdjs]",
where a world of **metal** web applications is conceived.  This library is
aimed to be a building block of early **metal** applications, whose performance
is near native.

[bdjs]: https://www.destroyallsoftware.com/talks/the-birth-and-death-of-javascript

To achieve this goal, this library is written from scratch, including math
functions.  The math functions, especially `float` ones, take advantage of
modern architectures to achieve both speed and accuracy.  Most of them produce
faithfully rounded results (error < 1 ulp) in reasonable time.

Dependencies
------------
### Compiler requirements ###
Currently only clang meet these requirements.

* WebAssembly backend
* GCC-compatible builtins
* ILP32 or LP64 data model

### Environment requirements for `node` driver ###
* node-gyp - To build a native addon for pesky syscalls

### Internal dependency ###
* dlmalloc - Doug Lea's malloc with other memory management routines
