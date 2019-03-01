/* Arctangent restricted to [-1, 1] */
static double _kernel_atanf(double x)
{
    const double n[] = {
        80.3271869581482272,
        72.2722811622844352,
        11.0979939649070095
    };

    const double d[] = {
        80.3271884011195215,
        99.0478590867740213,
        28.0510393790794482,
    };

    double x2 = x * x;
    double x4 = x2 * x2;
    double num = n[0] + n[1] * x2 + n[2] * x4;
    double den = d[0] + d[1] * x2 + (d[2] + x2) * x4;

    return x * num / den;
}
