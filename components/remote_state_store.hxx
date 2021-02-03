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

#include "state_store_base.hxx"

class RemoteStateStore: public StateStoreBase {
public:
    RemoteStateStore(int idx);
};

#include <utilities/singleton.hxx>
namespace Preset {
template<int R>
class RemoteStateStore: public Singleton<RemoteStateStore<R>>, public ::RemoteStateStore {
    friend class Singleton<RemoteStateStore<R>>;
    RemoteStateStore(): ::RemoteStateStore(R) {}
};
}
