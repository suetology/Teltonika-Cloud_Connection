#include "arg_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <syslog.h>

static char args_describtion[] = "Required four agruments (-p [Product ID] -i [Device ID] -s [Device Secret]) or one argument -f to specify the location of a config file containing device information in the following format:\n\tproduct_id [id]\n\tdevice_id [id]\n\tdevice_secret [secret]";

static struct argp_option options[] = {
        { "daemon", 'D', 0, 0, "Creates daemon process" },
        { "product_id", 'p', "Product_ID" },
        { "device_id", 'i', "Device_ID" },
        { "device_secret", 's', "Device_secret" },
        { "config_filepath", 'f', "Filepath", 0, "All the data will be read from this file" }, 
        { 0 }
};

struct arguments {
        unsigned daemon;
        char *product_id;
        char *device_id;
        char *device_secret;
        char *config_filepath;
};

static error_t parse_options(int key, char *arg, struct argp_state *state) 
{
        struct arguments *args = state->input;

        switch (key) {
        case 'D':
                args->daemon = 1;
                break;
        case 'p':
                args->product_id = arg;
                break;
        case 'i':
                args->device_id = arg;
                break;
        case 's':
                args->device_secret = arg;
                break;
        case 'f':
                args->config_filepath = arg;
                break;

        case ARGP_KEY_END:
                if ((args->product_id == NULL || args->device_id == NULL || args->device_secret == NULL) 
                  && args->config_filepath == NULL) {
                        argp_failure(state, 1, 0, "Required four agruments (-p [Product ID] -i [Device ID] -s [Device Secret]) or one argument -f to specify the location of a config file. Use --help or -? for more info");
                        exit(ARGP_ERR_UNKNOWN);
                        }
        }
        return 0;
}

static struct argp parser = { options, parse_options, 0, args_describtion };

int get_device_from_file(char *filepath, struct DeviceInfo *device_info) 
{
        FILE *file = fopen(filepath, "r");
        if (file == NULL) {
                syslog(LOG_ERR, "Failed to open config file");
                return -1;
        }

        char line[60];
        while (fgets(line, sizeof(line), file) != NULL) {
                if (strcmp(line, "\n") == 0)
                        continue;

                char *attribute = strtok(line, " ");
                char *value = strtok(NULL, "\n");

                if (strcmp(attribute, "product_id") == 0) {
                        strcpy(device_info->product_id, value);
                } else if (strcmp(attribute, "device_id") == 0) {
                        strcpy(device_info->device_id, value);
                } else if (strcmp(attribute, "device_secret") == 0) {
                        strcpy(device_info->device_secret, value);
                } else {
                        syslog(LOG_ERR, "Failed reading config file");
                        return -2;
                }
        }
        fclose(file);
        return 0;
}

int parse_arguments(int argc, char* argv[], unsigned *is_daemon, struct DeviceInfo *device_info)
{
        struct arguments args;
        
        args.daemon = 0;
        args.product_id = NULL;
        args.device_id = NULL;
        args.device_secret = NULL;
        args.config_filepath = NULL;

        int error_code = argp_parse(&parser, argc, argv, 0, 0, &args);

        if (args.config_filepath == NULL) {
                strcpy(device_info->product_id, args.product_id);
                strcpy(device_info->device_id, args.device_id);
                strcpy(device_info->device_secret, args.device_secret);
        } else {
                error_code = get_device_from_file(args.config_filepath, device_info);
        }
        *is_daemon = args.daemon;

        return error_code;
}
