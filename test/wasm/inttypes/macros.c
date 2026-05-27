#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    char buf[64];

    snprintf(buf, sizeof buf, "%" PRId64, INT64_C(-0x0123456789ABCDEF));
    assert(strcmp(buf, "-81985529216486895") == 0);

    snprintf(buf, sizeof buf, "%" PRIx64, UINT64_C(0xDEADBEEFCAFEBABE));
    assert(strcmp(buf, "deadbeefcafebabe") == 0);

    snprintf(buf, sizeof buf, "%" PRIuMAX, (uintmax_t)UINT64_C(0xFFFFFFFFFFFFFFFF));
    assert(strcmp(buf, "18446744073709551615") == 0);

    snprintf(buf, sizeof buf, "%" PRIdPTR, (intptr_t)-1);
    assert(strcmp(buf, "-1") == 0);

    snprintf(buf, sizeof buf, "%" PRId8 " %" PRIu16 " %" PRIo32,
             (int8_t)-7, (uint16_t)42, (uint32_t)8);
    assert(strcmp(buf, "-7 42 10") == 0);

    return 0;
}
