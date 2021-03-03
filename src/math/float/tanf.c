int __rem_pio2f(float x, double y[static 1]);

/* Restriction of (x -> √x cot(√x)) to [0, (π/4)^2] */
static double kernel_(double x)
{
    const double c[] = {
        9.9999999547970088679e-1,
       -3.3333297156778390385e-1,
       -2.2226847402057085257e-2,
       -2.0958255714778903209e-3,
       -2.4838647503256150943e-4
    };

    return (((c[4] * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0];
}

float tanf(float x)
{
    double y;
    unsigned q = __rem_pio2f(x, &y);
    double ycoty = kernel_(y * y);

    return q & 1 ? ycoty / -y : y / ycoty;
}
