Metallic
========
A try to build a C runtime library for WebAssembly

This name is from Gary Bernhardt "[The Birth & Death of JavaScript][bdjs]",
where a world of **metal** web applications is conceived.  This library is
aimed to be a building block of early **metal** applications, whose performance
is near native.

[bdjs]: https://www.destroyallsoftware.com/talks/the-birth-and-death-of-javascript

Dependencies
------------
### System requirements ###
Most environments with GCC-compatible compilers (e.g. GCC and clang) meet these
requirements.

* GCC-compatible builtins
* ILP32 or LP64 data model

### Other people's code ###
These masterpieces are incorporated in this project to avoid reinventing the
wheel.

* [dlmalloc][dlm] - Doug Lea's malloc with other memory management routines

[dlm]: http://g.oswego.edu/dl/html/malloc.html
