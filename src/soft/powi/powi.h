static Scalar _powu(Scalar x, unsigned i)
{
    Scalar y = 1;

    for (; i; i >>= 1) {
        if (i & 1)
            y *= x;
        x *= x;
    }
    return y;
}

static Scalar _powi(Scalar x, int i)
{
    if (i < 0)
        return _powu(1 / x, -i);

    return _powu(x, i);
}
