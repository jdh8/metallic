CC := clang --target=wasm32-unknown-unknown-wasm
CPPFLAGS := -MMD -MP -Iinclude -D_METALLIC
CFLAGS := -pipe -O3 -Wall -flto -fno-builtin-memcpy
LDFLAGS := -nostdlib

metallic.bc: $(patsubst %.c, %.o, $(wildcard src/*/*.c src/*/*/*.c))
	llvm-link -o $@ $^

check: $(patsubst %.c, %.out, $(wildcard test/wasm/*/*.c)) $(patsubst %.c, %.exe, $(wildcard test/native/*/*.c))

%.out: %.c metallic.bc test/wasm/index.mjs
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -MQ $@ -o $@ $< metallic.bc
	node --experimental-modules test/wasm/index.mjs $@

%.exe: %.c
	cc $(CFLAGS) -MMD -MP -MQ $@ -march=native -lm -o $@ $<
	$@

clean:
	$(RM) *.{a,bc} */*/*{,/*}.{o,d,out,exe,run}

.DELETE_ON_ERROR:

-include */*/*.d */*/*/*.d
