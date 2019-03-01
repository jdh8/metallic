void __wasm_call_ctors(void);
int main(void);

void _start(void)
{
    __wasm_call_ctors();
    main();
}
