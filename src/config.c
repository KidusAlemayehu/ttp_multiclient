#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

static char *config_file = "config.yaml";

void load_config(){
    const char *server_port = getenv("SERVER_PORT");
    if (server_port != NULL){
        printf("Loaded server port %s\n", server_port);
    }else{
        printf("SERVER_PORT not set\n");
    }

}