/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-07     imgcr       the first version
 */

#include "deserializer.hxx"

 Deserializer::Deserializer(std::shared_ptr<IStream> istream): istream(istream) {

 }

