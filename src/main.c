#include <syslog.h>
#include <unistd.h>
#include <stdlib.h>

#include "daemon.h"
#include "arg_parser.h"
#include "tuya_connection.h"

int main(int argc, char* argv[])
{
        openlog("Tuya-cloud", LOG_PID, 0);
        syslog(LOG_INFO, "Starting program");

        struct DeviceInfo *device_info = (struct DeviceInfo *)malloc(sizeof(struct DeviceInfo));  
        if (device_info == NULL) {
                syslog(LOG_ERR, "Failed to start program");
                closelog();
                return -1;
        }    
        if (parse_arguments(argc, argv, device_info) != 0) {
                syslog(LOG_ERR, "Failed to parse arguments");
                closelog();
                return -2;
        }
        if (make_daemon() != 0) {
                syslog(LOG_ERR, "Failed to create a daemon process");
                closelog();
                return -3;
        }
        syslog(LOG_INFO, "Created daemon program");

        if (tuya_connect(device_info) != 0) {
                syslog(LOG_ERR, "Failed to connect to Tuya cloud");
                closelog();
                return -4;
        }
        free(device_info);

        while (1) {
                tuya_update();
        }

        closelog();
        return 0;
}