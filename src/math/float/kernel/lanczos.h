static double lanczos_series_(double z)
{
    const double p[] = {
        2.5066282972608683788,
        93.724008703110370647,
       -85.030278048785721233,
        15.313090747320992728,
       -0.2376573970640603182
    };

    return p[4] / (z + 4) + p[3] / (z + 3) + (p[2] / (z + 2) + p[1] / (z + 1)) + p[0];
}

static const double lanczos_g_ = 4.3644453082153116114;
