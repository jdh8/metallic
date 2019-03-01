#define CAT2(a, b) a##b
#define CAT(a, b)  CAT2(a, b)

Integer CAT(PREFIX, abs)(Integer a)
{
    return a < 0 ? -a : a ? a : 0;
}
