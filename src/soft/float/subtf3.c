long double __addtf3(long double, long double);

long double __subtf3(long double x, long double y)
{
    return __addtf3(x, -y);
}
