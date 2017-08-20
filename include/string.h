#ifdef __cplusplus
extern "C" {
#endif

void* memchr(const void* source, int c, size_t length);
int memcmp(const void* lhs, const void* rhs, size_t length);
void* memcpy(void*__restrict destination, const void*__restrict source, size_t length);
void* memmove(void* destination, const void* source, size_t length);
void* memset(void* destination, int c, size_t length);

#ifdef __cplusplus
}
#endif
