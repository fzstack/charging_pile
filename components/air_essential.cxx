/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-18     imgcr       the first version
 */

#include "air_essential.hxx"
#include <stdexcept>

#define LOG_TAG "dev.air_ess"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>


using namespace std;

AirEssential::AirEssential(std::shared_ptr<Air724> owner): AirComponent<AirEssential>(owner) {

}

string AirEssential::getImei() {
    auto resp = createResp();
    auto atClient = getAtClient();
    if(at_obj_exec_cmd(atClient, resp.get(), "AT+CGSN") != RT_EOK) {
        throw runtime_error{"timeout when getting imei"};
    }
    return {at_resp_get_line(resp.get(), 2), 15};
}

string AirEssential::getIccid() {
    auto resp = createResp();
    auto atClient = getAtClient();
    if(at_obj_exec_cmd(atClient, resp.get(), "AT+ICCID") != RT_EOK) {
        throw runtime_error{"timeout when getting iccid"};
    }
    auto iccid = shared_ptr<char>(new char[22]);
    if(at_resp_parse_line_args_by_kw(resp.get(), "+ICCID:", "+ICCID: %s", iccid.get()) <= 0) {
        throw runtime_error{"iccid parse failed"};
    }
    return {iccid.get(), 20};
}

int AirEssential::getCsq() {
    auto resp = createResp();
    auto atClient = getAtClient();
    if(at_obj_exec_cmd(atClient, resp.get(), "AT+CSQ") != RT_EOK) {
        throw runtime_error{"timeout when getting CSQ"};
    }
    int result;
    if(at_resp_parse_line_args_by_kw(resp.get(), "+CSQ:", "+CSQ: %d", &result) <= 0) {
        throw runtime_error{"CSQ parse failed"};
    }
    return result;
}

void AirEssential::attachGprs(){
    auto resp = createResp();
    auto atClient = getAtClient();
    int cgattVal;
    do {
        if(at_obj_exec_cmd(atClient, resp.get(), "AT+CGATT?") != RT_EOK)
            throw runtime_error{"timeout when getting CGATT"};
        if(at_resp_parse_line_args_by_kw(resp.get(), "CGATT:", "+CGATT: %d", &cgattVal) <= 0)
            throw runtime_error{"CGATT parse failed"};
        if(cgattVal != 1)
            rt_thread_mdelay(1000);
    } while(cgattVal != 1);
}

void AirEssential::activatePdp() {
    auto resp = createResp();
    auto atClient = getAtClient();
    for(auto i = 0; i < 3; i++) {
        int cid, status;
        if(at_obj_exec_cmd(atClient, resp.get(), "AT+SAPBR=2,1") != RT_EOK)
            throw runtime_error{"timeout when activating PDP"};
        if(at_resp_parse_line_args_by_kw(resp.get(), "+SAPBR:", "+SAPBR: %d,%d", &cid, &status) < 2)
            throw runtime_error{"SAPBR parse failed"};
        if(status == 1)
            return;
        LOG_W("PDP not activated(%d)", i);
        if(at_obj_exec_cmd(atClient, resp.get(), "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"") != RT_EOK)
            throw runtime_error{"timeout when activating PDP"};
        if(at_obj_exec_cmd(atClient, resp.get(), "AT+SAPBR=3,1,\"APN\",\"CMIOT\"") != RT_EOK)
            throw runtime_error{"timeout when activating PDP"};
        if(at_obj_exec_cmd(atClient, resp.get(), "AT+SAPBR=1,1") != RT_EOK)
            throw runtime_error{"timeout when activating PDP"};
    }
    throw runtime_error{"too many attempts"};
}

void AirEssential::closeEcho() {
    auto resp = createResp();
    auto atClient = getAtClient();
    if(at_obj_exec_cmd(atClient, resp.get(), "ATE0") != RT_EOK) {
        throw runtime_error{"timeout when closing echo"};
    }
}
