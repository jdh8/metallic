/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2019 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
int __rem_pio2(double x, double y[static 2]);

static double _kernel_sin(double greater, double lesser)
{
    const double c[] = {
       -1.6666666666666664626e-01,
        8.3333333333309503628e-03,
       -1.9841269836761008913e-04,
        2.7557316103657515000e-06,
       -2.5051132049739777141e-08,
        1.5918142570874201633e-10
    };

    double y = greater + lesser;
    double x = y * y;
    double xx = x * x;

    return (c[0] + c[1] * x + (c[2] + c[3] * x) * xx + (c[4] + c[5] * x) * (xx * xx)) * (x * y) + lesser + greater;
}

static double _kernel_cos(double greater, double lesser)
{
    const double c[] = {
        4.1666666666666599556e-02,
       -1.3888888888874138650e-03,
        2.4801587289491835985e-05, 
       -2.7557314355244334647e-07,
        2.0875723684774487669e-09,
       -1.1359669885573144321e-11
    };

    double y = greater + lesser;
    double x = y * y;
    double xx = x * x;

    return 1 - 0.5 * x + (c[0] + c[1] * x) * xx + (c[2] + c[3] * x + (c[4] + c[5] * x) * xx) * (xx * xx);
}
