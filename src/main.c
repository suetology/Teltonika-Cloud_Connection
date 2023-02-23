#include "arg_parser.h"
#include "tuya_connection.h"

int main(int argc, char* argv[])
{
        struct DeviceInfo device_info;

        parse_arguments(argc, argv, &device_info);

        //make daemon

        tuya_connect(&device_info);
        while (1) {
                tuya_update();
        }
        return 0;
}