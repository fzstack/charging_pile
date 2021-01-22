/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     imgcr       the first version
 */
#ifndef DEVICES_HLW8112_REGS_HXX_
#define DEVICES_HLW8112_REGS_HXX_

#include <rtthread.h>

namespace Hlw8112ns::Regs {

template <class T, int Addr, int Size=0, bool Readonly=false>
struct def {
    using type = T;
    static const int addr = Addr;
    static const int size = Size;
    static const int readonly = Readonly;
};

struct def_syscon {
    rt_uint16_t pga_i_a: 3;
    rt_uint16_t pga_u: 3;
    rt_uint16_t pga_i_b: 3;
    rt_uint16_t adc1_on: 1;
    rt_uint16_t adc2_on: 1;
    rt_uint16_t adc3_on: 1;
};
struct syscon: public def<def_syscon, 0x00> {
    struct PGA {
        enum Value {
            x1 = 0, x2, x4, x8, x16,
        };
    };
};

struct def_emucon {
    rt_uint16_t pa_run: 1;
    rt_uint16_t pb_run: 1;
    rt_uint16_t reserved1: 2;
    rt_uint16_t hpf_u_off: 1;
    rt_uint16_t hpf_i_a_off: 1;
    rt_uint16_t hpf_i_b_off: 1;
    rt_uint16_t zxd0: 1;
    rt_uint16_t zxd1: 1;
    rt_uint16_t dc_mode: 1;
    rt_uint16_t pmode: 2;
    rt_uint16_t comp_off: 1;
    rt_uint16_t tensor_en: 1;
    rt_uint16_t tensor_step: 2;
};
using emucon = def<def_emucon, 0x01>;

struct def_emucon2 {
    rt_uint16_t vref_sel: 1;
    rt_uint16_t peak_en: 1;
    rt_uint16_t zx_en: 1;
    rt_uint16_t over_en: 1;
    rt_uint16_t sag_en: 1;
    rt_uint16_t wave_en: 1;
    rt_uint16_t p_factor_en: 1;
    rt_uint16_t chs_ib: 1;
    rt_uint16_t dup_sel: 2;
    rt_uint16_t epa_cb: 1;
    rt_uint16_t epb_cb: 1;
    rt_uint16_t sdo_cmos: 1;
    rt_uint16_t reserved2: 2;
    rt_uint16_t dotp_sel: 1;
};
struct emucon2: public def<def_emucon2, 0x13> {
    struct DupSel {
        enum Value {
            f3_4Hz,
            f6_8Hz,
            f13_65Hz,
            f27_3Hz,
        };
    };
};

struct def_pin {
    rt_uint16_t p1_sel: 4;
    rt_uint16_t p2_sel: 4;
};
struct pin: public def<def_pin, 0x1d> {
    struct PSel {
        enum Value {
            PfaOut,
            PfbOut,
            LeakageIndicate,
            IRQ, //发生中断则低电平
            POverloadIndicate,
            ChAMinusPIndicate,
            ChBMinusPIndicate,
            InstantValUpdateIRQ,
            AverageValUpdateIRQ,
            VoltageZeroCrossing,
            IAZeroCrossing,
            IBZeroCrossing,
            OverVoltageIndicate,
            UnderVoltageIndicate,
            ChAOverCurrentIndicate,
            ChBOverCurrentIndicate,
        };
    };
};

using angle = def<uint16_t, 0x22>;

struct def_rms_i_a {
    rt_uint32_t data: 24;
};
using rms_i_a = def<def_rms_i_a, 0x24, 3>;

struct def_rms_i_b {
    rt_uint32_t data: 24;
};
using rms_i_b = def<def_rms_i_b, 0x25, 3>;


struct def_rms_u {
    rt_uint32_t data: 24;
};
using rms_u = def<def_rms_u, 0x26, 3>;


struct def_peak_i_a {
    rt_uint32_t data: 24;
};
using peak_i_a = def<def_peak_i_a, 0x30, 3>;

struct def_peak_u {
    rt_uint32_t data: 24;
};
using peak_u = def<def_peak_u, 0x32, 3>;

using rms_i_a_c = def<uint16_t, 0x70>;

using rms_i_b_c = def<uint16_t, 0x71>;

using rms_u_c = def<uint16_t, 0x72>;

struct def_ie {
    rt_uint16_t dupd: 1; //<- 均值数据更新
    rt_uint16_t pfa: 1;
    rt_uint16_t pfb: 1;
    rt_uint16_t peao: 1;
    rt_uint16_t pebo: 1;
    rt_uint16_t reserved: 1;
    rt_uint16_t instan: 1;
    rt_uint16_t oia: 1;
    rt_uint16_t oib: 1;
    rt_uint16_t ov: 1;
    rt_uint16_t op: 1;
    rt_uint16_t sag: 1;
    rt_uint16_t zx_ia: 1;
    rt_uint16_t zx_ib: 1;
    rt_uint16_t zx_u: 1;
    rt_uint16_t leakage: 1;
};
using ie = def<def_ie, 0x40>;

struct def_if { //只读寄存器, 读清零
    rt_uint16_t dupd: 1; //<- 均值数据更新
    rt_uint16_t pfa: 1;
    rt_uint16_t pfb: 1;
    rt_uint16_t peao: 1;
    rt_uint16_t pebo: 1;
    rt_uint16_t reserved: 1;
    rt_uint16_t instan: 1;
    rt_uint16_t oia: 1;
    rt_uint16_t oib: 1;
    rt_uint16_t ov: 1;
    rt_uint16_t op: 1;
    rt_uint16_t sag: 1;
    rt_uint16_t zx_ia: 1;
    rt_uint16_t zx_ib: 1;
    rt_uint16_t zx_u: 1;
    rt_uint16_t leakage: 1;
};
using ifr = def<def_if, 0x41, 0, true>;

struct def_sys_status { //只读寄存器
    rt_int8_t rst: 1;
    rt_int8_t reserved1: 3;
    rt_int8_t wren: 1; //写使能
    rt_int8_t reserved2: 1;
    rt_int8_t clk_sel: 1;
};
struct sys_status: public def<def_sys_status, 0x43, 0, true> {
    struct ClkSel {
        enum Value {
            External,
            Internal,
        };
    };
};

}
#endif /* DEVICES_HLW8112_REGS_HXX_ */
