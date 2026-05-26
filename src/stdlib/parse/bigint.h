#ifndef METALLIC_STDLIB_PARSE_BIGINT_H_
#define METALLIC_STDLIB_PARSE_BIGINT_H_

#include "decimal.h"
#include <stdint.h>

/* 4096 bits.  Worst case: 770-digit decimal input (~2560 bits) times 2^673
 * for subnormal-boundary scaling, or M * 5^342 (~940 bits) on the other
 * side -- both fit comfortably. */
#define BIGINT_LIMBS_ 64

typedef struct {
    uint32_t len;
    uint64_t v[BIGINT_LIMBS_];
} bigint_t;

static void bigint_set_u64_(bigint_t* b, uint64_t x)
{
    b->len = x != 0;
    b->v[0] = x;
}

static void bigint_mul_u64_(bigint_t* b, uint64_t m)
{
    if (!m) {
        b->len = 0;
        return;
    }
    unsigned __int128 carry = 0;
    for (uint32_t i = 0; i < b->len; ++i) {
        unsigned __int128 p = (unsigned __int128)b->v[i] * m + carry;
        b->v[i] = (uint64_t)p;
        carry = p >> 64;
    }
    if (carry)
        b->v[b->len++] = (uint64_t)carry;
}

static void bigint_add_u64_(bigint_t* b, uint64_t a)
{
    uint64_t carry = a;
    for (uint32_t i = 0; carry; ++i) {
        if (i == b->len) {
            b->v[b->len++] = carry;
            return;
        }
        uint64_t s = b->v[i] + carry;
        carry = s < b->v[i];
        b->v[i] = s;
    }
}

/* 5^27 = 7450580596923828125 < 2^63 -- the largest 5^k fitting in u64. */
static void bigint_mul_pow5_(bigint_t* b, int k)
{
    while (k >= 27) {
        bigint_mul_u64_(b, UINT64_C(7450580596923828125));
        k -= 27;
    }
    if (k > 0) {
        uint64_t p = 1;
        for (int i = 0; i < k; ++i) p *= 5;
        bigint_mul_u64_(b, p);
    }
}

static void bigint_shl_(bigint_t* b, uint32_t n)
{
    if (!b->len)
        return;
    uint32_t limbs = n >> 6;
    uint32_t bits = n & 63;

    if (limbs) {
        for (int i = (int)b->len - 1; i >= 0; --i)
            b->v[i + limbs] = b->v[i];
        for (uint32_t i = 0; i < limbs; ++i)
            b->v[i] = 0;
        b->len += limbs;
    }

    if (bits) {
        uint64_t carry = 0;
        for (uint32_t i = 0; i < b->len; ++i) {
            uint64_t v = b->v[i];
            b->v[i] = (v << bits) | carry;
            carry = v >> (64 - bits);
        }
        if (carry)
            b->v[b->len++] = carry;
    }
}

static int bigint_cmp_(const bigint_t* a, const bigint_t* b)
{
    if (a->len != b->len)
        return a->len < b->len ? -1 : 1;
    for (int i = (int)a->len - 1; i >= 0; --i) {
        if (a->v[i] != b->v[i])
            return a->v[i] < b->v[i] ? -1 : 1;
    }
    return 0;
}

/* Compare the exact decimal value of d against the rational halfway
 * H = M * 2^q.  Returns -1/0/+1 for D < H, D == H, D > H.
 *
 * D = D_int * 10^p where D_int is the integer formed by all the digit
 * characters of d->digits and p = d->dec_exp.  The comparison is reduced
 * to a single bigint_cmp by multiplying both sides by enough 5s and 2s
 * to clear any negative exponents:
 *
 *   if p >= 0 and q-p >= 0:  D_int * 5^p  vs  M * 2^(q-p)
 *   if p >= 0 and q-p <  0:  D_int * 5^p * 2^(p-q)  vs  M
 *   if p <  0 and q-p >= 0:  D_int  vs  M * 5^(-p) * 2^(q-p)
 *   if p <  0 and q-p <  0:  D_int * 2^(p-q)  vs  M * 5^(-p)
 */
static int bigint_cmp_halfway_(const decimal_t* d, uint64_t M, int q)
{
    bigint_t lhs, rhs;
    /* d->dec_exp is the exponent paired with the truncated 38-digit mantissa.
     * When we build D_int from ALL digit characters, we need the "full" form:
     *   D = D_int_full * 10^(d->dec_exp - (total_digits - 38))   if total > 38.
     * (Equivalent: d->mant * 10^d->dec_exp ≈ D_int_full * 10^p, where the
     * truncated tail of 10^41 has been folded into the dec_exp.) */
    int n = d->total_digits;
    int p = d->dec_exp - (n > 38 ? n - 38 : 0);

    bigint_set_u64_(&lhs, 0);
    {
        const char* s = d->digits;
        int rem = n;
        while (rem > 0) {
            char c = *s++;
            if (c == '.')
                continue;
            bigint_mul_u64_(&lhs, 10);
            if (c > '0')
                bigint_add_u64_(&lhs, (uint64_t)(c - '0'));
            --rem;
        }
    }

    bigint_set_u64_(&rhs, M);

    if (p >= 0)
        bigint_mul_pow5_(&lhs, p);
    else
        bigint_mul_pow5_(&rhs, -p);

    int delta = q - p;
    if (delta >= 0)
        bigint_shl_(&rhs, (uint32_t)delta);
    else
        bigint_shl_(&lhs, (uint32_t)-delta);

    return bigint_cmp_(&lhs, &rhs);
}

#endif
