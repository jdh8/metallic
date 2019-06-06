#include <stddef.h>

void __wasm_call_ctors(void);

int __argc(void);

/*!
 * `argv` builder
 *
 * This function writes null-separated command line to non-null buffer.
 * For example, if the command line is `foo -c alfa`,
 * it is stored as `"foo\0-c\0alfa"`.
 * Note that there is an implied '\0' at the end of a string.
 *
 * \return Characters supposed to be written
 */
size_t __argv(char*);

static size_t _strlen(char s[static 1])
{
    for (size_t i = 0; ; ++i)
        if (!s[i])
            return i;
}

int main(int, char**);

int _start(void)
{
    int argc = __argc();
    char* argv[argc + 1];
    char args[__argv((void*)0)];

    __argv(args);
    argv[0] = args;

    for (int i = 0; i < argc - 1; ++i)
        argv[i + 1] = argv[i] + _strlen(argv[i]) + 1;

    argv[argc] = (void*)0;

    __wasm_call_ctors();
    main(argc, argv);
}
