/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-03     imgcr       the first version
 */
#pragma once

#include "state_store.hxx"

class LocalStateStore: public StateStore {
public:
    LocalStateStore(int idx);
};

#include <utilities/singleton.hxx>
namespace Preset {
template<int R>
class LocalStateStore: public Singleton<LocalStateStore<R>>, public ::LocalStateStore {
    friend class Singleton<LocalStateStore<R>>;
    LocalStateStore(): ::LocalStateStore(R) {}
};
}
