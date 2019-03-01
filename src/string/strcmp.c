int strcmp(const char lhs[static 1], const char rhs[static 1])
{
    const unsigned char* x = (const unsigned char*)lhs;
    const unsigned char* y = (const unsigned char*)rhs;

    while (*x == *y && *x) {
        ++x;
        ++y;
    }

    return *x - *y;
}

