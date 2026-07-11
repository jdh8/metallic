/* Rational approximation of atan on [-1, 1]: atan(x) ~ x * num(x^2)/den(x^2) */
static const double atanf_num_[] = {
    0.3300049005002112,
    0.8269936280545194,
    0.7536692262484512,
    0.30412501920352053,
    0.0525854645006143,
    0.003092811576351314,
    0.000026680446286035432
};

static const double atanf_den_[] = {
    0.33000490050021114,
    0.9369952615545891,
    1.0,
    0.4972028574382381,
    0.11550900511647666,
    0.010902245201868124,
    0.00027322693079551304
};

/* atan(x)/x as num(x^2)/den(x^2), for callers that scale the quotient
 * themselves (atanpif folds 1/pi into the leading factor). */
static inline double kernel_atanf_ratio_(double x)
{
    const double *n = atanf_num_;
    const double *d = atanf_den_;

    double xx = x * x;
    double num = ((((((n[6] * xx + n[5]) * xx + n[4]) * xx + n[3]) * xx + n[2]) * xx + n[1]) * xx + n[0]);
    double den = ((((((d[6] * xx + d[5]) * xx + d[4]) * xx + d[3]) * xx + d[2]) * xx + d[1]) * xx + d[0]);

    return num / den;
}

/* Arctangent restricted to [-1, 1] */
static inline double kernel_atanf_(double x)
{
    const double *n = atanf_num_;
    const double *d = atanf_den_;

    double xx = x * x;
    double num = ((((((n[6] * xx + n[5]) * xx + n[4]) * xx + n[3]) * xx + n[2]) * xx + n[1]) * xx + n[0]);
    double den = ((((((d[6] * xx + d[5]) * xx + d[4]) * xx + d[3]) * xx + d[2]) * xx + d[1]) * xx + d[0]);

    return x * num / den;
}
