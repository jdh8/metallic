Metallic
========
A try to build a C runtime on WebAssembly

This name is from Gary Bernhardt "[The Birth & Death of JavaScript][bdjs]",
where a world of **metal** web applications is conceived.  This library is
aimed to be a building block of early **metal** applications, whose performance
is near native.

[bdjs]: https://www.destroyallsoftware.com/talks/the-birth-and-death-of-javascript

Dependency
----------
Only freestanding C headers:

* stddef.h
* stdint.h

Compilers able to generate WebAssembly code are likely to provide these
headers.
