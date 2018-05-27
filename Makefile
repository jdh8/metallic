CC := clang --target=wasm32-unknown-none-wasm
CPPFLAGS := -MMD -MP -Iinclude
CFLAGS := -pipe -O3 -Wall -flto
LDFLAGS := -nostdlib -fno-lto

metallic.a: metallic.bc
	llc -filetype=obj -o $@ $<

metallic.bc: $(patsubst %.c, %.o, $(wildcard src/*/*.c src/*/*/*.c))
	llvm-link $^ | opt -mergefunc -std-link-opts -o $@

check: test/index.mjs $(patsubst %.c, %.out, $(wildcard test/*/*.c))
	node --experimental-modules $^

%.out: %.c metallic.a
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -MQ $@ -o $@ $< metallic.a

clean:
	$(RM) *.bc *.a */*/*.{o,d,out} */*/*/*.[od]

-include */*/*.d */*/*/*.d
