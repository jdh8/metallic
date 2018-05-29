CC := clang --target=wasm32-unknown-none-wasm
CPPFLAGS := -MMD -MP -Iinclude
CFLAGS := -pipe -O3 -Wall -flto
LDFLAGS := -nostdlib -fno-lto

metallic.a: metallic.bc
	llc -filetype=obj -o $@ $<

metallic.bc: $(patsubst %.c, %.o, $(wildcard src/*/*.c src/*/*/*.c))
	llvm-link $^ | opt -mergefunc -std-link-opts -o $@

check: check/wasm check/native

check/wasm: test/wasm/index.mjs $(patsubst %.c, %.out, $(wildcard test/wasm/*/*.c))
	node --experimental-modules $^

%.out: %.c metallic.a
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -MQ $@ -o $@ $< metallic.a

check/native: $(patsubst %.c, %.run, $(wildcard test/native/*/*.c))

%.run: %.exe
	$<

%.exe: %.c
	cc -MMD -MP -MQ $@ -pipe -O3 -Wall -march=native -lm -o $@ $<

clean:
	$(RM) *.{a,bc} */*/*{,/*}.{o,d,out}

-include */*/*.d */*/*/*.d
