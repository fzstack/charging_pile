/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-20     imgcr       the first version
 */
#ifndef APPLICATIONS_BACKUP_THING_DECO_HXX_
#define APPLICATIONS_BACKUP_THING_DECO_HXX_

#include "thing_deco.hxx"
#include <utilities/observable.hxx>
#include <config/bsp.hxx>
#include <components/timer.hxx>

class BackupThingDeco: public ThingDeco {
    friend outer_t;
    BackupThingDeco(outer_t* outer);
    virtual void init() override;

    Observable<bool> inited = false;

    template<int N>
    struct Backup {
        int leftSeconds = 0;
        int timerId = 0;
        float consumption = 0;
    };

private:
    struct PortSpec {
        std::shared_ptr<Timer> timer;
        bool stateHasTransitioned = false;
    };

    std::array<PortSpec, Config::Bsp::kPortNum> specs;
    static constexpr int kDuration = 10000;
    static const char* kTimer;

};



#endif /* APPLICATIONS_BACKUP_THING_DECO_HXX_ */
