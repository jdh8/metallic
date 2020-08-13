static double split_(double x)
{
    double s = (0x1p27 + 1) * x;
    double c = x - s;

    return s + c;
}
