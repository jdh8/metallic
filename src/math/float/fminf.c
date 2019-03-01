float fminf(float x, float y)
{
    return x <= y || y != y ? x : y;
}
