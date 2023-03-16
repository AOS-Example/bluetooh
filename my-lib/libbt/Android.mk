LOCAL_PATH := $(call my-dir)

#Config for new bluetooth module
ifneq ($(BOARD_HAVE_NEW_BLUETOOH),)

include $(CLEAR_VARS)

ifneq 


MYDROID_DIR := $(TOP_DIR)packages/modules/Bluetooth/system

ifeq ($(strip $(USE_MY_BLUETOOTH)), true)
LOCAL_CFLAGS += -DUSE_MY_BLUETOOTH
endif

LOCAL_CFLAGS += \
		-Wall \
		-Werror \

LOCAL_SRC_FILES := \
		src/bt_vendor.c \
		src/hardware.c \