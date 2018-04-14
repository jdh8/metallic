CC := clang --target=wasm32-unknown-none-wasm
CPPFLAGS := -MMD -MP -Iinclude
CFLAGS := -pipe -O3 -Wall -emit-llvm

metallic.a: $(patsubst %.c, %.o, $(filter-out test/%, $(wildcard */*.c */*/*.c)))
	llvm-link -o $@ $^

clean:
	$(RM) *.a */*.o */*/*.o */*.d */*/*.d

-include */*.d */*/*.d
