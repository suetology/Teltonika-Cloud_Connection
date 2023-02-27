#include <syslog.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "daemon.h"
#include "arg_parser.h"
#include "tuya_connection.h"

unsigned program_running;

void signal_handler(int sig) 
{
        program_running = 0;
}

int main(int argc, char* argv[])
{
        program_running = 1;

        openlog("Tuya-cloud", LOG_PID, 0);
        syslog(LOG_INFO, "Starting program");

        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);
        signal(SIGQUIT, signal_handler);

        struct DeviceInfo device_info;
        unsigned is_daemon;

        if (parse_arguments(argc, argv, &is_daemon, &device_info) != 0) {
                syslog(LOG_ERR, "Failed to parse arguments");
                closelog();
                return -2;
        }
        if (is_daemon && make_daemon() != 0) {
                syslog(LOG_ERR, "Failed to create a daemon process");
                closelog();
                return -3;
        }
        syslog(LOG_INFO, "Created daemon program");

        if (tuya_connect(&device_info) != 0) {
                syslog(LOG_ERR, "Failed to connect to Tuya cloud");
                closelog();
                return -4;
        }

        while (program_running) {
                tuya_update();
                sleep(15);
        }
        tuya_disconnect();

        closelog();
        return 0;
}