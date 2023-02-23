#include "tuya_connection.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "cJSON.h"
#include "tuya_log.h"
#include "tuya_error_code.h"
#include "system_interface.h"
#include "mqtt_client_interface.h"
#include "tuyalink_core.h"
#include "tuya_cacert.h"

tuya_mqtt_context_t client_instance;

void on_connected(tuya_mqtt_context_t* context, void* user_data)
{
    TY_LOGI("on connected");
}

void on_disconnect(tuya_mqtt_context_t* context, void* user_data)
{
    TY_LOGI("on disconnect");
}

void on_messages(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg)
{
    TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    printf("\r\n");
}

int tuya_connect(struct DeviceInfo *device_info)
{
        int ret = OPRT_OK;

        ret = tuya_mqtt_init(&client_instance, &(const tuya_mqtt_config_t) {
                .host = "m1.tuyacn.com",
                .port = 8883,
                .cacert = tuya_cacert_pem,
                .cacert_len = sizeof(tuya_cacert_pem),
                .device_id = device_info->device_id,
                .device_secret = device_info->device_secret,
                .keepalive = 100,
                .timeout_ms = 2000,
                .on_connected = on_connected,
                .on_disconnect = on_disconnect,
                .on_messages = on_messages
        });
        assert(ret == OPRT_OK);

        ret = tuya_mqtt_connect(&client_instance);
        assert(ret == OPRT_OK);

        return ret;
}

void tuya_update() 
{
      tuya_mqtt_loop(&client_instance);  
}