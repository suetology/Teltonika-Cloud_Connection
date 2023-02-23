#ifndef TUYA_CONNECTION_H
#define TUYA_CONNECTION_H

struct DeviceInfo {
        char *product_id;
        char *device_id;
        char *device_secret;
};

int tuya_connect(struct DeviceInfo *device_info);
void tuya_update();

#endif