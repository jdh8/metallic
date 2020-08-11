/* Error function restricted to [-0.84375, 0.84375] */
static double kernel_(double x)
{
    const double c[] = {
        1.2837916709551257358e-01,
       -3.7612638903183740346e-01,
        1.1283791670954367814e-01,
       -2.6866170644945538905e-02,
        5.2239776230901943479e-03,
       -8.5483268475636548560e-04,
        1.2055324632852127628e-04,
       -1.4925388936963054191e-05,
        1.6456634784970233105e-06,
       -1.6289507107602459607e-07,
        1.4124231832943601482e-08,
       -8.6697290255536863161e-10
    };

    double x2 = x * x;
    double x4 = x2 * x2;
    double x8 = x4 * x4;

    return x + x * (c[0] + c[1] * x2 + (c[2] + c[3] * x2) * x4
        + (c[4] + c[5] * x2 + (c[6] + c[7] * x2) * x4) * x8
        + (c[8] + c[9] * x2 + (c[10] + c[11] * x2) * x4) * (x8 * x8));
}
