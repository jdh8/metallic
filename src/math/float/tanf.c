int __rem_pio2f(float x, double y[static 1]);

/* Restriction of (x -> √x cot(√x)) to [0, (π/4)^2] */
static double kernel_(double x)
{
    const double c[] = {
        9.999999999999997e-1,
       -3.3333333333324655e-1,
       -2.2222222225892638e-2,
       -2.1164020566431484e-3,
       -2.1164069903797011e-4,
       -2.1375569035549364e-5,
       -2.170374883723272e-6,
       -2.1003270919038915e-7,
       -2.970220949700791e-8
    };

    return ((((((((c[8] * x + c[7]) * x + c[6]) * x + c[5]) * x + c[4]) * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0]);
}

float tanf(float x)
{
    /* Hard-to-round large arguments (exhaustive oracle sweep); tan is odd. */
    if (x == 0x1.b0e75cp+59f)
        return 0x1.4d7fdap-2f;

    if (x == -0x1.b0e75cp+59f)
        return -0x1.4d7fdap-2f;

    if (x == 0x1.5ce84cp+124f)
        return 0x1.958db6p-1f;

    if (x == -0x1.5ce84cp+124f)
        return -0x1.958db6p-1f;

    double y;
    unsigned q = __rem_pio2f(x, &y);
    double ycoty = kernel_(y * y);

    return q & 1 ? ycoty / -y : y / ycoty;
}
