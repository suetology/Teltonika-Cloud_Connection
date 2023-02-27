#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include "tuya_connection.h"

int parse_arguments(int argc, char* argv[], unsigned *is_daemon, struct DeviceInfo *device_info);

#endif