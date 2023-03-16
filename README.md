# Android Bluetooth

This repo have features:
- Support device driver for new bluetooh module
- Modify HAL and HIDL to support new feature for bluetooh
- App tests feature


# Vendor implementation
image.png

In the Android, the bluetooh will be implement by method 2 (use libbt-vendor and call directly
to UART driver).
By the Broadcom device, it uses libbt-vendor in `hardware/broadcom/libbt`
You need to read this source code to know that which APIs should be implemented in UART driver 

# HCI layer 
Refer to [Android source code](https://android.googlesource.com/platform/system/bt/+/refs/tags/android-platform-12.0.0_r16/vendor_libs/linux/)

# HIDL Layer
Refer to [Bluetooth HIDL code]()


# Support check bluetooth module has firmware or not
AOS call to check_fw_bt_nordic 
-> nrf module reply -> don't need flash firmware
-> nrf modlue doesn't reply in 2 request messages -> need to flash firmware

