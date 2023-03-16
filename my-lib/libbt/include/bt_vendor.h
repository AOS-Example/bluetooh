/******************************************************************************
 *
 *  Filename:      bt_vendor_brcm.h
 *
 *  Description:   A wrapper header file of bt_vendor_lib.h
 *
 *                 Contains definitions specific for interfacing with Broadcom
 *                 Bluetooth chipsets
 *
 ******************************************************************************/

#ifndef BT_VENDOR_H
#define BT_VENDOR_H

#include "bt_vendor_lib.h"
//#include "vnd_buildcfg.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE	(!FALSE)
#endif

#define STREAM_TO_UINT16(u16, p) {u16 = ((uint16_t)())}
#define UINT16_TO_STREAM(p, u16)
#define UINT_TO_STREAM(p, u32)

typedef enum {
	BT_SOC_DEFAULT = 0,
	BT_SOC_SMD = BT_SOC_DEFAULT,
	BT_SOC_ROME, // Need to add bluetooth type of mine, it will use with hw_<bluetooth-type>.h
	/* Add chipset type here */
	BT_SOC_RESERVED
} bt_soc_type;

typedef enum {
	BT_VND_OP_ANT_USERIAL_OPEN = 254,
	BT_VND_OP_ANT_USERIAL_CLOSE
}ant_serial;

extern bt_vendor_callbacks_t *bt_vendor_cbacks;
/* HW_NEED_END_WITH_HCI_RESET

    code implementation of sending a HCI_RESET command during the epilog
    process. It calls back to the callers after command complete of HCI_RESET
    is received.

    Default TRUE .
*/
#ifndef HW_NEED_END_WITH_HCI_RESET
#define HW_NEED_END_WITH_HCI_RESET TRUE
#endif

#define HCI_RESET 0x0C03
#define HCI_CMD_PREAMBLE_SIZE 3
#define HCI_EVT_CMD_CMPL_STATUS_RET_BYTE 5
#define HCI_EVT_CMD_CMPL_OPCODE 3

#endif /* BT_VENDOR_H */