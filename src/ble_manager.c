#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

#include "ble_manager.h"

LOG_MODULE_REGISTER(ble_manager, LOG_LEVEL_DBG);

#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)


/* STEP 2.1 - Declare the Company identifier (Company ID) */
#define COMPANY_ID_CODE 0x0059

/* STEP 2.2 - Declare the structure for your custom data  */
typedef struct adv_mfg_data {
	uint16_t company_code; /* Company Identifier Code. */
	uint16_t number_press; /* Number of times Button 1 is pressed */
} adv_mfg_data_type;


/* STEP 5.1 - Create the advertising parameter for connectable advertising */
static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(
	(BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and use identity address */
	800, /* Min Advertising Interval 500ms (800*0.625ms) */
	801, /* Max Advertising Interval 500.625ms (801*0.625ms) */
	NULL); /* Set to NULL for undirected advertising */

/* STEP 2.3 - Define and initialize a variable of type adv_mfg_data_type */
static adv_mfg_data_type adv_mfg_data = { COMPANY_ID_CODE, 0x00 };



static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
    /* STEP 3 - Include the Manufacturer Specific Data in the advertising packet. */
	BT_DATA(BT_DATA_MANUFACTURER_DATA, (unsigned char *)&adv_mfg_data, sizeof(adv_mfg_data)),
};

static unsigned char url_data[] ={0x17,'/','/','a','c','a','d','e','m','y','.',
                                 'n','o','r','d','i','c','s','e','m','i','.',
                                 'c','o','m'};

static const struct bt_data sd[] = {
        BT_DATA(BT_DATA_URI, url_data,sizeof(url_data)),
};


int ble_manager_init()
{
    LOG_DBG("BLE Manager initialized");

    int err;

    err = bt_enable(NULL);
    if (err) {
        LOG_ERR("Bluetooth init failed (err %d)", err);
        return -1;
    }

    LOG_DBG("Bluetooth initialized");

    err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad),sd, ARRAY_SIZE(sd));
    if (err) {
        LOG_ERR("Advertising failed to start (err %d)", err);	
        return -1;
    }

    LOG_DBG("Bluetooth advertising");

    return 0;
}