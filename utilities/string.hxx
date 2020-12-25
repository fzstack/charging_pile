/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-26     imgcr       the first version
 */
#ifndef UTILITIES_STRING_HXX_
#define UTILITIES_STRING_HXX_

#include <vector>
#include <string>
#include <string_view>

std::vector<std::string> split(std::string_view topic, char splitter);


#endif /* UTILITIES_STRING_HXX_ */
