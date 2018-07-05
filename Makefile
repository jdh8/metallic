CC := clang --target=wasm32-unknown-none-wasm
CPPFLAGS := -MMD -MP -Iinclude -D_METALLIC
CFLAGS := -pipe -O3 -Wall -flto
LDFLAGS := -nostdlib -fno-lto

metallic.a: metallic.bc
	llc -filetype=obj -o $@ $<

metallic.bc: $(patsubst %.c, %.o, $(wildcard src/*/*.c src/*/*/*.c))
	llvm-link $^ | opt -mergefunc -std-link-opts -o $@

check: $(patsubst %.c, %.run, $(wildcard test/*/*/*.c))

%.run: %.out test/wasm/index.mjs
	node --experimental-modules test/wasm/index.mjs $<

%.out: %.c metallic.a
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -MQ $@ -o $@ $< metallic.a

%.run: %.exe
	$<

%.exe: %.c
	cc -MMD -MP -MQ $@ -pipe -O3 -Wall -march=native -lm -o $@ $<

clean:
	$(RM) *.{a,bc} */*/*{,/*}.{o,d,out,exe}

-include */*/*.d */*/*/*.d
