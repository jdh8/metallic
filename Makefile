CC := clang --target=wasm32-unknown-none-wasm
CPPFLAGS := -MMD -MP
CFLAGS := -pipe -Iinclude -O3 -Wall -flto
LDFLAGS := -nostdlib -fno-lto

metallic.bc: $(patsubst %.c, %.o, $(filter-out test/%, $(wildcard */*.c */*/*.c)))
	llvm-link -o $@ $^

test/%.wasm: metallic.bc test/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	$(RM) *.a */*.o */*/*.o */*.d */*/*.d

-include */*.d */*/*.d
