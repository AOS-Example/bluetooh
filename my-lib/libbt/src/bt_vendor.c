#define LOG_TAG "bt_vendor"
#define BLUETOOTH_MAC_ADDR_BOOT_PROPERTY "ro.boot.btmacaddr"

#include <utils/Log.h>
#include <string.h>
#include <fcntl.h>
#include "bt_vendor.h"
#include "hci_uart.h"
#include "upio.h"
#define WAIT_TIMEOUT 200000
#define BT_VND_OP_GET_LINESPEED 12

#ifndef BTVND_DBG
#define BTVND_DBG FALSE
#endif

#if (BTVND_DBG == TRUE)
#define BTVNDDBG(param, ...) {ALOGD(param, ## __VA_ARGS__);}
#else
#define BTVNDDBG(param, ...) {}
#endif

void hw_config_start(void);
uint8_t hw_lpm_enable(uint8_t turn_on);
uint8_t hw_lpm_get_idle_timeout(void);
void hw_lpm_set_wake_state(uint8_t wake_assert);
#if (SCO_CFG_INCLUDED == TRUE)
void hw_sco_config(void);
#endif
void vnd_load_conf(const char *p_path);
#if (HW_END_WITH_HCI_RESET == TRUE)
void hw_epilog_process(void);
#endif

bt_vendor_callbacks_t *bt_vendor_cbacks = NULL;
uint8_t vnd_load_bd_addr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const tUSERIAL_CFG userial_init_cfg =
{
	(USERIAL_DATABITS_8 | USERIAL_PARITY_NONE | USERIAL_STOPBITS_1),
	USERIAL_BAUD_115200
};

/*****************************************************************************
**
**   BLUETOOTH VENDOR INTERFACE LIBRARY FUNCTIONS
**
*****************************************************************************/
static int init (const bt_vendor_callbacks_t* p_cb, unsigned char *local_bdaddr)
{
	ALOGI("Init");

	if (p_cb == NULL) {
		ALOGE("Init failed with no user callbacks!");
		return -1;
	}

	userial_vendor_init();
	upio_init();

	//vnd_load_conf(VENDOR_LIB_CONF_FILE);

	/* store reference to user callbacks */
	bt_vendor_cbacks = (bt_vendor_callbacks_t *)p_cb;

	/* This is handed over from the stack */
	memcpy(vnd_load_bd_addr, local_bdaddr, 6);

	return 0;
}

/* Requested operations */
static int op(bt_vendor_opcode_t opcode, void *param)
{
	int retval = 0;

	BTVNDDBG("op for %d", opcode);

	switch(opcode)
	{
		case BT_VND_OP_POWER_CTRL:
			{
				int *state = (int *)param;
				upio_set_bluetooth_power(UPIO_BT_POWER_OFF);
				if (*state == BT_VND_PWR_ON)
				{
					ALOGW("NOTE: BT_VND_PWR_ON now forces power-off first");
					upio_set_bluetooth_power(UPIO_BT_POWER_ON);
				} else {
					/* Make sure wakelock is released */
					hw_lpm_set_wake_state(false);
				}
			}
			break;

		case BT_VND_OP_USERIAL_OPEN:
		{
			int (*fd_array)[] = (int (*)[])param;
			int fd, idx;
			fs = userial_vendor_open()
		}
	}
}