CC := clang --target=wasm32-unknown-none-wasm
CPPFLAGS := -MMD -MP -Iinclude -D_METALLIC
CFLAGS := -pipe -O3 -Wall -flto
LDFLAGS := -nostdlib

metallic.bc: $(patsubst %.c, %.o, $(wildcard src/*/*.c src/*/*/*.c))
	llvm-link -o $@ $^

check: $(patsubst %.c, %.run, $(wildcard test/*/*/*.c))

test/wasm/%.run: test/wasm/%.out test/wasm/index.mjs
	node --experimental-modules test/wasm/index.mjs $< > $@

%.out: %.c metallic.bc
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -MQ $@ -o $@ $< metallic.bc

test/native/%.run: test/native/%.exe
	$< > $@

%.exe: %.c
	cc $(CFLAGS) -MMD -MP -MQ $@ -march=native -lm -o $@ $<

clean:
	$(RM) *.{a,bc} */*/*{,/*}.{o,d,out,exe,run}

.DELETE_ON_ERROR:

-include */*/*.d */*/*/*.d
