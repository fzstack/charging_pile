/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-20     imgcr       the first version
 */
#ifndef APPLICATIONS_APP_HXX_
#define APPLICATIONS_APP_HXX_

#ifdef __cplusplus
extern "C" {
#endif
void runApp();
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class App {
public:
    virtual void run() = 0;
};


#endif



#endif /* APPLICATIONS_APP_HXX_ */
