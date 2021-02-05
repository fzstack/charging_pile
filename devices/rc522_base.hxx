/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     imgcr       the first version
 */
#pragma once
#include <utilities/observer.hxx>

class Rc522Base {
    using card_id_t = std::optional<std::string>;
protected:
    Observable<card_id_t> cardId;
public:
    Observer<card_id_t> oCardId = cardId;
};


