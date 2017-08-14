inline int iscntrl(int c) { return c < 32u || c == 127; }
inline int isprint(int c) { return c - 32 < 95u; }
inline int isblank(int c) { return c == 32 || c == 9; }
inline int isspace(int c) { return c == 32 || c - 9 < 5u; }
inline int isgraph(int c) { return c - 33 < 94u; }

inline int isupper(int c) { return c - 'A' < 26u; }
inline int islower(int c) { return c - 'a' < 26u; }
inline int isalpha(int c) { return islower(c | 0x20); }

inline int isdigit(int c) { return c - '0' < 10u; }
inline int isxdigit(int c) { return isdigit(c) || (c | 0x20) - 'a' < 6u; }

inline int isalnum(int c) { return isdigit(c) || isalpha(c); }
inline int ispunct(int c) { return isgraph(c) && !isalnum(c); }
