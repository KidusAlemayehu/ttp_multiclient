#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include "socket_utils.h"

int main(){
    int server_fd = create_socket(8050);
    printf("Server listening on port 8050\n");
    while(1){
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0){
            perror("connection accept failure");
            continue;
        }
        printf("Connection accepted\n");
        close(client_fd);
    }
    close(server_fd);
    return 0;
}