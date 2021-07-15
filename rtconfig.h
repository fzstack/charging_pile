#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

/* RT-Thread Kernel */

#define RT_NAME_MAX 8
#define RT_ALIGN_SIZE 4
#define RT_THREAD_PRIORITY_32
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 1000
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_USING_IDLE_HOOK
#define RT_IDLE_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 256
#define RT_USING_TIMER_SOFT
#define RT_TIMER_THREAD_PRIO 4
#define RT_TIMER_THREAD_STACK_SIZE 2048

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MESSAGEQUEUE

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_SMALL_MEM
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 256
#define RT_CONSOLE_DEVICE_NAME "uart1"
#define RT_VER_NUM 0x40002
#define ARCH_ARM
#define RT_USING_CPU_FFS
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M3

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE 2048
#define RT_MAIN_THREAD_PRIORITY 10

/* C++ features */

#define RT_USING_CPLUSPLUS

/* Command shell */


/* Device virtual file system */


/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_PIPE_BUFSZ 128
#define RT_USING_SERIAL
#define RT_SERIAL_USING_DMA
#define RT_SERIAL_RB_BUFSZ 1024
#define RT_USING_I2C
#define RT_USING_I2C_BITOPS
#define RT_USING_PIN
#define RT_USING_RTC
#define RT_USING_SPI
#define RT_USING_WDT

/* Using USB */


/* POSIX layer and C standard library */

#define RT_USING_LIBC

/* Network */

/* Socket abstraction layer */


/* Network interface device */


/* light weight TCP/IP stack */


/* AT commands */


/* VBUS(Virtual Software BUS) */


/* Utilities */


/* RT-Thread online packages */

/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* IoT Cloud */


/* security packages */


/* language packages */


/* multimedia packages */


/* tools packages */


/* system packages */

#define PKG_USING_FAL
#define FAL_DEBUG_CONFIG
#define FAL_DEBUG 1
#define FAL_PART_HAS_TABLE_CFG
#define PKG_USING_FAL_V00500
#define PKG_FAL_VER_NUM 0x00500
#define PKG_USING_FLASHDB
#define FDB_USING_KVDB
#define FDB_USING_FAL_MODE
#define FDB_WRITE_GRAN_32BITS
#define FDB_WRITE_GRAN 32
#define FDB_DEBUG_ENABLE
#define PKG_USING_FLASHDB_V10000
#define PKG_FLASHDB_VER_NUM 0x10000

/* Micrium: Micrium software products porting for RT-Thread */


/* peripheral libraries and drivers */


/* AI packages */


/* miscellaneous packages */


/* samples: kernel and components samples */

#define PKG_USING_CRCLIB
#define CRCLIB_USING_CRC8
#define CRC8_USING_CONST_TABLE
#define CRC8_POLY_8C
#define CRC8_POLY 140
#define CRCLIB_USING_CRC16
#define CRC16_USING_CONST_TABLE
#define CRC16_POLY_A001
#define CRC16_POLY 40961
#define CRCLIB_USING_CRC32
#define CRC32_USING_CONST_TABLE
#define CRC32_POLY_EDB88320
#define CRC32_POLY 3988292384
#define PKG_USING_CRCLIB_V100

/* entertainment: terminal games and other interesting software packages */


/* samples: kernel and components samples */

/* App Config */

#define RUN_APP
#define ENABLE_REMOTE
#define USE_LOWER_END
#define LOWER_VERSION "0.2.22a"

/* Test Config */

#define RT_STUDIO_BUILT_IN

#endif
