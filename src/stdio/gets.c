#include <stdio.h>

char* gets(char buffer[static 1])
{
    for (char* s = buffer; ; ++s) {
        int c = getchar();

        switch (c) {
            case '\n':
                *s = '\0';
                return buffer;
            case EOF:
                *s = '\0';
                return (void*)0;
        }
        *s = c;
    }
}
