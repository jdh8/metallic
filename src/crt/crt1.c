#include <stddef.h>
#include <stdint.h>

#include "../wasi/wasi.h"

void __wasm_call_ctors(void);

static size_t strlen_(char s[static 1])
{
    for (size_t i = 0; ; ++i)
        if (!s[i])
            return i;
}

int __main_argc_argv(int, char**);

__attribute__((__weak__))
int main(void)
{
    size_t argc = 0;
    size_t buf_size = 0;
    if (__wasi_args_sizes_get(&argc, &buf_size) != 0)
        return __main_argc_argv(0, (char*[]){ NULL });

    /* `args_get` fills both a pointer array (one entry per arg) and a flat
     * null-separated buffer that those pointers reference.  Allocate both on
     * the caller's stack as VLAs — the pointer array is null-terminated so
     * crt1 can hand it directly to the user's main(int, char**) shim. */
    char* argv[argc + 1];
    char args[buf_size];

    if (__wasi_args_get((uint8_t**)argv, (uint8_t*)args) != 0)
        return __main_argc_argv(0, (char*[]){ NULL });

    /* WASI populates argv[] with pointers into args[]; rebase them so they
     * point into the local args[] buffer rather than wherever WASI wrote. */
    if (argc > 0) {
        argv[0] = args;
        for (size_t i = 0; i + 1 < argc; ++i)
            argv[i + 1] = argv[i] + strlen_(argv[i]) + 1;
    }
    argv[argc] = NULL;

    return __main_argc_argv((int)argc, argv);
}

extern uintptr_t __metallic_brk;

_Noreturn void _start(void)
{
    __metallic_brk = ((uintptr_t)__builtin_frame_address(0) & 0xFF) + 1;
    __wasm_call_ctors();

    int rc = main();

    __wasi_proc_exit((__wasi_exitcode_t)(unsigned)rc);
}
