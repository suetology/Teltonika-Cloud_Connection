#include "arg_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <argp.h>

static char args_describtion[] = "Required four agruments (-p [Product ID] -d [Device ID] -s [Device Secret]) or one argument -f to specify the location of a config file";

static struct argp_option options[] = {
        { "product_id", 'p', "Product_ID" },
        { "device_id", 'd', "Device_ID" },
        { "device_secret", 's', "Device_secret" },
        { "config_filepath", 'f', "Filepath", 0, "All the data will be read from this file" }
};

struct arguments {
        char *product_id;
        char *device_id;
        char *device_secret;
        char *config_filepath;
};

static error_t parse_options(int key, char *arg, struct argp_state *state) 
{
        struct arguments *args = state->input;

        switch (key) {
                case 'p':
                        args->product_id = arg;
                        break;
                case 'd':
                        args->device_id = arg;
                        break;
                case 's':
                        args->device_secret = arg;
                        break;
                case 'f':
                        args->config_filepath = arg;
                        break;

                case ARGP_KEY_END:
                        if ((args->product_id == NULL || args->device_id == NULL || args->device_secret == NULL) && args->config_filepath == NULL) {
                                argp_failure(state, 1, 0, "Required four agruments (-p [Product ID] -d [Device ID] -s [Device Secret]) or one argument -f to specify the location of a config file. Use --help or -? for more info");
                                exit(ARGP_ERR_UNKNOWN);
                            }
        }
        return 0;
}

static struct argp parser = { options, parse_options, 0, args_describtion };

int parse_arguments(int argc, char* argv[], struct DeviceInfo *device_info)
{
        struct arguments args;
        
        args.product_id = NULL;
        args.device_id = NULL;
        args.device_secret = NULL;
        args.config_filepath = NULL;

        int error_code = argp_parse(&parser, argc, argv, 0, 0, &args);

        //todo: get data from config file

        device_info->product_id = args.product_id;
        device_info->device_id = args.device_id;
        device_info->device_secret = args.device_secret;

        return error_code;
}
