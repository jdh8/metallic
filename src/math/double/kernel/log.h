/* Restriction of (x -> 2 atanh(x) / x - 2) to [-c, c], where
 *
 *     √2 - 1                  1 + c
 * c = ------  the solution to ----- = √2.
 *     √2 + 1,                 1 - c
 */
static double _kernel_log(double x)
{
    const double c[] = {
        0.66666666666666697269,
        0.39999999999898506593,
        0.28571428626371267947,
        0.22222211077397895313,
        0.18182892887251091476,
        0.15331607963815718446,
        0.14617736819453644924
    };

    x *= x;

    double xx = x * x;

    return c[0] * x + (c[1] + c[2] * x) * xx + (c[3] + c[4] * x + (c[5] + c[6] * x) * xx) * (xx * xx);
}
