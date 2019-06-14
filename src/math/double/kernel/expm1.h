/* Restriction of expm1(a + b) to [-0.5 ln 2, 0.5 ln 2]
 * where |b| < |a|
 */
static void kernel_expm1_(double y[static 2], double a, double b)
{
    const double c[] = {
       -3.33333333333271196132e-2,
        1.58730158656754696783e-3,
       -7.93650511970857863499e-5,
        4.00786528423672283783e-6,
       -1.99315105709308306083e-7
    };

    double s = a + b;
    double e = a - s + b;
    double h = 0.5 * s;
    double z = h * s;
    double zz = z * z;
    double r = 1 + c[0] * z + (c[1] + c[2] * z) * zz + (c[3] + c[4] * z) * (zz * zz);
    double t = r * h - 3;

    y[0] = s;
    y[1] = s * (e - (r + t) / (6 + s * t) * z) + e + z;
}
