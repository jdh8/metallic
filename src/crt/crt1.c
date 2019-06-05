#include <stdint.h>

void __wasm_call_ctors(void);

/*!
 * `argc` getter
 *
 * This function returns 0 if passing arguments to main() is unsupported.
 */
int __argc(void);

/*!
 * `argv` builder
 *
 * This function stores `argv` as offset from `argv[0]`,
 * and appends `sizeof(args)`.
 *
 * For example, if the command line is `foo -c alfa`,
 * { 0, 4, 7, 12 } will be written because
 *
 *     argv[0] = args + 0
 *     argv[1] = args + 4
 *     argv[2] = args + 7
 *     sizeof(args) = 12
 *
 * See __args() for format of `args`.
 */
void __argv(uintptr_t* argv);

/*!
 * This function stores null-separated command line.
 *
 * For example, if the command line is `foo -c alfa`,
 * it is stored as `"foo\0-c\0alfa"`.
 * Note that there is an implied '\0' at the end of a string.
 */
void __args(char* args);

int main(int argc, char** argv);

void _start(void)
{
    int argc = __argc();
    char* argv[argc + 1];
    __argv((uintptr_t*)argv);

    char args[argc ? (uintptr_t)argv[argc] : 0];
    __args(args);

    for (int i = 0; i < argc; ++i)
        argv[i] = args + (uintptr_t)argv[i];

    argv[argc] = (void*)0;

    __wasm_call_ctors();
    main(argc, argv);
}
