/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-02     imgcr       the first version
 */
#pragma once

#include "local_backup_man.hxx"
#include "remote_backup_man.hxx"

namespace Preset {
#ifdef UPPER_END
using BackupMan = LocalBackupMan;
#endif

#ifdef LOWER_END
using BackupMan = RemoteBackupMan;
#endif
}

