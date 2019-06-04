#include <bits/types/loff.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

int __pwrite(int, const void*, size_t, __loff_t);

#define PRINT(x, offset) __pwrite(1, x, sizeof(x) - 1, offset)

int main(void)
{
    assert(PRINT("Help, I'm turned into a parrot!\n", 16) >= 0);
}
