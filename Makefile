CC := clang --target=wasm32-unknown-none-wasm
CPPFLAGS := -MMD -MP -Iinclude
CFLAGS := -pipe -O3 -Wall
LDFLAGS := -nostdlib -Wl,--allow-undefined

metallic.so: $(patsubst %.c, %.o, $(wildcard */*.c */*/*.c))
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	$(RM) *.so */*.o */*/*.o */*.d */*/*.d

-include */*.d */*/*.d
