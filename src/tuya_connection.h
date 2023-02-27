#ifndef TUYA_CONNECTION_H
#define TUYA_CONNECTION_H

#define CAPACITY 30

struct DeviceInfo {
        char product_id[CAPACITY];
        char device_id[CAPACITY];
        char device_secret[CAPACITY];
};

int tuya_connect(struct DeviceInfo *device_info);
void tuya_update();
void tuya_disconnect();

#endif