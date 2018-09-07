/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "reinterpret.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>

static const uint64_t _data[] = {
                                            0x34007700D255F4FC, 0x825C326B5B2746ED,
    0x6DED727A8D39A9B8, 0x36169F3AC4A1A283, 0xB5D6DF8261DD9602, 0x4D7E6F5119A5ABF9,
    0x561408F1212A9DB5, 0x7E32B90F8EF5A7E7, 0xCA984E638270237C, 0xCE68B4ED09209F44,
    0x6919949A9529A828, 0xDC51A463DD27DDE1, 0xA365B1933D0B7CBD, 0x268A67F7BF920D2B,
    0x7BDBE5C23AC4EAF4, 0x2688893C78E4C4A8, 0x2A1A2C8012BF4390, 0x94792C8761107B4C,
    0xC5476243853B8621, 0xD3CFC50FAE5ADB86, 0x8A59C52BFA94C5C1, 0xFB49E956FFCA0F1C,
    0x39E6E389D49EE540, 0x086E59862A218347, 0xF65523882B55BA41, 0x5EA4677B7AACBAA2,
    0x42EB67DF9DFE5FD4, 0x9F405547DC3A74E1, 0x214A827B21CF98DC, 0x974C8836D1F9B392,
    0x0569F0B265A7887F, 0x51B133990ED7A948, 0x3826829BE7CAA40D, 0xAD38DC9367AAE855,
    0x56AE79E536228922, 0xC0549EFA864FFC06, 0x2C21D7F359987AD9, 0xDDDD1C2DA118AF46,
    0xC68351103ED84871, 0x17C99B5861BC57E1, 0xDD9B8E67EF3392B8, 0xEEBC34CC5DE8605E,
    0xB5F4F5CBFC2DBC34, 0x26EE523F0F117F11, 0xB8701A649569605A, 0x552AD6C6C096190B,
    0x538C994ECC2254DC, 0x6A4AFBD81C2F84B4, 0x2138C8D91B0AFC81, 0xEFC9352E61392F44,
    0x84D4E9A9B4FCD1EE, 0xF4E16D3B8E2C86E2, 0x3ECC5F114A0BFDFB, 0xB920D6AA5E6B9C2A,
    0x576F89562CE3CE99, 0xB40ECCCF2D891234, 0x36C59456A8D8B5A8, 0x97C427A831C36EF1,
    0xD913D9ECB9BA8BFC, 0xBEFDFDEF4556367E, 0x667E162901767A9F, 0x742C150D0C188194,
    0x24BA74607DE58AD8, 0xC9B55100B70DB2E2, 0xC6A9CFF7DF7329BA, 0x9D6ECD7B723E6A11,
    0x0893745D7CB2AD6B, 0xFFE26A6EC6FDB0C1, 0x35916FC5E0088DD7, 0xFD29F3D6E7CA7C22,
    0x0E423E1C87C461E9, 0xC6926E5D2EACED44, 0x18465F7555F5BDD2, 0x6913DAF03C3ABA46,
    0xC397A4D45877C5E3, 0x647CD8649B31BED9, 0x4EC8CAD641F3E8DE, 0x7BDC06CC45E0FA29,
    0x72953B06A33540C0, 0xB24855510EFB1EC3, 0x6B0701ACB08CD0C0, 0xDEC9F560BF11654D,
    0x800120CC86DD71B6, 0x5F57A4B13F149538, 0xC7F48D4D0DA63A20, 0x4FBCF82D985AD795,
    0x4F6A68A82A4A5AC4, 0x897BFEF3059EBFB9, 0x872AFDDAB6256B34, 0x640B148D4196DEBE,
    0x0027F0147F8607E1, 0xCA5C6A0817552E55, 0xC1DB9BBD17AB81A2, 0xF9B4CBDA11D0BE7D,
    0x20D3A2B30A5D2F42, 0x2D408DA0CD4F99A5, 0x698F77FF2780030C, 0x08D4B1EA21F716E4,
    0xC3B21D3632A1A4B7, 0x6E13E4305DA92E85, 0x50B7D51EC4F1335F, 0xE0566B0152993755,
    0x1262845CB9496170, 0x05A0DD5A476F21D2, 0xFD6CBFA200A4E46C, 0x6125CE70C9B8CB30,
    0x8416DF30432DC735, 0x3967955BBF3148D7, 0xBD4DAFE7B36E6D8F, 0x2833676186182AEA,
    0x156E85FF87FD073E, 0x76E4FCA90670803E, 0xFAFEDB89FDBE896C, 0x2A12162DB7DCFDE5,
    0x07DA0EAA76A1597B, 0xB6B42D297E804BA7, 0x9AAF009353AD0457, 0xC790B6DB2A3A25A3,
    0xDDBC5C3F6DDED01F, 0xF480C62FF0AC9AEC, 0xADC3DA4886A05DF7, 0xD12DF1B1C1998C77,
    0xBA66AACFCF761D02, 0xEBEF584A9BB7DAC4, 0x8953A6C6E26E6F00, 0x76F923048B7B7217,
    0xC1D20F3F0BCD785B, 0x331AAAF0A8654FA5, 0xE9424EA4BE13B523, 0xB33D8F1B5709CD36,
    0x040622FA1DE4D9A4, 0x3080ABF68C6615CB, 0xF96AF3BE75DF9328, 0x29BF56573AFF47B9,
    0x664D64B705ED3065, 0x15DFA1AE00DAFB0C, 0x6837DBFCABA1AE31, 0xDE6319EF8C76528B,
    0xA9E69492AB6842DD, 0xC6A5CA4ECD37A736, 0x5BC3D8C492F54BAD, 0x316D07B43915200C,
    0x03F6F0098C402B99, 0x9C7A2D9756C0833F, 0xEDAF12265C4DC26D, 0x0EAA3A51C2A31DAE,
    0x768364333B091A94, 0x22DBAA140A2F2689, 0xD2277549F6B6EBBB, 0xB989C7BD401004F2,
    0x185915A562BBCB61, 0x6C4BFFDB0F903876, 0x921110D8E80FAF80, 0x3715A0805C93805A,
    0xDDE7B7173BDF082B, 0xDE41319D39928C70, 0x3FA3FD94438C860D, 0x663DE06483DD9797,
    0x27FC00A871EA49C2, 0xA4049407777030F3, 0x9089D98850722CBE, 0xACCCC65414AD6AE2,
    0x6B49BA271D296996, 0xB4C576EA17F9877D, 0x9D072750045DDB3B, 0x090B69F52AD56614,
    0x5BD4714E6E5445C1, 0x10D86D324832754C, 0x8985295255C06437, 0xB25F3EDD035D407F,
    0x3398207E4BF56863, 0x91B45153D1F20ACF, 0x6EEF169FA6948EF6, 0x24D6BDA67DF77248,
    0xEE2C90AA4702E774, 0x30010C0D68084B58, 0x25318D3974F71C05, 0x5D8DB8159F09E5BC,
    0x486CA46742727132, 0x365F5F0EFDBBB49A, 0xE6E29731996BED88, 0x4189DFF49794E884,
    0x96CB314A1679E279, 0x550C90A7721FE76B, 0x2EC292472F327B6D, 0x9C12887B20AB9FB5,
    0x71C1B3092EF3450B, 0xB328F8C79405933E, 0xF53E1380F1ECC3E7, 0xBE00CC941EEECE70,
    0xD4E5492910D5FDFC, 0x91B24DF31700AD43, 0x4612C459C444C5C8, 0xC277C9121426049B,
    0x36D9CAD2A8288D61, 0x818D67C12645CA55, 0x6F63A62DCBBFF4EF, 0x78738A5A8CAFBDD7,
    0x775C83C2A3883C61, 0x0AB499D3F2A6067F, 0x425FAECE616AA428, 0x4A48D36710D8DDAA,
    0xF57FB0ADF2E91E43, 0x6212830148835B8E, 0x1DF35BE01834132E, 0x08CB7DE050C017A7,
    0x4D58E232CAC616E3, 0x9BDE2822D2E88628, 0x5DD7DE16DE3B5892, 0xCDC4EF09366CD43F,
    0x652289E83260BFE6, 0x9947FBACD87F7EB7, 0xFF319F6A1E666157, 0x1F001B0AF1DFCE19,
    0x24778AD623545AB9, 0xD9D63B3884A7CB23, 0xB07AE715175649C0, 0x64ABD770F87C6357,
    0x1810A3FC764D2A9D, 0xA7B4D55537F63ED7, 0x9B0062337CD2B497, 0x467D862D71E39AC6,
    0xC4AD414D2C5D000C, 0x15C614B59D19C3C2, 0xFA6ED5772D30433B, 0x87F121907C7C246A,
    0x9F3A1F35CAF27F1D, 0xC33D26EF6B1E5EF8, 0x32C2DE4F98327DBB, 0xA5FF07053F7E33E8,
    0xDDAF44D15719053E, 0x8359C4768B961CA6, 0x19C367CDDCE8092A, 0x60E27BC08C6B47C4,
    0x06061556CA73A8C9, 0x8DFFD8804D732731, 0x6599855F14A06840, 0xA9E391615EE61B08,
    0xF0CFBC209AF4361D, 0x56033046FC7B6BAB, 0x6BFB5FB11F8D5D08, 0x3D0739F78A5292EA,
    0x7527BAC7EBE5F17B, 0x4F463F669E5FEA2D, 0x6D367ECF27CB09B7, 0xEF2F118B5A0A6D1F,
    0x1FF897FFDE05980F, 0x9C845F8BBDF9283B, 0x3991D639835339F4, 0xE99C7026B45F7E41,
    0xE88235F52EBB4484, 0xFE1DEB1CB129A73E, 0x06492EEA09D1921C, 0xB7246E3A424DD2E0,
    0xFE5163ABDEBBC561, 0xDB6295993C439041, 0xFC2757D1F534DDC0, 0xA2F9836E4E441529,
                     0,                  0
};

static uint64_t _2opi_get(ptrdiff_t index, ptrdiff_t shift)
{
    const uint64_t* point = _data + sizeof(_data) / sizeof(uint64_t) - 2;

    if (shift)
        return point[-(index + 1)] >> (64 - shift) | point[-index] << shift;
    else
        return point[-index];
}

static uint64_t _2opi_64(ptrdiff_t precision)
{
    return _2opi_get(precision >> 6, precision & 63);
}

int __rem_pio2f(float x, double y[static 1])
{
    const double pi_2[] = { 1.57079631090164184570, 1.58932547735281966916e-8 };
    const double _2_pi = 0.63661977236758134308;

    int32_t i = reinterpret(int32_t, x);
    int32_t magnitude = i & 0x7FFFFFFF;

    if (magnitude < 0x4DC90FDB) {
        double q = rint(_2_pi * x) + 0;
        *y = x - q * pi_2[0] - q * pi_2[1];
        return q;
    }

    if (magnitude >= 0x7F800000) {
        *y = x - x;
        return 0;
    }

    const double pi_2_65 = 8.51530395021638647334e-20;

    int32_t exp = magnitude >> 23;
    uint64_t mantissa = (i & 0x007FFFFF) | 0x00800000;

    /* TODO Optimize 96-bit access */
    uint64_t high = _2opi_64(exp - 88);
    uint32_t low = _2opi_64(exp - 56);

    uint64_t product = mantissa * high + ((mantissa * low) >> 32);
    int64_t r = product << 2;
    int q = (product >> 62) + (r < 0);

    *y = copysign(pi_2_65, x) * r;

    return i < 0 ? -q : q;
}
