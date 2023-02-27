#include "tuya_connection.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <syslog.h>

#include "cJSON.h"
#include "tuya_error_code.h"
#include "system_interface.h"
#include "mqtt_client_interface.h"
#include "tuyalink_core.h"
#include "tuya_cacert.h"

tuya_mqtt_context_t client_instance;
struct DeviceInfo *device; 

void on_connect(tuya_mqtt_context_t* context, void* user_data)
{
	syslog(LOG_INFO, "Connected to Tuya cloud");
}

void on_disconnect(tuya_mqtt_context_t* context, void* user_data)
{
	syslog(LOG_INFO, "Disconnected from Tuya cloud");
}

void on_message(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg)
{
	syslog(LOG_INFO, "On message(id: %s, type: %d, code: %d)", msg->msgid, msg->type, msg->code);
	switch (msg->type) {
        case THING_TYPE_MODEL_RSP:
		syslog(LOG_INFO, "Model data: %s", msg->data_string);
		break;

        case THING_TYPE_PROPERTY_SET:
		syslog(LOG_INFO, "Property set: %s", msg->data_string);
            	break;
    	}
}

int tuya_connect(struct DeviceInfo *device_info)
{
        int ret = OPRT_OK;
	device = device_info;

        ret = tuya_mqtt_init(&client_instance, &(const tuya_mqtt_config_t) {
                .host = "m1.tuyacn.com",
                .port = 8883,
                .cacert = tuya_cacert_pem,
                .cacert_len = sizeof(tuya_cacert_pem),
                .device_id = device_info->device_id,
                .device_secret = device_info->device_secret,
                .keepalive = 100,
                .timeout_ms = 2000,
                .on_connected = on_connect,
                .on_disconnect = on_disconnect,
                .on_messages = on_message
        });
        if (ret != OPRT_OK) 
		return ret;

        ret = tuya_mqtt_connect(&client_instance);
        if (ret != OPRT_OK)
		return ret;

        return ret;
}

void send_uptime()
{
	struct sysinfo info;
	sysinfo(&info);
	long uptime = info.uptime;

	cJSON *data = cJSON_CreateObject();
	cJSON_AddNumberToObject(data, "uptime", uptime); 

	char *data_str = cJSON_Print(data);
	if (data_str != NULL) {
		tuyalink_thing_property_report(&client_instance, device->device_id, data_str);
		free(data_str);
	}
	cJSON_Delete(data);
}

void tuya_update() 
{
	send_uptime();
      	tuya_mqtt_loop(&client_instance); 
}

void tuya_disconnect()
{
	tuya_mqtt_disconnect(&client_instance);
	tuya_mqtt_deinit(&client_instance);
}