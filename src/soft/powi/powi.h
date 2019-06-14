static Scalar powu_(Scalar x, unsigned i)
{
    Scalar y = 1;

    for (; i; i >>= 1) {
        if (i & 1)
            y *= x;
        x *= x;
    }
    return y;
}

static Scalar powi_(Scalar x, int i)
{
    if (i < 0)
        return powu_(1 / x, -i);

    return powu_(x, i);
}
