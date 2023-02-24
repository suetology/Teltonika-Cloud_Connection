#include "daemon.h"

#include <syslog.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int make_daemon()
{
        switch(fork()) {
        case -1: return -1;
        case 0: break;                 
        default:
                closelog(); 
                _exit(EXIT_SUCCESS);   
        }

        if(setsid() == -1)                
                return -1;

        switch(fork()) {
        case -1: return -1;
        case 0: break;                  
        default:
                closelog(); 
                _exit(EXIT_SUCCESS);   
        }

        return 0;
}