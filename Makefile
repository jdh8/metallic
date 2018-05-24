CC := clang --target=wasm32-unknown-none-wasm
CPPFLAGS := -MMD -MP -Iinclude
CFLAGS := -pipe -O3 -Wall -flto
LDFLAGS := -nostdlib -fno-lto

metallic.a: metallic.bc
	llc -filetype=obj -o $@ $^

metallic.bc: $(patsubst %.c, %.o, $(wildcard src/*/*.c src/*/*/*.c))
	llvm-link $^ | opt -mergefunc -std-link-opts -o $@

check: test/index.mjs $(patsubst %.c, %.out, $(wildcard test/*/*.c))
	node --experimental-modules $^

test/%.out: metallic.a test/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -MQ $@ -o $@ $^

clean:
	$(RM) *.bc *.a */*/*.o */*/*/*.o */*/*.d */*/*/*.d test/*/*.out

-include */*/*.d */*/*/*.d
