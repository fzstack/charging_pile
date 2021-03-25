#include "hlw8112.hxx"
#include <rtthread.h>

using namespace std;
using namespace Hlw8112ns::Regs;

Hlw8112::Hlw8112() {
    writable.onChanged += [this](auto value) {
        if(!value) return;
        if(*value) {
          writeEnable();
        } else {
          writeDisable();
        }
    };

    inited.onChanged += [this](auto value) {
        if(!value) return;
        specCmd(0x96);
        rt_thread_mdelay(1);
        writable = nullopt;
        writable = true;
        {
            auto sess = makeSess<emucon2>();
            sess->chs_ib = true;
            sess->dup_sel = emucon2::DupSel::f3_4Hz;
            sess->sdo_cmos = false;

            sess->wave_en = true; //使能瞬时波形
            sess->zx_en = true; //使能相角检测
        } {
            auto sess = makeSess<emucon>();
            sess->hpf_i_a_off = false; //关闭高通滤波器
            sess->hpf_i_b_off = false;
            sess->hpf_u_off = false;
            sess->comp_off = true;
        } {
            auto sess = makeSess<pin>();
            sess->p1_sel = pin::PSel::IRQ;
        } {
            auto sess = makeSess<ie>();
            sess->dupd = true;
        } {
            auto sess = makeSess<syscon>();
            sess->adc1_on = true;
            sess->pga_i_a = syscon::PGA::x16;
            sess->adc2_on = true;
            sess->pga_i_b = syscon::PGA::x16;
            sess->adc3_on = true;
            sess->pga_u = syscon::PGA::x1;
        }
        writable = false;
    };
}

void Hlw8112::selectChannelA() {
    specCmd(0x5a);
}

void Hlw8112::selectChannelB() {
    specCmd(0xa5);
}

void Hlw8112::writeEnable() {
    specCmd(0xe5);
}

void Hlw8112::writeDisable() {
    specCmd(0xdc);
}

void Hlw8112::init() {
    inited = true;
}

void Hlw8112::specCmd(char cmd) {
    this->cmd(0xea, (void*)&cmd, 1);
}

void Hlw8112::writeReg(int addr, void* data, int len) {
    cmd(addr | 0x80, data, len);
}

