#include "arg_parser.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

static char args_describtion[] = "-D                    Creates daemon process\n-p [Product ID]         Specifies product ID\n-i [Device ID]            Specifies device ID\n-s [Device secret]         Specifies device secret\n-f [Filepath]           All the device data will be read from this file\n-?                    Shows this message\n";

struct Arguments {
        unsigned daemon;
        char *product_id;
        char *device_id;
        char *device_secret;
        char *config_filepath;
};

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

void parse_options(int argc, char* argv[], struct Arguments *args)
{
        int c;
        while ((c = getopt(argc, argv, "Dp:i:s:f:?")) != -1) {
                switch (c) {
                case 'D':
                        args->daemon = 1;
                        break;
                case 'p':
                        args->product_id = optarg;
                        break;
                case 'i':
                        args->device_id = optarg;
                        break;
                case 's':
                        args->device_secret = optarg;
                        break;
                case 'f':
                        args->config_filepath = optarg;
                        break;
                case '?':
                        printf("%s", args_describtion);
                        exit(0);
                }
        }

        if ((args->product_id == NULL || args->device_id == NULL || args->device_secret == NULL) 
        && args->config_filepath == NULL) {
                fprintf(stderr, "Required four agruments (-p [Product ID] -i [Device ID] -s [Device Secret]) or one argument -f to specify the location of a config file. Use -? for more info\n");
                exit(1);
        }   
}

int parse_arguments(int argc, char* argv[], unsigned *is_daemon, struct DeviceInfo *device_info) 
{
        struct Arguments args;
        args.daemon = 0;
        args.product_id = NULL;
        args.device_id = NULL;
        args.device_secret = NULL;
        args.config_filepath = NULL;

        parse_options(argc, argv, &args);

        int error_code = 0;

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