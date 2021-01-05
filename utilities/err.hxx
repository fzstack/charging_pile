/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-18     imgcr       the first version
 */
#ifndef UTILITIES_ERR_HXX_
#define UTILITIES_ERR_HXX_

#include <stdexcept>

class not_implemented: public std::runtime_error {
    using std::runtime_error::runtime_error;
public:
    not_implemented(): std::runtime_error("not implemented") {}
};

class timeout_error: virtual public std::runtime_error {
    using std::runtime_error::runtime_error;
};



#endif /* UTILITIES_ERR_HXX_ */
