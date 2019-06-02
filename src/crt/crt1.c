#include <stdint.h>

void __wasm_call_ctors(void);

int __argc(void);
void __argv(uintptr_t[static 1]);
void __args(char[static 1]);

int main(int, char**);

void _start(void)
{
    int argc = __argc();
    char* argv[argc + 1];
    __argv((uintptr_t*)argv);

    char args[(uintptr_t)argv[argc]];
    __args(args);

    for (int i = 0; i < argc; ++i)
        argv[i] = args + (uintptr_t)argv[i];

    argv[argc] = (void*)0;

    __wasm_call_ctors();
    main(argc, argv);
}
