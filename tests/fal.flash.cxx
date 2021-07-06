#include <board.h>
extern "C" {
#include <drv_spi.h>
}
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
extern "C" {
#include <spi_flash_sfud.h>
}
#include <stm32f1xx.h>

extern "C" {
#include <fal.h>
}

#include <flashdb.h>

#define SPI_BUS_NAME       "spi2"
#define SPI_DEVICE_NAME    "spi20"

#define SPI_CS_PIN         GET_PIN(B, 12)

static void lock(fdb_db_t db) {
    __disable_irq();
}

static void unlock(fdb_db_t db) {
    __enable_irq();
}

extern fdb_kvdb _global_kvdb;

static void write_test_key(int argc, char** argv) {
    fdb_kv_set(&_global_kvdb, "test", argv[1]);
}

static void read_test_key() {
    rt_kprintf("value: %s\n", fdb_kv_get(&_global_kvdb, "test"));
}

static int rt_hw_spi_flash_with_sfud_init(void) {
    rt_hw_spi_device_attach(SPI_BUS_NAME, SPI_DEVICE_NAME, GPIOB, GPIO_PIN_12);
    rt_sfud_flash_probe(NOR_FLASH_DEV_NAME, SPI_DEVICE_NAME);
    fal_init();

    fdb_kvdb_control(&_global_kvdb, FDB_KVDB_CTRL_SET_LOCK, (void*)lock);
    fdb_kvdb_control(&_global_kvdb, FDB_KVDB_CTRL_SET_UNLOCK, (void*)unlock);

    auto def_kv = fdb_default_kv{num: 0};
    fdb_kvdb_init(&_global_kvdb, "env", "fdb_kvdb", &def_kv, NULL);
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_with_sfud_init);
MSH_CMD_EXPORT(write_test_key, );
MSH_CMD_EXPORT(read_test_key, );
