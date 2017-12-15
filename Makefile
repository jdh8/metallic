CC := clang --target=wasm32-unknown-none-wasm
CFLAGS := -pipe -O3 -Iinclude
LDFLAGS := -nostdlib -Wl,--allow-undefined

metallic.so: $(patsubst %.c, %.o, $(wildcard */*.c */*/*.c))
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	$(RM) *.so */*.o */*/*.o
