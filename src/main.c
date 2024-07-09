
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "ble_manager.h"
#include "button_manager.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

int main(void)
{
	LOG_DBG("Hello World! %s\n", CONFIG_BOARD);

    int err;

	button_manager_init();

	err = ble_manager_init();
	if (err) {
        LOG_ERR("BLE Manager Failed (err %d)", err);
        return err;
    }

	while (1) {
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
