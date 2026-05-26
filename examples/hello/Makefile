# Standalone example: depends on a built metallic.a in the parent
# project.  From this directory:
#   make
#   wasmtime hello.out one two three

METALLIC ?= ../..

CC := clang --target=wasm32-unknown-unknown-wasm
CFLAGS := -O3 -Wall -nostdlib
LDLIBS := $(METALLIC)/metallic.a

hello.out: main.c $(LDLIBS)
	$(CC) $(CFLAGS) -I $(METALLIC)/include -o $@ $< $(LDLIBS)

.PHONY: run clean
run: hello.out
	wasmtime hello.out one two three

clean:
	$(RM) hello.out
